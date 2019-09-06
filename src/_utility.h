#pragma once
#include <initializer_list>
#include <cstddef>

namespace cype {

	using size_t = std::size_t;
	using diff_t = std::ptrdiff_t;

//	structure for dismiss return values
//	template <class T = int>
	struct _void {
		_void(std::initializer_list<int /*T*/>) {}
	};


//	inconstructible static class (inherit this from static class)
	class _static_class {
		_static_class() = delete;
	};


//	treat type as value
	template <class _Type>
	struct type_as_value {
		using type = _Type;
		type_as_value() = default;
	};


//	constexpr add operator (to avoid MSVC's bug)
	template <size_t _V1, size_t _V2>
	static constexpr size_t _index_add = _V1 + _V2;


}