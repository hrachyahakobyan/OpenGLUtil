#include "stdafx.h"
#include "InstanceSampleApp.h"

InstanceSampleApp::InstanceSampleApp(const std::string& windowName) :
glutil::ApplicationBase(windowName)
{
}

InstanceSampleApp::InstanceSampleApp(std::unique_ptr<glutil::WindowHandler> windowHandler) :
glutil::ApplicationBase(std::move(windowHandler))
{
}

void InstanceSampleApp::initialize()
{
	glutil::ApplicationBase::initialize();
	try {
		model.reset(new glutil::Model("models/rock/rock.obj"));
	}
	catch (std::exception& e){
		std::cout << e.what() << std::endl;
	}
	backgroundColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	shader.reset(new glutil::Shader("shaders/instanceSample/vertex.txt", "shaders/instanceSample/fragment.txt"));
	camera.reset(new glutil::Camera(glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f)));

	srand(glfwGetTime()); // initialize random seed	
	GLfloat radius = 50.0;
	GLfloat offset = 2.5f;
	std::vector<std::tuple<glm::mat4>> instanceData(rockCount);
	for (GLuint i = 0; i < rockCount; i++)
	{
		glm::mat4 model;
		// 1. Translation: displace along circle with 'radius' in range [-offset, offset]
		GLfloat angle = (GLfloat)i / (GLfloat)rockCount * 360.0f;
		GLfloat displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat x = sin(angle) * radius + displacement;
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat y = displacement * 0.4f; // y value has smaller displacement
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));
		// 2. Scale: Scale between 0.05 and 0.25f
		GLfloat scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));
		// 3. Rotation: add random rotation around a (semi)randomly picked rotation axis vector
		GLfloat rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
		// 4. Now add to list of matrices
		instanceData[i] = std::make_tuple(model);
	}

	modelArray.reset(new glutil::ModelArray<glutil::Model, glm::mat4>(*model.get(),
		instanceData));
	modelArray->initialAttrib = 3;
	modelArray->bufferData();
}

void InstanceSampleApp::updateData()
{
	// Camera/View transformation
	updateMovement();
	shader->use();
	glm::mat4 view = camera->view();
	// Projection 
	glm::mat4 projection = glm::perspective(camera->getZoom(), (GLfloat)windowHandler->getWindowWidth() / (GLfloat)windowHandler->getWindowHeight(), 0.1f, 100.0f);
	// Get the uniform locations
	GLint viewLoc = glGetUniformLocation(shader->getProgram(), "view");
	GLint projLoc = glGetUniformLocation(shader->getProgram(), "projection");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void InstanceSampleApp::render()
{
	modelArray->draw(*(shader.get()));
}

void InstanceSampleApp::updateMovement()
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

