#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
#include <string>
#include "utils.hpp"
#include <boost/program_options.hpp>
#include <glog/logging.h>
namespace po = boost::program_options;

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
  std::string get_log_level();
  std::string get_hostname();
  int get_port();
  std::string get_queuename();
  void connect();
  void create_tcp_socket();
  void open_tcp_socket();
  void login();
  void open_channel();
  void declare_queue();
  void set_queue_listener();
  void process();
  void close_channel();
  void close_connection();
  void disconnect();
  void set_hostname(std::string hostname);
  void set_port(int port);
  void set_queuename(std::string queuename);
  void set_log_level(std::string& lvl);
  void load_cfg(po::variables_map& vm);
  void run();
  ~Server() = default;
private:
  Server() = default;
  
  std::string m_hostname;
  int m_port, m_status;
  amqp_socket_t *m_socket = NULL;
  amqp_connection_state_t m_conn;
  std::string m_queuename;
  std::string m_log_level;
};


#endif