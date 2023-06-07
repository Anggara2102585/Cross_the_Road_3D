#pragma once
#include "../config.h"
#include "../model/scene.h"
#include "../view/engine.h"

class GameApp {
public:
	GameApp(int width, int height);
	gameState mainLoop();
	~GameApp();
private:
	GLFWwindow* makeWindow();
	void processInput();
	void calculateDeltaTime();
	
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	GLFWwindow* window;
	int width, height;

	gameState nextAction;
	Scene* scene;
	Engine* renderer;

	double lastTime;
	float deltaTime;

	bool firstMouse;
};