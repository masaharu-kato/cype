#pragma once
#include "typed_set.h"
#include "typed.h"
#include "_utility.h"

namespace cype {

	template <class ValType, class... Types>
	struct typed_map : typed_set<typed<Types, ValType>...> {
		using base_type = typed_set<typed<Types, ValType>...>;
		using base_type::typed_set;

		explicit typed_map(const ValType& value)
			: base_type(typed<Types, ValType>(value)...) {}

		template <class _Type>
		ValType get() const {
			return base_type::template get<typed<_Type, ValType>>();
		}

		template <class _Type>
		void set(const ValType& value) {
			return base_type::template set((typed<_Type, ValType>)value);
		}

		void fill(const ValType& value) {
			_void{(set<Types>(value), 0)...};
		}

	};


	template <class ValType, class... Types>
	auto make_map(const typed<Types, ValType>&... typed_values) {
		return typed_map<ValType, Types...>(typed_values...);
	}

}