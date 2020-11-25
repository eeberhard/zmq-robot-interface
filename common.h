#ifndef ZMQ_TESTS__COMMON_H_
#define ZMQ_TESTS__COMMON_H_

#include <zmq.hpp>
#include <ctime>
#include <string>
#include <vector>
#include <iostream>

namespace common {

// assuming zmq message needs static allocation,
// we can't use vectors or Eigen types that have dynamic size

struct JointState {
  float j1;
  float j2;
  float j3;
  float j4;
  float j5;
  float j6;
  float j7;
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

struct StateRepresentation {
  JointState jointPosition;
  JointState jointVelocity;
  JointState jointTorque;
  EEPose eePose;
  EETwist eeTwist;
};

struct CommandRepresentation {
  JointState jointTorque;
};


bool publishState(zmq::socket_t& publisher, const StateRepresentation& state) {
  zmq::message_t message(sizeof(StateRepresentation));
  memcpy(message.data(), &state, sizeof(StateRepresentation));
  auto res = publisher.send(message, zmq::send_flags::none);

  return res.has_value();
}

bool receiveState(zmq::socket_t& subscriber, StateRepresentation& state) {
  zmq::message_t message;
  auto res = subscriber.recv(message, zmq::recv_flags::none);
  if (res) {
    state = *message.data<StateRepresentation>();
  }
  return res.has_value();
}

bool publishCommand(zmq::socket_t& publisher, const CommandRepresentation& command) {
  zmq::message_t message(sizeof(CommandRepresentation));
  memcpy(message.data(), &command, sizeof(CommandRepresentation));
  auto res = publisher.send(message, zmq::send_flags::none);

  return res.has_value();
}

bool pollCommand(zmq::socket_t& subscriber, CommandRepresentation& command) {
  zmq::message_t message;
  auto res = subscriber.recv(message, zmq::recv_flags::dontwait);
  if (res) {
    command = *message.data<CommandRepresentation>();
  }
  return res.has_value();
}

void sleep(int msecs) {
  struct timespec t{};
  t.tv_sec = msecs / 1000;
  t.tv_nsec = (msecs % 1000) * 1000000;
  nanosleep(&t, nullptr);
}

}

#endif //ZMQ_TESTS__COMMON_H_
