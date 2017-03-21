#pragma once
#include "Mesh.h"

namespace glutil{

	class Shader;

	FORWARD_MODELARRAY()
	FORWARD_MODELARRAYBASE()

	/**
	*	@brief An abstraction of a graphic model.
	*	from learnopengl.com
	*	Optimized and adapted by
	*	@author Hrachya Hakobyan
	*/
	class Model
	{
	public:
		Model();
		Model(const std::vector<Mesh>&);
		/**
		*	Main constructor.
		*	@param path path to the model file.
		*	@throws std::exception if fails to load the model.
		*/
		explicit Model(const std::string& path);
		Model(const Model&);
		Model(Model&&);
		Model& operator=(const Model&m);
		Model& operator=(Model&&);
		~Model();
		void draw(const glutil::Shader& shader) const;
	private:
		FRIEND_MODELARRAY()

		std::vector<Mesh> meshes;
		std::string directory;
	
		void loadModel(const std::string& path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		void loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::vector<std::shared_ptr<Texture>>& textures);
		static std::shared_ptr<Texture> textureFromFile(const std::string& filename, aiTextureType type);
	};
} 