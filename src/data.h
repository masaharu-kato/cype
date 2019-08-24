#pragma once
#include "type_utils.h"

namespace cype {

	
	template <class T>
	struct _void {
		_void(std::initializer_list<T>) {}
	};

	
	template <class... Types>
	class Data : public Types... {
	public:
		using type_list = type_utils::list<Types...>;
		constexpr static size_t size = sizeof...(Types);

	//	Construct with specified types (corresponds to template argument deduction)
		Data(const Types&... args)
			: Types(args)... {}

	//	Construct with any types
		template <class... _Types>
		Data(const _Types&... vals)
			: Data(Data<_Types...>(vals...)) {}

	//	Construct with data of any types
		template <class... _Types>
		Data(const Data<_Types...>& data)
			: Types(data)... {}

	//	Get data of particial types
		template <class... _Types>
		Data<_Types...> get_data() const {
			return {*this};
		}

	//	Get value of specified type
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

		template <class... _Types>
		auto pushed_front(const _Types&... _vals) const {
			return Data<_Types..., Types...>(_vals..., *this);
		}

		template <class... _Types>
		auto pushed_back(const _Types&... _vals) const {
			return Data<Types..., _Types...>(*this, _vals...);
		}

		template <class... _Types>
		auto pushed_data_front(const Data<_Types...>& _data) const {
			return Data<_Types..., Types...>(_data, *this);
		}

		template <class... _Types>
		auto pushed_data_back(const Data<_Types...>& _data) const {
			return Data<Types..., _Types...>(*this, _data);
		}


		template <class _Type>
		void set(const _Type& val) {
			(_Type&)*this = val;
		}


		template <class _First, class... _Rests>
		void set(const _First& first_val, _Rests&&... rest_vals) {
			set(first_val);
		}
		
		template <class... _Types>
		void set(const Data<_Types...>& data) {
			_void{(set<_Types>(data), 0)...};
		}


		template <class... _Types>
		auto removed() const {
			return (typename type_list::remove<_Types...>::apply<Data>)*this;
		}


		template <class... _Types>
		auto overwritten(const Data<_Types...>& _data) const {
			return _data.pushed_data_front(removed<_Types...>());
		}


		template <class _Visiter>
		auto visit(_Visiter& func) const {
			return func(get<Types>()...);
		}

		template <class _StaticVisiter>
		auto visit() const {
			return _StaticVisiter::call(get<Types>()...);
		}
		
		template <class _Visiter>
		void visit_each(_Visiter& func) const {
			_void{(func(get<Types>()), 0)...};
		}
		
		template <class _Operator>
		auto operate(_Operator& func) const {
			return Data(func(get<Types>())...);
		}

		template <class _Operator, class... _Types>
		auto operate(_Operator& func, const Data<_Types...>& data) const {
			return Data(func(get<Types>(), data.get<_Types>())...);
		}

		template <class _StaticOperator>
		auto operate() const {
			return Data(_StaticOperator::call(get<Types>())...);
		}

		template <class _StaticOperator, class... _Types>
		auto operate(const Data<_Types...>& data) const {
			return Data(_StaticOperator::call(get<Types>(), data.get<_Types>())...);
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
		//void visit_each_ref(_Visiter& func) {
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