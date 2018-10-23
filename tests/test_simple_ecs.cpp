#include <gtest/gtest.h>

#include "../include/simple_ecs.hpp"

struct Position {
  int x;
  int y;

  Position(int x, int y) : x(x), y(y) {}

  void move(int dx, int dy) {
    x += dx;
    y += dy;
  }
};

struct Name {
  std::string name;
};

struct User {
  std::shared_ptr<Position> position;
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

  std::shared_ptr<ecs::entity> e = system.create_shared();

  system.add<Position>(*e, 1, 10);
  system.add<Name>(*e, "test");

  auto &pos = e->get<Position>();
  auto &name = e->get<Name>();

  ASSERT_EQ(pos->x, 1);
  ASSERT_EQ(pos->y, 10);

  ASSERT_EQ(name->name, "test");
}

TEST(TestSimpleECS, attempt_to_get_non_existing_component) {
  ecs::System system;

  std::shared_ptr<ecs::entity> e = system.create_shared();

  ASSERT_THROW(e->get<Position>(), ecs::ComponentNotFound);
}

TEST(TestSimpleECS, remove_component) {
  ecs::System system;

  std::shared_ptr<ecs::entity> e = system.create_shared();

  // Try to remove a component that wasn't yet added
  ASSERT_FALSE(e->remove<Position>());

  system.add<Position>(*e, 1, 1);

  auto &pos = e->get<Position>();

  ASSERT_TRUE(e->remove<Position>());
  ASSERT_THROW(e->get<Position>(), ecs::ComponentNotFound);
}

TEST(TestSimpleECS, add_nested_component) {
  ecs::System system;

  std::shared_ptr<ecs::entity> e = system.create_shared();

  system.add<User>(*e, std::make_shared<Position>(1, 1));

  auto &user = e->get<User>();

  ASSERT_EQ(user->position->x, 1);
  ASSERT_EQ(user->position->y, 1);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
