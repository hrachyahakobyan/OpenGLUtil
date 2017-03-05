#include "stdafx.h"
#include "Texture.h"

namespace glutil{

	std::map<std::string, std::shared_ptr<Texture>> Texture::textures;

	Resource<R_TEX>::~Resource()
	{
		glDeleteTextures(1, &resourceID);
	}

	Resource<R_TEX>::Resource(const std::string& path, GLenum textureType, MaterialType matType) :
		path(path), textureType(textureType), materialType(matType)
	{
	}

	void Resource<R_TEX>::bind() const
	{
		glBindTexture(textureType, resourceID);
	}

	bool Resource<R_TEX>::isLoaded(const std::string& path)
	{
		return (textures.find(path) != textures.end());
	}

	std::shared_ptr<Texture> Resource<R_TEX>::get(const std::string& path)
	{
		const auto iter = textures.find(path);
		if (iter == textures.end()){
			return std::shared_ptr<Texture>();
		}
		return iter->second;
	}

	std::shared_ptr<Texture> Texture::add(const std::string& path, GLenum textureType, MaterialType matType, bool overwrite)
	{
		if (!overwrite && isLoaded(path)){
			return std::shared_ptr<Texture>();
		}
		auto text = std::shared_ptr<Texture>(new Texture(path, textureType, matType));
		// Insert or update
		textures[text->path] = text;
		std::cout << "Texture Resource count " << textures.size() << std::endl;
		return text;
	}
}
