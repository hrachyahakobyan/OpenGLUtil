#include "stdafx.h"
#include "ModelSampleApp.h"

ModelSampleApp::ModelSampleApp(const std::string& windowName) :
glutil::ApplicationBase(windowName)
{
}

ModelSampleApp::ModelSampleApp(std::unique_ptr<glutil::WindowHandler> windowHandler) :
glutil::ApplicationBase(std::move(windowHandler))
{
}

void ModelSampleApp::initialize()
{
	glutil::ApplicationBase::initialize();
	try {
		model.reset(new glutil::Model("models/nanosuit/nanosuit.obj"));
	}
	catch (std::exception& e){
		std::cout << e.what() << std::endl;
	}
	backgroundColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	shader = glutil::Shader::fromFile("shaders/modelSample/vertex.txt", "shaders/modelSample/fragment.txt");
	camera.reset(new glutil::Camera(glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f)));
	std::vector<std::string> faces{
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"back.jpg",
		"front.jpg"
	};
	skybox = glutil::Skybox::make("textures\\skybox", faces);
}

void ModelSampleApp::updateData()
{
	// Camera/View transformation
	updateMovement();
	shader->use();
	view = camera->view();
	// Projection 
	proj = glm::perspective(camera->getZoom(), (GLfloat)windowHandler->getWindowWidth() / (GLfloat)windowHandler->getWindowHeight(), 0.1f, 100.0f);
	// Get the uniform locations
	GLint modelLoc = glGetUniformLocation(shader->getProgram(), "model");
	GLint viewLoc = glGetUniformLocation(shader->getProgram(), "view");
	GLint projLoc = glGetUniformLocation(shader->getProgram(), "projection");
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, -0.875f, 0.0f));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	glUniformMatrix4fv(glGetUniformLocation(shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
}

void ModelSampleApp::render()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	if (model){
		model->draw(*(shader.get()));
	}
	skybox->draw(view, proj);
}

void ModelSampleApp::updateMovement()
{
	if (keys[GLFW_KEY_W])
		camera->move(glutil::FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera->move(glutil::BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera->move(glutil::LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera->move(glutil::RIGHT, deltaTime);
}

