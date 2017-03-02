#pragma once
#include "ModelArrayBase.h"
#include "MultiContainer.h"

namespace glutil{

	template<typename M, typename... Types>
	class ModelArray : public ModelArrayBase<M>
	{
	public:
		typedef typename detail::MultiContainer<Types...>::DataTypes InstanceDataTypes;

		ModelArray() = delete;
		ModelArray(M& m);

		template <typename T>
		std::vector<T>& access()
		{
			return container.access<T>();
		}

		template<typename T>
		const std::vector<T>& access() const
		{
			return container.access<T>();
		}

		void push(const Types & ... t)
		{
			container.push(t...);
		}

		void erase(std::size_t index)
		{
			container.erase(index);
		}


	private:
		typedef detail::MultiContainer<Types...> Container;
		Container container;
		std::array<std::shared_ptr<VBO>, sizeof...(Types)> vbos;
	};
}


