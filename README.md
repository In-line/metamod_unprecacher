## Metamod Unprecacher  [![Build Status](https://travis-ci.org/In-line/metamod_unprecacher.svg?branch=master)](https://travis-ci.org/In-line/metamod_unprecacher)

Blocks precache & resource usage

## Compilation
cmake, boost, gcc, g++ packages may be required.
<pre>
sudo apt-get install libboost-dev cxxtest cmake gcc g++ # gcc > 4.8 required
sudo apt-get install gcc-multilib g++-multilib libc6-dev-i386 libc6-i386 # If cross compiling from 64 bit linux  
sed -i '/#define C_DLLEXPORT/c\//#define C_DLLEXPORT' third_party/metamod-hl1/metamod/osdep.h # Patch osdep.h
cmake .
make -j$((2 * `getconf _NPROCESSORS_ONLN`)) # Multicore build
</pre>

## Installation

1. Extract files from archive to cstrike/addons/
2. In  addons/metamod/plugins.ini write
`linux addons/unprecacher/libunprecacher.so`
3. Restart server.

## Configuring
In list.ini write file paths that need to be unprecached and blocked.
`path_to_file.extension flags replace_path`
After file name flags "a","b","c" are supported.
a - delete entity
b - not delete players (working if "a" was set)
c - replace file

Map prefixes also supported.
In maps/ directory create file `map_name.ini` or `prefix-my_map_prefix.ini`.

