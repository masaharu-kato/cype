#pragma once
#include "typed_set.h"
#include <vector>
#include <algorithm>

namespace cype {

	template <class... Types>
	class typed_vector : public std::vector<typed_set<Types...>> {
		using elm_type = typed_set<Types...>;
		using base_type = std::vector<elm_type>;
	public:
		using base_type::base_type;

		using base_type::size;
		using base_type::begin;
		using base_type::end;
		
		template <class _Type>
		void get_to(std::vector<_Type>& out) const {
			out.resize(size());
			std::transform(begin(), end(), out.begin(), [](const elm_type& elm){
				return elm.template get<_Type>();
			});
		}

		template <class... _Types>
		void extract_to(typed_vector<_Types...>& out) const {
			out.resize(size());
			std::transform(begin(), end(), out.begin(), [](const elm_type& elm){
				return elm.template extract<_Types...>();
			});
		}


		template <class _Type>
		typed_vector<_Type>
		get() const {
			typed_vector<_Type> out;
			get_to(out);
			return out;
		}

		template <class... _Types>
		typed_vector<_Types...>
		extract() const {
			typed_vector<_Types...> out;
			extract_to(out);
			return out;
		}

	};


}
