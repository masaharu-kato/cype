#pragma once
#include "type_utils.h"

namespace cype {
	
	template <class... Types>
	class Data : public virtual Types... {
	public:
		using This = Data<Types...>;

		template <class Type>
		static constexpr bool contains = type_utils<Types...>::contains<Type>;

		Data(const Types&... args)
			: Types(args)... {}

		template <class... _Types>
		Data(const _Types&... vals)
			: Data(Data<_Types...>(vals)) {}

		template <class... _Types>
		Data(const Data<_Types...>& data)
			: Types(data)... {}


		template <class... _Types>
		Data<_Types...> getData() const {
			return {*this};
		}

		template <class _Type>
		_Type get() const {
			return {*this};
		}

		template <class... _Types>
		Data<_Types&...> getRefData() {
			return {*this};
		}

		template <class _Type>
		_Type& getRef() {
			return {*this};
		}


		template <class _Type>
		This set(const _Type& val) {
			static_assert(contains<_Type>, "This data type does not contain specified type.");
			(_Type&)*this = val;
			return *this;
		}


		template <class _First, class... _Rests>
		This set(const _First& first_val, _Rests&&... rest_vals) {
			set(first_val);
			return set(rest_vals...);
		}
		
		template <class _First, class... _Rests>
		This set(const Data<_First, _Rests...>& data) {
			set(data.get<_First>());

		}

		template <class _Visiter>
		auto visit(const _Visiter& func) const {
			return func(get<Types>()...);
		}

		template <class _Visiter>
		auto visitRef(const _Visiter& func) {
			return func(getRef<Types>()...);
		}

		template <class _Type>
		_Type construct() const {
			return _Type((Types)(*this)...);	
		}

		template <class _Type>
		_Type* new_() const {
			return new _Type((Types)(*this)...);
		}

	};

}