#ifndef FONT_H
#define FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <map>

struct Character {
    GLuint TextureID;   // 字符紋理的ID
    int SizeX, SizeY;   // 字符大小
    int BearingX, BearingY; // 從基線到字符的左部和頂部的偏移
    GLuint Advance;     // 原點到下一個字符原點的距離
};

void render_text(FT_Face face, std::string text, float x, float y, float scale, float r, float g, float b, std::map<GLchar, Character> Characters);

#endif