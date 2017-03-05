#pragma once
#include "Resource.h"

namespace glutil{
	/**
	*	@brief Texture resource.
	*	For optimization, textures cannot be constructed directly.
	*	Texture resources are created/accessed via static methods.
	*	from www.learnopengl.com
	*/

	enum MaterialType{
		Diffusive, Specular
	};

	template<>
	class Resource<R_TEX> : public ResourceBase {
	public:
		Resource(const Resource&) = delete;
		Resource& operator=(const Texture&) = delete;
		~Resource();
		void bind() const;

		const std::string path;
		const GLenum textureType;
		const MaterialType materialType;

		inline static std::size_t resourceCount(){ return textures.size(); }

		/**
		*	Adds a new texture
		*	@param texture the texture to add
		*	@overwrite overwrite an existing texture in case of a duplication
		*/
		static std::shared_ptr<Texture> add(const std::string& path, GLenum textureType, MaterialType matType, bool overwrite = false);

		/**
		*	Checks if a texture with a given path is already loaded
		*	@param path the path to the texture
		*/
		static bool isLoaded(const std::string& path);

		/**
		*	Get the texture with the given path.
		*	@param path the path of the texture
		*	@param texture a reference to a texture object that will be filled with the texture data if one is found
		*	@return true if the texture was found, false otherwise
		*/
		static std::shared_ptr<Texture> get(const std::string& path);

	private:
		Resource(const std::string& path, GLenum textureType, MaterialType matType);
		static std::map<std::string, std::shared_ptr<Texture>> textures;
	};
}

