#pragma once
#include "typed_set.h"
#include "array.h"
#include "_utility.h"

namespace cype {

	template <class IndexList, class... Types>
	class typed_array_indexes_of : public array_type_indexes_of<IndexList, typed_set<Types...>> {
		using base_type = array_type_indexes_of<IndexList, typed_set<Types...>>;
	public:
		using base_type::base_type;

		template <class _Type>
		array_type_indexes_of<IndexList, _Type>
		get() const {
			return {*this};
		}


		template <class... _Types>
		typed_array_indexes_of<IndexList, _Types...>
		extract() const {
			return {*this};
		}

	};

	template <class IndexesType>
	struct _typed_array_preset_indexes : public _inconstructible {
		template <class... Types>
		using type = typed_array_indexes_of<IndexesType, Types...>;
	};

	template <class IndexesType, class TypeList>
	using typed_array_indexes_types_of = typename TypeList::template apply_to<_typed_array_preset_indexes<IndexesType>::template type>;


	template <size_t _Size, class... Types>
	using typed_array = typed_array_indexes_of<index_sequence<0, _Size - 1>, Types...>;



	template <class First, class... Rests>
	typed_array_indexes_types_of<index_sequence<0, sizeof...(Rests)>, typename First::type_list>
	make_typed_array(const First& first, Rests&&... rests) {
		return {first, rests...};
	}

}
