#pragma once
#include <initializer_list>

namespace cype {

	using size_t = std::size_t;

//	structure for dismiss return values
	template <class T = int>
	struct _void {
		_void(std::initializer_list<T>) {}
	};


//	inconstructible structure (inherit this)
	struct _inconstructible {
		_inconstructible() = delete;
	};


//	constexpr add operator (to avoid MSVC's bug)
	template <size_t _V1, size_t _V2>
	static constexpr size_t _index_add = _V1 + _V2;


}