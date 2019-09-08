#pragma once
#include "typed_set.h"
#include "array.h"
#include "_utility.h"

namespace cype {

//	multi-typed array (array with typed_set)
	template <class IdxList>
	struct typed_array_idxlist_of : _static_class {
		
		template <class... Types>
		class valtypes_of : public array_idxlist_valtype_of<IdxList, typed_set<Types...>> {
			using base_type = array_idxlist_valtype_of<IdxList, typed_set<Types...>>;
		public:
			using this_type = valtypes_of;

			template <class T>
			using array_of = array_idxlist_valtype_of<IdxList, T>;

			using base_type::base_type;
			using base_type::get;
			using base_type::get_to;
			using base_type::extract;
			using base_type::extract_to;

		//	get all values of specified type
			template <class T>
			array_of<T> get() const {
				return {*this};
			}

		//	get all values of specified type to specified array
			template <class T>
			void get_to(array_of<T>& out) const {
				out = get<T>();
			}

		//	extract specified types
			template <class... _Types>
			valtypes_of<_Types...> extract() const {
				return {*this};
			}
		
		//	extract specified types to specified typed_array
			template <class... _Types>
			void extract_to(valtypes_of<_Types...>& out) const {
				return out = extract<_Types...>();
			}

		};

		template <class ValTypeList>
		using valtypelist_of = typename ValTypeList::template apply_to<valtypes_of>;


	};

	template <class IdxList, class ValTypeList>
	using typed_array_idxlist_valtypelist_of = typename typed_array_idxlist_of<IdxList>::template valtypelist_of<ValTypeList>;


	template <size_t Sz, class... Types>
	using typed_array_size_valtypes = typename typed_array_idxlist_of<index_sequence<0, Sz - 1>>::template valtypes_of<Types...>;



	template <class First, class... Rests>
	typed_array_idxlist_valtypelist_of<index_sequence<0, sizeof...(Rests)>, typename First::this_type_list>
	make_typed_array(const First& first, Rests&&... rests) {
		return {first, rests...};
	}

}
