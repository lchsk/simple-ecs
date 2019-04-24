# simple-ecs

A simple header-only implementation of entity-component system

## Installation

Just add `include/simple_ecs.hpp` to your source code.

It's a part of SFML gamedev libraries: https://github.com/lchsk/knight-libs

## Example

```C++
#include "simple_ecs.hpp"

struct Position {
  int x;
  int y;

  Position(int x, int y) : x(x), y(y) {}
};

ecs::System system;

auto entity = system.create();

system.add<Position>(e, 3, 5);

auto &pos = entity->get<Position>();

std::cout << pos->x << std::endl; // prints 3

```

## Building tests

Make sure that you have [CMake](http://www.cmake.org/) and a C++ compiler installed.

You also need `pkg-config` and `googletest`.

Then open a terminal, go to the source directory and type the following commands:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

## Running tests

`$ ctest`

## License

MIT
