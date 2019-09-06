#include <iostream>
#include "array.h"


template <class ValList>
void output_tmplval_list() {
	std::cout << ValList::template get<0>;
	if constexpr (ValList::size > 1) {
		std::cout << ",";
		output_tmplval_list<typename ValList::rests_list>();
	}
};

template <class MdArray>
void output_md_array(const MdArray& mda) {
	mda.for_each([](auto v){
		using vtype = decltype(v);
		std::cout << "[";
		output_tmplval_list<typename vtype::indexes>();
		std::cout << "] " << v.value() << std::endl; 
	});
}



int main(void) {

	cype::md_array<int, 24> mdarr1;
	cype::md_array<int, 6, 4> mdarr2;
	cype::md_array<int, 3, 2, 4> mdarr3;

	mdarr1.for_each_ref([](auto& elm){
		elm.value() = std::remove_reference_t<decltype(elm)>::indexes::visit([](int v1){ 
			return 2 * v1;
		});
	});

	mdarr2.for_each_ref([](auto& elm){
		elm.value() = std::remove_reference_t<decltype(elm)>::indexes::visit([](int v1, int v2){
			return -3 * v1 * v1 + 4 * v2;
		});
	});

	mdarr3.for_each_ref([](auto& elm){
		elm.value() = std::remove_reference_t<decltype(elm)>::indexes::visit([](int v1, int v2, int v3){
			return 5 * v1 * v1 * v1 - 7 * v2 * v2 - v3;
		});
	});

	std::cout << "mdarr1: " << std::endl;
	output_md_array(mdarr1);

	std::cout << "mdarr2: " << std::endl;
	output_md_array(mdarr2);

	std::cout << "mdarr3: " << std::endl;
	output_md_array(mdarr3);

	return 0;
}
