#pragma once
#include <type_traits>

namespace cype {

//	value (non-type) template argument utilities
	template <class ValType>
	struct tmpl_value_utils {
		
	//	list of values (indexes)
		template <ValType... _Values>
		struct list;

	//	specialization of `list` which has value(s)
		template <ValType First, ValType... Rests>
		struct list<First, Rests...> {
	
		//	push new value(s) to back
			template <ValType... _Values>
			using push_back = list<First, Rests..., _Values...>;
		
		//	push new value(s) to front
			template <ValType... _Values>
			using push_front = list<_Values..., First, Rests...>;

		//	list of 2nd and later value(s)
			using rests_list = list<Rests...>;

		//	helper function for `get`
			template <std::size_t _Index>
			constexpr static ValType _get() {
				if constexpr(_Index == 0) {
					return First;
				}else{
					return rests_list::_get<_Index - 1>();
				}
			}
			
		//	get specified value
			template <std::size_t _Index>
			static constexpr ValType get = _get<_Index>();

		//	apply own value(s) to specified class
			template <template <ValType...> class _Class>
			using apply_to = _Class<First, Rests...>;


		//	helper function for `contains`
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

		//	returns whether contains specified value or not
			template <ValType _Value>
			constexpr static bool contains = _contains<_Value>();

		//	returns whether own value(s) are unique (no duplicates)
			constexpr static bool is_unique = _is_unique();

		//	remove duplicate value(s)
			using remove_duplicates = std::conditional_t<
				rests_list::template contains<First>,
				typename rests_list::remove_duplicates,
				typename rests_list::remove_duplicates::template push_front<First>
			>;

		//	get unioned list with new value(s)
			template <ValType... _Values>
			using union_with = typename list<First, Rests..., _Values...>::remove_duplicates;

		//	get list without specified value(s)
			template <ValType... _Values>
			using remove = std::conditional_t<
				list<_Values...>::template contains<First>,
				typename rests_list::template remove<_Values...>,
				typename rests_list::template remove<_Values...>::template push_front<First>
			>;

		};


	//	specialization of `list` which has no value
		template <>
		struct list<> {
	
		//	push new value(s) to back
			template <ValType... _Values>
			using push_back = list<_Values...>;
		
		//	push new value(s) to front
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


	//	helper class for `sequence`
		template <ValType First, ValType Last, ValType Inv>
		struct _sequence_helper {

		//	helper function for `type`
			static auto _type() {
				if constexpr(First <= Last) {
					return typename _sequence_helper<First + Inv, Last, Inv>::type::template push_front<First>{};
				}else{
					return list<>{};
				}
			}

		//	sequence type (list of values)
			using type = decltype(_type());

		};
		
	//	sequence type (range from `_First` to `_Last` (includes each limits), invertal of `_Inv`)
		template <ValType _First, ValType _Last, ValType _Inv = 1>
		using sequence = typename _sequence_helper<_First, _Last, _Inv>::type;

	};


}