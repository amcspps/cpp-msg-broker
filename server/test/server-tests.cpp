#include "../proto-files/message.pb.h"
#include "../src/server.hpp"
#include <gtest/gtest.h>
#include "../../qt-client-gui/src/client.h"
#include "boost/filesystem.hpp"


TEST(Server, LoadCfg) {
    Server& server = Server::get_instance();
    EXPECT_EQ(server.get_hostname(), "localhost");
    EXPECT_EQ(server.get_port(), 5672);
    EXPECT_EQ(server.get_queuename(), "rpc_queue");
}

TEST(Server, Connect) {
    Server& server = Server::get_instance();
    EXPECT_NO_THROW(server.connect());
    server.disconnect();
}

TEST(Server, CreateTCPSocket) {
    Server& server = Server::get_instance();
    EXPECT_NO_THROW(server.create_tcp_socket());
}

TEST(Server, OpenTCPSocket_ValidCredentials) {
    Server& server = Server::get_instance();
    server.set_hostname("localhost");
    server.set_port(5672);
    server.connect();
    server.create_tcp_socket();
    EXPECT_NO_THROW(server.open_tcp_socket());
    server.disconnect();
}

TEST(Server, OpenTCPSocket_InvalidHost) {
    Server& server = Server::get_instance();
    server.set_hostname("qwerty");
    server.set_port(5672);
    server.connect();
    server.create_tcp_socket();
    EXPECT_THROW(server.open_tcp_socket(), std::runtime_error);
    server.disconnect();
}

TEST(Server, OpenTCPSocket_InvalidPort) {
    Server& server = Server::get_instance();
    server.set_hostname("localhost");
    server.set_port(6666);
    server.connect();
    server.create_tcp_socket();
    EXPECT_THROW(server.open_tcp_socket(), std::runtime_error);
    server.disconnect();
}

TEST(Server, OpenTCPSocket_InvalidCredentials) {
    Server& server = Server::get_instance();
    server.set_hostname("qwerty");
    server.set_port(6666);
    server.connect();
    server.create_tcp_socket();
    EXPECT_THROW(server.open_tcp_socket(), std::runtime_error);
    server.disconnect();
    
}


TEST(Server, Login) {
    Server& server = Server::get_instance();
    server.set_hostname("localhost");
    server.set_port(5672);
    server.connect();
    server.create_tcp_socket();
    server.open_tcp_socket();
    EXPECT_NO_THROW(server.login());
    server.close_connection();
    server.disconnect();
}

TEST(Server, OpenChannel) {
    Server& server = Server::get_instance();
    server.set_hostname("localhost");
    server.set_port(5672);
    server.connect();
    server.create_tcp_socket();
    server.open_tcp_socket();
    server.login();
    EXPECT_NO_THROW(server.open_channel());
    server.close_channel();
    server.close_connection();
    server.disconnect();
}


TEST(Server, OpenChannel_NoLogin) {
    Server& server = Server::get_instance();
    server.set_hostname("localhost");
    server.set_port(5672);
    server.connect();
    server.create_tcp_socket();
    server.open_tcp_socket();
    EXPECT_THROW(server.open_channel(), std::runtime_error);
    server.disconnect();
}


TEST(Server, DeclareQueue) {
    Server& server = Server::get_instance();
    server.set_hostname("localhost");
    server.set_port(5672);
    server.connect();
    server.create_tcp_socket();
    server.open_tcp_socket();
    server.login();
    server.open_channel();
    EXPECT_NO_THROW(server.declare_queue());
    server.close_channel();
    server.close_connection();
    server.disconnect();
}

TEST(Server, SetQueueListener) {
    Server& server = Server::get_instance();
    server.set_hostname("localhost");
    server.set_port(5672);
    server.connect();
    server.create_tcp_socket();
    server.open_tcp_socket();
    server.login();
    server.open_channel();
    server.declare_queue();
    EXPECT_NO_THROW(server.set_queue_listener());
    server.close_channel();
    server.close_connection();
    server.disconnect();
}

TEST(Server, CloseChannel) {
    Server& server = Server::get_instance();
    server.set_hostname("localhost");
    server.set_port(5672);
    server.connect();
    server.create_tcp_socket();
    server.open_tcp_socket();
    server.login();
    server.open_channel();
    EXPECT_NO_THROW(server.close_channel());
    server.close_connection();
    server.disconnect();
}

TEST(Server, CloseNotExistingChannel) {
    Server& server = Server::get_instance();
    server.set_hostname("localhost");
    server.set_port(5672);
    server.connect();
    server.create_tcp_socket();
    server.open_tcp_socket();
    server.login();
    EXPECT_THROW(server.close_channel(), std::runtime_error);
    server.close_connection();
    server.disconnect();
}

TEST(Server, CloseConnection) {
    Server& server = Server::get_instance();
    server.set_hostname("localhost");
    server.set_port(5672);
    server.connect();
    server.create_tcp_socket();
    server.open_tcp_socket();
    server.login();
    EXPECT_NO_THROW(server.close_connection());
    server.disconnect();
}


TEST(Server, CloseUnauthorizedConnection) {
    Server& server = Server::get_instance();
    server.set_hostname("localhost");
    server.set_port(5672);
    server.connect();
    server.create_tcp_socket();
    server.open_tcp_socket();
    EXPECT_THROW(server.close_connection(), std::runtime_error);
    server.disconnect();
}
TEST(Server, Disconnect) {
    Server& server = Server::get_instance();
    server.connect();
    EXPECT_NO_THROW(server.disconnect());
}

TEST(Server, Disconnect_NonExistingConn) {
    Server& server = Server::get_instance();
    EXPECT_DEATH(server.disconnect(), ""); 
}

TEST(Server, xxx) {
    
}


// TEST(Server, xxx) {
    
// }


// TEST(Server, xxx) {
    
// }



int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    boost::filesystem::path currentPath = boost::filesystem::current_path();
    std::cout << currentPath.string() <<std::endl;
    po::options_description desc("Allowed options");
    desc.add_options()
      ("help,h", "produce help message")
      ("config,c", po::value<std::string>()->default_value("../../src/cfg.ini"), "configuration file");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
      std::cout << desc << std::endl;
      return 1;
    }
    Server& server = Server::get_instance();
    server.load_cfg(vm);
    return RUN_ALL_TESTS();
}