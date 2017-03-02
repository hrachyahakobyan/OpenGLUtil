#include "stdafx.h"
#include "WindowHandler.h"

namespace glutil{

	WindowHandler::WindowHandler() :
		windowName("Untitled"),
		initialized(false),
		allowWindowResize(true),
		handleWindowSizeCallback(true)
	{
		setWindowHeight(800);
		setWindowWidth(800);
	}

	WindowHandler::WindowHandler(const std::string& wName, bool allowResize, GLuint wWidth, GLuint wHeight)
		: windowName(wName), initialized(false), allowWindowResize(allowResize), handleWindowSizeCallback(true)
	{
		setWindowHeight(wHeight);
		setWindowWidth(wWidth);
	}

	WindowHandler::~WindowHandler()
	{
		glfwDestroyWindow(window);
	}

	void WindowHandler::initialize()
	{
		if (initialized){
			return;
		}
		// Set all the required options for GLFW
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, allowWindowResize ? GL_TRUE : GL_FALSE);
		// Create a GLFWwindow object that we can use for GLFW's functions
		window = glfwCreateWindow(windowWidth, windowHeight, windowName.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(window);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, s_windowResizeCallback);
		glfwSetKeyCallback(window, s_keyCallback);
		glfwSetCursorPosCallback(window, s_mousePositionCallback);
		glfwSetScrollCallback(window, s_mouseScrollCallback);
		glfwSetMouseButtonCallback(window, s_mouseButtonCallback);
		glViewport(0, 0, windowWidth, windowHeight);
		initialized = true;
	}

	int WindowHandler::shouldClose() const
	{
		return glfwWindowShouldClose(window);
	}

	void WindowHandler::registerForCallback(CallbackListener* listener, CallbackID id)
	{
		listenerMap[id].insert(listener);
	}

	void WindowHandler::unregisterForCallback(CallbackListener* listener, CallbackID id)
	{
		listenerMap[id].erase(listener);
	}

	void WindowHandler::windowSizeCallback(GLFWwindow* window, int width, int height)
	{
		if (!handleWindowSizeCallback){
			return;
		}
		setWindowHeight(static_cast<GLuint>(height));
		setWindowWidth(static_cast<GLuint>(width));
		glViewport(0, 0, windowWidth, windowHeight);
	}

	void WindowHandler::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		for (const auto& listener : listenerMap[KeyCallbackID]){
			listener->keyCallback(window, key, scancode, action, mode);
		}
	}

	void WindowHandler::mousePoistionCallback(GLFWwindow* window, double xpos, double ypos)
	{

		for (const auto& listener : listenerMap[MousePositionCallbackID]){
			listener->mousePositionCallback(window, xpos, ypos);
		}
	}

	void WindowHandler::mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{

		for (const auto& listener : listenerMap[MouseScrollCallbackID]){
			listener->mouseScrollCallback(window, xoffset, yoffset);
		}
	}

	void WindowHandler::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{

		for (const auto& listener : listenerMap[MouseButtonCallbackID]){
			listener->mouseButtonCallback(window, button, action, mods);
		}
	}
}