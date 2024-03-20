cmake_minimum_required(VERSION 3.15.0)

find_package(GTest REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options filesystem)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(
  server_tests
  server-tests.cpp
  ../src/server.cpp
  ../src/utils.cpp

)

target_link_libraries(
  server_tests
  GTest::gtest GTest::gtest_main
  Boost::program_options
  Boost::filesystem
  rabbitmq
  Message
)
target_link_directories(server_tests PRIVATE ../../rabbitmq-c-0.13.0/build/librabbitmq)

add_test(NAME Server-Tests COMMAND server_tests)