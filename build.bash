#!/bin/bash

sudo apt-get install --no-install-recommends -y libboost-dev cxxtest cmake gcc g++ # gcc > 4.8 required

if [ `uname -m` == 'x86_64' ] # If cross compiling from 64 bit linux  
then
	sudo apt-get install --no-install-recommends -y gcc-multilib g++-multilib libc6-dev-i386 libc6-i386  
fi

export CC=gcc
export CXX=g++

rm -rf build
mkdir build
cd build
cmake ../
make -j$((2 * `getconf _NPROCESSORS_ONLN`)) # Multicore build
