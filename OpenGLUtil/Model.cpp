#include "stdafx.h"
#include "Model.h"
#include "Shader.h"


namespace glutil{
	Model::Model()
	{
	}

	Model::Model(const Mesh& mesh) :
	meshes(1, mesh)
	{
	}

	Model::Model(const std::vector<Mesh>& meshes) :
	meshes(meshes)
	{
	}

	Model::Model(Mesh&& mesh):
	meshes(1, std::move(mesh))
	{
	}

	Model::Model(std::vector<Mesh>&& meshes):
	meshes(std::move(meshes))
	{
	}

	Model::Model(const std::string& path)
	{
		loadModel(path);
	}

	Model::Model(const Model& o):
		meshes(o.meshes), directory(o.directory)
	{
	}

	Model::Model(Model&& o) :
		meshes(std::move(o.meshes)), directory(std::move(o.directory))
	{
	}

	Model& Model::operator=(const Model& m)
	{
		meshes = m.meshes;
		directory = m.directory;
		return *this;
	}

	Model& Model::operator=(Model&& m)
	{
		meshes = std::move(m.meshes);
		directory = std::move(m.directory);
		return *this;
	}

	Model::~Model()
	{
	}

	void Model::draw(const Shader& shader) const
	{	
		shader.use();
		for (std::size_t i = 0; i < this->meshes.size(); i++)
			this->meshes[i].draw(shader);
	}

	void Model::add(const Mesh& mesh)
	{
		meshes.push_back(mesh);
	}

	void Model::loadModel(const std::string& path)
	{
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::string msg(import.GetErrorString());
			throw std::exception(("ERROR::ASSIMP::" + msg).c_str());
		}
		this->directory = path.substr(0, path.find_last_of('/'));
		this->processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		// Process all the node's meshes (if any)
		for (GLuint i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			this->meshes.push_back(this->processMesh(mesh, scene));
		}
		// Then do the same for each of its children
		for (GLuint i = 0; i < node->mNumChildren; i++)
		{
			this->processNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		// Data to fill
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<std::shared_ptr<Texture>> textures;

		// Walk through each of the mesh's vertices
		for (GLuint i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
			// Positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;
			// Normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
			// Texture Coordinates
			if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.texCoords = vec;
			}
			else
				vertex.texCoords = glm::vec2(0.0f, 0.0f);
			vertices.push_back(vertex);
		}
		// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (GLuint i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// Retrieve all indices of the face and store them in the indices vector
			for (GLuint j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		// Process materials
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
			// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
			// Same applies to other texture as the following list summarizes:
			// Diffuse: texture_diffuseN
			// Specular: texture_specularN
			// Normal: texture_normalN

			// 1. Diffuse maps
			std::vector<std::shared_ptr<Texture>> diffuseMaps;
			loadMaterialTextures(material, aiTextureType_DIFFUSE, diffuseMaps);
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. Specular maps
			std::vector<std::shared_ptr<Texture>> specularMaps;
			loadMaterialTextures(material, aiTextureType_SPECULAR, specularMaps);
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		// Return a mesh object created from the extracted mesh data
		return Mesh(vertices, indices, textures);
	}

	void Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type,  std::vector<std::shared_ptr<Texture>>& textures)
	{
		for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			std::string filename(str.C_Str());
			filename = directory + '/' + filename;
			auto texture = Texture::get(filename);
			if (texture == nullptr){
				MaterialType matType = (type == aiTextureType_DIFFUSE ? Diffusive : Specular);
				texture = Texture::fromFile(filename, GL_TEXTURE_2D, matType);
			}
			if (texture)
				textures.push_back(texture);
		}
	}

	std::shared_ptr<Model> Model::Cube()
	{
		std::vector<Vertex> vertices{Vertex({-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}),
			Vertex({0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}),
			Vertex({0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}),
			Vertex({0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}),
			Vertex({-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}),
			Vertex({-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}),

			Vertex({-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}),
			Vertex({0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}),
			Vertex({0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}),
			Vertex({0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}),
			Vertex({-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}),
			Vertex({-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}),

			Vertex({-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}),
			Vertex({-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}),
			Vertex({-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}),
			Vertex({-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}),
			Vertex({-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}),
			Vertex({-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}),

			Vertex({0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}),
			Vertex({0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}),
			Vertex({0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}),
			Vertex({0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}),
			Vertex({0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}),
			Vertex({0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}),

			Vertex({-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}),
			Vertex({0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}),
			Vertex({0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}),
			Vertex({0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}),
			Vertex({-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}),
			Vertex({-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}),

			Vertex({-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}),
			Vertex({0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}),
			Vertex({0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}),
			Vertex({0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}),
			Vertex({-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}),
			Vertex({-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f})};
		std::size_t triangleCount = 12;
		std::vector<GLuint> indices(vertices.size()); 
		std::iota(std::begin(indices), std::end(indices), 0); 
		return std::shared_ptr<Model>(new Model(std::vector<Mesh>{Mesh(vertices, indices, std::vector<std::shared_ptr<Texture>>())}));
	}
}
