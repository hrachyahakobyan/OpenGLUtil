#include "stdafx.h"
#include "Texture.h"

namespace glutil{

	unsigned int Resource<R_TEX>::resourceCount = 0;
	std::map<std::string, std::shared_ptr<Texture>> Texture::textures;

	Resource<R_TEX>::~Resource()
	{
		resourceCount--;
		std::cout << "Texture Resource Count " << resourceCount << std::endl;
		glDeleteTextures(1, &resourceID);
	}

	Resource<R_TEX>::Resource(const std::string& path, GLenum textureType, MaterialType matType) :
		path(path), textureType(textureType), materialType(matType)
	{
		resourceCount++;
		std::cout << "Texture Resource Count " << resourceCount << std::endl;
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

	std::shared_ptr<Texture> Texture::fromFile(const std::string& filename, GLenum textureType, MaterialType mType)
	{
		if (isLoaded(filename)){
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

	std::shared_ptr<Texture> Texture::cubemap(const std::string& path, const std::vector<std::string>& faces)
	{
		if (isLoaded(path)){
			return Texture::get(path);
		}
		auto tex = std::shared_ptr<Texture>(new Texture(path, GL_TEXTURE_CUBE_MAP, Diffusive));
		tex->bind();
		int width, height;
		unsigned char* image;
		for (GLuint i = 0; i < faces.size(); i++)
		{
			std::string imagePath = path + "\\" + faces[i];
			image = SOIL_load_image(imagePath.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
			if (!image){
				std::cout << "Failed to load texture at " << faces[i] << std::endl;
				tex.reset();
				return nullptr;
			}
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			SOIL_free_image_data(image);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		return tex;
	}

	void Texture::purge()
	{
		textures.clear();
	}
}
