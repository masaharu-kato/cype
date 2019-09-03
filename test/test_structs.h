#pragma once
#include <iostream>

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