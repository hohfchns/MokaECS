#pragma once
#include <memory>
#include <unordered_set>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>
#include <typeinfo>
#include "moka/singleton/singleton.hpp"
#include "moka/ecs/core/component.h"
#include "moka/ecs/core/defines.h"
#include "moka/event/signal.hpp"

namespace moka::ecs
{

class System;
class bad_dealloc;

class ECS
{
friend class misc::Singleton<ECS>;
template <class>
friend class ComponentPtr;
private:
  ECS() {}

  ~ECS()
  {
    for (auto itr : _components)
    {
      free(itr.second);
    }
  }

  Entity _numEntities = 0;

protected:
  std::unordered_map<const char*, void*> _components = std::unordered_map<const char*, void*>();
  std::unordered_map<const char*, std::unordered_map<Entity, size_t>> _componentIndexes = std::unordered_map<const char*, std::unordered_map<Entity, size_t>>();

public:
  static ECS& Get()
  {
    return moka::misc::Singleton<moka::ecs::ECS>::Ref();
  }

  Entity newEntity()
  {
    return ++_numEntities;
  }

  template<class T>
  bool AddComponent(Entity entity)
  {
    ComponentPtr<T> existingPtr = GetComponentP<T>(entity);
    if (existingPtr)
    {
      throw std::runtime_error("Tried to add duplicate component");
    }

    if (_components.find(typeid(T).name()) == _components.end())
    {
      _components[typeid(T).name()] = (void*)(new std::vector<T>);
      _componentIndexes[typeid(T).name()] = std::unordered_map<Entity, size_t>();
    }

    std::vector<T>* castPtr = (std::vector<T>*)(_components[typeid(T).name()]);
    T t = T();
    (*castPtr).push_back(t);

    T* component = &( *(castPtr->end() - 1) );
    component->_ownerEntity = entity;
    _componentIndexes[typeid(T).name()][entity] = castPtr->size() - 1;
    component->_Init();

    return true;
  }

  template<class T>
  ComponentPtr<T> GetComponentP(Entity entity) const;

  template<class T>
  std::vector<T>* GetComponentsOfType()
  {
    if (_components.find(typeid(T).name()) == _components.end())
    {
      return nullptr;
    }

    return (std::vector<T>*)(_components[typeid(T).name()]);
  }
};
}

#include "moka/ecs/core/component_ptr.hpp"

