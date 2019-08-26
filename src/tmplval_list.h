#pragma once
#include <type_traits>
#include "_utility.h"

namespace cype {

//	value (non-type like index) template list
	template <class ValType, ValType... _Values>
	struct tmplval_list;

//	specialization of `list` which has value(s)
	template <class ValType, ValType First, ValType... Rests>
	struct tmplval_list<ValType, First, Rests...> : _inconstructible {
	
	//	push new value(s) to back
		template <ValType... _Values>
		using push_back = tmplval_list<ValType, First, Rests..., _Values...>;
		
	//	push new value(s) to front
		template <ValType... _Values>
		using push_front = tmplval_list<ValType, _Values..., First, Rests...>;

	//	list of 2nd and later value(s)
		using rests_list = tmplval_list<ValType, Rests...>;

	//	helper function for `get`
		template <std::size_t _Index>
		constexpr static ValType _get() {
			if constexpr(_Index == 0) {
				return First;
			}else{
				return rests_list::template _get<_Index - 1>();
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
		using union_with = typename tmplval_list<ValType, First, Rests..., _Values...>::remove_duplicates;

	//	get list without specified value(s)
		template <ValType... _Values>
		using remove = std::conditional_t<
			tmplval_list<ValType, _Values...>::template contains<First>,
			typename rests_list::template remove<_Values...>,
			typename rests_list::template remove<_Values...>::template push_front<First>
		>;

	};



	//	specialization of `list` which has no value
	template <class ValType>
	struct tmplval_list<ValType> : _inconstructible {
	
	//	push new value(s) to back
		template <ValType... _Values>
		using push_back = tmplval_list<ValType, _Values...>;
		
	//	push new value(s) to front
		template <ValType... _Values>
		using push_front = tmplval_list<ValType, _Values...>;

		template <std::size_t _Index>
		using get = void;

		template <template <class...> class _Class>
		using apply = _Class<>;


		template <ValType _Value>
		static constexpr bool contains = false;

		using remove_duplicates = tmplval_list<ValType>;

		template <ValType... _Values>
		using remove = tmplval_list<ValType>;

	};


	

//	helper class for `sequence`
	template <class ValType, ValType First, ValType Last, ValType Inv>
	struct _sequence_helper : _inconstructible {

	//	helper function for `type`
		static auto _type() {
			if constexpr(First <= Last) {
				return typename _sequence_helper<ValType, First + Inv, Last, Inv>::type::template push_front<First>{};
			}else{
				return tmplval_list<ValType>{};
			}
		}

	//	sequence type (list of values)
		using type = decltype(_type());

	};
		
//	sequence type (range from `_First` to `_Last` (includes each limits), invertal of `_Inv`)
	template <class ValType, ValType _First, ValType _Last, ValType _Inv = 1>
	using sequence = typename _sequence_helper<ValType, _First, _Last, _Inv>::type;



}