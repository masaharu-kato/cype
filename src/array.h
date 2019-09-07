#pragma once
#include "typed_set.h"
#include "tmplval_list.h"
#include "indexed.h"

namespace cype {

//	array with `IType`(Index Type), `IndexedType`(index-templated value type) and index values
	template <class IType, template <IType> class IndexedType, IType... _Indexes>
	class array_index_args_of : public typed_set<IndexedType<_Indexes>...> {
		using base_type = typed_set<IndexedType<_Indexes>...>;

	//	to avoid MSVC's bug
		template <IType _Index>
		using original_type_at = typename IndexedType<_Index>::original_type;

	public:
		using this_type = array_index_args_of;

		template <IType _I>
		using my_indexed_type = IndexedType<_I>;

		using my_indexes = tmplval_list<IType, _Indexes...>;

		using base_type::base_type;
		using base_type::get;
		using base_type::get_ref;
		using base_type::extract;
		using base_type::extract_to;
		using base_type::set;
		using base_type::size;


	//	default constructor
		array_index_args_of() = default;
	
	//	construct from base type	
		array_index_args_of(const base_type& v) : base_type(v) {}

	//	construct with indexed values
		array_index_args_of(IndexedType<_Indexes>... vals) noexcept
			: base_type(vals...) {}

	//	construct with non-indexed (original) values
		array_index_args_of(original_type_at<_Indexes>... vals) noexcept
		 	: base_type(IndexedType<_Indexes>(vals)...) {}

	//	construct from different typed array
		template <template <IType> class _IndexedType>
		array_index_args_of(const array_index_args_of<IType, _IndexedType, _Indexes...>& arr)
			: base_type(IndexedType<_Indexes>(arr.template get<_Indexes>())...) {}

	//	construct with single value
		template <class _Type>
		explicit array_index_args_of(const typename IndexedType<0>::original_type& val)
			: base_type(IndexedType<_Indexes>(val)...) {}


	//	get value of specified index
		template <IType _Index>
		IndexedType<_Index>
		get() const noexcept {
			return base_type::template get<IndexedType<_Index>>();
		}
		
	//	extract values of specified indexes
		template <IType... __Indexes>
		array_index_args_of<IType, IndexedType, __Indexes...>
		extract() const noexcept {
			return {*this};
		}

	//	extract values to other indexed array
		template <IType... __Indexes>
		void extract_to(array_index_args_of<IType, IndexedType, __Indexes...>& out) noexcept {
			_void{(out.set<__Indexes>(get<__Indexes>()), 0)...};
		}

	//	get raw (original) typed value of specified index
		template <IType _Index>
		typename IndexedType<_Index>::original_type
		get_raw() const noexcept {
			return (typename IndexedType<_Index>::original_type)get<_Index>();
		}

	//	rearrange (change order of) values and get new array
		template <IType... __Indexes>
		this_type rearrange() const noexcept {
			return {get_raw<__Indexes>()...};
		}

	//	reindex (set new indexes to) values and get new array
		template <IType... __Indexes>
		array_index_args_of<IType, IndexedType, __Indexes...>
		reindex_with_args() const noexcept {
			return {get_raw<_Indexes>()...};
		}

	private:
	//	helper class for function call with index list
		template <IType... __Indexes>
		struct with_index_args_of : _static_class {
			
			static this_type
			rearrange(const this_type& arr) noexcept {
				return arr.rearrange<__Indexes...>();
			}

			static array_index_args_of<IType, IndexedType, __Indexes...>
			reindex_with_args(const this_type& arr) noexcept {
				return arr.reindex_with_args<__Indexes...>();
			}

		};

	public:
		template <class IndexList>
		this_type rearrange() const noexcept {
			return IndexList::template apply_to<with_index_args_of>::rearrange(*this);
		}
		
		template <class IndexList>
		auto reindex() const noexcept {
			return IndexList::template apply_to<with_index_args_of>::reindex_with_args(*this);
		}

		template <IType _IStart>
		auto reindex() const noexcept {
			return reindex<sequence<IType, _IStart, _IStart + size - 1>>();
		}

		auto reindex() const noexcept {
			return reindex<0>();
		}

		

	//	set value of specified index
		template <IType _Index>
		void set(const IndexedType<_Index>& val) noexcept {
			base_type::template set(val);
		}

	//	set value of specified index using raw (original) typed value
		template <IType _Index>
		void set(const typename IndexedType<_Index>::original_type& val) noexcept {
			base_type::template set((const IndexedType<_Index>&)val);
		}


	//	fill same value to all indexes
		template <class _Type>
		void fill(const _Type& val) noexcept {
			_void{(set<_Indexes>((const typename IndexedType<_Indexes>::original_type&)val), 0)...};
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
	using array_type_index_args_of = array_index_args_of<size_t, indexed_types<ValType, size_t>::template single_type, _Indexes...>;


//	preset template arguments `IType` and `IndexedType` of `array_index_args_of` class
	template <class IType, template <IType> class IndexedType>
	struct _array_preset_indexed : _static_class {
		template <IType... _Indexes>
		using indexes_of = array_index_args_of<IType, IndexedType, _Indexes...>;
	};


//	preset template arguments `IType` and `ValType` of `array_type_index_args_of` class
	template <class IType, class ValType>
	struct _array_preset_valtype : _static_class {
		template <IType... _Indexes>
		using indexes_of = array_type_index_args_of<IType, ValType, _Indexes...>;
	};


//	array with `IType`(index type), `IndexedType`(index-templated value type) and list of index values
	template <class IndexList, template <typename IndexList::tmplval_type> class IndexedType>
	using array_indexes_of = typename IndexList::template apply_to<_array_preset_indexed<typename IndexList::tmplval_type, IndexedType>::template indexes_of>;
		
//	array with `IType`(index type), `IndexedType`(index-templated value type) and range of index values
	template <class IType, template <IType> class IndexedType, IType _First, IType _Last, class DiffType = IType, DiffType _Inv = 1>
	using array_range_of = array_indexes_of<
		sequence<IType, _First, _Last, DiffType, _Inv>,
		IndexedType
	>;

//	array with `size_t` index, `IndexedType`(index-templated value type) and number of index values
	template <template <size_t> class IndexedType, size_t _Size>
	using array_of_indexed = array_range_of<size_t, IndexedType, 0, _Size - 1>;
	

//	array with `IType`(index type), `ValType`(value type) and list of index values
	template <class IndexList, class ValType>
	using array_type_indexes_of = typename IndexList::template apply_to<_array_preset_valtype<typename IndexList::tmplval_type, ValType>::template indexes_of>;
		
//	array with `IType`(index type), `ValType`(value type) and range of index values
	template <class IType, class ValType, IType _First, IType _Last, class DiffType = IType, DiffType _Inv = 1>
	using array_type_range_of = array_type_indexes_of<
		sequence<IType, _First, _Last, DiffType, _Inv>,
		ValType
	>;

//	array with size_t index, `ValType`(value type) and number of index values
	template <class ValType, size_t _Size>
	using array_of_type = array_type_range_of<size_t, ValType, 0, _Size - 1>;
	

//	alias of array_type_of
	template <class ValType, size_t _Size>
	using array = array_of_type<ValType, _Size>;


//	natural typed array (index starts from 1, not 0)
	template <class ValType, size_t _Size>
	using natural_array = array_type_range_of<size_t, ValType, 1, _Size>;


//	construct array with values
	template <class First, class... Rests>
	array_of_type<First, sizeof...(Rests) + 1>
	make_array(const First& first, Rests&&... rests) {
		return {first, rests...};
	}


}
