#pragma once
#include "type_utils.h"

namespace cype {

	
//	structure for dismiss return values
	template <class T>
	struct _void {
		_void(std::initializer_list<T>) {}
	};

	
//	typed set
	template <class... Types>
	class typed_set : public Types... {
	public:
		using type_list = type_utils::list<Types...>;
		constexpr static size_t size = sizeof...(Types);

	//	construct with specified types (corresponds to template argument deduction)
		typed_set(const Types&... args)
			: Types(args)... {}

	//	construct with any types
		template <class... _Types>
		typed_set(const _Types&... vals)
			: typed_set(typed_set<_Types...>(vals...)) {}

	//	construct with data of any types
		template <class... _Types>
		typed_set(const typed_set<_Types...>& data)
			: Types(data)... {}

	//	extract value(s) of specified type(s)
		template <class... _Types>
		typed_set<_Types...> extract() const {
			return {*this};
		}

	//	get value of specified type
		template <class _Type>
		_Type get() const {
			return {*this};
		}

	//	Get value of specified index
		template <size_t _Index>
		auto get() const {
			return get<type_list::get<_Index>>();
		}


		//template <class... _Types>
		//Data<_Types&...> get_ref_data() {
		//	return {*this};
		//}

		//template <class _Type>
		//_Type& get_ref() {
		//	return {*this};
		//}

		//template <size_t _Index>
		//auto get_ref() const {
		//	return get_ref<type_list::get<_Index>>();
		//}

	//	push values of new types to front
		template <class... _Types>
		auto pushed_front(const _Types&... _vals) const {
			return typed_set<_Types..., Types...>(_vals..., *this);
		}

	//	push values of new types to back
		template <class... _Types>
		auto pushed_back(const _Types&... _vals) const {
			return typed_set<Types..., _Types...>(*this, _vals...);
		}

	//	combine typed_set of new types to front
		template <class... _Types>
		auto combine_front(const typed_set<_Types...>& _data) const {
			return typed_set<_Types..., Types...>(_data, *this);
		}
		
	//	combine typed_set of new types to back
		template <class... _Types>
		auto combine_back(const typed_set<_Types...>& _data) const {
			return typed_set<Types..., _Types...>(*this, _data);
		}


	//	set value of specified type
		template <class _Type>
		void set(const _Type& val) {
			(_Type&)*this = val;
		}

	//	set values of types
		template <class _First, class... _Rests>
		void set(const _First& first_val, _Rests&&... rest_vals) {
			set(first_val);
		}
		
	//	set with another typed_set
		template <class... _Types>
		void set(const typed_set<_Types...>& data) {
			_void{(set<_Types>(data), 0)...};
		}


	//	remove specified type(s)
		template <class... _Types>
		auto removed() const {
			return (typename type_list::template remove<_Types...>::template apply_to<typed_set>)*this;
		}

	//	overwrite with value(s) of type(s)
		template <class... _Types>
		auto overwritten(const typed_set<_Types...>& _data) const {
			return _data.combine_front(removed<_Types...>());
		}


	//	call function `func` with all own values
		template <class _Function>
		auto visit(_Function& func) const {
			return func(get<Types>()...);
		}
		
	//	call function `_StaticFunction::call` with all own values
		template <class _StaticFunction>
		auto visit() const {
			return _StaticFunction::call(get<Types>()...);
		}

	//	call function `func` receives one value with each own values
		template <class _Function>
		void for_each(_Function& func) const {
			_void{(func(get<Types>()), 0)...};
		}

	//	call function `_StaticFunction::call` receives one value with each own values
		template <class _StaticFunction>
		void for_each() const {
			_void{(_StaticFunction::call(get<Types>()), 0)...};
		}
		
	//	operate (map) with function `func` (receives one value) on each own values
		template <class _Operator>
		auto operate(_Operator& func) const {
			return typed_set(func(get<Types>())...);
		}
		
	//	operate with function `func` (receives two values) on each pair of own and data's values
		template <class _Operator, class... _Types>
		auto operate(_Operator& func, const typed_set<_Types...>& data) const {
			return typed_set(func(get<Types>(), data.get<_Types>())...);
		}
		
	//	operate (map) with function `_StaticOperator::call` (receives one value) on each own values
		template <class _StaticOperator>
		auto operate() const {
			return typed_set(_StaticOperator::call(get<Types>())...);
		}
		
	//	operate with function `_StaticOperator::call` (receives two values) on each pair of own and data's values
		template <class _StaticOperator, class... _Types>
		auto operate(const typed_set<_Types...>& data) const {
			return typed_set(_StaticOperator::call(get<Types>(), data.get<_Types>())...);
		}


		//template <class _Visiter>
		//auto visit_ref(_Visiter& func) {
		//	return func(get_ref<Types>()...);
		//}

		//template <class _StaticVisiter>
		//auto visit_ref() {
		//	return _StaticVisiter::call(get_ref<Types>()...);
		//}

		//template <class _Visiter, size_t _Index = 0>
		//void for_each_ref(_Visiter& func) {
		//	func(get_ref<_Index>());
		//	if constexpr(_Index + 1 < size) visit_each_ref(func, _Index + 1);
		//}

	//	Construct specified type (apply own values to constructer arguments)
		template <class _Type>
		_Type construct() const {
			return _Type((Types)(*this)...);	
		}
		
	//	Generate specified type (apply own values to constructer arguments)
		template <class _Type>
		_Type* new_() const {
			return new _Type((Types)(*this)...);
		}

	};

}