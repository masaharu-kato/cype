#include "array_2d.h"
#include <iostream>

using namespace cype;


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