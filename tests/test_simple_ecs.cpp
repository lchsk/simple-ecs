#include <gtest/gtest.h>

#include "../include/simple_ecs.hpp"

struct Position {
  int x;
  int y;

  void move(int dx, int dy) {
    x += dx;
    y += dy;
  }
};

struct Name {
  std::string name;
};

TEST(TestSimpleECS, check_if_entity_has_component) {
  ecs::System system;

  std::unique_ptr<ecs::entity> e = system.create_unique();

  ASSERT_FALSE(e->has<Name>());

  system.add<Name>(*e, "test");

  ASSERT_TRUE(e->has<Name>());
}

TEST(TestSimpleECS, add_and_get_one_component) {
  ecs::System system;

  std::unique_ptr<ecs::entity> e = system.create_unique();

  system.add<Position>(*e, 3, 5);

  auto &pos = e->get<Position>();

  ASSERT_EQ(pos->x, 3);
  ASSERT_EQ(pos->y, 5);

  pos->move(1, 1);

  ASSERT_EQ(pos->x, 4);
  ASSERT_EQ(pos->y, 6);
}

TEST(TestSimpleECS, add_two_components) {
  ecs::System system;

  std::shared_ptr<ecs::entity> e = system.create_unique();

  system.add<Position>(*e, 1, 10);
  system.add<Name>(*e, "test");

  auto &pos = e->get<Position>();
  auto &name = e->get<Name>();

  ASSERT_EQ(pos->x, 1);
  ASSERT_EQ(pos->y, 10);

  ASSERT_EQ(name->name, "test");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
