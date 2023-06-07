#pragma once
#include "../config.h"

namespace util {
	image load_from_file(const char* filename);
	unsigned int loadCubemap(std::vector<std::string> faces);

	void free_image_memory(image oldImage);
}