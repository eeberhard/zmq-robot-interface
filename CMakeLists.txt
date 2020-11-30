cmake_minimum_required(VERSION 3.17)
project(zmq_tests)

set(CMAKE_CXX_STANDARD 17)

find_package(cppzmq)

include_directories(BEFORE inc)

add_executable(controller src/controller.cpp)
target_link_libraries(controller cppzmq)

add_executable(robot src/robot.cpp)
target_link_libraries(robot cppzmq)
