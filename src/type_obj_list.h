#pragma once
#include "type_obj.h"
#include <unordered_set>

namespace cype {

	class type_obj_list : public std::unordered_set<type_obj> {
	private:
		 using base_type = std::unordered_set<type_obj>;

	public:
		using base_type::base_type;


	};

}