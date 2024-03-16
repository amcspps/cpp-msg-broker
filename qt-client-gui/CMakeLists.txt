cmake_minimum_required(VERSION 3.5)

project(myclient VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Boost REQUIRED COMPONENTS program_options filesystem)
add_subdirectory(proto-files)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/proto-files)

#enable_testing()
#add_subdirectory(test)

set(PROJECT_SOURCES
        src/client.cpp
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        src/mainwindow.ui
        src/utils.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(myclient
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(myclient
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(myclient 
                      PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                      Message ${PROTOBUF_DEPS}
                      uuid
                      rabbitmq
                      Boost::program_options 
                      Boost::filesystem
                      )

target_link_directories(myclient PRIVATE ../rabbitmq-c-0.13.0/build/librabbitmq) # trouble with arch, manual library build


include(GNUInstallDirs)
install(TARGETS myclient
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(myclient)
endif()
