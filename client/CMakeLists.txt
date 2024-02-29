cmake_minimum_required(VERSION 3.15.0)
cmake_policy(SET CMP0016 NEW)

project(rabbitmq_client VERSION 1.0.0 DESCRIPTION "Работа с брокером сообщений, клиентская часть")

set(CMAKE_BUILD_TYPE Debug)


include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/proto-files)

add_subdirectory(proto-files)

add_executable(cl "src/main.cpp" "src/utils.cpp")

#enable_testing()
#add_subdirectory(test)

target_link_libraries(cl Message ${PROTOBUF_DEPS})
target_link_libraries(cl uuid)
target_link_directories(cl PRIVATE ../rabbitmq-c-0.13.0/build/librabbitmq)
target_link_libraries(cl rabbitmq)

