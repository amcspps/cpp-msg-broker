#include "server.hpp"
#include "../proto-files/message.pb.h"
#include <stdexcept>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem.hpp>

namespace pt = boost::property_tree;
namespace fs = boost::filesystem;

void Server::set_hostname(std::string hostname) {
  m_hostname = hostname;
};

void Server::set_port(int port) {
  m_port = port;
};

void Server::set_queuename(std::string queuename){
  m_queuename = queuename;
};

void Server::load_cfg(po::variables_map& vm) {
  pt::ptree tree;
  pt::ini_parser::read_ini(fs::absolute(vm["config"].as<std::string>()).string(), tree); 

  set_hostname(tree.get<std::string>("server.host"));
  set_port(tree.get<int>("server.port"));
  set_queuename(tree.get<std::string>("server.queuename"));
}

void Server::connect() {
  m_conn = amqp_new_connection() /*returns NULL or 0*/;
  if (!m_conn) {
    throw std::runtime_error("Connection creation failed");
  }
};

void Server::create_tcp_socket() {
  m_socket = amqp_tcp_socket_new(m_conn);
  if (!m_socket) {
    die("creating TCP socket");
  }
};

void Server::open_tcp_socket() {
  m_status = amqp_socket_open(m_socket, m_hostname.c_str(), m_port);
  if (m_status) {
    die("opening TCP socket");
  }
};

void Server::login() {
  die_on_amqp_error(amqp_login(m_conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN,
                               "guest", "guest"),
                    "Logging in");
};

void Server::open_channel() {
  amqp_channel_open(m_conn, 1);
  die_on_amqp_error(amqp_get_rpc_reply(m_conn), "Opening channel");
};

void Server::declare_queue() {
    amqp_queue_declare_ok_t *r = amqp_queue_declare(
        m_conn, 1, amqp_cstring_bytes(m_queuename.c_str()), 0, 0, 0, 1, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(m_conn), "Declaring queue");
};

void Server::set_queue_listener() {
  amqp_basic_consume(m_conn, 1, amqp_cstring_bytes(m_queuename.c_str()), amqp_empty_bytes,
                       0, 0, 0, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(m_conn), "Consuming");
};

void Server::close_channel() {
  die_on_amqp_error(amqp_channel_close(m_conn, 1, AMQP_REPLY_SUCCESS),
                    "Closing channel");
};

void Server::close_connection() {
  die_on_amqp_error(amqp_connection_close(m_conn, AMQP_REPLY_SUCCESS),
                    "Closing connection");
};

void Server::disconnect() {
  die_on_error(amqp_destroy_connection(m_conn), "Ending connection");
};

void Server::process() {
  for (;;) {
    amqp_rpc_reply_t res;
    amqp_envelope_t envelope;
    amqp_maybe_release_buffers(m_conn);

    res = amqp_consume_message(m_conn, &envelope, NULL, 0);

    if (AMQP_RESPONSE_NORMAL != res.reply_type) {
      break;
    }
    
    TestTask::Messages::Request request;
    request.ParseFromString(std::string((const char*)envelope.message.body.bytes));
    std::cout << "received from client: " << request.req() << std::endl;

    amqp_basic_properties_t reply_props;
    reply_props._flags = AMQP_BASIC_CORRELATION_ID_FLAG;
    reply_props.correlation_id = envelope.message.properties.correlation_id;
    

    TestTask::Messages::Response response;
    response.set_id("test-response");
    response.set_res(request.req()*2); /* response = request * 2*/
    std::string serialized_response;
    if (!response.SerializeToString(&serialized_response)) {
      fprintf(stderr, "Failed to serialize Protobuf response\n");
      /*send client info about protobuf serialization fail */
      //return 1;
    }
    die_on_error(amqp_basic_publish(m_conn, 1, amqp_empty_bytes,
                                    envelope.message.properties.reply_to, 0, 0,
                                    &reply_props, amqp_cstring_bytes(serialized_response.c_str())),
                 "Publishing");
    //std::cout << "successfully sent: " << response.res() << std::endl;
    amqp_destroy_envelope(&envelope);
  }
}

void Server::run() {
  connect();
  create_tcp_socket();
  open_tcp_socket();
  login();
  open_channel();
  declare_queue();
  set_queue_listener();
  process();
  close_channel();
  close_connection();
  disconnect();
}