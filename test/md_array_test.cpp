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


using namespace cype;


int main(void) {

	array_index_args_of<
		size_t,
		_sized_indexed_of<
			tmplval_list<size_t, 3, 2>,
			_indexed_preset_types<int, size_t>::template type
		>::template type,
		0, 1, 2, 3, 4, 5
	> mdarr0;

	cype::md_array<int, 3, 2, 4> mdarr1;

	std::cout << "Indexes of mdarr1: " << std::endl;
	mdarr1.for_each([](auto v){
		using vtype = decltype(v);
		std::cout << "[";
		output_tmplval_list<typename vtype::indexes>();
		std::cout << "]" << std::endl;
	});

	return 0;
}
