#pragma once
#include <unordered_map>
#include "type_obj.h"
#include "binary.h"

namespace cype {

	class type_objed_set : std::unordered_map<type_obj, binary> {
	public:
		type_objed_set() = default;

	//	TODO: Definitions
	};

}
