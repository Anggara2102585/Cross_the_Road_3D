#pragma once
#include "../config.h"
#include "obj_loader.h"

class ObjMesh {
public:
	unsigned int VBO, VAO, vertexCount;

	ObjMesh(const char* filename, glm::mat4 preTransform);
	~ObjMesh();
};