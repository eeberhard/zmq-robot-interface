#include "common.h"

#include <iostream>
#include <zmq.hpp>

int main() {
  std::cout << "Starting Robot process" << std::endl;

  zmq::context_t context(1);

  // bind state publisher to a port
  zmq::socket_t pubState(context, ZMQ_PUB);
  pubState.bind("tcp://*:5563");

  // connect command publisher to a port with no queue (conflate = 1)
  zmq::socket_t subCommand(context, ZMQ_SUB);
  subCommand.set(zmq::sockopt::conflate, 1);
  subCommand.set(zmq::sockopt::subscribe, "");
  subCommand.connect("tcp://localhost:5564");

  common::StateRepresentation state{};
  common::CommandRepresentation command{};

  // fast control loop
  int count = 0;
  while (true) {
    // non-blocking poll of command
    if (common::pollCommand(subCommand, command)) {
      // Do something with the command here (e.g. update applied command)
      std::cout << "Received command " << command.jointTorque.j1 << std::endl;
    }

    // here the robot would update the state representation from hardware

    // then send the state representation
    std::cout << "Send state " + std::to_string(count) << std::endl;
    state.jointPosition.j1 = static_cast<float>(count);
    common::publishState(pubState, state);

    // robot interface has real-time timing, but for demonstration we sleep here
    common::sleep(100);
    ++count;
  }
  return 0;
}
