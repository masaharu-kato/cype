#pragma once
#include "indexed_types.h"

namespace cype {

	template <class ValType, class IType, IType _I>
	using indexed_with_itype = typename indexed_types<ValType, IType>::template single_type<_I>;


	template <size_t _I, class ValType>
	using indexed = indexed_with_itype<ValType, size_t, _I>;


//	make indexed value with index value `_I`
	template <size_t _I, class ValType>
	auto make_indexed(const ValType& value) {
		return indexed<_I, ValType>(value);
	}


}