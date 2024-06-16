#ifndef FONT_H
#define FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <map>

struct Character {
    GLuint TextureID;
    int SizeX, SizeY;
    int BearingX, BearingY; // 從基線到字元的左部和頂部的偏移
    GLuint Advance; // 原點到下一個字元原點的距離
};

void render_text(FT_Face face, std::string text, float x, float y, float scale, float r, float g, float b, std::map<GLchar, Character> Characters);

#endif