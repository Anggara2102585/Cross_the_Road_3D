#pragma once
#include "../config.h"

struct PlayerCreateInfo {
	glm::vec3 position, eulers;
};

class Player {
public:
	glm::vec3 position, eulers, up, forwards, right;
	// Hitbox
	float width, length;
	std::vector<glm::vec2> hbVertices;
	// Jump
	float jump_power;
	float gravity;
	float upward_speed;
	bool airborne;

	Player(PlayerCreateInfo* createInfo);
	void update(float deltaTime);
	void spin(glm::vec3 dEulers);
	void jump();
	void updateHitBox();
};