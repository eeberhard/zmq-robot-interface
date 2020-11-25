# ZMQ for robot control interface

The robot (representing fast, realtime hardware layer)
reads some state variables (joint and end-effector)
and sends them out. Then it polls (non-blocking) a subscription
of a command variable, which it can update and apply if there
is a new value. It runs at a high frequency.

The controller (representing a slower, non-realtime software layer)
waits to receive a state message from the robot. Then, it calculates
some command message and sends it back before looping back. It 
takes a while to calculate the command so there is no guarantee
on timing - the robot cannot wait.

The controller should always be receiving the most up-to-date robot state,
and the robot should always be receiving the most up-to-date command. But,
the two processes are running asynchronously at different rates.

With ZMQ publisher and subscriber using no buffering, we 
can achieve the necessary interface.

## Setup

Use `sh build.sh` followed by `sh run.sh` to start a docker
container with the build and debug environment. Then
configure CLion accordingly.

Refer to [docker-clion-cpp-env](github.com/eeberhard/docker-clion-cpp-env) for more info.

## Running

Launch the robot and controller processes in parallel. 
(If using CLion, edit the Run / Debug configuration to "Allow parallel run" for both executables).

You should see the robot sending states at a high frequency, and the controller receiving
states at a low frequency. The controller drops old states, and replies with a command
that the robot receives.
