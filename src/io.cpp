#include "io.h"

#include "gl.h"

int mouse_button_callback(GLFWwindow* window, int button, int action, int mods, int result) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        result = 0;
        // Check if the mouse is inside the button
        if (xpos >= -0.5f && xpos <= 0.5f &&
            ypos >= 0.2f && ypos <= 0.5f) {
            result = 1;
        }
        if (xpos >= -0.5f && xpos <= 0.5f &&
            ypos >= -0.1f && ypos <= 0.1f) {
            result = 2;
        }
        if (xpos >= -0.5f && xpos <= 0.5f &&
            ypos >= -0.5f && ypos <= -0.2f) {
            result = 3;
        }
    }
    return result;
}