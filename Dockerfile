ARG BASE=clion/remote-cpp-env:latest
FROM $BASE

# get base dependencies
RUN sudo apt-get install -y libtool pkg-config autoconf automake

# get libsodium
RUN wget https://download.libsodium.org/libsodium/releases/libsodium-1.0.18.tar.gz \
  && tar -xzf libsodium-1.0.18.tar.gz \
  && cd libsodium-1.0.18 \
  && ./autogen.sh \
  && ./configure \
  && make check \
  && sudo make install \
  && sudo ldconfig \
  && cd .. \
  && rm -rf libsodium*

# get base libzmq
RUN wget https://github.com/zeromq/libzmq/releases/download/v4.3.3/zeromq-4.3.3.tar.gz \
  && tar -xzf zeromq-4.3.3.tar.gz \
  && cd zeromq-4.3.3 \
  && mkdir build \
  && cd build \
  && cmake .. -DWITH_PERF_TOOL=OFF -DZMQ_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release \
  && sudo make -j4 install \
  && cd ../.. \
  && rm -rf zeromq*

# get cppzmq bindings
RUN wget https://github.com/zeromq/cppzmq/archive/v4.7.1.tar.gz -O cppzmq-4.7.1.tar.gz \
  && tar -xzf cppzmq-4.7.1.tar.gz \
  && cd cppzmq-4.7.1 \
  && mkdir build \
  && cd build \
  && cmake .. -DCPPZMQ_BUILD_TESTS=OFF \
  && sudo make -j4 install \
  && cd ../.. \
  && rm -rf cppzmq*
