#include "WindowManager.hpp"

using namespace StoneCold::Engine;


bool WindowManager::Initialize(InputManager* inputManager) {
	try {
		_inputManager = inputManager;

		// Initialize GLFW
		if (!glfwInit())
			throw GameException("GLFW Error on window creation");

		// Settings needed for using OpenGL Version 410
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		return true;
	}
	catch (const std::exception & ex) {
		std::cout << ex.what() << std::endl;
		glfwTerminate();
		return false;
	}
}


bool WindowManager::SetupWindow(const std::string& titel, uint16 width, uint16 height, bool fullscreen) {
	try {
		// Create the OpenGL context window
		GLFWmonitor* primary = (fullscreen ? glfwGetPrimaryMonitor() : nullptr);
		_window = glfwCreateWindow(width, height, titel.c_str(), primary, nullptr);
		glfwMakeContextCurrent(_window);
		std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

		// GLEW init only works after a OpenGL context was created
		if (glewInit() != GLEW_OK)
			throw GameException("GLEW Error on init");

		// Setup global Error/Warning debug output
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(GlobalErrorCallback::OpenGlCallback, 0);

		// Enable V-Sync
		glfwSwapInterval(1);

		// Set some OpenGL options
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		// glEnable(GL_FRAMEBUFFER_SRGB);	gamma correction (SRGB / expotential)
		// glEnable(GL_DEPTH_CLAMP);		check if these are still needed
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		// glFrontFace(GL_CCW);

		// Set Input-Event callbacks
		glfwSetKeyCallback(_window, _inputManager->KeyboardCallback);
		glfwSetMouseButtonCallback(_window, _inputManager->MouseButtonCallback);
		glfwSetScrollCallback(_window, _inputManager->MouseWheelCallback);
		glfwSetCursorEnterCallback(_window, _inputManager->MouseInWindowCallback);

		// Disable cursor for unlimited mouse movement with the camera
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// Get the actual window size values in pixels
		glfwGetFramebufferSize(_window, &_actualWidth, &_actualHeight);
		glViewport(0, 0, _actualWidth, _actualHeight);

		return true;
	}
	catch (const std::exception & ex) {
		std::cout << ex.what() << std::endl;
		glfwDestroyWindow(_window);
		glfwTerminate();
		return false;
	}
}


void WindowManager::ChangeWindowSize(int width, int height)
{
	glfwSetWindowSize(_window, width, height);
	glfwGetFramebufferSize(_window, &_actualWidth, &_actualHeight);
	glViewport(0, 0, _actualWidth, _actualHeight);
}


void WindowManager::Clear() const {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


void WindowManager::Update() const {
	glfwSwapBuffers(_window);
	glfwPollEvents();
	_inputManager->MouseMotionCallback(_window);
}


void WindowManager::SetWindowAsRenderTarget() {
	// Set the default FrameBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Clear();
	glfwGetFramebufferSize(_window, &_actualWidth, &_actualHeight);
	glViewport(0, 0, _actualWidth, _actualHeight);
}
