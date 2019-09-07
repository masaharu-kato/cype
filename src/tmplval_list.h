#pragma once
#include <type_traits>
#include "_utility.h"

namespace cype {

//	value (non-type like index) template list
	template <class ValType, ValType... _Values>
	class tmplval_list;

//	specialization of `list` which has value(s)
	template <class ValType, ValType First, ValType... Rests>
	class tmplval_list<ValType, First, Rests...> : _static_class {
	public:
	//	friend declaration
		template <class _ValType, _ValType... __Values>
		friend class tmplval_list;

	//	type of template value
		using tmplval_type = ValType;

	//	tmplval_list with same value type
		template <ValType... __Values>
		using sameval_type = tmplval_list<ValType, __Values...>;

	//	list of 2nd and later value(s)
		using rests_list = sameval_type<Rests...>;


	//	size (number) of values
		static constexpr size_t size = 1 + sizeof...(Rests);

	//	get sum of values
		static constexpr ValType sum() {
			return First + rests_list::sum();
		}

	//	get sum of product of values
		static constexpr ValType sum_prod(){
			return First * rests_list::sum_prod();
		}

	
	//	push new value(s) to back
		template <ValType... _Values>
		using push_back = sameval_type<First, Rests..., _Values...>;
		
	//	push new value(s) to front
		template <ValType... _Values>
		using push_front = sameval_type<_Values..., First, Rests...>;


//	public, because clang doesn't allow delctype of private functions
	public:
	//	private:
		
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

	//	helper function for `contains`
		template <ValType _Value>
		static constexpr bool _contains() {
			if constexpr(First == _Value){
				return true;
			}else{
				if constexpr(sizeof...(Rests) > 0){
					return rests_list::template contains<_Value>;
				}
			}
			return false;
		}

	//	helper function for `index_of`
		template <ValType _Value>
		constexpr static size_t _index_of() {
			if constexpr(First == _Value){
				return 0;
			}else{
				if constexpr(sizeof...(Rests) > 0){
					return rests_list::template index_of<_Value> + 1;
				}
			}
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


	//	helper function for `merge_sorted_args`
		template <class ValList, class SortFunction>
		static constexpr auto _merge_of_sorted() {
			if constexpr(ValList::size > 0) {
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
	//	get specified value
		template <size_t _Index>
		static constexpr ValType get = _get<_Index>();

	//	get particial list
		template <size_t _FirstIndex, size_t _LastIndex>
		using range_of = typename decltype(_range_of<_FirstIndex, _LastIndex>())::type;


	//	apply own value(s) to specified class
		template <template <ValType...> class _Class>
		using apply_to = _Class<First, Rests...>;


	//	returns whether contains specified value or not
		template <ValType _Value>
		constexpr static bool contains = _contains<_Value>();

	//	returns the location (index) of contains specified value
		template <ValType _Value>
		constexpr static size_t index_of = _index_of<_Value>();

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

	//	merge with sorted with sort function
		template <class ValList, class SortFunction>
		using merge_of_sorted = typename decltype(_merge_of_sorted<ValList, SortFunction>())::type;
		
	//	sort with sort function
		template <class SortFunction>
		using sorted = typename decltype(_sorted<SortFunction>())::type;


	//	sort function for value sorting
		struct value_sort_function : _static_class {
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
		
		template <ValType __Index>
		static constexpr auto _own_sized_indexes() {
			if constexpr(First > 0) {
				if constexpr(size > 1) {
					return type_as_value<typename decltype(rests_list::template _own_sized_indexes<__Index / First>())::type::template push_front<__Index % First>>();
				}else{
					return type_as_value<sameval_type<__Index>>();
				}
			}else{
				return type_as_value<void>();
			}
		}


	//	visit static function
		template <class StaticFunction>
		constexpr static auto visit() {
			return StaticFunction::call(First, Rests...);
		}

	//	visit dynamic function
		template <class Function>
		constexpr static auto visit(const Function& func) {
			return func(First, Rests...);
		}

	};


	//	specialization of `list` which has no value
	template <class ValType>
	class tmplval_list<ValType> : _static_class {
	public:
	//	friend declaration
		template <class _ValType, _ValType... __Values>
		friend class tmplval_list;

	//	type of template value
		using tmplval_type = ValType;

	//	size (number) of values
		static constexpr size_t size = 0;

	//	get sum of values
		static constexpr ValType sum() {
			return 0;
		}

	//	get sum of product of values
		static constexpr ValType sum_prod() {
			return 1;
		}

	//	
		template <ValType... __Values>
		using sameval_type = tmplval_list<ValType, __Values...>;
	
	//	push new value(s) to back
		template <ValType... _Values>
		using push_back = sameval_type<_Values...>;
		
	//	push new value(s) to front
		template <ValType... _Values>
		using push_front = sameval_type<_Values...>;

	//	get (always return void)
		template <std::size_t _Index>
		using get = void;

	//	apply (apply nothing)
		template <template <class...> class _Class>
		using apply = _Class<>;

	//	contains (always return false)
		template <ValType _Value>
		static constexpr bool contains = false;

	//	remove duplicate value(s) (return same type)
		using remove_duplicates = tmplval_list;

	//	get unioned (return only _Values contained list)
		template <ValType... _Values>
		using union_with = sameval_type<_Values...>;

	//	get removed (return same type)
		template <ValType... _Values>
		using remove = tmplval_list;

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
		

	//	visit static function
		template <class StaticFunction>
		constexpr static auto visit() {
			return StaticFunction::call();
		}

	//	visit dynamic function
		template <class Function>
		constexpr static auto visit(const Function& func) {
			return func();
		}
	};


//	alias of tmplval_list when `ValType` = `size_t` 
	template <size_t... _Values>
	using tmplidx_list = tmplval_list<size_t, _Values...>;


	

//	helper class for `sequence`
	template <class ValType, ValType First, ValType Last, class DiffType = ValType, DiffType Inv = 1>
	struct _sequence_helper : _static_class {

	//	helper function for `type`
		static auto _type() {
			if constexpr(First <= Last) {
				return type_as_value<typename _sequence_helper<ValType, First + Inv, Last, DiffType, Inv>::type::template push_front<First>>();
			}else{
				return type_as_value<tmplval_list<ValType>>();
			}
		}

	//	sequence type (list of values)
		using type = typename decltype(_type())::type;

	};
	

//	sequence type (range from `_First` to `_Last` (includes each limits), invertal of `_Inv`)
	template <class ValType, ValType _First, ValType _Last, class DiffType = ValType, DiffType _Inv = 1>
	using sequence = typename _sequence_helper<ValType, _First, _Last, DiffType, _Inv>::type;

	template <size_t _First, size_t _Last, diff_t _Inv = 1>
	using index_sequence = sequence<size_t, _First, _Last, diff_t, _Inv>;



}