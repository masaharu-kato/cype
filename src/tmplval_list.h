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
		template <class _ValType, _ValType... __Values>
		friend struct tmplval_list;

		using tmplval_type = ValType;

		template <ValType... __Values>
		using sameval_type = tmplval_list<ValType, __Values...>;

	//	size (number) of values
		static constexpr size_t size = 1 + sizeof...(Rests);
	
	//	push new value(s) to back
		template <ValType... _Values>
		using push_back = sameval_type<First, Rests..., _Values...>;
		
	//	push new value(s) to front
		template <ValType... _Values>
		using push_front = sameval_type<_Values..., First, Rests...>;

	//	list of 2nd and later value(s)
		using rests_list = sameval_type<Rests...>;

	private:
	//	helper function for `get`
		template <size_t _Index>
		constexpr static ValType _get() {
			if constexpr(_Index == 0) {
				return First;
			}else{
				return rests_list::template get<_Index - 1>;
			}
		}

	//	helper function for `range_of`
		template <size_t _FirstIndex, size_t _LastIndex>
		constexpr static auto _range_of() {
			if constexpr(_LastIndex  < size) {
				if constexpr(_FirstIndex <= _LastIndex) {
					return type_as_value<typename range_of<_FirstIndex + 1, _LastIndex>::template push_front<get<_FirstIndex>>>();
				}else{
					return type_as_value<sameval_type<>>();
				}
			}else{
				return type_as_value<void>();
			}
		}
			
	public:
	//	get specified value
		template <size_t _Index>
		static constexpr ValType get = _get<_Index>();

	//	get particial list
		template <size_t _FirstIndex, size_t _LastIndex>
		using range_of = typename decltype(_range_of<_FirstIndex, _LastIndex>())::type;


	//	apply own value(s) to specified class
		template <template <ValType...> class _Class>
		using apply_to = _Class<First, Rests...>;


	private:
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

	public:
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
		using union_with = typename sameval_type<First, Rests..., _Values...>::remove_duplicates;

	//	get list without specified value(s)
		template <ValType... _Values>
		using remove = std::conditional_t<
			sameval_type<_Values...>::template contains<First>,
			typename rests_list::template remove<_Values...>,
			typename rests_list::template remove<_Values...>::template push_front<First>
		>;


	private:
	////	helpder function for `insert_sorted`
	//	template <ValType __Value>
	//	static constexpr auto _insert_sorted() {
	//		if constexpr(__Value < First) {
	//			return type_as_value<push_front<__Value>>();
	//		}else{
	//			return type_as_value<typename rests_list::template insert_sorted<__Value>::template push_front<First>>();
	//		}
	//	}

	//	helper function for `merge_sorted_args`
		template <class ValList, class SortFunction>
		static constexpr auto _merge_of_sorted() {
			if constexpr(ValList::size) {
				if constexpr(SortFunction::template call<ValList::template get<0>, First>()) {
					return type_as_value<typename push_front<ValList::template get<0>>::template merge_of_sorted<typename ValList::rests_list, SortFunction>>();
				}else{
					return type_as_value<typename rests_list::template merge_of_sorted<ValList, SortFunction>::template push_front<First>>();
				}
			}else{
				return type_as_value<tmplval_list>();
			}
		}

	//	helpr function for `sorted`
		template <class SortFunction>
		static constexpr auto _sorted() {
			if constexpr(size >= 2) {
				using left_part = range_of<0, size / 2 - 1>;
				using right_part = range_of<size / 2, size - 1>;
				return type_as_value<typename left_part::template sorted<SortFunction>::template merge_of_sorted<typename right_part::template sorted<SortFunction>, SortFunction>>();
			}else{
				return type_as_value<tmplval_list>();
			}
		}


	public:
	////	insert new value (with the state that own values are sorted)
	//	template <ValType __Value>
	//	using insert_sorted = typename decltype(_insert_sorted<__Value>())::type;

	//	merge with sorted with sort function
		template <class ValList, class SortFunction>
		using merge_of_sorted = typename decltype(_merge_of_sorted<ValList, SortFunction>())::type;
		
	//	sort with sort function
		template <class SortFunction>
		using sorted = typename decltype(_sorted<SortFunction>())::type;


	//	sort function for value sorting
		struct value_sort_function : _inconstructible {
			template <ValType Val1, ValType Val2>
			static constexpr bool call() {
				return Val1 < Val2;
			}
		};


	//	merge with sorted by values
		template <class ValList>
		using merge_of_value_sorted = merge_of_sorted<ValList, value_sort_function>;

	//	sort values
		using value_sorted = sorted<value_sort_function>;

	};



	//	specialization of `list` which has no value
	template <class ValType>
	struct tmplval_list<ValType> : _inconstructible {
		
		using tmplval_type = ValType;

		static constexpr size_t size = 0;

		template <ValType... __Values>
		using sameval_type = tmplval_list<ValType, __Values...>;
	
	//	push new value(s) to back
		template <ValType... _Values>
		using push_back = sameval_type<_Values...>;
		
	//	push new value(s) to front
		template <ValType... _Values>
		using push_front = sameval_type<_Values...>;

		template <std::size_t _Index>
		using get = void;

		template <template <class...> class _Class>
		using apply = _Class<>;


		template <ValType _Value>
		static constexpr bool contains = false;

		using remove_duplicates = sameval_type<>;

		template <ValType... _Values>
		using remove = sameval_type<>;

		
	//	insert new value (with the state that own values are sorted)
		template <ValType __Value>
		using insert_sorted = sameval_type<__Value>;

	//	merge with sorted by sort function
		template <class ValList, class SortFunction>
		using merge_of_sorted = ValList;
		
	//	sort by sort function
		template <class SortFunction>
		using sorted = tmplval_list;

	//	merge with sorted by values
		template <class ValList>
		using merge_of_value_sorted = ValList;
		
	//	sort by values
		using value_sorted = tmplval_list;

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

	template <size_t _First, size_t _Last, size_t _Inv = 1>
	using index_sequence = sequence<size_t, _First, _Last, _Inv>;



}