#include "stdafx.h"
#include "Resource.h"

namespace glutil{
	unsigned int ResourceBase::resourceCount = 0;

	ResourceBase::ResourceBase()
	{
		resourceCount++;
		std::cout << "Global Resource count " << resourceCount << std::endl;
	}

	ResourceBase::~ResourceBase()
	{
		resourceCount--;
		std::cout << "Global Resource count " << resourceCount << std::endl;
	}

	unsigned int Resource<R_VAO>::resourceCount = 0;

	Resource<R_VAO>::Resource()
	{
		glGenVertexArrays(1, &resourceID);
		resourceCount++;
		std::cout << "VAO Resource count " << resourceCount << std::endl;
	}

	Resource<R_VAO>::~Resource()
	{
		glDeleteVertexArrays(1, &resourceID);
		resourceCount--;
		std::cout << "VAO Resource count " << resourceCount << std::endl;
	}

	void Resource<R_VAO>::bind() const
	{
		glBindVertexArray(resourceID);
	}

	unsigned int Resource<R_VBO>::resourceCount = 0;

	Resource<R_VBO>::Resource()
	{
		glGenBuffers(1, &resourceID);
		resourceCount++;
		std::cout << "Buffer Object Resource count " << resourceCount << std::endl;
	}

	Resource<R_VBO>::~Resource()
	{
		glDeleteBuffers(1, &resourceID);
		resourceCount--;
		std::cout << "Buffer Object Resource count " << resourceCount << std::endl;
	}

	void Resource<R_VBO>::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, resourceID);
	}

	unsigned int Resource<R_EBO>::resourceCount = 0;

	Resource<R_EBO>::Resource()
	{
		glGenBuffers(1, &resourceID);
		resourceCount++;
		std::cout << "Element array Resource count " << resourceCount << std::endl;
	}

	Resource<R_EBO>::~Resource()
	{
		glDeleteBuffers(1, &resourceID);
		resourceCount--;
		std::cout << "Element array Resource count " << resourceCount << std::endl;
	}
}
