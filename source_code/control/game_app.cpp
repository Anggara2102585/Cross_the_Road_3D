#include "game_app.h"

GameApp::GameApp(int width, int height) {
	this->width = width;
	this->height = height;

	lastTime = glfwGetTime();
	deltaTime = 0;

	firstMouse = true;

	window = makeWindow();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetFramebufferSizeCallback(window, GameApp::framebuffer_size_callback);
	glfwSetWindowUserPointer(window, (void *)&scene);
	glfwSetScrollCallback(window, GameApp::scroll_callback);

	nextAction = gameState::CONTINUE;
	renderer = new Engine(&width, &height);
	scene = new Scene();
}

GLFWwindow* GameApp::makeWindow() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Cross the Road 3D", NULL, NULL);

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "GLAD initialization failed\n";
		return NULL;
	}

	glViewport(0, 0, width, height);

	return window;
}

void GameApp::processInput() {

	if (nextAction == gameState::CONTINUE) {
	// KEYBOARD / player position
		int forward = 0;
		int strafe = 0;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			forward += 1;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			strafe -= 1;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			forward -= 1;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			strafe += 1;
		}

		// Move player
		if (forward != 0 || strafe != 0) {
			scene->movePlayer(
				15.f * deltaTime * glm::normalize( glm::vec3{forward, 0.0f, strafe} )
			);
		}

		// Jump
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			scene->player->jump();
		}

	// MOUSE / camera position
		double mouse_x, mouse_y;
		glfwGetCursorPos(window, &mouse_x, &mouse_y);
		glfwSetCursorPos(window, static_cast<double>(width / 2), static_cast<double>(height / 2));

		if (firstMouse) {
			mouse_x = width / 2;
			mouse_y = height / 2;
			firstMouse = false;
		}

		float delta_x{ static_cast<float>(mouse_x - static_cast<double>(width / 2)) };
		float delta_y{ static_cast<float>(mouse_y - static_cast<double>(height / 2)) };

		// Move camera (based on player orientation)
		scene->player->spin(
			0.01f /* * deltaTime */ * glm::vec3{
				-delta_y, delta_x, 0.0f
			}
		);
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		nextAction = gameState::QUIT;
	}
	if ( (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) && (nextAction == gameState::PAUSE) ) {
		delete scene;
		scene = new Scene();
		nextAction = gameState::CONTINUE;
	}
}

gameState GameApp::mainLoop() {

	processInput();
	glfwPollEvents();

	//update
	if (nextAction == gameState::CONTINUE) {
		scene->update(deltaTime, &nextAction);
	}

	//draw
	renderer->render(scene, nextAction);
	glfwSwapBuffers(window);

	calculateDeltaTime();

	return nextAction;
}

GameApp::~GameApp() {
	//free memory
	delete scene;
	delete renderer;
	glfwTerminate();
}

void GameApp::calculateDeltaTime() {
	double currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;
}

void GameApp::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void GameApp::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	Scene::setCameraDistance((float)yoffset, window);
}