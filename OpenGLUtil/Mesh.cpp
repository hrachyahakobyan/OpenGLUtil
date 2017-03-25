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

	Mesh::Mesh(std::vector<glutil::Vertex>&& v,
		std::vector<GLuint>&& indices,
		const std::vector<std::shared_ptr<glutil::Texture>>& t) :
		vertices(std::move(v)), indices(std::move(indices)), textures(t)
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

	void Mesh::addTexture(std::shared_ptr<Texture> texture)
	{
		textures.push_back(texture);
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
		shader.use();
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

	Mesh Mesh::rectangle(const glm::vec3& topleft, int xWidth, int zWidth, float rectWidth, float rectHeight, int height, bool bottom, bool top)
	{
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		rectangle(topleft, xWidth, zWidth, rectWidth, rectHeight, height, bottom, top, vertices, indices);
		return Mesh(std::move(vertices), std::move(indices));
	}

	void Mesh::rectangle(const glm::vec3& topleft, int xWidth, int zWidth, float rectWidth, float rectHeight, int height, bool bottom, bool top, std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
	{
		if (xWidth == 0 || zWidth == 0 || rectWidth <= 0.0f || rectHeight <= 0.0f)
			throw std::exception("Invalid parameters.");
		glm::vec3 origin = topleft;
		glm::vec3 d1{ rectWidth, 0.0f, 0.0f };
		glm::vec3 d2{ 0.0f, rectHeight, 0.0f };
		glm::vec3 normal{ 0.0f, 0.0f, 1.0f };

		// Calculate the total count of vertices and indices
		std::size_t vertexTotalCount = 2 * (xWidth + zWidth) * (height + 1) + (height + 1) * 4 + (int(top) + int(bottom)) * (xWidth + 1) * (zWidth + 1);
		std::size_t indexTotalCount = 2 * (xWidth + zWidth) * height * 2 * 3 + (int(top) + int(bottom)) * xWidth * zWidth * 2 * 3;

		if (bottom){
			glm::vec3 d2{ 0.0f, 0.0f, rectWidth };
			glm::vec3 normal{ 0.0f, -1.0f, 0.0f };
			grid2D(origin, d1, d2, normal, xWidth, zWidth, vertices, indices);
		}
		grid2D(origin, d1, d2, normal, xWidth, height, vertices, indices);
		d1 = glm::vec3{ 0.0f, 0.0f, rectWidth };
		normal = glm::vec3{ 1.0f, 0.0f, 0.0f };
		origin = glm::vec3{ origin.x + float(xWidth) * rectWidth, origin.y, origin.z };
		grid2D(origin, d1, d2, normal, zWidth, height, vertices, indices);
		d1 = glm::vec3{ -rectWidth, 0.0f, 0.0f };
		normal = glm::vec3{ 0.0f, 0.0f, 1.0f };
		origin = glm::vec3{ origin.x, origin.y, origin.z + float(zWidth) * rectWidth };
		grid2D(origin, d1, d2, normal, xWidth, height, vertices, indices);
		d1 = glm::vec3{ 0.0f, 0.0f, -rectWidth };
		normal = glm::vec3{ -1.0f, 0.0f, 0.0f };
		origin = glm::vec3{ origin.x - float(xWidth) * rectWidth, origin.y, origin.z };
		grid2D(origin, d1, d2, normal, zWidth, height, vertices, indices);
		if (top){
			origin = topleft;
			origin.y += height * rectHeight;
			glm::vec3 d1{ rectWidth, 0.0f, 0.0f };
			glm::vec3 d2{ 0.0f, 0.0f, rectWidth };
			glm::vec3 normal{ 0.0f, 1.0f, 0.0f };
			grid2D(origin, d1, d2, normal, xWidth, zWidth, vertices, indices);
		}
	}

	void Mesh::grid2D(const glm::vec3& bottomleft, const glm::vec3& d1, const glm::vec3& d2, const glm::vec3& normal, 
					 int width, int height, 
					 std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
	{
		std::size_t vertexCount = (width + 1) * (height + 1);
		std::size_t triCount = width * height * 2;
		std::size_t vCounter = 0;
		std::size_t iCounter = 0;
		std::size_t initialVertexCount = vertices.size();
		for (int i = 0; i < height + 1; i++){
			float textY = (i % 2 == 0 ? 0.0f : 1.0f);
			for (int j = 0; j < width + 1; j++){
				float textX = (j % 2 == 0 ? 0.0f : 1.0f);
				vertices.push_back(Vertex(bottomleft + float(j) * d1 + float(i) * d2, normal, glm::vec2{ j, i }));
			}
		}
		for (int i = 0; i < width; i++){
			for (int j = 0; j < height; j++){
				indices.push_back(initialVertexCount + (j + 1) * (width + 1) + i);
				indices.push_back(initialVertexCount + j * (width + 1) + i);
				indices.push_back(initialVertexCount + j * (width + 1) + i + 1);
				indices.push_back(initialVertexCount + (j + 1) * (width + 1) + i + 1);
				indices.push_back(initialVertexCount + (j + 1) * (width + 1) + i);
				indices.push_back(initialVertexCount + j * (width + 1) + i + 1);
			}
		}
	}
}