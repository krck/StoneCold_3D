
#ifndef STONECOLD_INPUTMANAGER_H
#define STONECOLD_INPUTMANAGER_H

#include "Keyboard.hpp"
#include "Mouse.hpp"

namespace StoneCold::Game {

	static KeyboardServer* GlobalKeyboardServer = KeyboardServer::GetInstance();
	static MouseServer* GlobalMouseServer = MouseServer::GetInstance();

	//
	// InputManager
	//
	// Handles any GLFWwindow Input callbacks (Button presses, Mouse movements)
	// and provides a Keyboard and Mouse client to retrieve the current states 
	//
	class InputManager {
	public:
		InputManager() = default;

		// Handle the keyboard input
		static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
			switch (action) {
			case GLFW_PRESS:    GlobalKeyboardServer->OnKeyPressed(key);    break;
			case GLFW_RELEASE:  GlobalKeyboardServer->OnKeyReleased(key);   break;
			}
		}

		// Handle mouse button klicks
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
			switch (action) {
			case GLFW_PRESS:
				if (button == GLFW_MOUSE_BUTTON_LEFT) { GlobalMouseServer->OnLeftButtonPressed(); }
				else if (button == GLFW_MOUSE_BUTTON_RIGHT) { GlobalMouseServer->OnRightButtonPressed(); }
			case GLFW_RELEASE:
				if (button == GLFW_MOUSE_BUTTON_LEFT) { GlobalMouseServer->OnLeftButtonReleased(); }
				else if (button == GLFW_MOUSE_BUTTON_RIGHT) { GlobalMouseServer->OnRightButtonReleased(); }
			}
		}

		// Handle mouse movement
		inline void MouseMotionCallback(GLFWwindow* win) const { GlobalMouseServer->OnMouseMove(win); }

		// Handle mouswheel scrolling
		static inline void MouseWheelCallback(GLFWwindow* window, double xoffset, double yoffset) { GlobalMouseServer->OnWheelMove(xoffset, yoffset); }

		// Check if the mouse is inside the window
		static void MouseInWindowCallback(GLFWwindow* window, int entered) {
			if (entered) { GlobalMouseServer->OnWindowEnter(); }
			else { GlobalMouseServer->OnWindowLeave(); }
		}

		~InputManager() = default;
	};

}

#endif
