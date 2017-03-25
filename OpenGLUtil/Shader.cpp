#include "stdafx.h"
#include "Shader.h"

namespace glutil{
	Shader::Shader()
	{
	}

	Shader::~Shader()
	{
		glDeleteProgram(this->Program);
	}

	std::shared_ptr<Shader> Shader::fromFile(const std::string& vertexPath, const std::string& fragmentPath)
	{
		// 1. Retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensures ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);
		try
		{
			// Open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			if (vShaderFile.fail() || fShaderFile.fail())
			{
				std::cout << "Failed to read file" << std::endl;
				vShaderFile.close();
				fShaderFile.close();
				return nullptr;
			}
			std::stringstream vShaderStream, fShaderStream;
			// Read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// Convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
			vShaderFile.close();
			fShaderFile.close();
			return nullptr;
		}
		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar * fShaderCode = fragmentCode.c_str();
		std::shared_ptr<Shader> shader(new Shader());
		try{
			shader->initialize(vShaderCode, fShaderCode);
		}
		catch (...){
			shader.reset();
		}
		return shader;
	}

	std::shared_ptr<Shader> Shader::fromCode(const GLchar* vertexCode, const GLchar* fragmentCode)
	{
		std::shared_ptr<Shader> shader(new Shader());
		try{
			shader->initialize(vertexCode, fragmentCode);
		}
		catch (...){
			shader.reset();
		}
		return shader;
	}

	std::shared_ptr<Shader> Shader::fromCode(const std::string& vertexCode, const std::string& fragmentCode)
	{
		std::shared_ptr<Shader> shader(new Shader());
		try{
			shader->initialize(static_cast<const GLchar*>(vertexCode.c_str()), static_cast<const GLchar*>(fragmentCode.c_str()));
		}
		catch (...){
			shader.reset();
		}
		return shader;
	}

	void Shader::initialize(const GLchar* vShaderCode, const GLchar* fShaderCode)
	{
		// 2. Compile shaders
		GLuint vertex, fragment;
		GLint success;
		GLchar infoLog[512];
		// Vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		// Print compile errors if any
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			glDeleteShader(vertex);
			throw std::exception();
		}
		// Fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		// Print compile errors if any
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			glDeleteShader(vertex);
			glDeleteShader(fragment);
			throw std::exception();
		}
		// Shader Program
		this->Program = glCreateProgram();
		glAttachShader(this->Program, vertex);
		glAttachShader(this->Program, fragment);
		glLinkProgram(this->Program);
		// Print linking errors if any
		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			glDeleteShader(vertex);
			glDeleteShader(fragment);
			glDeleteProgram(this->Program);
			throw std::exception();
		}
		// Delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
}