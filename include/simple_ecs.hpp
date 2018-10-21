// TODO:
// * nested entities
// * container around component returned by get()
// * update function
// * create() should return a ptr?
// * view example
// * remove
// * game example

#include <iostream>

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <type_traits>
#include <typeindex>

namespace ecs {

struct BaseHolder {
  virtual ~BaseHolder() {}
};

template <typename T> struct Holder : BaseHolder {
  template <typename... Args> Holder(Args &&... args) : value(T{args...}) {}

  T &get() { return value; }

  T value;
};

struct entity {
  ~entity() {
    for (auto s : h) {
      delete s.second;
    }
  }

  template <typename T> T &get() {
    std::type_index key = std::type_index(typeid(T));

    BaseHolder *holder = NULL;

    try {
      holder = h[key];
    } catch (std::out_of_range &) {
    }

    Holder<T> *val = static_cast<Holder<T> *>(holder);
    auto &v = val->value;

    return v;
  }

  template <typename Component, typename... Args> void assign(Args &&... args) {
    Holder<Component> *holder =
        new Holder<Component>(std::forward<Args>(args)...);

    std::type_index key = std::type_index(typeid(Component));

    h[key] = holder;
  }

  std::unordered_map<std::type_index, BaseHolder *> h;
};

struct registry {
  std::unique_ptr<entity> create() { return std::make_unique<entity>(); }

  template <typename Component, typename... Args>
  void assign(std::unique_ptr<entity> &e, Args &&... args) {
    e->assign<Component>(std::forward<Args>(args)...);
  }
};
}
