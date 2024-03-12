#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
#include <string>
#include "utils.hpp"
#ifndef SERVER_H
#define SERVER_H


class Server {
public:
  Server(const Server&) = delete;
  Server(Server&&) = delete;
  Server& operator=(const Server&) = delete;
  Server& operator=(Server&&) = delete;
  static Server& get_instance()  {
    static Server instance = Server();
    return instance;
  }
  bool load_cfg();
  void run();
  ~Server() = default;
private:
  Server() = default;
  void connect();
  void create_tcp_socket();
  void open_tcp_socket();
  void login();
  void open_channel();
  void declare_queue(const std::string& rpc_queue);
  void set_queue_listener();
  void process();
  void close_channel();
  void close_connection();
  void disconnect();

  char const *m_hostname = "localhost";
  int m_port = 5672, m_status;
  amqp_socket_t *m_socket = NULL;
  amqp_connection_state_t m_conn;
  amqp_bytes_t m_queuename;
};

#endif