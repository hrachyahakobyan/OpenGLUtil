#pragma once
#include "Resource.h"
#include "Shader.h"
#define VERTEX_ATTRIB_VECTOR(vector) \
   		glEnableVertexAttribArray(initialAttrib); \
		glVertexAttribPointer(initialAttrib, vector::length(), ValueType<vector::value_type>::type, normalize, sizeof(vector), (GLvoid*)0); \
		glVertexAttribDivisor(initialAttrib, divisor); \
		finalAttrib = initialAttrib + 1;

#define VERTEX_ATTRIB_MATRIX(matrix) \
   			GLuint rowCount = matrix::col_type::length(); \
			GLuint colCount = matrix::row_type::length(); \
			for (GLuint i = 0; i < colCount; i++){ \
				glEnableVertexAttribArray(initialAttrib + i); \
				glVertexAttribPointer(initialAttrib + i, rowCount, ValueType<matrix::value_type>::type, normalize, sizeof(matrix), (GLvoid*)(i * rowCount)); \
				glVertexAttribDivisor(initialAttrib + i, divisor); \
												} \
			finalAttrib = initialAttrib + colCount

namespace glutil{


	template<typename T>
	struct ValueType{
	};

	template<>
	struct ValueType<float>{ static const int type = GL_FLOAT; };

	template<>
	struct ValueType<int>{ static const int type = GL_INT; };

	template<>
	struct ValueType<unsigned int>{ static const int type = GL_UNSIGNED_INT; };

	template<>
	struct ValueType<double>{ static const int type = GL_DOUBLE; };

	template<typename T>
	struct VertexAttribPointer{
		void operator()(GLuint initialAttrib, GLuint& finalAttrib, GLuint divisor, GLboolean normalize){
			static_assert(false, "Vertex Attribute Pointers for fundamental types is not implemented");
		}
	};

	template<typename T>
	struct VertexAttribPointer <glm::tvec4<T, glm::packed_highp>>{
		void operator()(GLuint initialAttrib, GLuint& finalAttrib, GLuint divisor, GLboolean normalize){
			typedef glm::tvec4<T, glm::packed_highp> vector;
			VERTEX_ATTRIB_VECTOR(vector);
		}
	};

	template<typename T>
	struct VertexAttribPointer <glm::tvec3<T, glm::packed_highp>>{
		void operator()(GLuint initialAttrib, GLuint& finalAttrib, GLuint divisor, GLboolean normalize){
			typedef glm::tvec3<T, glm::packed_highp> vector;
			VERTEX_ATTRIB_VECTOR(vector);
		}
	};

	template<typename T>
	struct VertexAttribPointer <glm::tvec2<T, glm::packed_highp>>{
		void operator()(GLuint initialAttrib, GLuint& finalAttrib, GLuint divisor, GLboolean normalize){
			typedef glm::tvec2<T, glm::packed_highp> vector;
			VERTEX_ATTRIB_VECTOR(vector);
		}
	};

	template<typename T>
	struct VertexAttribPointer <glm::tvec1<T, glm::packed_highp>>{
		void operator()(GLuint initialAttrib, GLuint& finalAttrib, GLuint divisor, GLboolean normalize){
			typedef glm::tvec1<T, glm::packed_highp> vector;
			VERTEX_ATTRIB_VECTOR(vector);
		}
	};

	template<typename T>
	struct VertexAttribPointer <glm::tmat4x4<T, glm::packed_highp>>{
		void operator()(GLuint initialAttrib, GLuint& finalAttrib, GLuint divisor, GLboolean normalize){
			typedef glm::tmat4x4<T, glm::packed_highp> matrix;
			VERTEX_ATTRIB_MATRIX(matrix);
		}
	};

	template<typename M>
	class ModelArrayBase
	{
	public:
		ModelArrayBase() = delete;
		ModelArrayBase(M& model);
		virtual void draw(const Shader&) const;
	protected:
		M model;
	};

	template<typename M>
	ModelArrayBase<M>::ModelArrayBase(M& m) :
		model(std::move(m))
	{
	}

	template<typename M>
	void ModelArrayBase<M>::draw(const Shader& shader) const
	{
		shader.use();
		for (GLuint i = 0; i < model.meshes.size(); i++)
		{
			glBindVertexArray(model.meshes[i].VAO);
			glDrawElementsInstanced(GL_TRIANGLES, model.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
			glBindVertexArray(0);
		}
	}
}