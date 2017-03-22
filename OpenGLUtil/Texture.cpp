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
		glGenTextures(1, &resourceID);
		glBindTexture(textureType, resourceID);
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

	std::shared_ptr<Texture> Texture::fromFile(const std::string& filename, GLenum textureType, MaterialType mType, bool overwrite)
	{
		if (!overwrite && isLoaded(filename)){
			return Texture::get(filename);
		}
		int width, height;
		unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		if (!image){
			std::cout << "Failed to load texture at " << filename << std::endl;
			return nullptr;
		}
		auto tex = std::shared_ptr<Texture>(new Texture(filename, textureType, mType));
		tex->bind();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		// Parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		SOIL_free_image_data(image);
		// Insert or update
		textures[tex->path] = tex;
		return tex;
	}
}
