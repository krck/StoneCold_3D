
#ifndef STONECOLD_STONECOLDBASE_H
#define STONECOLD_STONECOLDBASE_H

#ifdef _WIN32
    // Windows
    #define GLEW_STATIC
    #include <GLEW/glew.h>
#else
    // MacOS X
    #define GL_SILENCE_DEPRECATION
    #include <OpenGL/OpenGL.h>
    #include <OpenGL/glu.h>
    #include <OpenGL/gl3.h>
    #include <OpenGL/glext.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image\stb_image.h>

#endif
