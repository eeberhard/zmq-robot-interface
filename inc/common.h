#ifndef ZMQ_TESTS__COMMON_H_
#define ZMQ_TESTS__COMMON_H_

#include <zmq.hpp>
#include <string>
#include <array>

#include <ctime>
#include <iostream>

namespace common {

template<std::size_t DOF>
struct Joints {
  float& operator[](std::size_t i) {
    return data[i % DOF];
  }
  std::array<float, DOF> data;
};

struct Vec3D {
  float x;
  float y;
  float z;
};

struct Quaternion {
  float w;
  float x;
  float y;
  float z;
};

struct EEPose {
  Vec3D position;
  Quaternion orientation;
};

struct EETwist {
  Vec3D linear;
  Vec3D angular;
};

template<std::size_t DOF>
struct StateMessage {
  Joints<DOF> jointPosition;
  Joints<DOF> jointVelocity;
  Joints<DOF> jointTorque;
  EEPose eePose;
  EETwist eeTwist;
};

template<std::size_t DOF>
struct CommandMessage {
  Joints<DOF> jointTorque;
};

template<std::size_t DOF>
bool publishState(zmq::socket_t& publisher, const StateMessage<DOF>& state) {
  zmq::message_t message(sizeof(state));
  memcpy(message.data(), &state, sizeof(state));
  auto res = publisher.send(message, zmq::send_flags::none);

  return res.has_value();
}

template<std::size_t DOF>
bool receiveState(zmq::socket_t& subscriber, StateMessage<DOF>& state) {
  zmq::message_t message;
  auto res = subscriber.recv(message, zmq::recv_flags::none);
  if (res) {
    state = *message.data<StateMessage<DOF>>();
  }
  return res.has_value();
}

template<std::size_t DOF>
bool publishCommand(zmq::socket_t& publisher, const CommandMessage<DOF>& command) {
  zmq::message_t message(sizeof(command));
  memcpy(message.data(), &command, sizeof(command));
  auto res = publisher.send(message, zmq::send_flags::none);

  return res.has_value();
}

template<std::size_t DOF>
bool pollCommand(zmq::socket_t& subscriber, CommandMessage<DOF>& command) {
  zmq::message_t message;
  auto res = subscriber.recv(message, zmq::recv_flags::dontwait);
  if (res) {
    command = *message.data<CommandMessage<DOF>>();
  }
  return res.has_value();
}

template<std::size_t DOF>
void printState(StateMessage<DOF> state) {
  std::cout << "Joint positions: ";
  for (std::size_t joint = 0; joint < DOF; ++joint) {
    std::cout << state.jointPosition[joint] << "  ";
  }
  std::cout << std::endl << "Joint velocities: ";
  for (std::size_t joint = 0; joint < DOF; ++joint) {
    std::cout << state.jointVelocity[joint] << "  ";
  }
  std::cout << std::endl << "Joint torques: ";
  for (std::size_t joint = 0; joint < DOF; ++joint) {
    std::cout << state.jointTorque[joint] << "  ";
  }
  std::cout << std::endl;
}


template<std::size_t DOF>
void printCommand(CommandMessage<DOF> command) {
  std::cout << "Command torques: ";
  for (std::size_t joint = 0; joint < DOF; ++joint) {
    std::cout << command.jointTorque[joint] << "  ";
  }
  std::cout << std::endl;
}

void sleep(int msecs) {
  struct timespec t{};
  t.tv_sec = msecs / 1000;
  t.tv_nsec = (msecs % 1000) * 1000000;
  nanosleep(&t, nullptr);
}

}

#endif //ZMQ_TESTS__COMMON_H_
