#include "typed_set.h"
#include "type_utils.h"
#include <string>
#include <iostream>
#include <type_traits>
#include <optional>
using namespace cype;

//	sample classes `ID`
struct ID {
	constexpr static auto TYPENAME = "ID";
	int id;
};

//	sample classes `Name`
struct Name {
	constexpr static auto TYPENAME = "Name";
	std::string name;
};

//	sample classes `Age`
struct Age {
	constexpr static auto TYPENAME = "Age";
	int age;
};

//	sample classes `Belongs`
struct Belongs {
	constexpr static auto TYPENAME = "Belongs";
	std::string belongs;
};

//	sample classes `Phone`
struct Phone {
	constexpr static auto TYPENAME = "Phone";
	std::string phone;
};

std::ostream& operator <<(std::ostream& os, const ID& val) { return os << val.id; }
std::ostream& operator <<(std::ostream& os, const Name& val) { return os << val.name; }
std::ostream& operator <<(std::ostream& os, const Age& val) { return os << val.age; }
std::ostream& operator <<(std::ostream& os, const Belongs& val) { return os << val.belongs; }
std::ostream& operator <<(std::ostream& os, const Phone& val) { return os << val.phone; }


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

	return 0;
}