#include "test_structs.h"
#include "typed_array.h"
#include <iostream>

using namespace cype;

int main(void) {

	auto arr = make_typed_array(
		make_set(ID{254}, Name{"Hog Fuga"}, Age{23}),
		make_set(ID{221}, Name{"Piyoo Puyo"}, Age{27}),
		make_set(ID{153}, Name{"Taro Tanaka"}, Age{19})
	);

	arr.for_each([](auto elm){
		std::cout << "[" << elm.index << "]" << std::endl;
		elm.value().for_each([](auto val){
			std::cout << decltype(val)::TYPENAME << ": " << val << std::endl;
		});
	});

	auto arr_id_list = arr.get<ID>();

	std::cout << std::endl;

	std::cout << "IDs in arr: ";
	arr_id_list.for_each([](auto val){ std::cout << val << " "; });
	std::cout << std::endl;

	auto arr_id_name_list = arr.extract<ID, Name>();
	arr_id_name_list.for_each([](auto elm){
		std::cout << "[" << elm.index << "] ID:" << (ID)elm << ", Name:" << (Name)elm << std::endl;
	});

	return 0;
}