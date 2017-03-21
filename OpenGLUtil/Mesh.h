#pragma once
#include "Texture.h"
#include "Resource.h"

namespace glutil{
	/**
	*	@brief simple struct to store vertex attributes
	*	from www.learnopengl.com
	*/
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
		Vertex(){}
		Vertex(const glm::vec3& pos) : position(pos){}
		Vertex(const glm::vec3& pos, const glm::vec3& norm) : position(pos), normal(norm){}
		Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec3& tex) : position(pos), normal(norm), texCoords(tex){}
	};

	class Shader;

	FORWARD_MODELARRAY()
	FORWARD_MODELARRAYBASE()

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
		Mesh(const std::vector<glutil::Vertex>&,
			const std::vector<GLuint>& indices,
			const std::vector<std::shared_ptr<glutil::Texture>>&);
		Mesh(const Mesh&);
		Mesh& operator=(const Mesh&);
		Mesh(Mesh&&) NOEXCEPT;
		Mesh& operator=(Mesh&&);
		~Mesh() = default;

		const std::vector<glutil::Vertex>& getVertices() const { return vertices; }
		const std::vector<GLuint> getIndices() const { return indices; }
		const std::vector<std::shared_ptr<glutil::Texture>>& getTextures() const { return textures; }

		/**
		*	Renders the mesh.
		*	@param shader the shader object to be used for rendering.
		*/
		void draw(const glutil::Shader& shader) const;
	private:
		FRIEND_MODELARRAY()

		std::vector<glutil::Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<std::shared_ptr<glutil::Texture>> textures;
		std::shared_ptr<VAO> vao;
		std::shared_ptr<VBO> vbo;
		std::shared_ptr<EBO> ebo;

		/**
		*	Creates the VAO, VBO, EBO and buffers data.
		*	Called in the constructors and during assignments.
		*/
		void setup();
		void bindTextures(const Shader& shader) const;
		void unbindTextures() const;
	};

}

