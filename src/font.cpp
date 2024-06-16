#include "font.h"

#include <string>
#include <map>

void render_text(FT_Face face, std::string text, float x, float y, float scale, float r, float g, float b, std::map<GLchar, Character> Characters) {
    // 啟用混合模式來渲染字體
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 設置文字顏色
    glColor3f(r, g, b);

    // 遍歷字符串中的所有字符
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = Characters[*c];

        float xpos = x + ch.BearingX * scale;
        float ypos = y - (ch.SizeY - ch.BearingY) * scale;

        float w = ch.SizeX * scale;
        float h = ch.SizeY * scale;
        // 繪製字符的紋理到一個矩形中
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBegin(GL_QUADS);
        glTexCoord2d(0, 0); glVertex2f(xpos, ypos);
        glTexCoord2d(1, 0); glVertex2f(xpos + w, ypos);
        glTexCoord2d(1, 1); glVertex2f(xpos + w, ypos + h);
        glTexCoord2d(0, 1); glVertex2f(xpos, ypos + h);
        glEnd();

        // 更新x到下一個字符的位置
        x += (ch.Advance >> 6) * scale; // 位移是以1/64像素為單位，因此需要除以64
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
}
