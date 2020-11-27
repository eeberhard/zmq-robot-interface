#ifndef ZMQ_TESTS__COMMON_H_
#define ZMQ_TESTS__COMMON_H_

#include <zmq.hpp>
#include <string>
#include <array>

#include <ctime>
#include <iostream>

namespace common {

// --- Underlying message data types --- //

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
  Vec3D position{};
  Quaternion orientation{};
};

struct EETwist {
  Vec3D linear{};
  Vec3D angular{};
};


// --- Message structures --- //

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


// --- Transceiving function helpers --- //

template<typename T>
bool send(zmq::socket_t& publisher, const T& obj) {
  zmq::message_t message(sizeof(obj));
  memcpy(message.data(), &obj, sizeof(obj));
  auto res = publisher.send(message, zmq::send_flags::none);

  return res.has_value();
}

template<typename T>
bool receive(zmq::socket_t& subscriber, T& obj, const zmq::recv_flags flags = zmq::recv_flags::none) {
  zmq::message_t message;
  auto res = subscriber.recv(message, flags);
  if (res) {
    obj = *message.data<T>();
  }
  return res.has_value();
}

template<typename T>
bool poll(zmq::socket_t& subscriber, T& obj) {
  return receive(subscriber, obj, zmq::recv_flags::dontwait);
}

// --- Demo utilities --- //

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
  std::cout << std::endl << "EE Pose [p, q]: ";
  std::cout << state.eePose.position.x << "  " << state.eePose.position.y << "  " << state.eePose.position.z << ",  "
            << state.eePose.orientation.w << "  " << state.eePose.orientation.x << "  "
            << state.eePose.orientation.y << "  " << state.eePose.orientation.z;
  std::cout << std::endl << "EE Twist [v, w]: ";
  std::cout << state.eeTwist.linear.x << "  " << state.eeTwist.linear.y << "  " << state.eeTwist.linear.z << ",  "
            << state.eeTwist.angular.x << "  " << state.eeTwist.angular.y << "  " << state.eeTwist.angular.z;
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
