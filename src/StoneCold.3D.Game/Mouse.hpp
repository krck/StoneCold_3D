
#ifndef STONECOLD_MOUSE_H
#define STONECOLD_MOUSE_H

#include "StoneColdBase.hpp"

namespace StoneCold::Game {

	//
	// MouseServer (Singelton)
	//
	// Global Mouse Server that holds the Mouse position, the current state of both Mouse Buttons and the Wheel
	// This allows for quick "is key pressed?" checks as well as reading the exact Mouse (and Mouse wheel) X/Y position
	//
	class MouseServer {
	private:
		// Private default constructor to force the use of GetInstance()
		MouseServer() : _posX(-1.0), _posY(-1.0), _wheelX(0.0), _wheelY(0.0), _isInWindow(false), _leftIsPressed(false), _rightIsPressed(false) { }

	public:
		// Delete copy ctr and copy assignment
		MouseServer(const MouseServer&) = delete;
		MouseServer& operator=(const MouseServer&) = delete;

		static MouseServer* GetInstance() {
			// Static variable in fucntion will be instanciated on  
			// first function call and destroyed on programm exit
			static MouseServer instance;
			return &instance;
		}

		inline void OnMouseMove(GLFWwindow* win) { glfwGetCursorPos(win, &_posX, &_posY); }
		inline void OnWheelMove(double x, double y) { _wheelX = x; _wheelY = y; }
		inline void OnResetWheel() { _wheelX = 0.0; _wheelY = 0.0; }
		inline void OnLeftButtonPressed() { _leftIsPressed = true; }
		inline void OnLeftButtonReleased() { _leftIsPressed = false; }
		inline void OnRightButtonPressed() { _rightIsPressed = true; }
		inline void OnRightButtonReleased() { _rightIsPressed = false; }
		inline void OnWindowLeave() { _isInWindow = false; }
		inline void OnWindowEnter() { _isInWindow = true; }

		inline double GetX() const { return _posX; }
		inline double GetY() const { return _posY; }
		inline double GetWheelX() const { return _wheelX; }
		inline double GetWheelY() const { return _wheelY; }
		inline bool GetLeftPressed() const { return _leftIsPressed; }
		inline bool GetRightPressed() const { return _rightIsPressed; }
		inline bool GetInWindow() const { return _isInWindow; }

	private:
		double _posX, _posY;
		double _wheelX, _wheelY;
		bool _leftIsPressed;
		bool _rightIsPressed;
		bool _isInWindow;
	};

	//
	// MouseClient
	//
	// Each MouseClient is linked to the global MouseServer and can check
	// if any specific key was pressed or if the Mouse was moved in any way
	//
	class MouseClient {
	public:
		MouseClient(MouseServer* server) : _mouseServer(server) { }

		inline double GetMouseX() const { return _mouseServer->GetX(); }
		inline double GetMouseY() const { return _mouseServer->GetY(); }
		inline double GetWheelX() const { return _mouseServer->GetWheelX(); }
		inline double GetWheelY() const { return _mouseServer->GetWheelY(); }
		inline bool GetLeftIsPressed() const { return _mouseServer->GetLeftPressed(); }
		inline bool GetRightIsPressed() const { return _mouseServer->GetRightPressed(); }
		inline bool GetIsInWindow() const { return _mouseServer->GetInWindow(); }
		inline void ResetWheel() { _mouseServer->OnResetWheel(); }

	private:
		MouseServer* _mouseServer;
	};

}

#endif
