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

struct Nested {
  ecs::Entity* entity;
};

TEST(TestSimpleECS, add_and_remove_entity) {
  ecs::System system;

  ASSERT_EQ(system.size(), 0);

  auto e = system.create();

  ASSERT_EQ(system.size(), 1);

  ASSERT_TRUE(system.remove(e));
  ASSERT_EQ(system.size(), 0);

  ASSERT_FALSE(system.remove(e));
  ASSERT_EQ(system.size(), 0);
}

TEST(TestSimpleECS, check_if_entity_has_component) {
  ecs::System system;

  auto e = system.create();

  ASSERT_FALSE(e->has<Name>());

  system.add<Name>(e, "test");

  ASSERT_TRUE(e->has<Name>());
}

TEST(TestSimpleECS, add_and_get_one_component) {
  ecs::System system;

  auto e = system.create();

  system.add<Position>(e, 3, 5);

  auto &pos = e->get<Position>();

  ASSERT_EQ(pos->x, 3);
  ASSERT_EQ(pos->y, 5);

  pos->move(1, 1);

  ASSERT_EQ(pos->x, 4);
  ASSERT_EQ(pos->y, 6);
}

TEST(TestSimpleECS, add_two_components) {
  ecs::System system;

  auto e = system.create();

  system.add<Position>(e, 1, 10);
  system.add<Name>(e, "test");

  auto &pos = e->get<Position>();
  auto &name = e->get<Name>();

  ASSERT_EQ(pos->x, 1);
  ASSERT_EQ(pos->y, 10);

  ASSERT_EQ(name->name, "test");
}

TEST(TestSimpleECS, attempt_to_get_non_existing_component) {
  ecs::System system;

  auto e = system.create();

  ASSERT_THROW(e->get<Position>(), ecs::ComponentNotFound);
}

TEST(TestSimpleECS, remove_component) {
  ecs::System system;

  auto e = system.create();

  // Try to remove a component that wasn't yet added
  ASSERT_FALSE(e->remove<Position>());

  system.add<Position>(e, 1, 1);

  auto &pos = e->get<Position>();

  ASSERT_TRUE(e->remove<Position>());
  ASSERT_THROW(e->get<Position>(), ecs::ComponentNotFound);
}

TEST(TestSimpleECS, add_nested_component) {
  ecs::System system;

  auto e = system.create();

  system.add<User>(e, std::make_shared<Position>(1, 1));

  auto &user = e->get<User>();

  ASSERT_EQ(user->position->x, 1);
  ASSERT_EQ(user->position->y, 1);
}

TEST(TestSimpleECS, nested_entities) {
  ecs::System system;

  auto e1 = system.create();
  auto e2 = system.create();

  system.add<Nested>(e1, e2);
  system.add<Position>(e2, 2, 5);

  auto &nested = e1->get<Nested>();

  auto &pos1 = nested->entity->get<Position>();
  auto &pos2 = e2->get<Position>();

  ASSERT_EQ(pos1->x, pos2->x);
  ASSERT_EQ(pos1->y, pos2->y);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
