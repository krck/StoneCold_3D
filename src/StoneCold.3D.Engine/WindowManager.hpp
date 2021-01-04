
#ifndef STONECOLD_WINDOWMANAGER_H
#define STONECOLD_WINDOWMANAGER_H

#include <string>
#include <memory>
#include <iostream>
#include "Types.hpp"
#include "Settings.hpp"
#include "StoneColdBase.hpp"
#include "InputManager.hpp"
#include "Exception.hpp"
#include "ErrorCallback.hpp"

namespace StoneCold::Engine {

	using namespace StoneCold::Base;

	class WindowManager {
	public:
		WindowManager() : _actualWidth(0), _actualHeight(0), _window(nullptr), _inputManager(nullptr) { }

		bool Initialize(InputManager* inputManager);
		bool SetupWindow(const std::string& titel, uint16 width, uint16 height, bool fullscreen);

		void ChangeWindowSize(int width, int height);
		inline void ChangeWindowPosition(int xpos, int ypos) { glfwSetWindowPos(_window, xpos, ypos); }
		inline void ChangeWindowTitel(const std::string& titel) { glfwSetWindowTitle(_window, titel.c_str()); }

		inline int32 GetWidth() const { return _actualWidth; }
		inline int32 GetHeight() const { return _actualHeight; }
		inline bool IsClosed() const { return glfwWindowShouldClose(_window); }

		inline void Clear() const;
		void Update() const;

		void SetWindowAsRenderTarget();

		~WindowManager() { glfwDestroyWindow(_window); glfwTerminate(); }

	private:
		int32 _actualWidth;
		int32 _actualHeight;
		GLFWwindow* _window;
		InputManager* _inputManager;
	};

}

#endif
