#pragma once
#include "moka/ecs/core/ecs.hpp"
#include "moka/ecs/core/defines.h"

namespace moka::ecs
{

class System
{
public: 
  System() 
  { 
    _entity = ECS::Get().newEntity(); 
  } 

  System(int parentSystem) 
  { 
    _entity = parentSystem; 
  } 

  System(System& parentSystem) 
  { 
    _entity = parentSystem.GetEntity(); 
  }

  template<typename T>
  T& GetComponent()
  {
    auto get = ECS::Get().GetComponentP<T>(_entity);
    if (!get)
    {
      throw std::invalid_argument("ERROR::ECS! Tried to GetComponent that does not exist!\n Consider checking with HasComponent, or using ECS::Get().GetComponent directly.");
    }

    return *get;
  }

  template<typename T>
  bool HasComponent() const
  {
    auto get = ECS::Get().GetComponentP<T>(_entity);
    if (get)
    {
      return true;
    }

    return false;
  }

  template<typename T>
  const T& GetComponent() const
  {
    auto get = ECS::Get().GetComponentP<T>(_entity);
    if (!get)
    {
      throw std::invalid_argument("ERROR::ECS! Tried to GetComponent that does not exist!\n Consider checking with HasComponent, or using ECS::Get().GetComponent directly.");
    }

    return *get;
  }

  const Entity GetEntity() const
  {
    return _entity;
  }

protected:

  int _entity;

  template<typename T>
  void AddComponent()
  {
    ECS::Get().AddComponent<T>(_entity);
  }
};

}
