cmake_minimum_required(VERSION 3.17)
project(zmq_tests)

set(CMAKE_CXX_STANDARD 17)

find_package(cppzmq)

add_executable(controller controller.cpp common.h)
target_link_libraries(controller cppzmq)

add_executable(robot robot.cpp common.h)
target_link_libraries(robot cppzmq)
