#pragma once
#include "ModelArrayBase.h"

namespace glutil{

	template<typename M, typename... Types>
	class ModelArray : public ModelArrayBase<M>
	{
	public:
		typedef std::tuple<Types...> Tuple;
		typedef std::vector<Tuple> InstanceData;
		const std::size_t TypeCount = sizeof...(Types);
		const GLsizei DataSize = sizeof(Tuple)

		ModelArray() = delete;
		ModelArray(M& m);	
		ModelArray(M& m, const InstanceData& data);

		InstanceData instanceData;
		std::shared_ptr<VBO> vbo;
		GLuint initialAttrib;

		void bufferData();
	private:
	
	};

	template<typename M, typename... Types>
	ModelArray<M, Types...>::ModelArray(M& m):
		ModelArrayBase<M>(m),
		vbo(new VBO())
	{
	}

	template<typename M, typename... Types>
	ModelArray<M, Types...>::ModelArray(M& m, const InstanceData& iData) :
		ModelArrayBase<M>(m), instanceData(iData)
		vbo(new VBO())
	{
		bufferData();
	}

	template<typename M, typename... Types>
	void ModelArray<M, Types...>::bufferData()
	{
		vbo->bind();
		glBufferData(GL_ARRAY_BUFFER, instanceData.size(), &instanceData.data(), GL_STATIC_DRAW);
		GLsizei offset = 0;
		GLuint initAttrib = initialAttrib;
		GLuint finalAttrib = 0;
		for (std::size_t i = 0; i < TypeCount; i++){
			m.meshes[i].vao->bind();
			AttribHelper<TypeCount - 1, Tuple>::attrib(initAttrib, finalAttrib, 1, GL_FALSE, 0, DataSize);
			glBindVertexArray(0);
		}
	}
}


