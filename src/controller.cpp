#include "common.h"

#include <iostream>
#include <zmq.hpp>

// define the robot degrees of freedom for the message types
const std::size_t dof = 3;

int main() {
  std::cout << "Starting controller process" << std::endl;

  zmq::context_t context(1);

  zmq::socket_t subState(context, ZMQ_SUB);
  subState.set(zmq::sockopt::conflate, 1);
  subState.set(zmq::sockopt::subscribe, "");
  subState.connect("tcp://localhost:5563");

  zmq::socket_t pubCommand(context, ZMQ_PUB);
  pubCommand.bind("tcp://*:5564");

  common::StateMessage<dof> state{};
  common::CommandMessage<dof> command{};

  while (subState.connected()) {
    // blocking receive until we get a state from the robot
    if (common::receiveState(subState, state)) {
      common::printState(state);

      // then we generate a command and send it on another port
      command.jointTorque[0] = state.jointPosition[0];
      common::publishCommand(pubCommand, command);
    }

    // for demonstration the control loop sleeps to be much
    // slower than the robot, to show it drops old messages
    common::sleep(1000);
  }
  return 0;
}
