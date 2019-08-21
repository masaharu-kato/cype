#pragma once
#include "data.h"
#include <memory>

namespace cype {

	template <class... Types>
	class RefData : public Data<std::shared_ptr<Types>...> {
	public:
		using Data<std::shared_ptr<Types>...>::Data;
	};

}