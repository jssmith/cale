# C++ Actor Log Engine

CALE is an experiment aimed at creating a lightweight log-oriented
persistence engine for use with [C++ Actor Framework (CAF)][caf] and
CAF streams.

# Installation 

Building CALE involves the following steps:

    ./configure
    make
    make test
    make install

Required dependencies:

- A C++ compiler
  - GCC (>= 6)
  - Clang (>= 3.5)
- [CMake](http://www.cmake.org)
- [CAF][caf] (develop branch)

[caf]: https://github.com/actor-framework/actor-framework
