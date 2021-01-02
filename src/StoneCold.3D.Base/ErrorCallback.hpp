
#ifndef STONECOLD_ERRORCALLBACK_H
#define STONECOLD_ERRORCALLBACK_H

#include <iostream>
#include <windows.h> 
#include "StoneColdBase.hpp"

namespace StoneCold::Base {

	static HANDLE GlobalConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	class GlobalErrorCallback {
	public:
		//
		// OpenGL Global DEBUG Error callback function
		//
		static void GLAPIENTRY OpenGlCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
			// Print (red) Error in case of DEBUG_SEVERITY_HIGH, DEBUG_SEVERITY_MEDIUM, DEBUG_SEVERITY_LOW
			if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
			{
				SetConsoleTextAttribute(GlobalConsoleHandle, 4);
				std::cout << "[OpenGL Error](" << type << "): " << message << std::endl;
			}
			// Print (yellow) Warning in case of GL_DEBUG_SEVERITY_NOTIFICATION
			else
			{
				SetConsoleTextAttribute(GlobalConsoleHandle, 6);
				std::cout << "[OpenGL Warning](" << type << "): " << message << std::endl;
			}

			// Reset the console text color to white
			SetConsoleTextAttribute(GlobalConsoleHandle, 7);
		}
	};

}

#endif
