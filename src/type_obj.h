#pragma once
#include "_utility.h"
#include <string>
#include <functional>
#include <type_traits>

namespace cype {

	class type_obj {
	private:
		using type_name_t = std::string;
		using new_func_t = std::function<void*(const auto&)>;
		using new_array_func_t = std::function<void*(size_t)>;

		size_t type_size;
		type_name_t type_name;
		new_func_t new_func;
		new_array_func_t new_array_func;

	public:
		template <class T>
		type_obj(type_as_value<T>, type_name_t type_name) :
			type_size(sizeof(T)),
			type_name_t(type_name),
			new_func([](const auto& arg) -> void* {
				return new T(arg);
			}),
			new_array_func([](size_t n) -> void* {
				return new T[n];
			})
		{}

	};

}