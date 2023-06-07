#pragma once
#include "../config.h"

class Car {
public:
    glm::vec3 position;
    glm::vec3 euler;
    int type;
    // Hitbox
	float width, length;
	std::vector<glm::vec2> hbVertices;
    
    Car(
        glm::vec3 position,
        int type = 1,
        float speed = 10.f
    );
    void update(float deltaTime);
    void updateHitBox();

private:
    int direction;  // either 1 (to +x) or -1 (to -x)
    float speed;
};