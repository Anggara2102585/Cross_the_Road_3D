#include "scene.h"

Scene::Scene() {

	PlayerCreateInfo playerInfo;
	playerInfo.eulers = { -30.0f, -90.0f,0.0f };
	playerInfo.position = { 0.0f, 0.0f, -0.1f };
	player = new Player(&playerInfo);
	
	roads = new Road(&cars, &score);

	distFromPlayer = 10.f;
	maxDist = 20.f;
	minDist = 5.f;
	calculateCameraPosition();

}

Scene::~Scene() {
	delete player;
	delete roads;
	for (auto i : cars) {
		delete i;
	}
	cars.clear();
}

void Scene::update(float deltaTime, gameState *nextAction) {
	player->update(deltaTime);
	roads->update(player->position.z, deltaTime);

	// Cars
	for (auto i = cars.begin(); i != cars.end(); i++) {
		Car* car = *i;
		car->update(deltaTime);
		// If the car passed the spawnpoint bound
		if (abs(car->position.x) > (roads->spawnerOffsetX)) {
			delete car;
			cars.erase(i);
			i--;
		}

		// Collision detection (player and car) (if the car is still exists)
		else if (*nextAction == gameState::CONTINUE) {
			if (collide(player, car)) {
				// delete car;
				// cars.erase(i);
				i--;
				*nextAction = gameState::PAUSE;
				std::cout << "You've been HIT!" << std::endl;
				std::cout << "Press ENTER to restart." << std::endl;
			}
		}
	}

	calculateCameraPosition();
}

void Scene::movePlayer(glm::vec3 dPos) {
	// rotate dPos based on player Yaw angle
	glm::mat4 dirRotation = glm::mat4(1.f);
	dirRotation = glm::rotate(dirRotation, glm::radians(-player->eulers.y), glm::vec3(0.f, 1.f, 0.f));
	dPos = glm::vec3(dirRotation * glm::vec4(dPos, 1.0));
	
	player->position += dPos;
	if (player->position.z > roads->activeWallZ) {
		player->position.z = roads->activeWallZ;
	}
	if (abs(player->position.x) > roads->activeWallX) {
		if (player->position.x > 0) {
			player->position.x = roads->activeWallX;
		} else {
			player->position.x = roads->activeWallX * -1;
		}
	}
}

void Scene::calculateCameraPosition() {
	float horizontalDist, verticalDist;
	horizontalDist = (float) (distFromPlayer * cos(glm::radians(player->eulers.x)));
	verticalDist = (float) (distFromPlayer * sin(glm::radians(player->eulers.x)));

	float offsetX, offsetZ;
	offsetX = (float) (horizontalDist * cos(glm::radians(player->eulers.y)));
	offsetZ = (float) (horizontalDist * sin(glm::radians(player->eulers.y)));

	cameraPos.x = player->position.x - offsetX;
	cameraPos.z = player->position.z - offsetZ;
	cameraPos.y = std::max(player->position.y - verticalDist, 0.1f);

	if (cameraPos.x < -roads->activeWallX) {
		cameraPos.x = -roads->activeWallX;
	} else if (cameraPos.x > roads->activeWallX) {
		cameraPos.x = roads->activeWallX;
	}
}

void Scene::setCameraDistance(float scrollOffset, GLFWwindow* window) {
	Scene* tmpScene;
    tmpScene = (Scene *)glfwGetWindowUserPointer(window);
	tmpScene->distFromPlayer -= scrollOffset * 0.001f;
	if (tmpScene->distFromPlayer > maxDist)
		tmpScene->distFromPlayer = maxDist;
	if (tmpScene->distFromPlayer < minDist)
		tmpScene->distFromPlayer = minDist;
}

bool Scene::collide(Player* player, Car* car) {
	std::vector<glm::vec2> vert1 = player->hbVertices;
	std::vector<glm::vec2> vert2 = car->hbVertices;

	for (int i=0; i<2; i++) {
		if (i == 1) {
			vert1 = car->hbVertices;
			vert2 = player->hbVertices;
		}

		for (int a=0; a<vert1.size(); a++) {
			int b = (a+1) % vert1.size();
			// Get the normal vector of edge a-b from shape vert1
			glm::vec2 axisProj = { glm::vec2(-(vert1[b].y - vert1[a].y), vert1[b].x - vert1[a].x) };
			float d = sqrt(axisProj.x * axisProj.x + axisProj.y * axisProj.y);
			axisProj = { axisProj.x / d, axisProj.y / d };

			// Work out min and max 1D points for shape vert1
			float min_s1 = INFINITY, max_s1 = -INFINITY;
			for (int p=0; p<vert1.size(); p++) {
				float q = (vert1[p].x * axisProj.x + vert1[p].y * axisProj.y);
				min_s1 = std::min(min_s1, q);
				max_s1 = std::max(max_s1, q);
			}

			// Work out min and max 1D points for shape vert2
			float min_s2 = INFINITY, max_s2 = -INFINITY;
			for (int p=0; p<vert2.size(); p++) {
				float q = (vert2[p].x * axisProj.x + vert2[p].y * axisProj.y);
				min_s2 = std::min(min_s2, q);
				max_s2 = std::max(max_s2, q);
			}

			// AABB (Axis Aligned Bounding Box) checking
			// If we found the separating axis (no collision)
			if (!(max_s2 >= min_s1 && max_s1 >= min_s2))
				return false;
		}
	}
	
	// if there is no separating axis
	return true;
}