#pragma once
#include <stdexcept>
#include <string>
#include "moka/ecs/core/defines.h"
#include "moka/ecs/core/ecs.hpp"
#include "moka/helpers/exceptions.h"


namespace moka::ecs
{

template <class T>
class ComponentPtr {

private:
  Entity _entity;

public:
  ComponentPtr(const ComponentPtr& from)
    : _entity(from._entity)
  {
  }
  ComponentPtr()
    : _entity(MOKA_INVALID_ENTITY)
  {
  }
  ComponentPtr(Entity entity)
    : _entity(entity)
  {
  }

  const T* GetPtr() const
  {
    return __GetPtr();
  }

  T* GetPtr()
  {
    return __GetPtr();
  }

  T* __GetPtr() const
  {
    if (_entity == MOKA_INVALID_ENTITY)
    {
      throw moka::bad_dealloc("Attempted to dereference ComponentPtr with Invalid entity");
    }
    if (ECS::Get()._components.find(typeid(T).name()) == ECS::Get()._components.end())
    {
      throw moka::bad_dealloc(std::string("Attempted to derefrence a ComponentPtr of a nonexistent component type") + typeid(T).name() + " (in components vector)");
    }
    if (ECS::Get()._componentIndexes.find(typeid(T).name()) == ECS::Get()._componentIndexes.end())
    {
      throw moka::bad_dealloc(std::string("Attempted to derefrence a ComponentPtr of a nonexistent component type") + typeid(T).name() + " (in index vector)");
    }
    if (ECS::Get()._componentIndexes[typeid(T).name()].find(_entity) == ECS::Get()._componentIndexes[typeid(T).name()].end())
    {
      throw moka::bad_dealloc(std::string("Entity ") + std::to_string((size_t)_entity) + " Not found in index vector for component of type " + typeid(T).name());
    }

    std::vector<T>* compVec = (std::vector<T>*)ECS::Get()._components[typeid(T).name()];
    size_t index = ECS::Get()._componentIndexes[typeid(T).name()][_entity];

    if (index == MOKA_INVALID_COMPONENT_INDEX)
    {
      throw moka::bad_dealloc("Attempted to dereference a ComponentPtr with an invalid index");
    }

    T* comp = &(*compVec)[index];
    if (comp->_ownerEntity != _entity)
    {
      throw moka::bad_dealloc("Moka ECS critical failure! Entity ID map is wrong, or component's _ownerEntity is wrong.");
    }

    return comp;
  }

  const T& operator*() const
  {
    return *GetPtr();
  }

  T& operator*()
  {
    return *GetPtr();
  }

  const T* operator->() const
  {
    return GetPtr();
  }

  T* operator->() 
  {
    return GetPtr();
  }

  T& operator=(const T& other)
  {
    _entity = other.entity;
    return *this;
  }

  template<Entity>
  T& operator=(Entity entity)
  {
    _entity = entity;
    return *this;
  }

  operator bool() const
  {
    if (_entity == MOKA_INVALID_ENTITY)
    {
      return false;
    }

    bool valid = true;
    try {
      // volatile auto _ = *this;
      GetPtr();
    }
    catch (moka::bad_dealloc const&) {
      valid = false;
    }

    return valid;
  }
};

template<class T>
ComponentPtr<T> ECS::GetComponentP(Entity entity) const
{
  return ComponentPtr<T>(entity);
}

template<class T>
ComponentPtr<T> Component::GetComponentP() const
{
  return ECS::Get().GetComponentP<T>(_ownerEntity);
}

template<class T>
T& Component::GetComponent() const
{
  return *ECS::Get().GetComponentP<T>(_ownerEntity);
}

}
