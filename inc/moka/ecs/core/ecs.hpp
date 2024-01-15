#pragma once
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <typeinfo>
#include "moka/singleton/singleton.hpp"
#include "moka/ecs/core/component.h"
#include "moka/ecs/core/defines.h"

#ifndef MOKA_ECS_NO_LOG
#include "moka/logger/logger.h"
#define _MOKA_ECS_LOGGING_ENABLED
#define _MOKA_ECS_LOGGING_LEVEL INFO
#ifndef MOKA_ECS_NO_LOG_FILE
#include "moka/project/project.h"
#define _MOKA_ECS_LOGGING_FILE_ENABLED
#endif
#endif

#ifdef _MOKA_ECS_LOGGING_ENABLED
#define _MOKA_ECS_LOG(level, str) \
  ECS::Get().GetLogger().Log(moka::log::LogLevel::level, str, __LINE__, __FILE__)
#define _MOKA_ECS_LOGF(level, format, ...) \
  ECS::Get().GetLogger().LogFormat(moka::log::LogLevel::level, format, __LINE__, __FILE__, __VA_ARGS__)
#else
#define _MOKA_ECS_LOG(level, str)
#define _MOKA_ECS_LOGF(level, format, ...)
#endif

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
  ECS()
  {
#ifdef _MOKA_ECS_LOGGING_FILE_ENABLED
  moka::log::LoggerConfig loggerConfig{moka::project::exeDir() + "/ECS.log"};
  loggerConfig.level = moka::log::LogLevel::_MOKA_ECS_LOGGING_LEVEL;
  _logger.SetConfig(loggerConfig);
#endif
  }

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

#ifdef _MOKA_ECS_LOGGING_ENABLED
private:
  moka::log::Logger _logger;
#endif

public:
  static ECS& Get()
  {
    return moka::misc::Singleton<moka::ecs::ECS>::Ref();
  }

#ifdef _MOKA_ECS_LOGGING_ENABLED
  const moka::log::Logger& GetLogger()
  {
    return _logger;
  }
#endif

  Entity newEntity()
  {
    _MOKA_ECS_LOGF(DEBUG, "Adding new entity %u", _numEntities + 1);
    return ++_numEntities;
  }

  template<class T>
  bool AddComponent(Entity entity)
  {
    _MOKA_ECS_LOGF(DEBUG, "Adding Component of typeid %s to entity %u", typeid(T).name(), entity);
    ComponentPtr<T> existingPtr = GetComponentP<T>(entity);
    if (existingPtr)
    {
      std::string msg = std::string("Failed to add component of typeid") + typeid(T).name() + "to entity" + std::to_string(entity) + "as it already has one";
      _MOKA_ECS_LOG(ERROR, msg);
      throw std::runtime_error(msg);
    }

    if (_components.find(typeid(T).name()) == _components.end())
    {
      _components[typeid(T).name()] = (void*)(new std::vector<T>);
      _componentIndexes[typeid(T).name()] = std::unordered_map<Entity, size_t>();
      _MOKA_ECS_LOGF(DEBUG, "Created Component Vector of typeid %s", typeid(T).name());
    }

    std::vector<T>* castPtr = (std::vector<T>*)(_components[typeid(T).name()]);
    T t = T();
    (*castPtr).push_back(t);

    T* component = &( *(castPtr->end() - 1) );
    component->_ownerEntity = entity;
    _componentIndexes[typeid(T).name()][entity] = castPtr->size() - 1;
    _MOKA_ECS_LOGF(DEBUG, "Added Component of typeid %s to entity %u with index %u", typeid(T).name(), entity, castPtr->size() - 1);
    _MOKA_ECS_LOG(DEBUG, "Calling Component's _Init() function.");
    component->_Init();

    return true;
  }

  // Defined in component_ptr.hpp;
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

