#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>

#include <assert.h>

#include "utils.hpp"

int main(int argc, char *argv[]) {
  char const *hostname;
  int port, status;
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
    props.correlation_id = amqp_cstring_bytes("1");

    /*
      publish
    */
    die_on_error(amqp_basic_publish(conn, 1, amqp_empty_bytes,
                                    amqp_cstring_bytes("rpc_queue"), 0, 0,
                                    &props, amqp_cstring_bytes("---REQUEST---")),
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

      printf("Delivery %u, exchange %.*s routingkey %.*s\n",
             (unsigned)envelope.delivery_tag, (int)envelope.exchange.len,
             (char *)envelope.exchange.bytes, (int)envelope.routing_key.len,
             (char *)envelope.routing_key.bytes);

      if (envelope.message.properties._flags & AMQP_BASIC_CONTENT_TYPE_FLAG) {
        printf("Content-type: %.*s\n",
               (int)envelope.message.properties.content_type.len,
               (char *)envelope.message.properties.content_type.bytes);
      }
      printf("----\n");

      amqp_dump(envelope.message.body.bytes, envelope.message.body.len);

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
