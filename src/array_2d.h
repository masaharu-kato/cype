#pragma once
#include "array.h"
#include "indexed_2d.h"

namespace cype {
	
//	base type of `array_2d_iseqs` class
	template <
		template <size_t, size_t> class DblIdxed,
		class ISeq1,
		class ISeq2
	>
	using array_2d_of_indexed_base = array_idxlist_idxed_of<
		typename _preset_double_index<DblIdxed>::template preset_index_sequences<ISeq1, ISeq2>::indexes,
		_preset_double_index<DblIdxed>::template preset_index_sequences<ISeq1, ISeq2>::template type
	>;

//	2-dimensional array
	template <
		template <size_t, size_t> class DblIdxed,	//	2 indexes-templated value type
		class ISeq1,	//	sequence (tmplval_list) of index-1
		class ISeq2		//	sequence (tmplval_list) of index-2
	>
	class array_2d_dblidxed_iseqs : public array_2d_of_indexed_base<DblIdxed, ISeq1, ISeq2> {
	public:
		using this_type = array_2d_dblidxed_iseqs;
		using this_indexes_1 = ISeq1;
		using this_indexes_2 = ISeq2;

		template <size_t _I1, size_t _I2>
		using this_double_indexed_type = DblIdxed<_I1, _I2>;

		using base_type = array_2d_of_indexed_base<DblIdxed, ISeq1, ISeq2>;
		using base_type::base_type;
		using base_type::get;
		using base_type::get_raw;
		using base_type::set;

	//	get value of specified index pair
		template <size_t _I1, size_t _I2>
		DblIdxed<_I1, _I2>
		get() const noexcept {
			return base_type::template get<DblIdxed<_I1, _I2>>();
		}

	//	get raw (original) typed value of specified index
		template <size_t _I1, size_t _I2>
		typename DblIdxed<_I1, _I2>::original_type
		get_raw() const noexcept {
			return (typename DblIdxed<_I1, _I2>::original_type)get<_I1, _I2>();
		}


	//	set value of specified index
		template <size_t _I1, size_t _I2>
		void set(const DblIdxed<_I1, _I2>& val) noexcept {
			base_type::template set(val);
		}

	//	set value of specified index using raw (original) typed value
		template <size_t _I1, size_t _I2>
		void set(const typename DblIdxed<_I1, _I2>::original_type& val) noexcept {
			base_type::template set((const DblIdxed<_I1, _I2>&)val);
		}

	//	cast to another indexed-type
		template <template <size_t, size_t> class _DblIdxed>
		array_2d_dblidxed_iseqs<_DblIdxed, ISeq1, ISeq2>
		cast_to() const {
			return {*this};
		}


	private:
		template <size_t _I1>
		class _index_1_of : _static_class {
		private:
			template <size_t _I2>
			static constexpr size_t c_index = _preset_double_index<DblIdxed>
				::template preset_index_sequences<ISeq1, ISeq2>
				::template preset_index_1<_I1>
				::template index<_I2>;

		public:
			template <size_t... _I2s>
			struct indexes_2_of : _static_class {

				static auto get(const this_type& arr) {
					return arr.template extract<c_index<_I2s>...>();
				}

			};

			static auto get(const this_type& arr) {
				return ISeq2::template apply_to<indexes_2_of>::get(arr);
			}

		};

		template <size_t _I2>
		struct _index_2_of : _static_class {
		private:
			template <size_t _I1>
			static constexpr size_t c_index = _preset_double_index<DblIdxed>
				::template preset_index_sequences<ISeq1, ISeq2>
				::template preset_index_2<_I2>
				::template index<_I1>;

		public:
			template <size_t... _I1s>
			struct indexes_1_of : _static_class {

				static auto get(const this_type& arr) {
					return arr.template extract<c_index<_I1s>...>();
				}

			};

			static auto get(const this_type& arr) {
				return ISeq1::template apply_to<indexes_1_of>::get(arr);
			}

		};


		template <size_t... _Is>
		struct _indexes_of : _static_class {

			template <class Function>
			static void for_each_i1s(const this_type& arr, const Function& func) {
				_void{(func(arr.get_elms_on_i1<_Is>()), 0)...};
			}

			template <class Function>
			static void for_each_i2s(const this_type& arr, const Function& func) {
				_void{(func(arr.get_elms_on_i2<_Is>()), 0)...};
			}

		};

	public:
		template <size_t _I1>
		auto get_elms_on_i1() const {
			return _index_1_of<_I1>::get(*this);
		}

		template <size_t _I2>
		auto get_elms_on_i2() const {
			return _index_2_of<_I2>::get(*this);
		}

		template <class Function>
		void for_each_i1s(const Function& func) const {
			ISeq1::template apply_to<_indexes_of>::for_each_i1s(*this, func);
		}

		template <class Function>
		void for_each_i2s(const Function& func) const {
			ISeq2::template apply_to<_indexes_of>::for_each_i2s(*this, func);
		}

	};



	template <class ValType, class ISeq1, class ISeq2>
	using array_2d_type_iseqs = array_2d_dblidxed_iseqs<
		indexed_types<ValType, size_t>::template double_type,
		ISeq1,
		ISeq2
	>;
	

	template <class ValType, size_t Sz1, size_t Sz2>
	using array_2d_type_of = array_2d_type_iseqs<
		ValType,
		index_sequence<0, Sz1 - 1>,
		index_sequence<0, Sz2 - 1>
	>;

	template <class ValType, size_t Sz1, size_t Sz2>
	using array_2d = array_2d_type_of<ValType, Sz1, Sz2>;

	template <class ValType, size_t Sz1, size_t Sz2>
	using natural_array_2d = array_2d_type_iseqs<
		ValType,
		index_sequence<1, Sz1>,
		index_sequence<1, Sz2>
	>;

}
