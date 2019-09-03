#pragma once
#include "typed_set.h"
#include "array.h"

namespace cype {

	template <class IndexesType, class... Types>
	class typed_array_indexes_of : public array_type_indexes_of<size_t, typed_set<Types...>, IndexesType> {
		using base_type = array_type_indexes_of<size_t, typed_set<Types...>, IndexesType>;
	public:
		using base_type::array_type_indexes_of;

		template <class... _Types>
		typed_array_indexes_of<IndexesType, _Types...>
		extract() const {
			return {*this};
		}

	};

	template <size_t _Size, class... Types>
	using typed_array = typed_array_indexes_of<index_sequence<0, _Size - 1>, Types...>;

}
