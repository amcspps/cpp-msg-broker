#include <iostream>
#include "../proto-files/message.pb.h"

auto main(int argc, char ** argv) -> int {
    TestTask::Messages::Request req;
    TestTask::Messages::Response res;
    
    std::cout << "hello from client" << std::endl;
    return 0;
}