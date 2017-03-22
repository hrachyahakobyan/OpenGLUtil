#include "stdafx.h"
#include "TextureTestApp.h"

TextureTestApp::TextureTestApp(const std::string& windowName) :
glutil::ApplicationBase(windowName)
{
}

TextureTestApp::TextureTestApp(std::unique_ptr<glutil::WindowHandler> windowHandler) :
glutil::ApplicationBase(std::move(windowHandler))
{
}

void TextureTestApp::initialize()
{
	glutil::ApplicationBase::initialize();
	backgroundColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	shader.reset(new glutil::Shader("shaders/textureTest/vertex.txt", "shaders/textureTest/fragment.txt"));
	camera.reset(new glutil::Camera(glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f)));
	initializeModel();
	//glPointSize(5.0f);
}

void TextureTestApp::updateData()
{
	// Camera/View transformation
	updateMovement();
	shader->use();
	glm::mat4 view = camera->view();
	// Projection 
	glm::mat4 projection = glm::perspective(camera->getZoom(), (GLfloat)windowHandler->getWindowWidth() / (GLfloat)windowHandler->getWindowHeight(), 0.1f, 100.0f);
	// Get the uniform locations
	GLint modelLoc = glGetUniformLocation(shader->getProgram(), "model");
	GLint viewLoc = glGetUniformLocation(shader->getProgram(), "view");
	GLint projLoc = glGetUniformLocation(shader->getProgram(), "projection");
	glUniformMatrix4fv(glGetUniformLocation(shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void TextureTestApp::initializeModel()
{
	using namespace glutil;
	std::vector<Vertex> vertices{

		// 0 1 2
		// 5 4 3
		Vertex({ -0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }),
		Vertex({ 0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f }),
		Vertex({ 1.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 2.0f, 0.0f }),
		Vertex({ 1.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 2.0f, 1.0f }),
		Vertex({ 0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }),
		Vertex({ -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f })
	};
	std::vector<GLuint> indices{ 0, 5, 1, 1, 5, 4, 2, 1, 4, 2, 4, 3 };
	auto tex = Texture::fromFile("textures/window.png", GL_TEXTURE_2D, Diffusive);
	model.reset(new Model(std::vector<Mesh>{Mesh(vertices, indices, std::vector<std::shared_ptr<Texture>>{tex})}));
}

void TextureTestApp::render()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	if (model){
		model->draw(*(shader.get()));
	}
}

void TextureTestApp::updateMovement()
{
	if (keys[GLFW_KEY_W])
		camera->move(glutil::FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera->move(glutil::BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera->move(glutil::LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera->move(glutil::RIGHT, deltaTime);
	if (keys[GLFW_KEY_UP]){
		modelMat = glm::scale(modelMat, glm::vec3{ 1.0f, 1.0f + deltaTime * 0.5f, 1.0f });
	}
	if (keys[GLFW_KEY_DOWN]){
		modelMat = glm::scale(modelMat, glm::vec3{ 1.0f, 1.0f - deltaTime * 0.5f, 1.0f });
	}
	if (keys[GLFW_KEY_LEFT]){
		modelMat = glm::scale(modelMat, glm::vec3{ 1.0f + deltaTime * 0.5f,1.0f, 1.0f });
	}
	if (keys[GLFW_KEY_RIGHT]){
		modelMat = glm::scale(modelMat, glm::vec3{ 1.0f - deltaTime * 0.5f, 1.0f, 1.0f });
	}
}

glutil::Mesh makeStore(const glm::vec3& topleft, const glm::vec3 botright, int width, int height)
{
	using namespace glutil;
	std::vector<Vertex> vertices(2*(width + height));

}

