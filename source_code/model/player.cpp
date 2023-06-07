#include "player.h"

Player::Player(PlayerCreateInfo* createInfo) {
	position = createInfo->position;
	eulers = createInfo->eulers;
	
	for (int i=0; i<4; i++) {
		hbVertices.push_back(glm::vec2(0.f, 0.f));
	}
	width = 1.45745f;
	length = 0.45f;
	updateHitBox();
	
	jump_power = 5.f;
	gravity = -10.f;
	upward_speed = 0.f;
	airborne = false;
}

void Player::update(float deltaTime) {
	// Orientation
	forwards = {
			glm::cos(glm::radians(eulers.y)) * glm::cos(glm::radians(eulers.x)),
			glm::sin(glm::radians(eulers.x)),
			glm::sin(glm::radians(eulers.y)) * glm::cos(glm::radians(eulers.x))
	};
	glm::vec3 globalUp{ 0.0f, 1.0f, 0.0f };
	right = glm::cross(forwards, globalUp);
	up = glm::cross(right,forwards);

	// Hitbox
	updateHitBox();

	// Jump mvt
	if (airborne) {
		upward_speed += gravity * deltaTime;
		position.y += upward_speed * deltaTime;
	}
	// Landing
	if (position.y < 0) {
		upward_speed = 0;
		position.y = 0;
		airborne = false;
	}
}

void Player::spin(glm::vec3 dEulers) {
	eulers += dEulers;

	if (eulers.y < 0) {
		eulers.y += 360;
	}
	else if (eulers.y > 360) {
		eulers.y -= 360;
	}

	eulers.x = std::max(std::min(eulers.x, 89.0f), -89.0f);
}

void Player::jump() {
	if (!airborne) {
		airborne = true;
		upward_speed = jump_power;
	}
}

void Player::updateHitBox() {
	// Local space
	hbVertices[0] = glm::vec2( (length/2),  (width/2));
	hbVertices[1] = glm::vec2(-(length/2),  (width/2));
	hbVertices[2] = glm::vec2(-(length/2), -(width/2));
	hbVertices[3] = glm::vec2( (length/2), -(width/2));
	
	// Rotate within local space
	for (auto& vertex : hbVertices) {
		float x = glm::cos(glm::radians(eulers.y))*vertex.x - glm::sin(glm::radians(eulers.y))*vertex.y;
		float z = glm::cos(glm::radians(eulers.y))*vertex.y + glm::sin(glm::radians(eulers.y))*vertex.x;
		vertex = glm::vec2(x, z);
	}

	// translate to player position
	hbVertices[0] = glm::vec2(hbVertices[0].x + position.x, hbVertices[0].y + position.z);
	hbVertices[1] = glm::vec2(hbVertices[1].x + position.x, hbVertices[1].y + position.z);
	hbVertices[2] = glm::vec2(hbVertices[2].x + position.x, hbVertices[2].y + position.z);
	hbVertices[3] = glm::vec2(hbVertices[3].x + position.x, hbVertices[3].y + position.z);
}