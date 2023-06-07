#include "spawner.h"

Spawner::Spawner(glm::vec3 position, float spawnRate) {
    this->position = position;
    direction = (position.x / abs(position.x)) * -1;

    cooldown = 1.f;
    timer = cooldown;   // immediately spawn car
    this->spawnRate = spawnRate;
    if (this->spawnRate > 90.f/100.f) {
        this->spawnRate = 90.f/100.f;
    }
}

void Spawner::update(float deltaTime, std::vector<Car*>& cars) {
    if (timer > cooldown) {
        if ( (float)rand()/RAND_MAX < spawnRate ) {
            spawnCar(cars);
        }
        timer = 0.f;
    }
    timer += deltaTime;
}

void Spawner::spawnCar(std::vector<Car*>& cars) {
    int type = rand() % 2 + 1;
    cars.push_back(new Car(position, type));
}