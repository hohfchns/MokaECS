#include <gtest/gtest.h>
#include <random>
#include "moka/ecs/core/component.h"
#include "moka/ecs/core/ecs.hpp"
#include "moka/logger/logger.h"
#include "moka/helpers/exceptions.h"

using namespace moka::ecs;
using namespace moka::log;

namespace moka::tests
{

using namespace misc;

// Function to generate a random float within a given range
float generateRandomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

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
TEST(ECSSystemTest, StressTest) {
    moka::log::Logger::RegisterLogger("ecs");
     
    const int numEntities = 1000; // Adjust the number of entities based on your system's capabilities

    // Create a large number of entities with random combinations of components
    for (int i = 0; i < numEntities; ++i) {
        Entity entity = ECS::Get().newEntity();

        // Randomly decide which components to add
        if (generateRandomFloat(0.0f, 1.0f) < 0.8f) {
            ECS::Get().AddComponent<PositionComponent>(entity);
        }

        if (generateRandomFloat(0.0f, 1.0f) < 0.6f) {
            ECS::Get().AddComponent<VelocityComponent>(entity);
        }

        if (generateRandomFloat(0.0f, 1.0f) < 0.4f) {
            ECS::Get().AddComponent<PhysicsComponent>(entity);
        }

        if (generateRandomFloat(0.0f, 1.0f) < 0.7f) {
            ECS::Get().AddComponent<RenderableComponent>(entity);
        }

        if (generateRandomFloat(0.0f, 1.0f) < 0.5f) {
            ECS::Get().AddComponent<RenderSystem>(entity);
        }
    }

    // Simulate an update loop for a random number of frames
    const int numFrames = generateRandomFloat(50, 200);
    for (int frame = 0; frame < numFrames; ++frame) {
        // Iterate through the entities with PositionComponent
        auto* positionComponents = ECS::Get().GetComponentsOfType<PositionComponent>();
        for (auto& position: *positionComponents) {
            Entity entity = position.GetEntity();
            // Simulate various component interactions based on your system's logic
            // For simplicity, we'll just print the entity ID and some random values

            MOKA_LOGF_INFO("ecs", "Frame %d, Entity %u:", frame, entity);

            try
            {
                auto velocity = ECS::Get().GetComponentP<VelocityComponent>(entity);

                if (velocity) {
                    MOKA_LOGF_INFO("ecs", "Velocity(%f, %f)", velocity->vx, velocity->vy);
                }
            }
            catch (const moka::bad_dealloc e)
            {
                FAIL();
                return;
            }
        }

        // ... Add more update logic as needed
    }

    // ... rest of the test remains unchanged
}

}
