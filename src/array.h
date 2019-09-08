#pragma once
#include "typed_set.h"
#include "tmplval_list.h"
#include "indexed.h"

namespace cype {

//	array with index type, index-templated value type, index values
	template <
		class IType,					//	index type
		template <IType> class Idxed	//	index-templated value type
	>
	struct array_itype_idxed_of : _static_class {

		template <IType... Is>						//	index value(s)
		class iargs_of : public typed_set<Idxed<Is>...> {
			using base_type = typed_set<Idxed<Is>...>;

		//	to avoid MSVC's bug
			template <IType I>
			using original_type_at = typename Idxed<I>::original_type;

		public:
			using this_type = iargs_of;
			using this_indexes = tmplval_list<IType, Is...>;

			template <IType _I>
			using this_indexed_type = Idxed<_I>;

			using base_type::base_type;
			using base_type::get;
			using base_type::get_ref;
			using base_type::extract;
			using base_type::extract_to;
			using base_type::set;
			using base_type::size;


		//	default constructor
			iargs_of() = default;
	
		//	construct from base type	
			iargs_of(const base_type& v) : base_type(v) {}

		//	construct with indexed values
			iargs_of(Idxed<Is>... vals) noexcept
				: base_type(vals...) {}

		//	construct with non-indexed (original) values
			iargs_of(original_type_at<Is>... vals) noexcept
		 		: base_type(Idxed<Is>(vals)...) {}

		//	construct from different typed array
			template <class _IType, template <_IType> class _Idxed>
			iargs_of(const typename array_itype_idxed_of<_IType, _Idxed>::template iargs_of<Is...>& arr)
				: base_type(Idxed<Is>(arr.template get<Is>())...) {}

		//	construct with single value
			explicit iargs_of(const typename Idxed<0>::original_type& val)
				: base_type(Idxed<Is>(val)...) {}


		//	get value of specified index
			template <IType I>
			Idxed<I>
			get() const noexcept {
				return base_type::template get<Idxed<I>>();
			}
		
		//	extract values of specified indexes
			template <IType... _Is>
			iargs_of<_Is...>
			extract() const noexcept {
				return {*this};
			}

		//	extract values to other indexed array
			template <IType... _Is>
			void extract_to(iargs_of<_Is...>& out) noexcept {
				_void{(out.set<_Is>(get<_Is>()), 0)...};
			}

		//	get raw (original) typed value of specified index
			template <IType I>
			typename Idxed<I>::original_type
			get_raw() const noexcept {
				return (typename Idxed<I>::original_type)get<I>();
			}

		//	rearrange (change order of) values and get new array
			template <IType... _Is>
			this_type rearrange() const noexcept {
				return {get_raw<_Is>()...};
			}

		//	reindex (set new indexes to) values and get new array
			template <IType... _Is>
			iargs_of<_Is...>
			reindex_with_args() const noexcept {
				return {get_raw<Is>()...};
			}

		private:
		//	helper class for function call with index list
			template <IType... _Is>
			struct with_iargs : _static_class {
			
				static this_type
				rearrange(const this_type& arr) noexcept {
					return arr.rearrange<_Is...>();
				}

				static iargs_of<_Is...>
				reindex_with_args(const this_type& arr) noexcept {
					return arr.reindex_with_args<_Is...>();
				}

			};

		public:
			template <class IdxList>
			this_type rearrange() const noexcept {
				return IdxList::template apply_to<with_iargs>::rearrange(*this);
			}
		
			template <class IdxList>
			auto reindex() const noexcept {
				return IdxList::template apply_to<with_iargs>::reindex_with_args(*this);
			}

			template <IType _IStart>
			auto reindex() const noexcept {
				return reindex<sequence<IType, _IStart, _IStart + size - 1>>();
			}

			auto reindex() const noexcept {
				return reindex<0>();
			}

		

		//	set value of specified index
			template <IType I>
			void set(const Idxed<I>& val) noexcept {
				base_type::template set(val);
			}

		//	set value of specified index using raw (original) typed value
			template <IType I>
			void set(const typename Idxed<I>::original_type& val) noexcept {
				base_type::template set((const Idxed<I>&)val);
			}


		//	fill same value to all indexes
			template <class T>
			void fill(const T& val) noexcept {
				_void{(set<Is>((const typename Idxed<Is>::original_type&)val), 0)...};
			}

		//	cast to another indexed-type
			template <template <IType> class _Idxed>
			iargs_of<Is...>
			cast_to() const {
				return {*this};
			}


		};


		template <class IdxList>
		using index_list_of = typename IdxList::template apply_to<iargs_of>;
		
	//	array with index type, index-templated value type, range of index values
		template <
			IType _First,					//	first value of index
			IType _Last,					//	last value of index
			class DiffType = IType,			//	difference type of index
			DiffType _Inv = 1				//	interval of index values
		>
		using range_of = index_list_of<sequence<IType, _First, _Last, DiffType, _Inv>>;

	//	array with `size_t` index, `Idxed`(index-templated value type), number of index values
		template <size_t Sz>
		using size_of = range_of<0, Sz - 1>;
	

	};

//	array with `IType`(Index Type), `ValType`(Value Type) and index values
	template <class IType,	class ValType>
	using array_itype_valtype_of = array_itype_idxed_of<
		IType,
		indexed_types<ValType, IType>::template single_type
	>;

	template <class IdxList, template <typename IdxList::tmplval_type> class Idxed>
	using array_idxlist_idxed_of = 
		typename array_itype_idxed_of<typename IdxList::tmplval_type, Idxed>
		::template index_list_of<IdxList>;

//	array with `IType`(index type), `ValType`(value type) and list of index values
	template <class IdxList, class ValType>
	using array_idxlist_valtype_of = 
		typename array_itype_valtype_of<typename IdxList::tmplval_type, ValType>
		::template index_list_of<IdxList>;
		
//	array with index type, value type, range of index values
	template <
		class IType,			//	index type
		class ValType,			//	value type
		IType _First,			//	first value of index
		IType _Last,			//	last value of index
		class DiffType = IType,	//	difference type of index
		DiffType _Inv = 1		//	interval of index values
	>
	using array_itype_valtype_range_of = array_idxlist_valtype_of<
		sequence<IType, _First, _Last, DiffType, _Inv>,
		ValType
	>;

//	array with size_t index, `ValType`(value type) and number of index values
	template <class ValType, size_t _Size>
	using array_valtype_size_of = array_itype_valtype_range_of<size_t, ValType, 0, _Size - 1>;
	

//	alias of array_type_of
	template <class ValType, size_t _Size>
	using array = array_valtype_size_of<ValType, _Size>;


//	natural typed array (index starts from 1, not 0)
	template <class ValType, size_t _Size>
	using natural_array = array_itype_valtype_range_of<size_t, ValType, 1, _Size>;


//	construct array with values
	template <class First, class... Rests>
	array<First, sizeof...(Rests) + 1>
	make_array(const First& first, Rests&&... rests) {
		return {first, rests...};
	}


}
