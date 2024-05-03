#ifndef GL_H
#define GL_H

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include <GLUT/glut.h>
    #include <GLFW/glfw3.h>
#endif

#ifdef __linux__
    #include <GL/glew.h>
    #include <GL/gl.h>
    #include <GLFW/glfw3.h>
    // #include <glad/glad.h>
#endif

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

#endif