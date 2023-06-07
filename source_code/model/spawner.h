#pragma once
#include "../config.h"
#include "car.h"

class Spawner {
public:
    float spawnRate;    // Probability of spawning car

    Spawner(glm::vec3 position, float spawnRate);

    void update(float deltaTime, std::vector<Car*>& cars);
    void spawnCar(std::vector<Car*>& cars);

private:
    glm::vec3 position;
    int direction; // either 1 or -1
    float timer;
    float cooldown;
};