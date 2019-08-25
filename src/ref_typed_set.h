#pragma once
#include "typed_set.h"
#include <memory>

namespace cype {

//	referenced typed set
	template <class... Types>
	class ref_typed_set : public typed_set<std::shared_ptr<Types>...> {
	public:
		using typed_set<std::shared_ptr<Types>...>::typed_set;
	};

}