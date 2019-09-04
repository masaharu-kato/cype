#include "tmplval_list.h"
#include <type_traits>
using namespace cype;

int main(void) {

	using vals1 = tmplval_list<size_t, 16, 35, 50>;
	using vals2 = tmplval_list<size_t, 13, 24, 57>;
	using vals3 = tmplval_list<size_t, 14, 38, 43, 59>;

	using vals12 = vals1::merge_of_value_sorted<vals2>;
	using vals123 = vals12::merge_of_value_sorted<vals3>;

	static_assert(std::is_same_v<vals12 , tmplval_list<size_t, 13, 16, 24, 35, 50, 57>>);
	static_assert(std::is_same_v<vals123, tmplval_list<size_t, 13, 14, 16, 24, 35, 38, 43, 50, 57, 59>>);

	using vals123_l = vals123::range_of<0, 4>;
	using vals123_r = vals123::range_of<5, 9>;
	
	static_assert(std::is_same_v<vals123_l, tmplval_list<size_t, 13, 14, 16, 24, 35>>);
	static_assert(std::is_same_v<vals123_r, tmplval_list<size_t, 38, 43, 50, 57, 59>>);

	using val4 = tmplval_list<size_t, 40, 13, 25, 10, 7>;

	static_assert(val4::size == 5);

	using val4_sorted = val4::value_sorted;
	static_assert(std::is_same_v<val4_sorted, tmplval_list<size_t, 7, 10, 13, 25, 40>>);

	return 0;
}