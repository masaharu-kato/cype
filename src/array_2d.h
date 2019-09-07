#include "md_array.h"

namespace cype {
	
	template <template <size_t, size_t> class DblIdxType, class ISeq1, class ISeq2>
	using array_2d_of_indexed_base = array_indexes_of<
		typename _preset_dblidx<DblIdxType>::template preset_iseqs<ISeq1, ISeq2>::indexes,
		_preset_dblidx<DblIdxType>::template preset_iseqs<ISeq1, ISeq2>::template type
	>;

	template <template <size_t, size_t> class DblIdxType, class ISeq1, class ISeq2>
	class array_2d_of_indexed : public array_2d_of_indexed_base<DblIdxType, ISeq1, ISeq2> {
	public:
		using base_type = array_2d_of_indexed_base<DblIdxType, ISeq1, ISeq2>;
		using base_type::base_type;
		using base_type::get;
		using base_type::set;

	//	get value of specified index pair
		template <size_t _I1, size_t _I2>
		DblIdxType<_I1, _I2>
		get() const noexcept {
			return base_type::template get<DblIdxType<_I1, _I2>>();
		}

	//	get raw (original) typed value of specified index
		template <size_t _I1, size_t _I2>
		typename DblIdxType<_I1, _I2>::original_type
		get_raw() const noexcept {
			return (typename DblIdxType<_I1, _I2>::original_type)get<_I1, _I2>();
		}


	//	set value of specified index
		template <size_t _I1, size_t _I2>
		void set(const DblIdxType<_I1, _I2>& val) noexcept {
			base_type::template set(val);
		}

	//	set value of specified index using raw (original) typed value
		template <size_t _I1, size_t _I2>
		void set(const typename DblIdxType<_I1, _I2>::original_type& val) noexcept {
			base_type::template set((const DblIdxType<_I1, _I2>&)val);
		}

	//	cast to another indexed-type
		template <template <size_t, size_t> class _DblIdxType>
		array_2d_of_indexed<_DblIdxType, ISeq1, ISeq2>
		cast_to() const {
			return {*this};
		}


	private:
	//	helpder class for `get_index_1_of` and `get_index_2_of`
		template <template <size_t> class _IdxType>
		struct _using_indexed : _static_class {

			using c_preset_dblidx = _preset_dblidx<DblIdxType>;
		
			template <size_t _I1>
			struct index_1_of : _static_class {
				
				template <size_t... _I2s>
				struct indexes_2_of : _static_class {
					static auto get(const array_2d_of_indexed& arr) {
						return array_index_args_of<size_t, _IdxType, _I2s...>(
							typename c_preset_dblidx::template preset_iseq_1<_I1>::template type<_I2s>(arr)...
						);
					}
				};

				static array_indexes_of<ISeq2, _IdxType>
				get(const array_2d_of_indexed& arr) {
					return ISeq2::template apply_to<indexes_2_of>::get(arr);
				}

			};

			template <size_t _I2>
			struct index_2_of : _static_class {
				
				template <size_t... _I1s>
				struct indexes_1_of : _static_class {
					static auto get(const array_2d_of_indexed& arr) {
						return array_index_args_of<size_t, _IdxType, _I1s...>(
							typename c_preset_dblidx::template preset_iseq_2<_I2>::template type<_I1s>(arr)...
						);
					}
				};

				static array_indexes_of<ISeq1, _IdxType>
				get(const array_2d_of_indexed& arr) {
					return ISeq1::template apply_to<indexes_1_of>::get(arr);
				}

			};

		};

	public:
		template <template <size_t> class _IdxType, size_t _I1>
		array_indexes_of<ISeq2, _IdxType>
		get_elms_on_i1() const {
			return _using_indexed<_IdxType>::template index_1_of<_I1>::get(*this);
		}

		template <size_t _I1, template <size_t> class _IdxType>
		void get_elms_on_i1_to(array_indexes_of<ISeq2, _IdxType>& out) const {
			out = get_elms_on_i1<_IdxType, _I1>();
		}
		

		template <template <size_t> class _IdxType, size_t _I2>
		array_indexes_of<ISeq1, _IdxType>
		get_elms_on_i2() const {
			return _using_indexed<_IdxType>::template index_2_of<_I2>::get(*this);
		}

		template <size_t _I2, template <size_t> class _IdxType>
		void get_elms_on_i2_to(array_indexes_of<ISeq1, _IdxType>& out) const {
			out = get_elms_on_i2<_IdxType, _I2>();
		}




	};


	template <
		template <size_t, size_t> class DblIdxType,
		template <size_t> class IdxType1,
		template <size_t> class IdxType2,
		class ISeq1,
		class ISeq2
	>
	class array_2d_of_indexed_types : public array_2d_of_indexed<DblIdxType, ISeq1, ISeq2> {
	public:
		using base_type = array_2d_of_indexed<DblIdxType, ISeq1, ISeq2>;
		using base_type::base_type;

		template <size_t _I1>
		auto get_elms_on_i1() const {
			return base_type::template get_elms_on_i1<IdxType1, _I1>();
		}
		

		template <size_t _I2>
		auto get_elms_on_i2() const {
			return base_type::template get_elms_on_i2<IdxType2, _I2>();
		}

	};



	template <class ValType, class ISeq1, class ISeq2>
	using array_2d_of_type = array_2d_of_indexed_types<
		_indexed_preset_types<ValType, size_t>::template double_type,
		_indexed_preset_types<ValType, size_t>::template single_type,
		_indexed_preset_types<ValType, size_t>::template single_type,
		ISeq1,
		ISeq2
	>;
	

	template <class ValType, size_t _Sz1, size_t _Sz2>
	using array_2d = array_2d_of_type<
		ValType,
		index_sequence<0, _Sz1 - 1>,
		index_sequence<0, _Sz2 - 1>
	>;

	template <class ValType, size_t _Sz1, size_t _Sz2>
	using natural_array_2d = array_2d_of_type<
		ValType,
		index_sequence<1, _Sz1>,
		index_sequence<1, _Sz2>
	>;


}
