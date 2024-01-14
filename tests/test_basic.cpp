#include <gtest/gtest.h>
#include "moka/ecs/core/ecs.hpp"
#include "moka/ecs/core/component.h"
#include "moka/logger/logger.h"

using namespace moka::ecs;
using namespace moka::log;

namespace moka::tests
{

using namespace misc;

// Component classes for testing
struct PositionComponent : public Component {
    float x, y;

    PositionComponent() : x(0.0f), y(0.0f) {}

    void _Init() override {
        // Initialization logic, if needed
    }

    ~PositionComponent() {
        // Cleanup logic, if needed
    }
};

struct VelocityComponent : public Component {
    float vx, vy;
    Entity positionEntity; // Store the Entity instead of directly storing PositionComponent*

    VelocityComponent() : vx(0.0f), vy(0.0f), positionEntity(MOKA_INVALID_ENTITY) {}

    void _Init() override {
        // Initialization logic, if needed
    }

    ~VelocityComponent() {
        // Cleanup logic, if needed
    }
};

struct RenderableComponent : public Component {
    std::string texture;

    RenderableComponent() : texture("default.png") {}

    void _Init() override {
        // Initialization logic, if needed
    }

    ~RenderableComponent() {
        // Cleanup logic, if needed
    }
};

struct PhysicsComponent : public Component {
    Entity velocityEntity; // Store the Entity instead of directly storing VelocityComponent*

    PhysicsComponent() : velocityEntity(MOKA_INVALID_ENTITY) {}

    void _Init() override {
        // Initialization logic, if needed
    }

    ~PhysicsComponent() {
        // Cleanup logic, if needed
    }
};

struct RenderSystem : public Component {
    Entity renderableEntity; // Store the Entity instead of directly storing RenderableComponent*

    RenderSystem() : renderableEntity(MOKA_INVALID_ENTITY) {}

    void _Init() override {
        // Initialization logic, if needed
    }

    ~RenderSystem() {
        // Cleanup logic, if needed
    }
};

// Test case for the ECS system using Singleton
TEST(ECSSystemTest, RealisticUsage) {
    // Create entities with different components
    Entity entity1 = ECS::Get().newEntity();
    ECS::Get().AddComponent<PositionComponent>(entity1);
    Entity positionEntity1 = entity1; // Store the Entity for PositionComponent

    ECS::Get().AddComponent<VelocityComponent>(entity1);
    Entity velocityEntity1 = entity1; // Store the Entity for VelocityComponent

    ECS::Get().AddComponent<PhysicsComponent>(entity1);
    Entity physicsEntity1 = entity1; // Store the Entity for PhysicsComponent

    Entity entity2 = ECS::Get().newEntity();
    ECS::Get().AddComponent<RenderableComponent>(entity2);
    Entity renderableEntity2 = entity2; // Store the Entity for RenderableComponent

    ECS::Get().AddComponent<RenderSystem>(entity2);
    Entity renderSystemEntity2 = entity2; // Store the Entity for RenderSystem

    // Simulate an update loop
    for (int frame = 0; frame < 60; ++frame) {
        // Update physics - Apply velocity to position
        auto physics1 = ECS::Get().GetComponentP<PhysicsComponent>(physicsEntity1);
        auto velocity1 = ECS::Get().GetComponentP<VelocityComponent>(velocityEntity1);
        auto position1 = ECS::Get().GetComponentP<PositionComponent>(positionEntity1);

        if (physics1 && velocity1 && position1) {
            // Simulate physics: Update position based on velocity
            position1->x += velocity1->vx;
            position1->y += velocity1->vy;
        }

        // Render the scene
        auto renderSystem2 = ECS::Get().GetComponentP<RenderSystem>(renderSystemEntity2);
        auto renderable2 = ECS::Get().GetComponentP<RenderableComponent>(renderableEntity2);

        if (renderSystem2 && renderable2) {
            // Simulate rendering: Output the entity and texture
            MOKA_LOGF_INFO("Rendering Entity %u with texture: %s", renderSystemEntity2, renderable2->texture);
        }

        // ... Add more update logic as needed
    }

    // ... rest of the test remains unchanged
}

}

