#include <gtest/gtest.h>
#include "moka/event/signal.hpp"
#include "moka/ecs/wrappers/signal_wrapper.hpp"

class A : public moka::ecs::Component
{
friend class moka::ecs::ECS;
public:
  void init(int arg) {
    std::cout << "TEST " << arg << std::endl;
  }
};

TEST(ECSSystemTest, Signals) {
  moka::event::Signal<int> event_test;

  moka::ecs::Entity a_ent = moka::ecs::ECS::Get().newEntity();
  moka::ecs::ECS::Get().AddComponent<A>(a_ent);

  MOKA_SIGNAL_CONNECT_COMPONENT(event_test, a_ent, A, init, 1, int);

  event_test.Emit(5);
}
