#include "car.h"

// Car::Car(glm::vec3 position, Direction direction, float speed) {
Car::Car(glm::vec3 position, int type, float speed) {
    this->position = position;
    this->speed = speed;
	this->type = type;
    direction = (position.x / abs(position.x)) * -1;    // if it's on the positive side, direction going to the left and vice versa
    euler = (direction == 1) ? 
        glm::vec3(0.f, 180.f, 0.f) : glm::vec3(0.f, 0.f, 0.f)
    ;

    for (int i=0; i<4; i++) {
		hbVertices.push_back(glm::vec2(0.f, 0.f));
	}
	width = 2.f;
	length = 6.7f;
	updateHitBox();
}

void Car::update(float deltaTime) {
    position.x = position.x + (direction*speed*deltaTime);

    updateHitBox();
}

void Car::updateHitBox() {
	// Local space
	hbVertices[0] = glm::vec2( (length/2),  (width/2));
	hbVertices[1] = glm::vec2(-(length/2),  (width/2));
	hbVertices[2] = glm::vec2(-(length/2), -(width/2));
	hbVertices[3] = glm::vec2( (length/2), -(width/2));
	
	// Rotate within local space
	for (auto& vertex : hbVertices) {
		float x = glm::cos(glm::radians(0.f))*vertex.x - glm::sin(glm::radians(0.f))*vertex.y;
		float z = glm::cos(glm::radians(0.f))*vertex.y + glm::sin(glm::radians(0.f))*vertex.x;
		vertex = glm::vec2(x, z);
	}

	// translate to player position
	hbVertices[0] = glm::vec2(hbVertices[0].x + position.x, hbVertices[0].y + position.z);
	hbVertices[1] = glm::vec2(hbVertices[1].x + position.x, hbVertices[1].y + position.z);
	hbVertices[2] = glm::vec2(hbVertices[2].x + position.x, hbVertices[2].y + position.z);
	hbVertices[3] = glm::vec2(hbVertices[3].x + position.x, hbVertices[3].y + position.z);
}