#pragma once
#include "../config.h"
#include "shader.h"

#include <ft2build.h>
#include FT_FREETYPE_H

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

struct TextSize {
    float height;
    float length;
};

class Text {
public:
	Text(const char* filepath);
	~Text();
    void renderText(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color);
    TextSize calculateSize(std::string text, float scale);

private:
    unsigned int VBO, VAO;
    
    std::map<char, Character> Characters;
};
