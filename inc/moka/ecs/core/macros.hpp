#pragma once

#define _DEF_OPERATOR(component, callback) \
{ \
  auto* components = moka::ecs::ECS::Get().GetComponentsOfType<component>(); \
  if (components == nullptr) \
  { \
    return; \
  } \
\
  for (auto& c : *components) \
  { \
    c.callback(); \
  } \
}

#define _DEF_OPERATOR_ARG(component, callback, argtype, argname) \
{ \
  auto* components = moka::ecs::ECS::Get().GetComponentsOfType<component>(); \
  if (components == nullptr) \
  { \
    return; \
  } \
\
  for (auto& c : *components) \
  { \
    c.callback(*((argtype*)argname)); \
  } \
}

#define _DEF_OPERATOR_TYPED_ARG(component, callback, argtype, argname) \
{ \
  auto* components = moka::ecs::ECS::Get().GetComponentsOfType<component>(); \
  if (components == nullptr) \
  { \
    return; \
  } \
\
  for (auto& c : *components) \
  { \
    c.callback(argname); \
  } \
}

#define DEC_DEF_OPERATOR(component, callback) \
static void _OPERATOR_##callback() \
_DEF_OPERATOR(component, callback)

#define DEC_DEF_OPERATOR_ARG(component, callback, argtype, argname) \
static void _OPERATOR_##callback(void* argname) \
_DEF_OPERATOR_ARG(component, callback, argtype, argname)

#define DEC_DEF_OPERATOR_TYPED_ARG(component, callback, argtype, argname) \
static void _OPERATOR_##callback(argtype argname) \
_DEF_OPERATOR_TYPED_ARG(component, callback, argtype, argname)

#define DEC_OPERATOR(component, callback) \
static void _OPERATOR_##callback()

#define DEC_OPERATOR_ARG(component, callback) \
static void _OPERATOR_##callback(void*)

#define DEC_OPERATOR_TYPED_ARG(component, callback, argtype) \
static void _OPERATOR_##callback(argtype)

#define DEF_OPERATOR(component, callback) \
void component::_OPERATOR_##callback() \
_DEF_OPERATOR(component, callback)

#define DEF_OPERATOR_ARG(component, callback, argtype, argname) \
void component::_OPERATOR_##callback(void* argname) \
_DEF_OPERATOR_ARG(component, callback, argtype, argname)

#define DEF_OPERATOR_TYPED_ARG(component, callback, argtype, argname) \
void component::_OPERATOR_##callback(argtype argname) \
_DEF_OPERATOR_TYPED_ARG(component, callback, argtype, argname)

