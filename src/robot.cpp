#include "common.h"

#include <iostream>
#include <zmq.hpp>

// define the robot degrees of freedom for the message types
const std::size_t dof = 3;

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

  common::StateMessage<dof> state = {};
  common::CommandMessage<dof> command = {};

  // fast control loop
  int count = 0;
  while (pubState.connected()) {
    // non-blocking poll of command
    if (common::pollCommand(subCommand, command)) {
      // Do something with the command here (e.g. update applied command)
      common::printCommand(command);
    }

    // here the robot would update the state representation from hardware
    state.jointPosition[0] = static_cast<float>(count);

    // then send the state representation
    std::cout << "Send state " + std::to_string(count) << std::endl;
    common::publishState(pubState, state);

    // robot interface has real-time timing, but for demonstration we sleep here
    common::sleep(100);
    ++count;
  }
  return 0;
}
