#pragma once
#include "../config.h"

#include "spawner.h"

class Road {
public:
    std::deque<glm::vec3> roadsPos;
    float spawnerOffsetX;
    float activeWallZ;
    float activeWallX;
    
    Road(std::vector<Car*>* cars, int *score);
    ~Road();

    void update(float playerPosZ, float deltaTime);

private:
    void shiftRoad();
    void spawnSpawner(float segmentZ);

    float segmentWidth;
    int segmentAmount;
    float originOffset;
    float height;

    int laneAmount; // for one direction
    float spawnerOffsetZ;
    float spawnerSpacing;
    std::deque<Spawner*> spawners;
    float carSpawnRate;
    float carSpawnRateDelta;

    std::vector<Car*>* cars;

    int *score;
};