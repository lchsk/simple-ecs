///////////////////////////////////////////////////////////////////////////////
//                                 Simple ECS                                //
///////////////////////////////////////////////////////////////////////////////

// Simple ECS
// Author: Maciej Lechowski
// Version: 0.1.1
// https://github.com/lchsk/simple-ecs

#ifndef SIMPLE_ECS_HPP
#define SIMPLE_ECS_HPP

#include <algorithm>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

#define ECS_GET_TYPE_INDEX(T) std::type_index(typeid(T));

namespace ecs {

struct ComponentNotFound : public std::runtime_error {
  ComponentNotFound(const std::string &what) : std::runtime_error(what) {}
};

namespace internal {
struct BaseHolder {
  virtual ~BaseHolder() {}
};
}

template <typename Component> struct Holder : internal::BaseHolder {
  template <typename... Args>
  Holder(Args &&... args) : value(Component{args...}) {}

  Component *operator->() { return &value; }

private:
  Component value;
};

struct Entity {
  ~Entity() {
    for (auto holder : components) {
      if (holder.second) {
        delete holder.second;
      }
    }
  }

  template <typename Component> bool has() {
    const auto component_index = ECS_GET_TYPE_INDEX(Component);

    if (components.find(component_index) == components.end())
      return false;

    return true;
  }

  template <typename Component> Holder<Component> &get() {
    if (!has<Component>()) {
      const std::string name = typeid(Component).name();

      char msg[1024];
      snprintf(msg, 1024, "Component '%s' not found", name.c_str());

      throw ComponentNotFound(msg);
    }

    const auto component_index = ECS_GET_TYPE_INDEX(Component);

    internal::BaseHolder *holder = components[component_index];

    return *static_cast<Holder<Component> *>(holder);
  }

  template <typename Component> bool remove() {
    const auto component_index = ECS_GET_TYPE_INDEX(Component);
    const auto component_it = components.find(component_index);

    if (component_it == components.end())
      return false;

    internal::BaseHolder *bh = component_it->second;

    if (bh) {
      delete bh;
    }

    components.erase(component_it);

    return true;
  }

  template <typename Component, typename... Args> void add(Args &&... args) {
    Holder<Component> *holder =
        new Holder<Component>(std::forward<Args>(args)...);

    const std::type_index component_index = std::type_index(typeid(Component));

    components[component_index] = holder;
  }

  std::unordered_map<std::type_index, internal::BaseHolder *> components;
};

struct System {

  Entity *create() {
    Entity *e = new Entity;

    entities.push_back(e);

    return e;
  }

  ~System() {
    for (Entity *e : entities) {
      if (e) {
        delete e;
      }
    }
  }

  template <typename Component, typename... Args>
  void add(Entity *entity, Args &&... args) {
    entity->add<Component>(std::forward<Args>(args)...);
  }

  bool remove(Entity *entity) {
    const auto it = std::remove(entities.begin(), entities.end(), entity);

    if (it == entities.end())
      return false;

    entities.erase(it, entities.end());

    if (entity) {
      delete entity;
    }

    return true;
  }

  std::size_t size() const { return entities.size(); }

  friend struct VectorView;

private:
  std::vector<Entity *> entities;
};

struct VectorView {
  template <typename... Components>
  static VectorView create(ecs::System &system) {
    VectorView view;

    for (Entity *entity : system.entities) {
      const bool has_components = view.has_components<Components...>(entity);

      if (has_components) {
        view.m_entities.push_back(entity);
      }
    }

    return view;
  }

  const std::vector<Entity *> entities() const { return m_entities; }

  std::size_t size() const { return m_entities.size(); }

private:
  template <typename Component> bool has_components(Entity *entity) {
    return entity->has<Component>();
  }

  template <typename FirstComponent, typename SecondComponent,
            typename... OtherComponents>
  bool has_components(Entity *entity) {
    return has_components<FirstComponent>(entity) &&
           has_components<SecondComponent, OtherComponents...>(entity);
  }

  std::vector<Entity *> m_entities;
};
}

#endif /* SIMPLE_ECS_HPP */
