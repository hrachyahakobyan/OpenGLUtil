#pragma once
#include "Mesh.h"


namespace shared{

	class Shader;

	/**
	*	@brief An abstraction of a graphic model.
	*	from learnopengl.com
	*	Optimized and adapted by
	*	@author Hrachya Hakobyan
	*/
	class Model
	{
	public:
		Model() = delete;
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
		void draw(const shared::Shader& shader) const;
	private:
		/*  Model Data  */
		std::vector<Mesh> meshes;
		std::string directory;
		/*  Functions   */
		void loadModel(const std::string& path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		void loadMaterialTextures(aiMaterial* mat, aiTextureType type,
			const std::string& typeName, std::vector<Texture>& textures);
		static GLint textureFromFile(const char* path, const std::string& dir);
	};
} 