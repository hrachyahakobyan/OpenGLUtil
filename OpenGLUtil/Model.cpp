#include "stdafx.h"
#include "Model.h"


namespace shared{
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
		for (std::size_t i = 0; i < this->meshes.size(); i++)
			this->meshes[i].draw(shader);
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
		std::vector<Texture> textures;

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
			std::vector<Texture> diffuseMaps;
			loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", diffuseMaps);
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. Specular maps
			std::vector<Texture> specularMaps;
			loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", specularMaps);
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		// Return a mesh object created from the extracted mesh data
		return Mesh(vertices, indices, textures);
	}

	void Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName, std::vector<Texture>& textures)
	{
		for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			Texture texture;
			if (!Texture::get(str.C_Str(), texture)){
				texture.id = textureFromFile(str.C_Str(), this->directory);
				texture.type = typeName;
				texture.path = std::string(str.C_Str());
				Texture::add(texture);
			}
			textures.push_back(texture);
		}
	}

	GLint Model::textureFromFile(const char* path, const std::string& directory)
	{
		//Generate texture ID and load texture data 
		std::string filename(path);
		filename = directory + '/' + filename;
		GLuint textureID;
		glGenTextures(1, &textureID);
		int width, height;
		unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		// Assign texture to ID
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		SOIL_free_image_data(image);
		return textureID;
	}
}
