
#ifndef STONECOLD_INPUTMANAGER_H
#define STONECOLD_INPUTMANAGER_H

#include "Keyboard.hpp"
#include "Mouse.hpp"

namespace StoneCold::Engine {

	//
	// InputManager
	//
	// Handles any GLFWwindow Input callbacks (Button presses, Mouse movements)
	// and provides a Keyboard and Mouse client to retrieve the current states 
	//
	class InputManager {
	public:
		InputManager() = default;

		bool Initialize(KeyboardServer* keyboardServer, MouseServer* mouseServer) {
			if (keyboardServer != nullptr && mouseServer != nullptr) {
				//_keyboardServer = keyboardServer;
				//_mouseServer = mouseServer;
				return true;
			}
			return false;
		}

		// Handle the keyboard input
		static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
			switch (action) {
			case GLFW_PRESS:    KeyboardServer::GetInstance()->OnKeyPressed(key);    break;
			case GLFW_RELEASE:  KeyboardServer::GetInstance()->OnKeyReleased(key);   break;
			}
		}

		// Handle mouse button klicks
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
			switch (action) {
			case GLFW_PRESS:
				if (button == GLFW_MOUSE_BUTTON_LEFT) { MouseServer::GetInstance()->OnLeftButtonPressed(); }
				else if (button == GLFW_MOUSE_BUTTON_RIGHT) { MouseServer::GetInstance()->OnRightButtonPressed(); }
			case GLFW_RELEASE:
				if (button == GLFW_MOUSE_BUTTON_LEFT) { MouseServer::GetInstance()->OnLeftButtonReleased(); }
				else if (button == GLFW_MOUSE_BUTTON_RIGHT) { MouseServer::GetInstance()->OnRightButtonReleased(); }
			}
		}

		// Handle mouswheel scrolling
		static inline void MouseWheelCallback(GLFWwindow* window, double xoffset, double yoffset) { MouseServer::GetInstance()->OnWheelMove(xoffset, yoffset); }

		// Check if the mouse is inside the window
		static void MouseInWindowCallback(GLFWwindow* window, int entered) {
			if (entered) { MouseServer::GetInstance()->OnWindowEnter(); }
			else { MouseServer::GetInstance()->OnWindowLeave(); }
		}

		// Handle mouse movement
		inline void MouseMotionCallback(GLFWwindow* win) const { MouseServer::GetInstance()->OnMouseMove(win); }

		~InputManager() = default;

	//private:
	//	static KeyboardServer* _keyboardServer;
	//	static MouseServer* _mouseServer;
	};

}

#endif
