#include "client.h"

int main() {
  Client& c = Client::get_instance();
  //c.create_request();
  c.connect();
  c.create_tcp_socket();
  c.open_tcp_socket();
  c.login();
  c.open_channel();
  c.create_reply_queue();
  //c.set_request_properties();
  c.publish_request(3);
  c.set_consumer();
  c.process_response();
  c.close_channel();
  c.close_connection();
  c.disconnect();
  return 0;
}