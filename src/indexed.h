
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

	};

//	preset template argument `IType`(index type), `Type`(original type) of `indexed` class
	template <class ValType, class IType>
	struct _indexed_preset_types : _static_class {
		template <IType... _DimIndexes>
		using type = _indexed_prototype<ValType, IType, _DimIndexes...>;

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

	
//	convert multi-dimensional indexes to single index value
//
//	example:
//		_SizeList = <3, 2, 4>
//
//	_DimIndex:(Indexes...)
//	0:(0, 0, 0) 1:(1, 0, 0) 2:(2, 0, 0) 3:(0, 1, 0) 4:(1, 1, 0) 5:(2, 1, 0)
//	6:(0, 0, 1) 7:(1, 0, 1) 8:(2, 0, 1) 9:(0, 1, 1) ...
//
	template <class _SizeList, template <typename _SizeList::tmplval_type...> class MultiIndexedType>
	struct _sized_indexed_of : _static_class {

		template <size_t _DimIndex>
		using type = typename _SizeList::template own_sized_indexes<_DimIndex>::template apply_to<MultiIndexedType>;

		static constexpr auto all_size = _SizeList::sum_prod();

		using indexes = index_sequence<0, all_size - 1>;

	};

	template <class _SizeList, class ValType>
	using _sized_type_of = _sized_indexed_of<_SizeList, _indexed_preset_types<ValType, typename _SizeList::tmplval_type>::template type>;


}