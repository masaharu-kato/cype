#include "test_structs.h"
#include "typed_vector.h"
#include <iostream>

using namespace cype;

int main(void) {

	typed_vector<ID, Name, Age> persons{
		{ID{254}, Name{"Hog Fuga"}, Age{23}},
		{ID{221}, Name{"Piyoo Puyo"}, Age{27}},
		{ID{153}, Name{"Taro Tanaka"}, Age{19}}
	};


	persons.emplace_back(ID{94}, Name{"Ichiro Suzuki"}, Age{26});
	persons.emplace_back(Age{41}, Name{"Hiro Ohara"}, ID{109});
	persons.emplace_back(Name{"Jiro Kiyo"}, Age{53}, ID{523});

	for(auto person : persons) {
		person.for_each([](auto elm){
			std::cout << decltype(elm)::TYPENAME << ": " << elm << std::endl;
		});
		std::cout << std::endl;
	}

	auto person_ages = persons.get<Age>();

	std::cout << "person ages:";
	for(auto age : person_ages) std::cout << age << " ";
	std::cout << std::endl;


	std::vector<ID> person_ids;
	persons.get_to(person_ids);

	std::cout << "person IDs:";
	for(auto id : person_ids) std::cout << id << " ";
	std::cout << std::endl;
	

	auto person_id_ages = persons.extract<ID, Age>();
	for(auto elm : person_id_ages) std::cout << "ID: " << elm.get<ID>() << ", Age:" << elm.get<Age>() << std::endl;


	typed_vector<Name, ID> person_name_ids;
	persons.extract_to(person_name_ids);
	for(auto elm : person_name_ids) std::cout << "ID: " << elm.get<ID>() << ", Name:" << elm.get<Name>() << std::endl;



	return 0;
}