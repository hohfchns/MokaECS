#pragma once
#include "moka/ecs/core/component_ptr.hpp"
#include <functional>

#define MOKA_SIGNAL_CONNECT_COMPONENT(signal, component_ptr, component_type, method, num_args, ...) \
signal.Connect(std::bind(component_ptr_wrapper<component_type, __VA_ARGS__>, component_ptr, &component_type::method, std::placeholders::_##num_args))

template <class T, typename ...F>
void component_ptr_wrapper(moka::ecs::ComponentPtr<T> ptr, void(T::*callback)(F...), const F& ...arg)
{
  std::invoke(callback, ptr.GetPtr(), arg...);
}




