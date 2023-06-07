#pragma once
#include "../config.h"
#include "image.h"
#include "shader.h"

class Skybox {
public:
    Skybox(std::string folderPath);
    void drawSkybox(Shader *shader, glm::mat4 view, glm::mat4 projection);

    unsigned int VAO, VBO, textureID;
};