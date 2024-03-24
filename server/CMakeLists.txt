cmake_minimum_required(VERSION 3.15.0)
cmake_policy(SET CMP0016 NEW)

project(rabbitmq_server VERSION 1.0.0 DESCRIPTION "Работа с брокером сообщений, серверная часть")

set(CMAKE_CXX_STANDARD 17 REQUIRED)
set(CMAKE_BUILD_TYPE Debug)

find_package(Boost REQUIRED COMPONENTS program_options filesystem)
find_package(glog REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/proto-files)
include_directories(${GLOG_INCLUDE_DIRS})
add_subdirectory(proto-files)

add_executable(srv "src/main.cpp" "src/utils.cpp" "src/server.cpp")

enable_testing()
add_subdirectory(test)

target_link_libraries(srv PUBLIC
                      Message
                      ${PROTOBUF_DEPS}
                      glog::glog
                      rabbitmq
                      Boost::program_options
                      Boost::filesystem
                     )
target_link_directories(srv PRIVATE ../rabbitmq-c-0.13.0/build/librabbitmq)

