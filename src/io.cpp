#include "io.h"

#include "gl.h"

int mouse_button_callback(GLFWwindow* window, int button, int action,
                          int mods) {
    if (action == GLFW_PRESS) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            return 1;
        } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            return 2;
        }
    }
    return 0;
}