cmake_minimum_required(VERSION 3.5)

project(cl VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Boost REQUIRED COMPONENTS program_options)
find_package(glog REQUIRED)
add_subdirectory(proto-files)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/proto-files)

set(PROJECT_SOURCES
        src/client.cpp
        src/client.h
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        src/mainwindow.ui
        src/utils.cpp
        src/settings-dialog.h
        src/settings-dialog.cpp
        src/connect-widget.cpp
        src/connect-widget.h
        src/main-widget.h
        src/main-widget.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(cl
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(cl
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(cl 
                      PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                      Message ${PROTOBUF_DEPS}
                      glog::glog
                      rabbitmq
                      Boost::program_options
)

#target_link_directories(cl PRIVATE ../rabbitmq-c-0.14.0/build/librabbitmq) 
#manual library build, archlinux error: librabbitmq.so not found (???),
#on ubuntu works fine, can be commented 


include(GNUInstallDirs)
install(TARGETS cl
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(cl)
endif()
