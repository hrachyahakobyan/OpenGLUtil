#include "stdafx.h"
#include "ApplicationBase.h"
#include "CameraBase.h"

namespace shared{

	ApplicationBase::ApplicationBase(const std::string& windowName) :
		windowHandler(new WindowHandler(windowName)),
		backgroundColor({ 0.2f, 0.3f, 0.3f, 1.0f })
	{
	}

	ApplicationBase::ApplicationBase(std::unique_ptr<WindowHandler> window) :
		windowHandler(std::move(window)),
		backgroundColor({ 0.2f, 0.3f, 0.3f, 1.0f })
	{
	}

	ApplicationBase::~ApplicationBase()
	{
		glfwTerminate();
	}

	void ApplicationBase::setCamera(std::unique_ptr<CameraBase> newCamera)
	{
		camera = std::move(newCamera);
	}

#pragma message(Reminder "Handle initialization failures")
	void ApplicationBase::initialize()
	{
		if (initialized){
			return;
		}
		int glfwI = glfwInit();
		if (glfwI == GLFW_FALSE){
			throw std::exception("ERROR: ApplicationBase: Initialization failed. Reason: failed to initialize glfw.");
		}
		windowHandler->initialize();
		if (!windowHandler->isInitialized()){
			throw std::exception("ERROR: ApplicationBase: Initialization failed. Reason: failed to initialize window.");
		}
		glewExperimental = GL_TRUE;
		glewInit();
		glEnable(GL_DEPTH_TEST);
		initialized = true;
		windowHandler->registerForCallback(this, WindowHandler::KeyCallbackID);
		windowHandler->registerForCallback(this, WindowHandler::MouseButtonCallbackID);
		windowHandler->registerForCallback(this, WindowHandler::MousePositionCallbackID);
		windowHandler->registerForCallback(this, WindowHandler::MouseScrollCallbackID);
	}

	void ApplicationBase::run(){
		if (!initialized){
			return;
		}
		while (!windowHandler->shouldClose())
		{
			updateDeltaFrame();
			glfwPollEvents();
			glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			updateRenderMode();
			updateData();
			render();
			windowHandler->swapBuffers();
		}
	}

	void ApplicationBase::updateRenderMode()
	{
		if (keys[GLFW_KEY_P])
			polygonMode = GL_POINT;
		else if (keys[GLFW_KEY_T])
			polygonMode = GL_FILL;
		else if (keys[GLFW_KEY_L])
			polygonMode = GL_LINE;
		glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
	}

	void ApplicationBase::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		{
			rightButtonPressed = true;
			isFirstMouse = true;
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
			rightButtonPressed = false;
	}

	void ApplicationBase::mouseScrollCallback(GLFWwindow*, double xoffset, double yoffset)
	{
		if (camera && allowZoom)
			camera->zoom(static_cast<GLfloat>(yoffset));
	}

	void ApplicationBase::mousePositionCallback(GLFWwindow*, double xpos, double ypos)
	{
		if (!rightButtonPressed)
			return;
		if (isFirstMouse)
		{
			lastMouseX = static_cast<GLfloat>(xpos);
			lastMouseY = static_cast<GLfloat>(ypos);
			isFirstMouse = false;
		}
		GLfloat xoffset = static_cast<GLfloat>(xpos)-lastMouseX;
		GLfloat yoffset = lastMouseY - static_cast<GLfloat>(ypos);

		lastMouseX = static_cast<GLfloat>(xpos);
		lastMouseY = static_cast<GLfloat>(ypos);

		if (camera)
			camera->rotate(xoffset, yoffset);
	}

	void ApplicationBase::keyCallback(GLFWwindow*, int key, int scancode, int action, int mode)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			windowHandler->setShouldClose(true);
		if (key >= 0 && key < 1024)
		{
			if (action == GLFW_PRESS)
				keys[key] = true;
			else if (action == GLFW_RELEASE)
				keys[key] = false;
		}
	}

	void ApplicationBase::updateDeltaFrame()
	{
		GLfloat currentFrame = static_cast<GLfloat>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}

	void ApplicationBase::updateData()
	{

	}

	void ApplicationBase::render()
	{

	}
}