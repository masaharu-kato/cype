#pragma once
#include <type_traits>
#include <utility>

namespace cype::type_utils {

	template <class Type1, class Type2>
	constexpr bool is_same = std::is_same_v<Type1, Type2>;

	template <class... Types>
	struct list;

	template <class First, class... Rests>
	struct list<First, Rests...> {
	
		template <class... Types>
		using push_back = list<First, Rests..., Types...>;
		
		template <class... Types>
		using push_front = list<Types..., First, Rests...>;

		using rests_list = list<Rests...>;

		template <class Type>
		static constexpr bool _contains() {
			if constexpr(is_same<First, Type>){
				return true;
			}else{
				if constexpr(sizeof...(Rests)){
					return rests_list::template contains<Type>;
				}
			}
			return false;
		}

		static constexpr bool _is_unique() {
			if constexpr(!sizeof...(Rests)) {
				return true;
			}else{
				if constexpr(!rests_list::template contains<First>){
					return rests_list::_is_unique();
				}
			}
			return false;
		}

		template <class Type>
		constexpr static bool contains = _contains<Type>();

		constexpr static bool is_unique = _is_unique();

		using remove_duplicates = std::conditional_t<
			rests_list::template contains<First>,
			typename rests_list::remove_duplicates,
			typename rests_list::remove_duplicates::template push_front<First>
		>;

		template <class... Types>
		using union_with = typename list<First, Rests..., Types...>::remove_duplicates;

		template <class... Types>
		using remove = std::conditional_t<
			list<Types...>::template contains<First>,
			typename rests_list::template remove<Types...>,
			typename rests_list::template remove<Types...>::template push_front<First>
		>;

	};


	template <>
	struct list<> {
	
		template <class... Types>
		using push_back = list<Types...>;
		
		template <class... Types>
		using push_front = list<Types...>;

		template <class Type>
		static constexpr bool contains = false;

		using remove_duplicates = list<>;

		template <class... Types>
		using remove = list<>;

	};

}