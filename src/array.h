#pragma once
#include "data.h"
#include "tmpl_value_utils.h"
#include "indexed.h"

namespace cype {

	template <class IType, template <IType> class IndexedType, IType... _Indexes>
	class array_indexes_of : public Data<IndexedType<_Indexes>...> {
		using base_type = Data<IndexedType<_Indexes>...>;

	public:
		using base_type::Data;

		array_indexes_of(IndexedType<_Indexes>... vals) noexcept
			: base_type(vals...) {}

		array_indexes_of(typename IndexedType<_Indexes>::original_type... vals) noexcept
			: base_type(IndexedType<_Indexes>(vals)...) {}

		using base_type::get;

		template <IType _Index>
		IndexedType<_Index> get() const noexcept {
			return base_type::get<typename IndexedType<_Index>>();
		}
			
		template <IType... __Indexes>
		array_indexes_of<IType, IndexedType, __Indexes...> extract() const noexcept {
			return *this;
		}

		template <IType _Index>
		auto get_raw() const noexcept {
			return (typename IndexedType<_Index>::original_type)get<_Index>();
		}

		template <IType... __Indexes>
		array_indexes_of rearrange() const noexcept {
			return {get_raw<__Indexes>()...};
		}

		using base_type::set;

		template <IType _Index>
		void set(const IndexedType<_Index>& val) noexcept {
			base_type::set<typename IndexedType<_Index>>(val);
		}

	};

	template <class IType, template <IType> class IndexedType>
	struct array_type_of {
		template <IType... _Indexes>
		using indexes_of = array_indexes_of<IType, IndexedType, _Indexes...>;
	};

	template <class IType, template <IType> class IndexedType, class IndexList>
	using array = typename IndexList::template apply_to<typename array_type_of<IType, IndexedType>::indexes_of>;
		
	template <class IType, template <IType> class IndexedType, IType _First, IType _Last, IType _Inv = 1>
	using array_range_of = typename array<IType, IndexedType, typename tmpl_value_utils<IType>::template sequence<_First, _Last, _Inv>>;


	template <template <size_t> class IndexedType, size_t _First, size_t _Last>
	using array_of_indexed = array_range_of<size_t, IndexedType, _First, _Last>;

	template <class Type, size_t _First, size_t _Last>
	using array_of_type = array_range_of<size_t, typename indexed_of<size_t, Type>::type, _First, _Last>;
	

	template <class First, class... Rests>
	array_of_type<First, 0, sizeof...(Rests)> make_array(const First& first, Rests&&... rests) 
	{
		return {first, rests...};
	}



}