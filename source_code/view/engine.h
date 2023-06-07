#pragma once
#include "../config.h"
#include "../model/scene.h"
#include "shader.h"
#include "obj_mesh.h"
#include "material.h"
#include "text.h"
#include "skybox.h"

class Engine {
public:
	Engine(int *width, int *height);
	~Engine();

	void createMaterials();
	void createModels();
	void render(Scene* scene, gameState state);

	Shader* shader, *shader_text, *shader_skybox;
	Material* playerMaterial, *roadMaterial, *carMaterial1, *carMaterial2;
	ObjMesh* playerMesh, *roadMesh, *carMesh1, *carMesh2;
	Skybox* skyboxMaterial;

private:
	int *width, *height;
	
	Text* text_renderer;
};