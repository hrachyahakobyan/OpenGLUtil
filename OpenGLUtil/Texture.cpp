#include "stdafx.h"
#include "Texture.h"

namespace glutil{

	std::map<std::string, Texture> Texture::textures;

	bool Texture::isLoaded(const std::string& path)
	{
		return (textures.find(path) != textures.end());
	}

	bool Texture::get(const std::string& path, Texture& texture)
	{
		const auto iter = textures.find(path);
		if (iter == textures.end()){
			return false;
		}
		texture = iter->second;
		return true;
	}

	void Texture::add(const Texture& texture, bool overwrite)
	{
		if (!overwrite && isLoaded(texture.path)){
			return;
		}
		textures.insert({texture.path, texture });
	}
}
