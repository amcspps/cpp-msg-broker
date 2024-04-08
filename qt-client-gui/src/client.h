#ifndef CLIENT_H
#define CLIENT_H


#include <uuid/uuid.h>
#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
#include "../proto-files/message.pb.h"
#include <boost/program_options.hpp>
#include <glog/logging.h>
#include <tuple>


namespace po = boost::program_options;


class Client {
public:
  Client(const Client&) = delete;
  Client(Client&&) = delete;
  Client& operator= (const Client&) = delete;
  Client& operator= (Client&&) = delete;
  static Client& get_instance() {
    static Client instance = Client();
    return instance;
  }
  void start_logging();
  void load_cfg(po::variables_map& vm);
  void load_cfg();
  void connect();
  void create_tcp_socket();
  void open_tcp_socket();
  void login();
  void create_reply_queue();
  void open_channel();
  void publish_request(int num);
  void set_consumer();
  std::tuple<bool, std::string> process_response();
  void close_channel();
  void close_connection();
  void disconnect();
  ~Client() = default;

  std::string get_log_dir();
  std::string get_cfg_path();

  void set_cfg_path(po::variables_map& vm);
  void set_hostname(std::string hostname);
  void set_port(int port);
  void set_log_dir(std::string log_dir);
  void set_log_lvl(std::string log_lvl);
private:
  Client() = default;

  std::string m_hostname, m_cfg_path, m_log_dir, m_log_lvl;
  int m_port, m_status;
  amqp_connection_state_t m_conn;
  amqp_socket_t *m_socket = NULL;
  amqp_bytes_t m_reply_to_queue;
};


#endif
