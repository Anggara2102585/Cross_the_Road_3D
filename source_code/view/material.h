#pragma once
#include "../config.h"
#include "image.h"

class Material {
public:

	unsigned int ID;
	
	Material(const char* filename);
	~Material();
	void use();
};