#pragma once
#include "_utility.h"
#include <string>
#include <functional>
#include <type_traits>
#include <unordered_map>

namespace cype {

	class typeid_list : _static_class {
	public:
		using type_name_t = std::string;
		using new_func_t = std::function<void*()>;

	private:
		std::unordered_map<type_name_t, new_func_t> new_funcs;

	public:
		template <class T>
		static void add(type_name_t type_name) {
			new_funcs[type_name] = [](const auto& arg) -> void* {
				return new T(arg);
			};
		}

	};

}