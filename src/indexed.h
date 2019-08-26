#pragma once
#include "tmplval_list.h"

namespace cype {

//	indexed value with `IType`(index type), `Type`(original type), indexes of each dimension
	template <class IType, class Type, IType... _DimIndexes>
	class indexed {
	public:
		using original_type = Type;
		using indexes = tmplval_list<IType, _DimIndexes...>;

		constexpr static IType index = indexes::template get<0>;

		template <std::size_t __Index>
		constexpr static IType index_of = indexes::template get<__Index>;

	private:
		original_type value;

	public:
		indexed(original_type value) noexcept
			: value(value) {}

		template <class _IType, class _Type, _IType... __DimIndexes>
		explicit indexed(const indexed<_IType, _Type, __DimIndexes...>& v)
			: value(v.value) {}

		operator original_type() const noexcept {
			return value;
		}

	};

//	preset template argument `IType`(index type), `Type`(original type) of `indexed` class
	template <class IType, class Type>
	struct indexed_of : _inconstructible {
		template <IType... _DimIndexes>
		using type = indexed<IType, Type, _DimIndexes...>;
	};

//	`indexed` class with index list
	template <class IType, class Type, class _IndexList>
	using indexed_listed = typename _IndexList::template apply_to<typename indexed_of<IType, Type>::type>;

//	convert multi-dimensional indexes to single index value
	template <class IType, class Type, IType... _Sizes>
	struct indexed_sized_of : _inconstructible {
	//	TODO: correct definition
		template <IType _DimIndex>
		using type = indexed<IType, Type, _DimIndex>;
	};



}