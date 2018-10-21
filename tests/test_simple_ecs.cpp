#include <gtest/gtest.h>

#include "../include/simple_ecs.hpp"

struct position {
  int x;
  int y;
};

struct name {
  std::string name;
};

TEST(TestSimpleECS, simple_ecs) {
  ecs::registry registry;

  auto entity = registry.create();

  registry.assign<position>(entity, 3, 5);
  registry.assign<name>(entity, "Test");

  auto &pos = entity->get<position>();
  auto &name_ = entity->get<name>();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
