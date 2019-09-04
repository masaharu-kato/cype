#pragma once
#include "typed_set.h"
#include "tmplval_list.h"
#include "indexed.h"

namespace cype {

//	array with `IType`(Index Type), `IndexedType`(index-templated value type) and index values
	template <class IType, template <IType> class IndexedType, IType... _Indexes>
	class array_index_args_of : public typed_set<IndexedType<_Indexes>...> {
		using base_type = typed_set<IndexedType<_Indexes>...>;

	public:
	//	using base_type::typed_set;
		
		array_index_args_of(const base_type& v) : base_type(v) {}

	//	construct with indexed values
		array_index_args_of(IndexedType<_Indexes>... vals) noexcept
			: base_type(vals...) {}

	//	construct with non-indexed (original) values
		array_index_args_of(typename IndexedType<_Indexes>::original_type... vals) noexcept
			: base_type(IndexedType<_Indexes>(vals)...) {}

	//	construct from different typed array
		template <template <IType> class _IndexedType>
		array_index_args_of(const array_index_args_of<IType, _IndexedType, _Indexes...>& arr)
			: base_type(IndexedType<_Indexes>(arr.template get<_Indexes>())...) {}

		using base_type::get;


	//	get value of specified index
		template <IType _Index>
		IndexedType<_Index> get() const noexcept {
			return base_type::template get<IndexedType<_Index>>();
		}
		
	//	extract values of specified indexes
		template <IType... __Indexes>
		array_index_args_of<IType, IndexedType, __Indexes...>
		extract() const noexcept {
			return {*this};
		}

	//	get raw (original) typed value of specified index
		template <IType _Index>
		auto get_raw() const noexcept {
			return (typename IndexedType<_Index>::original_type)get<_Index>();
		}

	//	rearrange (change order of) values and get new array
		template <IType... __Indexes>
		array_index_args_of rearrange() const noexcept {
			return {get_raw<__Indexes>()...};
		}

		using base_type::set;
		

	//	set value of specified index
		template <IType _Index>
		void set(const IndexedType<_Index>& val) noexcept {
			base_type::template set<IndexedType<_Index>>(val);
		}


	//	cast to another indexed-type
		template <template <IType> class _IndexedType>
		array_index_args_of<IType, _IndexedType, _Indexes...>
		cast_to() const {
			return {*this};
		}


	};



//	array with `IType`(Index Type), `ValType`(Value Type) and index values
	template <class IType, class ValType, IType... _Indexes>
	using array_type_index_args_of = array_index_args_of<size_t, _indexed_sized_of<size_t, ValType>::template type, _Indexes...>;


//	preset template arguments `IType` and `IndexedType` of `array_index_args_of` class
	template <class IType, template <IType> class IndexedType>
	struct _array_indexed_type_of : _inconstructible {
		template <IType... _Indexes>
		using indexes_of = array_index_args_of<IType, IndexedType, _Indexes...>;
	};


//	preset template arguments `IType` and `ValType` of `array_type_index_args_of` class
	template <class IType, class ValType>
	struct _array_type_indexed_type_of : _inconstructible {
		template <IType... _Indexes>
		using indexes_of = array_type_index_args_of<IType, ValType, _Indexes...>;
	};


//	array with `IType`(index type), `IndexedType`(index-templated value type) and list of index values
	template <class IndexList, template <typename IndexList::tmplval_type> class IndexedType>
	using array_indexes_of = typename IndexList::template apply_to<_array_indexed_type_of<typename IndexList::tmplval_type, IndexedType>::template indexes_of>;
		
//	array with `IType`(index type), `IndexedType`(index-templated value type) and range of index values
	template <class IType, template <IType> class IndexedType, IType _First, IType _Last, IType _Inv = 1>
	using array_range_of = array_indexes_of<sequence<IType, _First, _Last, _Inv>, IndexedType>;

//	array with size_t index, `IndexedType`(index-templated value type) and number of index values
	template <template <size_t> class IndexedType, size_t _Size>
	using array_of_indexed = array_range_of<size_t, IndexedType, 0, _Size - 1>;
	

//	array with `IType`(index type), `ValType`(value type) and list of index values
	template <class IndexList, class ValType>
	using array_type_indexes_of = typename IndexList::template apply_to<_array_type_indexed_type_of<typename IndexList::tmplval_type, ValType>::template indexes_of>;
		
//	array with `IType`(index type), `ValType`(value type) and range of index values
	template <class IType, class ValType, IType _First, IType _Last, IType _Inv = 1>
	using array_type_range_of = array_type_indexes_of<sequence<IType, _First, _Last, _Inv>, ValType>;

//	array with size_t index, `ValType`(value type) and number of index values
	template <class ValType, size_t _Size>
	using array_of_type = array_type_range_of<size_t, ValType, 0, _Size - 1>;




//	alias of array_type_of
	template <class ValType, size_t _Size>
	using array = array_of_type<ValType, _Size>;


//	construct array with values
	template <class First, class... Rests>
	array_of_type<First, sizeof...(Rests) + 1>
	make_array(const First& first, Rests&&... rests) {
		return {first, rests...};
	}


}