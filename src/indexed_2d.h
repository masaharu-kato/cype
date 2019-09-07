#pragma once
#include "indexed_types.h"

namespace cype {
	
//	preset double-indexed-type
	template <template <size_t, size_t> class DoubleIndexed>
	struct _preset_double_index : _static_class {

	//	preset first index
		template <size_t _I1>
		struct preset_index_1 : _static_class {
			template <size_t _I2>
			using type = DoubleIndexed<_I1, _I2>;
		};

	//	preset second index
		template <size_t _I2>
		struct preset_index_2 : _static_class {
			template <size_t _I1>
			using type = DoubleIndexed<_I1, _I2>;
		};

	//	preset index sequences
		template <class _ISeq1, class _ISeq2>
		struct preset_index_sequences : _static_class {

			template <size_t _I>
			using type = DoubleIndexed<
				_ISeq1::template get<_I % _ISeq1::size>,
				_ISeq2::template get<_I / _ISeq1::size>
			>;

			static constexpr size_t all_size = _ISeq1::size * _ISeq2::size;

			using indexes = index_sequence<0, all_size - 1>;


			template <size_t _I1, size_t _I2>
			static constexpr size_t to_array_index = _ISeq1::template index_of<_I1> + _ISeq2::template index_of<_I2> * _ISeq1::size;


			template <size_t _I1>
			struct preset_index_1 : _static_class {
				template <size_t _I2>
				static constexpr size_t index = to_array_index<_I1, _I2>;
			};

			template <size_t _I2>
			struct preset_index_2 : _static_class {
				template <size_t _I1>
				static constexpr size_t index = to_array_index<_I1, _I2>;
			};

		};

	};
	

}