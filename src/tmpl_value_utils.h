#pragma once
#include <type_traits>

namespace cype {

	template <class ValType>
	struct tmpl_value_utils {

		template <ValType... _Values>
		struct list;

		template <ValType First, ValType... Rests>
		struct list<First, Rests...> {
	
			template <ValType... _Values>
			using push_back = list<First, Rests..., _Values...>;
		
			template <ValType... _Values>
			using push_front = list<_Values..., First, Rests...>;

			using rests_list = list<Rests...>;

			template <std::size_t _Index>
			static constexpr ValType get = (_Index == 0) ? First : rests_list::template get<_Index - 1>;

			template <template <ValType...> class _Class>
			using apply_to = _Class<First, Rests...>;


			template <ValType _Value>
			static constexpr bool _contains() {
				if constexpr(First == _Value){
					return true;
				}else{
					if constexpr(sizeof...(Rests) != 0){
						return rests_list::template contains<_Value>;
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

			template <ValType _Value>
			constexpr static bool contains = _contains<_Value>();

			constexpr static bool is_unique = _is_unique();

			using remove_duplicates = std::conditional_t<
				rests_list::template contains<First>,
				typename rests_list::remove_duplicates,
				typename rests_list::remove_duplicates::template push_front<First>
			>;

			template <ValType... _Values>
			using union_with = typename list<First, Rests..., _Values...>::remove_duplicates;

			template <ValType... _Values>
			using remove = std::conditional_t<
				list<_Values...>::template contains<First>,
				typename rests_list::template remove<_Values...>,
				typename rests_list::template remove<_Values...>::template push_front<First>
			>;

		};


		template <>
		struct list<> {
	
			template <ValType... _Values>
			using push_back = list<_Values...>;
		
			template <ValType... _Values>
			using push_front = list<_Values...>;

			template <std::size_t _Index>
			using get = void;

			template <template <class...> class _Class>
			using apply = _Class<>;


			template <ValType _Value>
			static constexpr bool contains = false;

			using remove_duplicates = list<>;

			template <ValType... _Values>
			using remove = list<>;

		};


		
		template <ValType First, ValType Last, ValType Inv>
		struct _sequence_helper {

			static auto _type() {
				if constexpr(First <= Last) {
					return typename _sequence_helper<First + Inv, Last, Inv>::type::template push_front<First>{};
				}else{
					return list<>{};
				}
			}

			using type = decltype(_type());

		};
		
		template <ValType _First, ValType _Last, ValType _Inv = 1>
		using sequence = typename _sequence_helper<_First, _Last, _Inv>::type;

	};


}