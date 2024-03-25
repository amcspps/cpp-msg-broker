#include "../proto-files/message.pb.h"
#include "../src/server.hpp"
#include "../../qt-client-gui/src/client.h"
#include <gtest/gtest.h>
#include <filesystem>
#include <thread>

namespace fs = std::filesystem;

TEST(Server, LoadCfg) {
    Server& server = Server::get_instance();
    EXPECT_EQ(server.get_hostname(), "localhost");
    EXPECT_EQ(server.get_port(), 5672);
    EXPECT_EQ(server.get_log_dir(), fs::absolute("../log").string());
    EXPECT_EQ(server.get_log_lvl(), "INFO");
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

TEST(Server, RPCProcess) {
    Server& server = Server::get_instance();
    server.set_hostname("localhost");
    server.set_port(5672);
    std::thread server_thread([&server](){server.run();});

    Client& client = Client::get_instance();
    client.set_hostname("localhost");
    client.set_port(5672);
    client.connect();
    client.create_tcp_socket();
    client.open_tcp_socket();
    client.login();
    client.open_channel();
    
    std::thread client_thread([&client](){
        for (int i = 0; i < 1000; i++) {
        client.create_reply_queue();
        client.publish_request(i);
        client.set_consumer();
        auto response = client.process_response();
        ASSERT_EQ(std::get<0>(response), true);
        ASSERT_EQ(std::stoi(std::get<1>(response)), i*2);
    }
    });

    server_thread.join();
    client_thread.join();

    client.close_channel();
    client.close_connection();
    client.disconnect();

}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    fs::path currentPath = fs::current_path();
    std::cout << currentPath.string() <<std::endl;
    po::options_description desc("Allowed options");
    desc.add_options()
      ("help,h", "produce help message")
      ("config,c", po::value<std::string>()->default_value("../../src/for-test-cfg.ini"), "configuration file");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
      std::cout << desc << std::endl;
      return 1;
    }
    Server& server = Server::get_instance();
    Client& client = Client::get_instance();
    server.load_cfg(vm);
    return RUN_ALL_TESTS();
}