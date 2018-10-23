// TODO:
// * nested entities
// * update function
// * view example
// * remove
// * game example

#include <memory>
#include <typeindex>
#include <unordered_map>

#define ECS_GET_TYPE_INDEX(T) std::type_index(typeid(T));

namespace ecs {

  struct ComponentNotFound : public std::runtime_error {
    ComponentNotFound(const std::string& what) : std::runtime_error(what) {}
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

struct entity {
  ~entity() {
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
  std::unique_ptr<entity> create_unique() { return std::make_unique<entity>(); }
  std::shared_ptr<entity> create_shared() { return std::make_shared<entity>(); }

  template <typename Component, typename... Args>
  void add(entity &entity, Args &&... args) {
    entity.add<Component>(std::forward<Args>(args)...);
  }
};
}
