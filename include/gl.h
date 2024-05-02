#ifndef GL_H
#define GL_H

#ifdef __APPLE__
    #include <OpenGL/gl.h>
#endif

#ifdef __linux__
    #include <GL/gl.h>
    #include <GLFW/glfw3.h>
    // #include <glad/glad.h>
#endif

#endif