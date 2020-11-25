#!/bin/sh

BASE_IMAGE=clion/remote-cpp-env
BASE_TAG=latest

# check if base image exists
if [ -z "$(docker images -q ${BASE_IMAGE}:${BASE_TAG})" ] ;
then
  echo "Base development image ${BASE_IMAGE}:${BASE_TAG} does not exist locally!"
  echo "Building the base image first (this may take a while!)"
  if [ "$BASE_TAG" = "latest" ] ;
  then
    (cd docker-clion-cpp-env && sh build.sh -n ${BASE_IMAGE} && cd ..) || exit
  else
    (cd docker-clion-cpp-env && sh build.sh -n ${BASE_IMAGE} -v ${BASE_TAG} && cd ..) || exit
  fi
fi

docker build -t clion/remote-cpp-env/zmq-tests . --build-arg BASE="$BASE_IMAGE":"$BASE_TAG"