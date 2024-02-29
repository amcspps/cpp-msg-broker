#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>

#include <assert.h>

#include "utils.hpp"


int main(int argc, char const *const *argv) {
  char const *hostname;
  int port, status;
  amqp_socket_t *socket = NULL;
  amqp_connection_state_t conn;


  if (argc < 3) {
    fprintf(stderr, "Usage: ./srv host port\n");
    return 1;
  }

  hostname = argv[1];
  port = atoi(argv[2]);


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
     create rpc_queue
  */

  {
    amqp_bytes_t queuename = amqp_cstring_bytes("rpc_queue");
    amqp_queue_declare_ok_t *r = amqp_queue_declare(
        conn, 1, queuename, 0, 0, 0, 1, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Declaring queue");
  
    amqp_basic_consume(conn, 1, queuename, amqp_empty_bytes,
                       0, 0, 0, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Consuming");
  
  }
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

    amqp_basic_properties_t reply_props;
    reply_props._flags = AMQP_BASIC_CORRELATION_ID_FLAG;
    reply_props.correlation_id = envelope.message.properties.correlation_id;
    printf("UUID: %s\n", (char *)envelope.message.properties.correlation_id.bytes);
    die_on_error(amqp_basic_publish(conn, 1, amqp_empty_bytes,
                                    envelope.message.properties.reply_to, 0, 0,
                                    &reply_props, amqp_cstring_bytes("answer")),
                 "Publishing");

    amqp_destroy_envelope(&envelope);
  }

  die_on_amqp_error(amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS),
                    "Closing channel");
  die_on_amqp_error(amqp_connection_close(conn, AMQP_REPLY_SUCCESS),
                    "Closing connection");
  die_on_error(amqp_destroy_connection(conn), "Ending connection");


  return 0;
}