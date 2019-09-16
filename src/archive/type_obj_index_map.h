#pragma once
#include "type_obj.h"
#include <unordered_map>

namespace cype {

	class type_obj_index_map : public std::unordered_map<type_obj, size_t> {
		using base_type = std::unordered_map<type_obj, size_t>;
	public:
		using base_type::base_type;

		template <class T>
		size_t index_of() const;

		size_t all_size() const noexcept;

	};

}
