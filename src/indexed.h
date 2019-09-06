
#pragma once
#include "tmplval_list.h"
#include "_utility.h"

namespace cype {

//	indexed value with `IType`(index type), `ValType`(original type), indexes of each dimension
	template <class ValType, class IType, IType... _DimIndexes>
	class _indexed_prototype {
	public:
		using original_type = ValType;
		using indexes = tmplval_list<IType, _DimIndexes...>;

		constexpr static IType index = indexes::template get<0>;

		template <std::size_t __Index>
		constexpr static IType index_of = indexes::template get<__Index>;

	private:
		original_type _value;

	public:
	//	default constructor
		_indexed_prototype() = default;

	//	construct with original type
		_indexed_prototype(original_type _value) noexcept
			: _value(_value) {}

		template <class _IType, class _Type, _IType... __DimIndexes>
		explicit _indexed_prototype(const _indexed_prototype<_IType, _Type, __DimIndexes...>& v)
			: _value(v.value()) {}

		operator original_type() const noexcept {
			return _value;
		}

		original_type value() const noexcept {
			return _value;
		}

		original_type& value() noexcept {
			return _value;
		}

	};

//	preset template argument `IType`(index type), `Type`(original type) of `indexed` class
	template <class ValType, class IType>
	struct _indexed_preset_types : _static_class {
		template <IType... _DimIndexes>
		using multi_type = _indexed_prototype<ValType, IType, _DimIndexes...>;

		template <IType _DimIndex>
		using single_type = _indexed_prototype<ValType, IType, _DimIndex>;
	};

//	`indexed` class with index list
	template <class ValType, class _IndexList>
	using _indexed_listed = typename _IndexList::template apply_to<typename _indexed_preset_types<typename _IndexList::tmplval_type, ValType>::type>;

//	single dimensional `indexed` class
	template <class ValType, size_t _Index>
	using indexed = _indexed_prototype<size_t, ValType, _Index>;
	
//	multi dimensional `indexed` class
	template <class ValType, size_t... _Indexes>
	using multi_indexed = _indexed_prototype<size_t, ValType, _Indexes...>;

//	make indexed value with index value `_Index`
	template <size_t _Index, class ValType>
	auto make_indexed(const ValType& value) {
		return indexed<ValType, _Index>(value);
	}


	template <class IType, IType _Index, IType... Sizes>
	struct _misa_sized_indexes_type {
		constexpr static auto _call() {
			return type_as_value<tmplval_list<IType>>();
		}
		using type = typename decltype(_call())::type;
	};


	template <class IType, IType _Index, IType FirstSize, IType... RestSizes>
	struct _misa_sized_indexes_type<IType, _Index, FirstSize, RestSizes...> {
		constexpr static auto _call() {
			if constexpr(sizeof...(RestSizes) > 0) {
				return type_as_value<typename _misa_sized_indexes_type<IType, _Index / FirstSize, RestSizes...>::type::template push_front<_Index % FirstSize>>();
			}else{
				return type_as_value<tmplval_list<IType, _Index>>();
			}
		}
		using type = typename decltype(_call())::type;
	};

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