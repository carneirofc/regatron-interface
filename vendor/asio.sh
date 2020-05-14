#!/bin/bash
set -x
set -e
wget https://github.com/chriskohlhoff/asio/archive/asio-1-16-1.tar.gz
tar -zvxf asio-1-16-1.tar.gz
cd asio-asio-1-16-1/asio/
./autogen.sh
make -j$(grep -c ^processor /proc/cpuinfo)
