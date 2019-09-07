#pragma once
#include "indexed_types.h"

namespace cype {
	
//	multi dimensional `indexed` class
	template <class ValType, size_t... _Ies>
	using multi_indexed = typename indexed_types<ValType, size_t>::template multi_type<_Ies...>;


//	helper class for `_md_indexed_size_args_of`
	template <class IType, IType _I, IType... Sizes>
	struct _misa_sized_indexes_type : _static_class {
		constexpr static auto _call() {
			return type_as_value<tmplval_list<IType>>();
		}
		using type = typename decltype(_call())::type;
	};

	
//	specialization of `_misa_sized_indexes_type`
	template <class IType, IType _I, IType FirstSize, IType... RestSizes>
	struct _misa_sized_indexes_type<IType, _I, FirstSize, RestSizes...> : _static_class {
		constexpr static auto _call() {
			if constexpr(sizeof...(RestSizes) > 0) {
				return type_as_value<typename _misa_sized_indexes_type<IType, _I / FirstSize, RestSizes...>::type::template push_front<_I % FirstSize>>();
			}else{
				return type_as_value<tmplval_list<IType, _I>>();
			}
		}
		using type = typename decltype(_call())::type;
	};

//	calulation for `_md_indexed_size_args_of`
	template <class IType, IType First, IType... Rests>
	constexpr IType _sum_product() {
		if constexpr(sizeof...(Rests) > 0) {
			return First * _sum_product<IType, Rests...>();
		}else{
			return First;
		}
	}
	
//	convert multi-dimensional indexes to single index value
//
//	example:
//		_SizeList = <3, 2, 4>
//
//	_DimIndex:(Indexes...)
//	0:(0, 0, 0) 1:(1, 0, 0) 2:(2, 0, 0) 3:(0, 1, 0) 4:(1, 1, 0) 5:(2, 1, 0)
//	6:(0, 0, 1) 7:(1, 0, 1) 8:(2, 0, 1) 9:(0, 1, 1) ...
//
	template <class IType, template <IType...> class MultiIndexedType, IType... _Sizes>
	struct _md_indexed_size_args_of : _static_class {

		template <size_t _DimIndex>
		using type = typename _misa_sized_indexes_type<IType, _DimIndex, _Sizes...>::type::template apply_to<MultiIndexedType>;

		static constexpr auto all_size = _sum_product<IType, _Sizes...>();

		using indexes = index_sequence<0, all_size - 1>;

	};


}
