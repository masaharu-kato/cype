#pragma once
#include <type_traits>
#include <utility>

namespace cype {
namespace type_utils {

//	returns true if _Type1 equals to _Type2
	template <class _Type1, class _Type2>
	constexpr bool is_same = std::is_same_v<_Type1, _Type2>;

//	list of types
	template <class... Types>
	struct list;
	
//	specialization of `list` which has type(s)
	template <class First, class... Rests>
	struct list<First, Rests...> {
	
	//	push new value(s) to back
		template <class... _Types>
		using push_back = list<First, Rests..., _Types...>;
		
	//	push new value(s) to front
		template <class... _Types>
		using push_front = list<_Types..., First, Rests...>;
		
	//	list of 2nd and later value(s)
		using rests_list = list<Rests...>;
		
	//	get specified value
		template <std::size_t _Index>
		using get = std::conditional_t<
			_Index == 0,
			First,
			typename rests_list::template get<_Index - 1>
		>;

	//	apply own type(s) to specified class
		template <template <class...> class _Class>
		using apply_to = _Class<First, Rests...>;

	
	//	helper function for `contains`
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

	//	helper function for `is_unique`
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

	//	returns whether contains specified type or not
		template <class _Type>
		constexpr static bool contains = _contains<_Type>();

	//	returns whether own type(s) are unique (no duplicates)
		constexpr static bool is_unique = _is_unique();

	//	remove duplicate type(s)
		using remove_duplicates = std::conditional_t<
			rests_list::template contains<First>,
			typename rests_list::remove_duplicates,
			typename rests_list::remove_duplicates::template push_front<First>
		>;

	//	get unioned list with new type(s)
		template <class... _Types>
		using union_with = typename list<First, Rests..., _Types...>::remove_duplicates;

	//	get list without specified type(s)
		template <class... _Types>
		using remove = std::conditional_t<
			list<_Types...>::template contains<First>,
			typename rests_list::template remove<_Types...>,
			typename rests_list::template remove<_Types...>::template push_front<First>
		>;

	};


//	specialization of `list` which has no type
	template <>
	struct list<> {
	
	//	push new type(s) to back
		template <class... _Types>
		using push_back = list<_Types...>;
		
	//	push new type(s) to front
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