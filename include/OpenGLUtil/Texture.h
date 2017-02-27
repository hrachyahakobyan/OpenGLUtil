#pragma once

namespace shared{
	/**
	*	@brief simple struct to store Texture metadata
	*	from www.learnopengl.com
	*/
	struct Texture {
		GLuint id;
		std::string type;
		std::string path;
		inline static std::size_t textureCount(){ return textures.size(); }
		inline static const std::map<std::string, Texture>& getTextures() { return textures; }

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
		static bool get(const std::string& path, Texture& texture);
		/**
		*	Adds a new texture
		*	@param texture the texture to add
		*	@overwrite overwrite an existing texture in case of a duplication
		*/
		static void add(const Texture& texture, bool overwrite = false);

	private:
		static std::map<std::string, Texture> textures;
	};
}

