#include "array_2d.h"
#include <iostream>

using namespace cype;

//template <size_t R, size_t C>
//using matrix = cype::array_2d<double, R, C>;

//template <size_t R, size_t C>
//void output_matrix(const matrix<4, 4>& m) {
//	std::cout << "[" << std::endl;
//	//std::cout << m.get<0, 0>() << "\t" << m.get<0, 1>() << "\t" << m.get<0, 2>() << "\t" << m.get<0, 3>() << std::endl;
//	//std::cout << m.get<1, 0>() << "\t" << m.get<1, 1>() << "\t" << m.get<1, 2>() << "\t" << m.get<1, 3>() << std::endl;
//	//std::cout << m.get<2, 0>() << "\t" << m.get<2, 1>() << "\t" << m.get<2, 2>() << "\t" << m.get<2, 3>() << std::endl;
//	//std::cout << m.get<3, 0>() << "\t" << m.get<3, 1>() << "\t" << m.get<3, 2>() << "\t" << m.get<3, 3>() << std::endl;
//	std::cout << "]" << std::endl;
//}


template <class ArrayType>
void output_array(std::string name, const ArrayType& arr) {
	std::cout << name << ": [";
	arr.for_each([](auto elm){
		std::cout << elm.value() << "\t";
	});
	std::cout << "]" << std::endl;
}

template <class MatrixType>
void output_matrix(const MatrixType& mat) {
	mat.for_each_i1s([](auto arr){
		arr.for_each([](auto elm){
			std::cout << elm.value() << "\t";
		});
		std::cout << std::endl;
	});
}


using matrix44 = natural_array_2d<double, 4, 4>;

//template <size_t NRow, size_t NCol>
//class matrix : public natural_array_2d<double, 4, 4> {
//
//};
//
//template <class ValType>
//struct _matrix_valtype_of : _static_class {
//
//	template <size_t IRow, size_t ICol>
//	using MVal = indexed_types<ValType, size_t>::template double_type<IRow, ICol>;
//
//	template <size_t NRows, size_t NCols>
//	using Matrix = natural_array_2d<ValType, NRows, NCols>;
//
//	template <size_t... IRows>
//	struct rows_of : _static_class {
//
//		using row_indexes = tmplval_list<size_t, IRows...>;
//		static constexpr size_t NRows = row_indexes::size;
//
//
//		template <size_t... ICols>
//		struct cols_of : _static_class {
//			
//			using col_indexes = tmplval_list<size_t, ICols...>;
//			static constexpr size_t NCols = col_indexes::size;
//
//			
//			using Mat = Matrix<NRows, NCols>; 
//
//			static Mat add(const Mat& mat1, const Mat& mat2) {
//
//			}
//
//			template <size_t... IRCols>
//			struct lcols_of : _static_class {
//
//				using lcol_indexes = tmplval_list<size_t, ILCols...>;
//				static constexpr size_t NRCols = lcol_indexes::size;
//
//				using LMat = Mat;
//				using RMat = Matrix<NCols, NRCols>;
//				using MMat = Matrix<NRows, NRCols>;
//
//				struct mul_operation {
//					const LMat lmat;
//					const RMat rmat;
//
//					mul_operation(const LMat& lmat, const RMat& rmat)
//						: lmat(lmat), rmat(rmat) {}
//
//
//					MMat mul() const {
//						return mul_lines<row_indexes.first, row_indexes.last>();
//					};
//
//					template <size_t IRow, size_t ICol>
//					MVal<IRow, ICol> lget() const { return lmat.template get<IRow, ICol>(); }
//
//					template <size_t IRow, size_t ICol>
//					MVal<IRow, ICol> rget() const { return rmat.template get<IRow, ICol>(); }
//
//					template <size_t ILine, size_t ILineLast, class... Args>
//					MMat mul_lines(Args&&... args) const {
//						if constexpr (ILine > ILineLast) {
//							return MMat(args...);
//						}else{
//							return mul_lines<ILine + 1>(mul_elm<ILine, ICols>()...);
//						}
//					}
//
//					template <size_t ILRow, size_t IRCol>
//					MVal<ILRow, IRCol> mul_elm() const {
//						return (lget<IRows, IRCol>() * rget<ILRow, ICols>() + ...;
//					}
//
//				};
//
//
//			};
//
//
//		};
//
//	};
//
//};
//


int main(void) {

	matrix44 mat0;
	mat0.fill(0);
	mat0.set<1, 1>(1);
	mat0.set<2, 2>(1);
	mat0.set<3, 3>(1);
	mat0.set<4, 4>(1);

	mat0.set<1, 2>(1.2);
	mat0.set<1, 3>(1.3);
	mat0.set<1, 4>(1.4);
	mat0.set<2, 3>(2.3);
	mat0.set<2, 4>(2.4);
	mat0.set<3, 4>(3.4);

	std::cout << "mat0: " << std::endl;
	output_matrix(mat0);
	std::cout << std::endl;

	auto vec1x = mat0.get_elms_on_i1<1>();
	auto vec2x = mat0.get_elms_on_i1<2>();
	auto vec3x = mat0.get_elms_on_i1<3>();
	auto vec4x = mat0.get_elms_on_i1<4>();
	
	auto vecx1 = mat0.get_elms_on_i2<1>();
	auto vecx2 = mat0.get_elms_on_i2<2>();
	auto vecx3 = mat0.get_elms_on_i2<3>();
	auto vecx4 = mat0.get_elms_on_i2<4>();

	output_array("vec1x", vec1x);
	output_array("vec2x", vec2x);
	output_array("vec3x", vec3x);
	output_array("vec4x", vec4x);
	std::cout << std::endl;

	output_array("vecx1", vecx1);
	output_array("vecx2", vecx2);
	output_array("vecx3", vecx3);
	output_array("vecx4", vecx4);
	std::cout << std::endl;

	return 0;
}