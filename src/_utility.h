#pragma once
#include <initializer_list>

namespace cype {

	static_assert(__cplusplus == 201703L, "C++17 required.");

//	structure for dismiss return values
	template <class T = int>
	struct _void {
		_void(std::initializer_list<T>) {}
	};


//	inconstructible structure (inherit this)
	struct _inconstructible {
		_inconstructible() = delete;
	};

	using size_t = std::size_t;

}