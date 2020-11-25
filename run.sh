#!/bin/bash

NAME="clion_zmq_env"
PORT=2222

(cd docker-clion-cpp-env && sh ./run.sh -p "$PORT" -n "$NAME" -i clion/remote-cpp-env/zmq-tests && cd ..) || exit
