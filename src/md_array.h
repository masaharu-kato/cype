#include "array.h"

namespace cype {
	
//	multi-dimensional array with `size_t`, `IndexedType`(indexes-templated value type) and sizes of each dimension
	template <template <size_t...> class MultiIndexedType, size_t... _Sizes>
	using _md_array_of_indexed_base = array_indexes_of<
		typename _md_indexed_size_args_of<size_t, MultiIndexedType, _Sizes...>::indexes,
		_md_indexed_size_args_of<size_t, MultiIndexedType, _Sizes...>::template type
	>;

	template <template <size_t...> class MultiIndexedType, size_t... _Sizes>
	class md_array_of_indexed : public _md_array_of_indexed_base<MultiIndexedType, _Sizes...> {
	public:
		using base_type = _md_array_of_indexed_base<MultiIndexedType, _Sizes...>;
		using base_type::base_type;

		using base_type::get;
		
	//	get value of specified location
		template <size_t... _Indexes>
		MultiIndexedType<_Indexes...>
		get() const noexcept {
			return base_type::template get<MultiIndexedType<_Indexes...>>();
		}
		
	//	get raw (original) typed value of specified location
		template <size_t... _Indexes>
		typename MultiIndexedType<_Indexes...>::original_type
		get_raw() const noexcept {
			return (typename MultiIndexedType<_Indexes...>::original_type)get<_Indexes...>();
		}

		
		using base_type::set;
		
	//	set value of specified location
		template <size_t... _Indexes>
		void set(const MultiIndexedType<_Indexes...>& val) noexcept {
			base_type::template set((MultiIndexedType<_Indexes...>)val);
		}

	//	set value of specified location using raw(original) typed value
		template <size_t... _Indexes>
		void set(const typename MultiIndexedType<_Indexes...>::original_type& val) noexcept {
			base_type::template set((MultiIndexedType<_Indexes...>)val);
		}

		
	//	cast to another indexed-type
		template <template <size_t...> class _MultiIndexedType>
		md_array_of_indexed<_MultiIndexedType, _Sizes...>
		cast_to() const {
			return {*this};
		}

	};


//	multi-dimensional array with `size_t`, `IndexedType`(indexes-templated value type) and sizes of each dimension
	template <class ValType, size_t... _Sizes>
	using md_array_of_type = md_array_of_indexed<
		_indexed_preset_types<ValType, size_t>::template multi_type,
		_Sizes...
	>;
	
//	alias of md_array_of_type
	template <class ValType, size_t... _Sizes>
	using md_array = md_array_of_type<ValType, _Sizes...>;

}