#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>

#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>

#include <assert.h>

#include "utils.hpp"
#include "../proto-files/message.pb.h"


int main(int argc, char *argv[]) {
  char const *hostname;
  int port, status;
  uuid_t uuid;
  char uuid_str[37];
  amqp_connection_state_t conn;
  amqp_socket_t *socket = NULL;
  amqp_bytes_t reply_to_queue;

  hostname = argv[1];
  port = atoi(argv[2]);



  if (argc < 3) { /* minimum number of mandatory arguments */
    fprintf(stderr,
            "usage: ./cl host port");
    return 1;
  }

  /*
    generate a random number and wrap it to a protobuf format. placeholder before qt usage
  */

  int num;
  srand(time(NULL));  
  TestTask::Messages::Request request;
  request.set_id("test-request");
  num = rand() % 129;
  std::cout << "num to send:" << num << std::endl; 
  request.set_req(num);
  std::string serialized_request;
  if (!request.SerializeToString(&serialized_request)) {
      fprintf(stderr, "Failed to serialize Protobuf request\n");
      return 1;
  }

  /*
     establish a channel that is used to connect RabbitMQ server
  */

  conn = amqp_new_connection();

  socket = amqp_tcp_socket_new(conn);
  if (!socket) {
    die("creating TCP socket");
  }

  status = amqp_socket_open(socket, hostname, port);
  if (status) {
    die("opening TCP socket");
  }

  die_on_amqp_error(amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN,
                               "guest", "guest"),
                    "Logging in");
  amqp_channel_open(conn, 1);
  die_on_amqp_error(amqp_get_rpc_reply(conn), "Opening channel");

  /*
     create private reply_to queue
  */

  {
    amqp_queue_declare_ok_t *r = amqp_queue_declare(
        conn, 1, amqp_empty_bytes, 0, 0, 0, 1, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Declaring queue");
    reply_to_queue = amqp_bytes_malloc_dup(r->queue);
    if (reply_to_queue.bytes == NULL) {
      fprintf(stderr, "Out of memory while copying queue name");
      return 1;
    }
  }

  {
    /*
      set properties
    */
    amqp_basic_properties_t props;
    props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG |
                   AMQP_BASIC_DELIVERY_MODE_FLAG | AMQP_BASIC_REPLY_TO_FLAG |
                   AMQP_BASIC_CORRELATION_ID_FLAG;
    props.content_type = amqp_cstring_bytes("text/plain");
    props.delivery_mode = 2; /* persistent delivery mode */
    props.reply_to = amqp_bytes_malloc_dup(reply_to_queue);
    if (props.reply_to.bytes == NULL) {
      fprintf(stderr, "Out of memory while copying queue name");
      return 1;
    }
    
    uuid_generate(uuid);
    uuid_unparse(uuid, uuid_str);
    props.correlation_id = amqp_cstring_bytes(uuid_str);

    /*
      publish
    */
    die_on_error(amqp_basic_publish(conn, 1, amqp_empty_bytes,
                                    amqp_cstring_bytes("rpc_queue"), 0, 0,
                                    &props, amqp_cstring_bytes(serialized_request.c_str())),
                 "Publishing");

    amqp_bytes_free(props.reply_to);
  }

  {
    amqp_basic_consume(conn, 1, reply_to_queue, amqp_empty_bytes, 0, 1, 0,
                       amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Consuming");
    amqp_bytes_free(reply_to_queue);

    {
    for (;;) {
      amqp_rpc_reply_t res;
      amqp_envelope_t envelope;

      amqp_maybe_release_buffers(conn);

      res = amqp_consume_message(conn, &envelope, NULL, 0);

      if (AMQP_RESPONSE_NORMAL != res.reply_type) {
        break;
      }

      TestTask::Messages::Response response;
      response.ParseFromString(std::string((const char*)envelope.message.body.bytes));
      std::cout << "received from server: " << response.res() << std::endl;
      amqp_destroy_envelope(&envelope);
      break;
    }
    
  }
  }

  /*
     closing
  */

  die_on_amqp_error(amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS),
                    "Closing channel");
  die_on_amqp_error(amqp_connection_close(conn, AMQP_REPLY_SUCCESS),
                    "Closing connection");
  die_on_error(amqp_destroy_connection(conn), "Ending connection");


  return 0;

}