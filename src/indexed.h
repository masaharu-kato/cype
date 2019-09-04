
#pragma once
#include "tmplval_list.h"
#include "_utility.h"

namespace cype {

//	indexed value with `IType`(index type), `Type`(original type), indexes of each dimension
	template <class IType, class ValType, IType... _DimIndexes>
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
	template <class IType, class Type>
	struct _indexed_of : _inconstructible {
		template <IType... _DimIndexes>
		using type = _indexed_prototype<IType, Type, _DimIndexes...>;
	};

//	`indexed` class with index list
	template <class IType, class Type, class _IndexList>
	using _indexed_listed = typename _IndexList::template apply_to<typename _indexed_of<IType, Type>::type>;

//	convert multi-dimensional indexes to single index value
	template <class IType, class Type, IType... _Sizes>
	struct _indexed_sized_of : _inconstructible {
	//	TODO: correct definition
		template <IType _DimIndex>
		using type = _indexed_prototype<IType, Type, _DimIndex>;
	};


	template <class ValType, size_t _Index>
	using indexed = _indexed_prototype<size_t, ValType, _Index>;
	
	template <class ValType, size_t... _Indexes>
	using multi_indexed = _indexed_prototype<size_t, ValType, _Indexes...>;


	template <size_t _Index, class ValType>
	auto make_indexed(const ValType& value) {
		return indexed<ValType, _Index>(value);
	}

}