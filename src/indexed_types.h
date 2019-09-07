#pragma once
#include "typed.h"
#include "tmplval_list.h"
#include "_utility.h"


namespace cype {

//	preset template arguments `ValType`(original value type) , `IType`(index type) of indexed classes
	template <class ValType, class IType>
	class indexed_types : _static_class {

		template <IType _I>
		using single_type_base = typed<tmplval_list<IType, _I>, ValType>;

		template <IType _I1, IType _I2>
		using double_type_base = typed<tmplval_list<IType, _I1, _I2>, ValType>;

		template <IType... _Is>
		using multi_type_base = typed<tmplval_list<IType, _Is...>, ValType>;

	public:
		template <IType _I>
		class single_type : public single_type_base<_I> {
			using base_type = single_type_base<_I>;
		public:
			static constexpr IType index = _I;

			using base_type::base_type;
		};

		template <IType _I1, IType _I2>
		class double_type : public double_type_base<_I1, _I2> {
			using base_type = double_type_base<_I1, _I2>;
		public:
			static constexpr IType index_1 = _I1;
			static constexpr IType index_2 = _I2;

			using base_type::base_type;
		};

		template <IType... _Is>
		class multi_type : public multi_type_base<_Is...> {
			using base_type = multi_type_base<_Is...>;
		public:
			using indexes = tmplval_list<IType, _Is...>;

			template <IType __I>
			static constexpr IType index_of = indexes::template get<__I>;


			using base_type::base_type;
		};

		template <class IdxList>
		using multi_type_by_index_list = typename IdxList::template apply_to<multi_type>;


	};

}
