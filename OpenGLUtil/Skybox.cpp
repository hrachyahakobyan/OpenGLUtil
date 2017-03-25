#include "stdafx.h"
#include "Skybox.h"

namespace glutil{

	std::shared_ptr<Skybox> Skybox::make(const std::string& cubemapPath, const std::vector<std::string>& faces)
	{
		try {
			std::shared_ptr<Skybox> box(new Skybox(cubemapPath, faces));
			return box;
		}
		catch (...){
			return nullptr;
		}
	}

	std::shared_ptr<Skybox> Skybox::make(const std::string& cubemapPath, const std::vector<std::string>& faces, std::shared_ptr<Shader> shader)
	{
		try {
			std::shared_ptr<Skybox> box(new Skybox(cubemapPath, faces, shader));
			return box;
		}
		catch (...){
			return nullptr;
		}
	}

	Skybox::Skybox(const std::string& cubemapPath, const std::vector<std::string>& faces):
	shader(Shader::fromCode(Skybox::vertexCode, Skybox::fragmentCode)), vao(new VAO()), vbo(new VBO()), cubemap(Texture::cubemap(cubemapPath, faces))
	{
		initialize();
	}

	Skybox::Skybox(const std::string& cubemapPath, const std::vector<std::string>& faces, std::shared_ptr<Shader> shader) :
		shader(shader), vao(new VAO()), vbo(new VBO()), cubemap(Texture::cubemap(cubemapPath, faces))
	{
		initialize();
	}
	
	Skybox::~Skybox()
	{
	}

	void Skybox::initialize()
	{
		vao->bind();
		vbo->bind();
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 108, &positions, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glBindVertexArray(0);
	}

	void Skybox::draw() const
	{
		draw(glm::mat4(), glm::mat4());
	}

	void Skybox::draw(const glm::mat4& view) const
	{
		draw(view, glm::mat4());
	}

	void Skybox::draw(const glm::mat4& view, const glm::mat4& proj) const
	{
		glDepthFunc(GL_LEQUAL);
		shader->use();
		glm::mat4 viewTrimmed = glm::mat4(glm::mat3(view));	
		glUniformMatrix4fv(glGetUniformLocation(shader->getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(viewTrimmed));
		glUniformMatrix4fv(glGetUniformLocation(shader->getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(proj));
		// skybox cube
		vao->bind();
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(shader->getProgram(), "skybox"), 0);
		cubemap->bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default
	}

	const GLfloat Skybox::positions[] = {
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	const GLchar Skybox::vertexCode[] = "#version 330 core \n"
										"layout(location = 0) in vec3 position;"
										"out vec3 TexCoords;"
										"uniform mat4 projection;"
										"uniform mat4 view;"
										"void main(){"
											"vec4 pos = projection * view * vec4(position, 1.0);"
											"gl_Position = pos.xyww;"
											"TexCoords = position;"
										"}";
	const GLchar Skybox::fragmentCode[] = "#version 330 core \n"
		"in vec3 TexCoords;"
		"out vec4 color;"
		"uniform samplerCube skybox;"
		"void main(){"
		"color = texture(skybox, TexCoords);"
		"}";
}
