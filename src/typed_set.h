#pragma once
#include "type_utils.h"
#include "_utility.h"

namespace cype {


//	typed set
	template <class... Types>
	class typed_set : public Types... {
	public:
		using this_type = typed_set<Types...>;
		using this_type_list = type_list<Types...>;

		template <size_t I>
		using type_of = typename this_type_list::template get<I>;

		constexpr static size_t size = sizeof...(Types);

	//	default constructor
		typed_set() = default;

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

	//	get value of specified type
		template <class T>
		T get() const {
			return {*this};
		}

		template <class T>
		void get_to(const T& out) const {
			out = get<T>();
		}

	//	Get value of specified index
		template <size_t I>
		type_of<I> get() const {
			return get<type_of<I>>();
		}

		template <class T>
		T& get_ref() {
			return {*this};
		}

		template <size_t I>
		type_of<I>& get_ref() const {
			return get_ref<type_of<I>&>();
		}
		

	//	extract value(s) of specified type(s)
		template <class... _Types>
		typed_set<_Types...> extract() const {
			return {*this};
		}

		template <class... _Types>
		void extract_to(typed_set<_Types...>& out) const {
			_void{((_Types&)out = get<_Types>(), 0)...};
		}

		template <class... _Types>
		void extarct_to(_Types&... outs) const {
			_void{(outs = get<_Types>(), 0)...};
		}

		template <class... _Types>
		typed_set<_Types&...> extract_ref() {
			return {*this};
		}


	//	Get offset of specified type
		template <class T>
		size_t offset_of() const {
		//	offset value
			static size_t offset = (size_t)-1;

			if(offset == (size_t)-1) {	
			// calc offset value
				offset = (char*)&(const T&)*this - (char*)this;
			}

			return offset;
		}


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
		template <class T>
		void set(const T& val) {
			(T&)*this = val;
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
			return (typename this_type_list::template remove<_Types...>::template apply_to<typed_set>)*this;
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

	//	call function `func` which receives one value with each own values
		template <class _Function>
		void for_each(const _Function& func) const {
			_void{(func(get<Types>()), 0)...};
		}
		
	//	call function `func` which receives one value with each own values, can modify values
		template <class _Function>
		void for_each_ref(const _Function& func) {
			_void{(func(get_ref<Types>()), 0)...};
		}

	//	call function `_StaticFunction::call` which receives one value with each own values
		template <class _StaticFunction>
		void for_each() const {
			_void{(_StaticFunction::call(get<Types>()), 0)...};
		}

	//	call function `_StaticFunction::call` which receives one value with each own values, can modify values
		template <class _StaticFunction>
		void for_each() {
			_void{(_StaticFunction::call(get_ref<Types>()), 0)...};
		}
		
	//	operate (map) with function `func`, which receives one value, on each own values
		template <class _Operator>
		auto operate(const _Operator& func) const {
			return typed_set(func(get<Types>())...);
		}
		
	//	operate with function `func`, which receives two values, on each pair of own and data's values
		template <class _Operator, class... _Types>
		auto operate(const _Operator& func, const typed_set<_Types...>& data) const {
			return typed_set(func(get<Types>(), data.template get<_Types>())...);
		}

	//	reduce with function `func`, which receives two values, from specified index (to last index)
		template <size_t I, class _Reducer, class _ValType>
		auto reduce_from(const _Reducer& reducer, const _ValType& val) const {
			if constexpr(I < size) {
				return reduce_from<_index_add<I, 1>>(reducer, reducer(val, get<I>()));
			}else{
				return val;
			}
		}
		
	//	reduce with function `func`, which receives two values
		template <class _Reducer, class _ValType>
		auto reduce(const _Reducer& reducer, const _ValType& val = 0) const {
			return reduce_from<0>(reducer, val);
		}

		
	//	operate (map) with function `_StaticOperator::call`, which receives one value, on each own values
		template <class _StaticOperator>
		auto operate() const {
			return typed_set(_StaticOperator::call(get<Types>())...);
		}
		
	//	operate with function `_StaticOperator::call`, which receives two values, on each pair of own and data's values
		template <class _StaticOperator, class... _Types>
		auto operate(const typed_set<_Types...>& data) const {
			return typed_set(_StaticOperator::call(get<Types>(), data.template get<_Types>())...);
		}

	//	reduce with function `_StaticReducer::call`, which receives two values, from specified index (to last index)
		template <size_t I, class _StaticReducer, class _ValType>
		auto reduce_from(const _ValType& val) const {
			if constexpr(I < size) {
				return reduce_from<_index_add<I, 1>, _StaticReducer>(_StaticReducer::call(val, get<I>()));
			}else{
				return val;
			}
		}
		
	//	reduce with function `_StaticReducer::call`, which receives two values
		template <class _StaticReducer, class _ValType>
		auto reduce(const _ValType& val = 0) const {
			return reduce_from<0, _StaticReducer>(val);
		}


		//template <class _Visiter>
		//auto visit_ref(_Visiter& func) {
		//	return func(get_ref<Types>()...);
		//}

		//template <class _StaticVisiter>
		//auto visit_ref() {
		//	return _StaticVisiter::call(get_ref<Types>()...);
		//}

	//	Construct specified type (apply own values to constructer arguments)
		template <class T>
		T construct() const {
			return T((Types)(*this)...);	
		}

	//	Create instances of specified type which receives one type as template argument
		template <template <class> class _Class>
		typed_set<_Class<Types>...> construct_each() const {
			return {_Class<Types>(get<Types>())...};
		}
		
	//	Generate specified type (apply own values to constructer arguments)
		template <class T>
		T* new_() const {
			return new T((Types)(*this)...);
		}

	//	Generate instances of specified type which receives one type as template argument
		template <template <class> class _Class>
		typed_set<_Class<Types>*...> new_each() const {
			return {new _Class<Types>(get<Types>())...};
		}

	};


	template <class... Types>
	auto make_set(const Types&... vals) {
		return typed_set<Types...>(vals...);
	}


}