#include "font.h"

#include <string>
#include <map>

void render_text(FT_Face face, std::string text, float x, float y, float scale,
                 float r, float g, float b,
                 std::map<GLchar, Character> Characters) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor3f(r, g, b);

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = Characters[*c];

        float xpos = x + ch.BearingX * scale;
        float ypos = y - (ch.SizeY - ch.BearingY) * scale;

        float w = ch.SizeX * scale;
        float h = ch.SizeY * scale;
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBegin(GL_QUADS);
        glTexCoord2d(0, 0);
        glVertex2f(xpos, ypos);
        glTexCoord2d(1, 0);
        glVertex2f(xpos + w, ypos);
        glTexCoord2d(1, 1);
        glVertex2f(xpos + w, ypos + h);
        glTexCoord2d(0, 1);
        glVertex2f(xpos, ypos + h);
        glEnd();

        // 更新 x 到下一個字元的位置
        x += (ch.Advance >> 6) * scale;
        // 位移是以 1/64 pixel 為單位，因此需要除以 64
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
}
