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

	Data person1(ID{152}, Name{"Masaharu Kato"}, Age{21});
	Data person2(Age{23}, ID{182}, Belongs{"Student"});

	Data person(person1, person2);

	person.visit(OutputFunctions(std::cout));

	return 0;
}