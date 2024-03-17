#include "client.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem.hpp>

namespace po = boost::program_options;
namespace pt = boost::property_tree;
namespace fs = boost::filesystem;


void Client::set_hostname(std::string hostname) {
  m_hostname = hostname;
};

void Client::set_port(int port) {
  m_port = port;
};

void Client::load_cfg(po::variables_map& vm) {
  pt::ptree tree;
  pt::ini_parser::read_ini(fs::absolute(vm["config"].as<std::string>()).string(), tree); 

  set_hostname(tree.get<std::string>("client.host"));
  set_port(tree.get<int>("client.port"));
}
void Client::publish_request(int num) {
  uuid_t uuid;
  char uuid_str[37];  
  TestTask::Messages::Request request;
  request.set_id("test-request");
  std::cout << "num to send:" << num << std::endl; 
  request.set_req(num);
  std::string serialized_request;
  if (!request.SerializeToString(&serialized_request)) {
      throw std::runtime_error("Failed to serialize Protobuf request\n");
  }

  amqp_basic_properties_t props;
    props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG |
                   AMQP_BASIC_DELIVERY_MODE_FLAG | AMQP_BASIC_REPLY_TO_FLAG |
                   AMQP_BASIC_CORRELATION_ID_FLAG;
    props.content_type = amqp_cstring_bytes("text/plain");
    props.delivery_mode = 2; /* persistent delivery mode */
    props.reply_to = amqp_bytes_malloc_dup(m_reply_to_queue);
    if (props.reply_to.bytes == NULL) {
      throw std::runtime_error("Out of memory while copying queue name");
    
    }
    
    uuid_generate(uuid);
    uuid_unparse(uuid, uuid_str);
    props.correlation_id = amqp_cstring_bytes(uuid_str);
  
  die_on_error(amqp_basic_publish(m_conn, 1, amqp_empty_bytes,
                                  amqp_cstring_bytes("rpc_queue"), 0, 0,
                                  &props, amqp_cstring_bytes(serialized_request.c_str())),
                "Publishing");

  amqp_bytes_free(props.reply_to);
}

void  Client::set_consumer() {
  amqp_basic_consume(m_conn, 1, m_reply_to_queue, amqp_empty_bytes, 0, 1, 0,
                       amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(m_conn), "Consuming");
    amqp_bytes_free(m_reply_to_queue);
}

void Client::process_response() {
  for (;;) {
    amqp_rpc_reply_t res;
    amqp_envelope_t envelope;
    amqp_maybe_release_buffers(m_conn);

    res = amqp_consume_message(m_conn, &envelope, NULL, 0);

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


void Client::connect() {
  m_conn = amqp_new_connection() /*returns NULL or 0*/;
  if (!m_conn) {
    throw std::runtime_error("Connection creation failed");
  }
}


void Client::create_tcp_socket() {
  m_socket = amqp_tcp_socket_new(m_conn);
  if (!m_socket) {
    die("creating TCP socket");
  }
}

void Client::open_tcp_socket() {
  m_status = amqp_socket_open(m_socket, m_hostname.c_str(), m_port);
  if (m_status) {
    die("opening TCP socket");
  }
}

void Client::login() {
  die_on_amqp_error(amqp_login(m_conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN,
                               "guest", "guest"),
                    "Logging in");
}

void Client::open_channel() {
  amqp_channel_open(m_conn, 1);
  die_on_amqp_error(amqp_get_rpc_reply(m_conn), "Opening channel");
}

void Client::create_reply_queue() {
  amqp_queue_declare_ok_t *r = amqp_queue_declare(
        m_conn, 1, amqp_empty_bytes, 0, 0, 0, 1, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(m_conn), "Declaring queue");
    m_reply_to_queue = amqp_bytes_malloc_dup(r->queue);
    if (m_reply_to_queue.bytes == NULL) {
      throw std::runtime_error("Out of memory while copying queue name");
    }
}

void Client::close_channel() {
  die_on_amqp_error(amqp_channel_close(m_conn, 1, AMQP_REPLY_SUCCESS),
                    "Closing channel");
}

void Client::close_connection() {
  die_on_amqp_error(amqp_connection_close(m_conn, AMQP_REPLY_SUCCESS),
                    "Closing connection");
}

void Client::disconnect() {
  die_on_error(amqp_destroy_connection(m_conn), "Ending connection");
}