#pragma once

namespace glutil{
	namespace detail{

		template <class T1, class T2>
		struct SameType
		{
			static const bool value = false;
		};

		template<class T>
		struct SameType<T, T>
		{
			static const bool value = true;
		};


		template<typename... Types>
		class MultiContainer
		{
		public:
			typedef std::tuple<std::vector<Types>...> DataTypes;
			static const std::size_t TypeCount = sizeof...(Types);

			inline bool empty() const;
			std::size_t size() const
			{
				return std::get<0>(vectors).size();
			}

			template <typename T>
			std::vector<T>& access()
			{
				return MatchingField<0, T, vtype,
					VectorOfType<0, T>::value>::get(vectors);
			}

			template<typename T>
			const std::vector<T>& access() const
			{
				return MatchingField<0, T, vtype,
					VectorOfType<0, T>::value>::get(vectors);
			}

			void push_back(const Types & ... t)
			{
				std::tuple<Types...> tup = std::make_tuple(t...);
				PushTuple<sizeof...(Types)-1, Types...>::push(vectors, tup);
			}

			void erase(std::size_t index)
			{
				EraseTuple<TypeCount, index>(vectors);
			}

			std::tuple<Types...> operator[](std::size_t index) const
			{
				if (index >= size())
					throw std::exception("Index out of bounds");
				std::tuple<Types...> tuple;
				FillTuple<TypeCount - 1, Types...>().fill(vectors, tuple, index);
				return tuple;
			}

		private:
			DataTypes vectors;

			template<int N, typename T>
			struct VectorOfType : SameType<T,
				typename std::tuple_element<N, DataTypes>::type::value_type>
			{ };

			template <int N, class T, class Tuple,
				bool Match = false>
			struct MatchingField
			{
				static std::vector<T>& get(Tuple& tp)
				{
					return MatchingField<N + 1, T, Tuple,
						VectorOfType<N + 1, T>::value>::get(tp);
				}
			};

			template <int N, class T, class Tuple>
			struct MatchingField<N, T, Tuple, true>
			{
				static std::vector<T>& get(Tuple& tp)
				{
					return std::get<N>(tp);
				}
			};

			template<std::size_t N, class... T>
			struct PushTuple
			{
				static void push(DataTypes& v, const std::tuple<T...>& tuple){
					std::get<N>(v).push_back(std::get<N>(tuple));
					PushTuple<N - 1, T...>::push(v, tuple);
				}
			};

			template<class... T>
			struct PushTuple<0, T...>
			{
				static void push(DataTypes& v, const std::tuple<T...>& tuple){
					std::get<0>(v).push_back(std::get<0>(tuple));
				}
			};

			template<std::size_t N, class... T>
			struct EraseTuple
			{
				static void serase(DataTypes& v, const std::size_t index)
				{
					auto& vectorRef = std::get<N>(v);
					vectorRef.erase(vectorRef.begin() + index);
					EraseTuple<N - 1, T...>::erase(v, index);
				}
			};

			template<class... T>
			struct EraseTuple<0, T...>
			{
				static void serase(DataTypes& v, const std::size_t index)
				{
					auto& vectorRef = std::get<N>(v);
					vectorRef.erase(vectorRef.begin() + index);
				}
			};

			template<std::size_t N, class... T>
			struct FillTuple
			{
				static void fill(const DataTypes& v, std::tuple<Types...>& tuple, const std::size_t index)
				{
					std::get<N>(tuple) = std::get<N>(v)[index];
					FillTuple<N - 1, T...>::fill(v, tuple, index);
				}
			};

			template<class... T>
			struct FillTuple<0, T...>
			{
				static void fill(const DataTypes& v, std::tuple<Types...>& tuple, const std::size_t index)
				{
					std::get<0>(tuple) = std::get<0>(v)[index];
				}
			};
		};
	}
}

