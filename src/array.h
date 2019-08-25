#pragma once
#include "typed_set.h"
#include "tmpl_value_utils.h"
#include "indexed.h"

namespace cype {

//	array with `IType`(Index Type), `IndexedType`(Value Type) and index values
	template <class IType, template <IType> class IndexedType, IType... _Indexes>
	class array_indexes_of : public typed_set<IndexedType<_Indexes>...> {
		using base_type = typed_set<IndexedType<_Indexes>...>;

	public:
		using base_type::typed_set;

	//	construct with indexed values
		array_indexes_of(IndexedType<_Indexes>... vals) noexcept
			: base_type(vals...) {}

	//	construct with non-indexed (original) values
		array_indexes_of(typename IndexedType<_Indexes>::original_type... vals) noexcept
			: base_type(IndexedType<_Indexes>(vals)...) {}

		using base_type::get;

	//	get value of specified index
		template <IType _Index>
		IndexedType<_Index> get() const noexcept {
			return base_type::get<typename IndexedType<_Index>>();
		}
		
	//	extract values of specified indexes
		template <IType... __Indexes>
		array_indexes_of<IType, IndexedType, __Indexes...> extract() const noexcept {
			return *this;
		}

	//	get raw (original) typed value of specified index
		template <IType _Index>
		auto get_raw() const noexcept {
			return (typename IndexedType<_Index>::original_type)get<_Index>();
		}

	//	rearrange (change order of) values and get new array
		template <IType... __Indexes>
		array_indexes_of rearrange() const noexcept {
			return {get_raw<__Indexes>()...};
		}

		using base_type::set;
		

	//	set value of specified index
		template <IType _Index>
		void set(const IndexedType<_Index>& val) noexcept {
			base_type::set<typename IndexedType<_Index>>(val);
		}

	};

//	preset template arguments `IType` and `IndexedType` of `array_indexes_of` class
	template <class IType, template <IType> class IndexedType>
	struct array_type_of {
		template <IType... _Indexes>
		using indexes_of = array_indexes_of<IType, IndexedType, _Indexes...>;
	};

//	array with `IType`(index type), `IndexedType`(value type) and list of index values
	template <class IType, template <IType> class IndexedType, class IndexList>
	using array = typename IndexList::template apply_to<typename array_type_of<IType, IndexedType>::indexes_of>;
		
//	array with `IType`(index type), `IndexedType`(value type) and range of index values
	template <class IType, template <IType> class IndexedType, IType _First, IType _Last, IType _Inv = 1>
	using array_range_of = typename array<IType, IndexedType, typename tmpl_value_utils<IType>::template sequence<_First, _Last, _Inv>>;

//	array with std::size_t index, `IndexedType`(value type) and range of index values
	template <template <std::size_t> class IndexedType, std::size_t _First, std::size_t _Last>
	using array_of_indexed = array_range_of<std::size_t, IndexedType, _First, _Last>;
	
//	array with std::size_t index, specified type and range of index values
	template <class Type, std::size_t _First, std::size_t _Last>
	using array_of_type = array_range_of<std::size_t, typename indexed_of<std::size_t, Type>::type, _First, _Last>;
	

//	construct array with values
	template <class First, class... Rests>
	array_of_type<First, 0, sizeof...(Rests)> make_array(const First& first, Rests&&... rests) {
		return {first, rests...};
	}



}