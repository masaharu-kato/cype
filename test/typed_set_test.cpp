#include "typed_set.h"
#include "typed_map.h"
#include "type_utils.h"
#include "test_structs.h"
#include <string>
#include <iostream>
#include <type_traits>
#include <optional>
using namespace cype;


//	sample output class for testing `type_set::visit()`
template <class OS>
class OutputFunctions {
private:
	OS& os;

public:
	OutputFunctions(OS& os) : os(os) {}

	template <class First, class... Rests>
	void operator ()(const First& first, Rests&&... rests) const {
		os << First::TYPENAME << ":" << first << std::endl;
		return operator ()(rests...);
	}

	void operator ()() const {};

};


int main(void) {

	auto data1 = make_set(ID{152}, Name{"Masaharu Kato"}, Age{21});
	auto data2 = make_set(Belongs{"Student"});
	auto data3 = make_set(ID{163}, Age{25}, Phone{"090-1234-5678"});

	auto data1_opt = data1.construct_each<std::optional>();

	static_assert(std::is_same_v<decltype(data1), typed_set<ID, Name, Age>> , "type of data1 is incorrect.");
	static_assert(std::is_same_v<decltype(data2), typed_set<Belongs>>       , "type of data2 is incorrect.");
	static_assert(std::is_same_v<decltype(data3), typed_set<ID, Age, Phone>>, "type of data3 is incorrect.");

	static_assert(std::is_same_v<decltype(data1_opt), typed_set<std::optional<ID>, std::optional<Name>, std::optional<Age>>>, "type of data1_opt is incorrect.");

	typed_set person1 = data1.combine_back(data2);
	typed_set person2 = data1.overwritten(data3);

	auto output_func = OutputFunctions(std::cout);

	person1.visit(output_func);
	person2.visit(output_func);

	person1.for_each([](auto value){
		std::cout << value << std::endl;	
	});



	auto map1 = make_map(make_typed<ID>(25), make_typed<Age>(36), make_typed<Phone>(13));

	static_assert(std::is_same_v<decltype(map1), typed_map<int, ID, Age, Phone>> , "type of map1 is incorrect.");
	
	std::cout << "map1: " << std::endl;
	map1.for_each([](auto value){
		std::cout << decltype(value)::type::TYPENAME << ": " << value.get() << std::endl;	
	});

	typed_map<size_t, ID, Name, Belongs, Phone> map2((size_t)0);
	map2.set<Name>(123);
	map2.set<Phone>(456);

	std::cout << "map2: " << std::endl;
	map2.for_each([](auto value){
		std::cout << decltype(value)::type::TYPENAME << ": " << value.get() << std::endl;	
	});
	
	auto map3 = make_map(make_typed<Age>(32), make_typed<ID>(-105), make_typed<Belongs>(3));
	map3.fill(12345);

	std::cout << "map3: " << std::endl;
	map3.for_each([](auto value){
		std::cout << decltype(value)::type::TYPENAME << ": " << value.get() << std::endl;	
	});

	return 0;
}