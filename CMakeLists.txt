cmake_minimum_required(VERSION 3.10)
project(Moka)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/helpers.cmake")

set(MOKA_ECS_DEPS MOKA_UTILS)
if(NOT SKIP_MOKA_LOGGER)
  list(APPEND MOKA_ECS_DEPS MOKA_LOGGER)
endif()
DEPENDENCIES(MOKA_ECS)

# list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

include_directories("inc/")
include_directories(${MOKA_ECS_DEP_INCS})

file(
  GLOB SOURCE_FILES 
  "${CMAKE_CURRENT_SOURCE_DIR}/src/moka/ecs/core/ecs.cpp"
)

file (
  GLOB TESTS_SOURCE_FILES
  "tests/test_basic.cpp"
  "tests/test_extreme.cpp"
  "tests/tests_main.cpp"
)

add_library(MokaECS STATIC ${SOURCE_FILES})
target_link_libraries(MokaECS ${MOKA_ECS_DEP_LIBS})
# add_link_options(-no-pie)

set_property(TARGET MokaECS PROPERTY CXX_STANDARD 20)
set_property(TARGET MokaECS PROPERTY CXX_STANDARD_REQUIRED 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function")

if(UNIT_TESTS)
  if (TESTS_SOURCE_FILES)
    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )

    if (WIN32)
      # For Windows: Prevent overriding the parent project's compiler/linker settings
      set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
      FetchContent_MakeAvailable(googletest)
    endif()

    enable_testing()

    include(GoogleTest)
    add_executable(MokaECS_UnitTests  ${TESTS_SOURCE_FILES})
    target_link_libraries(MokaECS_UnitTests gtest ${MOKA_ECS_DEP_LIBS} MokaECS)
    set_property(TARGET MokaECS_UnitTests PROPERTY CXX_STANDARD 20)
    set_property(TARGET MokaECS_UnitTests PROPERTY CXX_STANDARD_REQUIRED 20)

    gtest_discover_tests(MokaECS_UnitTests)
  endif()
endif()

set_property(GLOBAL PROPERTY MOKA_ECS 1)
set_property(GLOBAL PROPERTY MOKA_ECS_INCS "${CMAKE_CURRENT_SOURCE_DIR}/inc" ${MOKA_ECS_DEP_INCS})
set_property(GLOBAL PROPERTY MOKA_ECS_LIBS MokaECS)
