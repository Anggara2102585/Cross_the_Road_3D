#include "engine.h"

Engine::Engine(int *width, int *height) {

	shader = new Shader("shaders/textured.vert", "shaders/textured.frag");
	shader_text = new Shader("shaders/text.vert", "shaders/text.frag");
	shader_skybox = new Shader("shaders/skybox.vert", "shaders/skybox.frag");

	text_renderer = new Text("fonts/arcade.ttf");

	shader->setVec3("skyColor", glm::vec3(0.5f, 0.5f, 0.5f));

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // greenish
	
	// OpenGL state
    // ------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	this->width = width;
	this->height = height;

	createModels();
	createMaterials();
}

Engine::~Engine() {
	glDeleteProgram(shader->ID);
	delete shader;
	glDeleteProgram(shader_text->ID);
	delete shader_text;
	glDeleteProgram(shader_skybox->ID);
	delete shader_skybox;

	delete playerMaterial;
	delete roadMaterial;
	delete carMaterial1;
	delete carMaterial2;

	delete playerMesh;
	delete roadMesh;
	delete carMesh1;
	delete carMesh2;

	delete skyboxMaterial;
}

void Engine::createModels() {
	glm::mat4 preTransform = glm::mat4(1.f);
	playerMesh = new ObjMesh("obj/steve.obj", preTransform);

	preTransform = glm::mat4(1.f);
	roadMesh = new ObjMesh("obj/road.obj", preTransform);
	
	preTransform = glm::mat4(1.f);
	carMesh1 = new ObjMesh("obj/sedan.obj", preTransform);
	preTransform = glm::mat4(1.f);
	carMesh2 = new ObjMesh("obj/hatchback.obj", preTransform);
}

void Engine::createMaterials() {
	playerMaterial = new Material("textures/Steve.jpg");
	
	roadMaterial = new Material("textures/road.png");
	carMaterial1 = new Material("textures/sedan_clr.png");
	carMaterial2 = new Material("textures/hatchback_clr.png");

	skyboxMaterial = new Skybox("textures/skybox");
}



void Engine::render(Scene* scene, gameState state) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader->use();
	shader->setVec3("skyColor", glm::vec3(0.5f, 0.5f, 0.5f));
	
// Projection transform
	float aspectRatio = (float)*width / float(*height);
	glm::mat4 projection_transform = glm::perspective(45.0f, aspectRatio, 0.1f, 150.0f);
	shader->setMat4("projection", projection_transform);
// View transform
	glm::mat4 view_transform{
		glm::lookAt(
			scene->cameraPos,
			scene->player->position,
			glm::vec3(0, 1, 0)
		)
	};
	shader->setMat4("view", view_transform);

// LIGHT
	glm::vec3 lightPos = glm::vec3(0.f, 30.f, scene->player->position.z+20);
	shader->setVec3("lightPos", lightPos);
	shader->setVec3("viewPos", scene->cameraPos);


// PLAYER
	playerMaterial->use();
	shader->setInt("basicTexture", playerMaterial->ID);
	
	glm::mat4 model_transform = glm::mat4(1.0f);
	model_transform = glm::translate(model_transform, scene->player->position);
	model_transform = glm::rotate(
		model_transform, 
		glm::radians(-scene->player->eulers.y), 
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
	// model_transform = glm::scale(model_transform, glm::vec3(0.5));
	shader->setMat4("model", model_transform);

	glBindVertexArray(playerMesh->VAO);
	glDrawArrays(GL_TRIANGLES, 0, playerMesh->vertexCount);


// ROADS!!
	roadMaterial->use();
	shader->setInt("basicTexture", roadMaterial->ID);
	
	std::deque<glm::vec3> roadsPos = scene->roads->roadsPos;
	for (std::deque<glm::vec3>::iterator i = roadsPos.begin(); i != roadsPos.end(); ++i) {
		glm::vec3 roadPos = *i;
		model_transform = glm::mat4(1.0f);
		model_transform = glm::translate(model_transform, roadPos);
		shader->setMat4("model", model_transform);

		glBindVertexArray(roadMesh->VAO);
		glDrawArrays(GL_TRIANGLES, 0, roadMesh->vertexCount);
	}


// CARS
	for (auto car : scene->cars) {
		model_transform = glm::mat4(1.0f);
		model_transform = glm::translate(model_transform, car->position);
		model_transform = model_transform * glm::eulerAngleXYZ(
			car->euler.x, glm::radians(car->euler.y), car->euler.z
		);
		shader->setMat4("model", model_transform);

		switch (car->type) {
			case 1:
				carMaterial1->use();
				shader->setInt("basicTexture", carMaterial1->ID);
				glBindVertexArray(carMesh1->VAO);
				glDrawArrays(GL_TRIANGLES, 0, carMesh1->vertexCount);
				break;
			case 2:
				carMaterial2->use();
				shader->setInt("basicTexture", carMaterial2->ID);
				glBindVertexArray(carMesh2->VAO);
				glDrawArrays(GL_TRIANGLES, 0, carMesh2->vertexCount);
				break;
		}
	}


// SKYBOX
// Projection transform
	aspectRatio = (float)*width / float(*height);
	projection_transform = glm::perspective(45.0f, aspectRatio, 0.1f, 150.0f);
// View transform
	view_transform = glm::lookAt(
		scene->cameraPos,
		scene->player->position,
		glm::vec3(0, 1, 0)
	);
	skyboxMaterial->drawSkybox(shader_skybox, view_transform, projection_transform);


// TEXT!!
	shader_text->use();
	projection_transform = glm::ortho(0.0f, (float)*width, 0.0f, (float)*height);
	shader_text->setMat4("projection", projection_transform);

	if (state == gameState::CONTINUE) {
		std::string string = "Score: ";
		string.append(std::to_string(scene->score));
		float scale = 0.8f;
		TextSize size = text_renderer->calculateSize(string, scale);
		float xpos = 15.f;
		float ypos = *height - size.height - 25.f;
		text_renderer->renderText(*shader_text, string, xpos, ypos, scale, glm::vec3(0.89f, 0.34f, 0.101f));
	} 
	else if (state == gameState::PAUSE) {	
		std::string string = "You've been HIT!";
		float scale = 0.8f;
		TextSize size = text_renderer->calculateSize(string, scale);
		float xpos = *width/2 - size.length/2;
		float ypos = *height/2 + size.height*2;
		text_renderer->renderText(*shader_text, string, xpos, ypos, scale, glm::vec3(0.98f, 0.019f, 0.278f));

		string = "Your score: ";
		string.append(std::to_string(scene->score));
		scale = 0.6f;
		size = text_renderer->calculateSize(string, scale);
		xpos = *width/2 - size.length/2;
		ypos = *height/2 - size.height/2;
		text_renderer->renderText(*shader_text, string, xpos, ypos, scale, glm::vec3(0.09f, 0.325f, 0.909f));

		string = "Press ENTER to restart";
		scale = 0.8f;
		size = text_renderer->calculateSize(string, scale);
		xpos = *width/2 - size.length/2;
		ypos = *height/2 - size.height*2;
		text_renderer->renderText(*shader_text, string, xpos, ypos, scale, glm::vec3(0.09f, 0.325f, 0.909f));
	}
}
