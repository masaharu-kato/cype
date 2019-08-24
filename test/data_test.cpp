#include "data.h"
#include "type_utils.h"
#include <string>
#include <iostream>
#include <type_traits>
using namespace cype;

struct ID {
	constexpr static auto TYPENAME = "ID";
	int id;
};

struct Name {
	constexpr static auto TYPENAME = "Name";
	std::string name;
};

struct Age {
	constexpr static auto TYPENAME = "Age";
	int age;
};

struct Belongs {
	constexpr static auto TYPENAME = "Belongs";
	std::string belongs;
};

struct Phone {
	constexpr static auto TYPENAME = "Phone";
	std::string phone;
};

std::ostream& operator <<(std::ostream& os, const ID& val) { return os << val.id; }
std::ostream& operator <<(std::ostream& os, const Name& val) { return os << val.name; }
std::ostream& operator <<(std::ostream& os, const Age& val) { return os << val.age; }
std::ostream& operator <<(std::ostream& os, const Belongs& val) { return os << val.belongs; }
std::ostream& operator <<(std::ostream& os, const Phone& val) { return os << val.phone; }



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

	Data data1(ID{152}, Name{"Masaharu Kato"}, Age{21});
	Data data2(Belongs{"Student"});
	Data data3(ID{163}, Age{25}, Phone{"090-1234-5678"});

	Data person1 = data1.pushed_data_back(data2);
	Data person2 = data1.overwritten(data3);

	person1.visit(OutputFunctions(std::cout));
	person2.visit(OutputFunctions(std::cout));

	person1.for_each([](auto value){
		std::cout << value << std::endl;	
	});

	return 0;
}