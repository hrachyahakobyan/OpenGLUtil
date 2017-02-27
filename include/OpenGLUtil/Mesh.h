#pragma once
#include "Texture.h"

namespace shared{
	/**
	*	@brief simple struct to store vertex attributes
	*	from www.learnopengl.com
	*/
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
	};

	class Shader;

	/**
	*	@brief class to represent a mesh.
	*	A mesh is made of vertices, corresponding indices and zero or more textures
	*	adapted and optimized from www.learnopengl.com
	*	@author Hrachya Hakobyan
	*/
	class Mesh
	{
	public:
		Mesh() = delete;
		Mesh(const std::vector<shared::Vertex>&,
			const std::vector<GLuint>& indices,
			const std::vector<shared::Texture>&);
		Mesh(const Mesh&);
		Mesh& operator=(const Mesh&);
		Mesh(Mesh&&);
		Mesh& operator=(Mesh&&);
		~Mesh() = default;

		const std::vector<shared::Vertex>& getVertices() const { return vertices; }
		const std::vector<GLuint> getIndices() const { return indices; }
		const std::vector < shared::Texture>& getTextures() const { return textures; }

		/**
		*	Renders the mesh.
		*	@param shader the shader object to be used for rendering.
		*/
		void draw(const shared::Shader& shader) const;
	private:
		std::vector<shared::Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<shared::Texture> textures;
		GLuint VAO, VBO, EBO;

		/**
		*	Creates the VAO, VBO, EBO and buffers data.
		*	Called in the constructors and during assignments.
		*/
		void setup();
	};

}

