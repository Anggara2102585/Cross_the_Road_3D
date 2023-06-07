#pragma once
#include "../config.h"
#include "player.h"
#include "road.h"

class Scene {
public:
	Scene();
	~Scene();
	void update(float deltaTime, gameState *nextAction);
	void movePlayer(glm::vec3 dPos);
	void calculateCameraPosition();
	
	static void setCameraDistance(float scrollOffset, GLFWwindow* window);
	
	// Collision detection using SAT (Separating Axis Theorem) + AABB (Axis Aligned Bounding Box)
	bool collide(Player* player, Car* car);

	Player* player;
	Road* roads;
	
	glm::vec3 cameraPos;
	float distFromPlayer;
	inline static float maxDist;
	inline static float minDist;
	int score = 0;
	
	std::vector<Car*> cars;
};