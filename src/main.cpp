#include "data.h"
#include "type_utils.h"
#include <string>
#include <iostream>
#include <type_traits>
using namespace cype;

struct ID {
	int id;
};

struct Name {
	std::string name;
};

struct Age {
	int age;
};

struct Belongs {
	std::string belongs;
};

struct Phone {
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
		os << first << std::endl;
		return operator ()(rests...);
	}

	void operator ()() const {};

};


int main(void) {

	//constexpr bool f1 = type_utils::list<Age, ID, Name>::is_unique;
	//constexpr bool f2 = type_utils::list<Name, ID, Name>::is_unique;
	//constexpr bool f3 = type_utils::list<Age, Age, Name>::is_unique;

	//std::cout << f1 << ", " << f2 << ", " << f3 << "\n";


	std::cout << type_utils::list<Age, ID, ID, Name>::is_unique << std::endl;
	std::cout << type_utils::list<ID, Age, ID, Name>::is_unique << std::endl;

	std::cout << typeid(type_utils::list<ID, Age, Name, Name, Age>::remove_duplicates).name() << std::endl;
	std::cout << typeid(type_utils::list<ID>::remove_duplicates).name() << std::endl;
	std::cout << typeid(type_utils::list<Age, Name, Age, Age>::remove_duplicates).name() << std::endl;

	using Type1 = type_utils::list<ID, Name, Age>::union_with<ID, Name, Belongs>;
	using Type2 = type_utils::list<Belongs, Name, Age, ID>::remove<Age, Phone, Belongs>;
	using Type3 = Data<Data<ID, Name, Age>, Data<Age, ID, Belongs>>;

	std::cout << typeid(Type1).name() << std::endl;
	std::cout << typeid(Type2).name() << std::endl;
	std::cout << typeid(Type3).name() << std::endl;


	Data person(ID{152}, Name{"Masaharu Kato"}, Age{21});

	person.visit(OutputFunctions(std::cout));

	Data<ID, Name> person_id_name(person);

	Data person_copied(person);

	std::cout << typeid(decltype(person_copied)).name() << std::endl;

	return 0;
}