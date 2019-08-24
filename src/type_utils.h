#pragma once
#include <type_traits>
#include <utility>

namespace cype {
namespace type_utils {

	template <class _Type1, class _Type2>
	constexpr bool is_same = std::is_same_v<_Type1, _Type2>;

	template <class... Types>
	struct list;

	template <class First, class... Rests>
	struct list<First, Rests...> {
	
		template <class... _Types>
		using push_back = list<First, Rests..., _Types...>;
		
		template <class... _Types>
		using push_front = list<_Types..., First, Rests...>;

		using rests_list = list<Rests...>;

		template <size_t _Index>
		using get = std::conditional_t<
			_Index == 0,
			First,
			typename rests_list::template get<_Index - 1>
		>;

		template <template <class...> class _Class>
		using apply_to = _Class<First, Rests...>;


		template <class Type>
		static constexpr bool _contains() {
			if constexpr(is_same<First, Type>){
				return true;
			}else{
				if constexpr(sizeof...(Rests) != 0){
					return rests_list::template contains<Type>;
				}
			}
			return false;
		}

		static constexpr bool _is_unique() {
			if constexpr(sizeof...(Rests) == 0) {
				return true;
			}else{
				if constexpr(!rests_list::template contains<First>){
					return rests_list::_is_unique();
				}
			}
			return false;
		}

		template <class _Type>
		constexpr static bool contains = _contains<_Type>();

		constexpr static bool is_unique = _is_unique();

		using remove_duplicates = std::conditional_t<
			rests_list::template contains<First>,
			typename rests_list::remove_duplicates,
			typename rests_list::remove_duplicates::template push_front<First>
		>;

		template <class... _Types>
		using union_with = typename list<First, Rests..., _Types...>::remove_duplicates;

		template <class... _Types>
		using remove = std::conditional_t<
			list<_Types...>::template contains<First>,
			typename rests_list::template remove<_Types...>,
			typename rests_list::template remove<_Types...>::template push_front<First>
		>;

	};


	template <>
	struct list<> {
	
		template <class... _Types>
		using push_back = list<_Types...>;
		
		template <class... _Types>
		using push_front = list<_Types...>;

		template <size_t _Index>
		using get = void;

		template <template <class...> class _Class>
		using apply_to = _Class<>;


		template <class _Type>
		static constexpr bool contains = false;

		using remove_duplicates = list<>;

		template <class... _Types>
		using remove = list<>;

	};

}
}