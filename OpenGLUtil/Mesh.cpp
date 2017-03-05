#include "stdafx.h"
#include "Mesh.h"
#include "Shader.h"

namespace glutil{
	Mesh::Mesh(const std::vector<glutil::Vertex>& v,
		const std::vector<GLuint>& i,
		const std::vector<std::shared_ptr<glutil::Texture>>& t) :
		vertices(v), indices(i), textures(t)
	{
		setup();
	}

	Mesh::Mesh(const Mesh& o) :
		vertices(o.vertices), indices(o.indices), textures(o.textures)
	{
		setup();
	}

	Mesh::Mesh(Mesh&& o) :
		vertices(std::move(o.vertices)), indices(std::move(o.indices)), textures(std::move(o.textures)),
		vao(std::move(o.vao)), vbo(std::move(o.vbo)), ebo(std::move(o.ebo))
	{
	}

	Mesh& Mesh::operator=(const Mesh& o)
	{
		vertices = o.vertices;
		textures = o.textures;
		indices = o.indices;
		setup();
		return *this;
	}

	Mesh& Mesh::operator=(Mesh&& o)
	{
		vertices = std::move(o.vertices);
		textures = std::move(o.textures);
		indices = std::move(o.indices);
		vao = std::move(o.vao);
		vbo = std::move(o.vbo);
		ebo = std::move(o.ebo);
		return *this;
	}

	void Mesh::setup()
	{
		vao.reset(new Resource<R_VAO>());
		vbo.reset(new Resource<R_VBO>());
		ebo.reset(new Resource<R_EBO>());
	
		glBindVertexArray(vao->getResourceID());
		glBindBuffer(GL_ARRAY_BUFFER, vbo->getResourceID());

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
			vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->getResourceID());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
			indices.data(), GL_STATIC_DRAW);

		// Vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(GLvoid*)0);
		// Vertex Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(GLvoid*)offsetof(Vertex, normal));
		// Vertex Texture Coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(GLvoid*)offsetof(Vertex, texCoords));

		glBindVertexArray(0);
	}

	void Mesh::draw(const glutil::Shader& shader) const
	{
		
		bindTextures(shader);
		vao->bind();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		unbindTextures();
		return;
		
		GLuint diffuseNr = 1;
		GLuint specularNr = 1;
		GLuint tSize = static_cast<GLuint>(textures.size());
		for (GLuint i = 0; i < tSize; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding
			// Retrieve texture number (the N in diffuse_textureN)
			std::string name = (textures[i]->materialType == Diffusive ? "texture_diffuse" : "texture_specular");
			std::string number;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++); // Transfer GLuint to stream
			else if (name == "texture_specular")
				number = std::to_string(specularNr++); // Transfer GLuint to stream

			glUniform1i(glGetUniformLocation(shader.getProgram(), (name + number).c_str()), i);
			textures[i]->bind();
		}
		// Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
		glUniform1f(glGetUniformLocation(shader.getProgram(), "material.shininess"), 16.0f);

		// Draw mesh
		vao->bind();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Always good practice to set everything back to defaults once configured.
		for (GLuint i = 0; i < this->textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		
	}

	void Mesh::bindTextures(const Shader& shader) const
	{
		GLuint diffuseNr = 1;
		GLuint specularNr = 1;
		GLuint tSize = static_cast<GLuint>(textures.size());
		for (GLuint i = 0; i < tSize; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding
			// Retrieve texture number (the N in diffuse_textureN)
			std::string name = (textures[i]->materialType == Diffusive ? "texture_diffuse" : "texture_specular");
			std::string number;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++); // Transfer GLuint to stream
			else if (name == "texture_specular")
				number = std::to_string(specularNr++); // Transfer GLuint to stream

			glUniform1i(glGetUniformLocation(shader.getProgram(), (name + number).c_str()), i);
			textures[i]->bind();
		}
		// Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
		glUniform1f(glGetUniformLocation(shader.getProgram(), "material.shininess"), 16.0f);
	}

	void Mesh::unbindTextures() const
	{
		// Always good practice to set everything back to defaults once configured.
		for (GLuint i = 0; i < this->textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}