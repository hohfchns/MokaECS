#pragma once
#include <stdio.h>
#include <typeinfo>
#include <string>
#include <unordered_map>
#include "moka/ecs/core/defines.h"

namespace moka::ecs
{

class ECS;

template<typename>
class ComponentPtr;

class Component
{
  friend class moka::ecs::ECS;
  template <class>
  friend class ComponentPtr;
public: 
  Component() {} 

  std::string componentName;

  template<typename T>
  T& GetComponent() const;

  template<typename T>
  ComponentPtr<T> GetComponentP() const;

  Entity GetEntity() const { return _ownerEntity; }

  virtual void _Init() {}

protected:
  Entity _ownerEntity;

public:
  virtual ~Component() = default;
};

}

#include "moka/ecs/core/ecs.hpp"

