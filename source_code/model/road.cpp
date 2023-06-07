#include "road.h"

Road::Road(std::vector<Car*>* cars, int *score) {
    segmentWidth = 35.8981f;
    originOffset = segmentWidth*4 + 2.f;
    segmentAmount = 9;
    height = 0;         // road height

    activeWallX = 40.f;

    laneAmount = 2;
    spawnerOffsetX = 78.f;
    spawnerOffsetZ = -15.3722f;
    spawnerSpacing = -5.8626f;

    carSpawnRate = 20.f/100.f;
    carSpawnRateDelta = 10.f/100.f;  // slowly increase

    for (int i=0; i<segmentAmount; i++) {
        // Spawn road
        float segmentZ = (-i*segmentWidth)+originOffset;std::cout <<segmentZ << std::endl;
        roadsPos.push_back(glm::vec3(0.f, height, segmentZ));

        // Car spawner
        spawnSpawner(segmentZ);
    }

    this->cars = cars;
    this->score = score;
}

Road::~Road() {
    roadsPos.clear();

    for (auto i : spawners) {
        delete i;
    }
    spawners.clear();
}

void Road::update(float playerPosZ, float deltaTime) {
    /* if the player step to the next segment of the road */
    float backRoadZ = roadsPos.front().z;
    if ( (backRoadZ - originOffset - segmentWidth) >= playerPosZ) {
        shiftRoad();std::cout << "shift" << std::endl;
        *score += 1;
    }
    
    // Update spawner
    for (auto i : spawners) {
        i->update(deltaTime, *cars);
    }

    // Update backWall
    activeWallZ = backRoadZ - originOffset;
}

void Road::shiftRoad() {
    // Delete back road
    roadsPos.pop_front();
    
    // Delete back spawner
    for (int i=0; i<(2*laneAmount); i++) {
        Spawner* delSpawner = spawners.front();
        delete delSpawner;
        spawners.pop_front();
    }

    // Increase carSpawnRate
    carSpawnRate += carSpawnRateDelta;

    // Spawn front road
    float frontRoadZ = roadsPos.back().z;
    roadsPos.push_back( glm::vec3(0.f, height, frontRoadZ-segmentWidth) );
    
    // Spawn front spawner
    spawnSpawner(frontRoadZ - segmentWidth);
}

void Road::spawnSpawner(float segmentZ) {
    int j = 0;
    for(j=j; j<laneAmount; j++) {       // lane from right
        spawners.push_back(new Spawner( glm::vec3(
            spawnerOffsetX,
            height,
            spawnerOffsetZ+(j*spawnerSpacing) + segmentZ
        ), carSpawnRate ));
    }
    for(j=j; j<2*laneAmount; j++) {       // lane from left
        spawners.push_back(new Spawner( glm::vec3(
            -spawnerOffsetX,
            height,
            spawnerOffsetZ+(j*spawnerSpacing) + segmentZ
        ), carSpawnRate ));
    }
}