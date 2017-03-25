#include "stdafx.h"
#include "DynamicInstanceSampleApp.h"

DynamicInstanceSampleApp::DynamicInstanceSampleApp(const std::string& windowName) :
glutil::ApplicationBase(windowName)
{
}

DynamicInstanceSampleApp::DynamicInstanceSampleApp(std::unique_ptr<glutil::WindowHandler> windowHandler) :
glutil::ApplicationBase(std::move(windowHandler))
{
}

void DynamicInstanceSampleApp::initialize()
{
	glutil::ApplicationBase::initialize();
	backgroundColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	shader = glutil::Shader::fromFile("shaders/dynamicInstanceSample/vertex.txt", "shaders/dynamicInstanceSample/fragment.txt");
	camera.reset(new glutil::Camera(glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f)));
	initializeModel();

	std::cout << "Use WASD to move the camera" << std::endl;
	std::cout << "Use Arrow keys to move the active cube in the XY plane." << std::endl;
	std::cout << "Use left shift + Arrow up/down to move the active cube in the Z plane" << std::endl;
	std::cout << "Use SPACE to add a new cube " << std::endl;
	std::cout << "Use key 'i' to switch between cubes. The active cube is in red color" << std::endl;
}

void DynamicInstanceSampleApp::updateData()
{
	updateMovement();
	shader->use();
	glm::mat4 view = camera->view();
	// Projection 
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)windowHandler->getWindowWidth() / (GLfloat)windowHandler->getWindowHeight(), 0.1f, 10000.0f);
	GLint viewLoc = glGetUniformLocation(shader->getProgram(), "view");
	GLint projLoc = glGetUniformLocation(shader->getProgram(), "projection");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void DynamicInstanceSampleApp::initializeModel()
{
	using namespace glutil;
	modelArray.reset(new ModelArray<Model, glm::vec3, glm::mat4>(*Model::Cube().get()));
	modelArray->initialAttrib = 3;
	modelArray->instanceData.push_back(make_tuple(glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::mat4()));
	modelArray->bufferData();
}

void DynamicInstanceSampleApp::render()
{
	modelArray->draw(*shader.get());
}

void DynamicInstanceSampleApp::updateMovement()
{
	if (keys[GLFW_KEY_W])
		camera->move(glutil::FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera->move(glutil::BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera->move(glutil::LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera->move(glutil::RIGHT, deltaTime);
	if (keys[GLFW_KEY_I]){
		get<0>(modelArray->instanceData[currentModel]) = glm::vec3{ 0.0f, 0.0f, 0.0f };
		currentModel = (currentModel + 1) % modelArray->instanceData.size();
		get<0>(modelArray->instanceData[currentModel]) = glm::vec3{ 1.0f, 0.0f, 0.0f };
		modelArray->bufferData();
		keys[GLFW_KEY_I] = false;
	}
	if (keys[GLFW_KEY_SPACE]){
		modelArray->instanceData.push_back(make_tuple(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::mat4()));
		modelArray->bufferData();
		keys[GLFW_KEY_SPACE] = false;
	}
	if (keys[GLFW_KEY_LEFT]){
		auto modelMat = get<1>(modelArray->instanceData[currentModel]);
		get<1>(modelArray->instanceData[currentModel]) = glm::translate(modelMat, glm::vec3{ -deltaTime * 1.0, 0.0f, 0.0f });
		modelArray->bufferData();
	}
	if (keys[GLFW_KEY_RIGHT]){
		auto modelMat = get<1>(modelArray->instanceData[currentModel]);
		get<1>(modelArray->instanceData[currentModel]) = glm::translate(modelMat, glm::vec3{ deltaTime * 1.0, 0.0f, 0.0f });
		modelArray->bufferData();
	}
	if (keys[GLFW_KEY_UP] && keys[GLFW_KEY_LEFT_SHIFT]){
		auto modelMat = get<1>(modelArray->instanceData[currentModel]);
		get<1>(modelArray->instanceData[currentModel]) = glm::translate(modelMat, glm::vec3{ 0.0f, 0.0f, -deltaTime * 1.0f });
		modelArray->bufferData();
	}
	else	if (keys[GLFW_KEY_UP]){
		auto modelMat = get<1>(modelArray->instanceData[currentModel]);
		get<1>(modelArray->instanceData[currentModel]) = glm::translate(modelMat, glm::vec3{ 0.0f, deltaTime * 1.0f, 0.0f });
		modelArray->bufferData();
	}
	if (keys[GLFW_KEY_DOWN] && keys[GLFW_KEY_LEFT_SHIFT]){
		auto modelMat = get<1>(modelArray->instanceData[currentModel]);
		get<1>(modelArray->instanceData[currentModel]) = glm::translate(modelMat, glm::vec3{ 0.0f, 0.0f, deltaTime * 1.0 });
		modelArray->bufferData();
	}
	else if (keys[GLFW_KEY_DOWN]){
		auto modelMat = get<1>(modelArray->instanceData[currentModel]);
		get<1>(modelArray->instanceData[currentModel]) = glm::translate(modelMat, glm::vec3{ 0.0f, -deltaTime * 1.0f, 0.0f });
		modelArray->bufferData();
	}

}

