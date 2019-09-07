#include <iostream>
#include "array.h"

//	sample class `Value`
template <size_t I>
struct Value {
public:
	using original_type = double;

	original_type value;

	Value() = default;

	Value(original_type value)
		: value(value) {}

	operator original_type() const {
		return value;
	}

	Value(const Value&) = default;

	template <size_t _I>
	Value(const Value<_I>&) = delete;

	void show() const {
		std::cout << "[" << I << "] " << value << std::endl;
	};

	Value operator +() const { return +value; }
	Value operator -() const { return -value; }

	Value operator +(Value v) const { return value + v.value; }
	Value operator -(Value v) const { return value - v.value; }



};

//	sample class `Vector`
template <size_t N>
struct Vector : public cype::array_range_of<size_t, ::Value, 1, N> {
private:

	struct _OpPlus  { template <class _T> static _T call(_T v) { return +v; } };
	struct _OpMinus { template <class _T> static _T call(_T v) { return -v; } };

	struct _OpAdd { template <class _T1, class _T2> static auto call(_T1 v1, _T2 v2) { return v1 + v2; } };
	struct _OpSub { template <class _T1, class _T2> static auto call(_T1 v1, _T2 v2) { return v1 - v2; } };

public:

	using _base_type = cype::array_range_of<size_t, ::Value, 1, N>;

	using _base_type::_base_type;

	Vector() = default;

	Vector(const _base_type& v) : _base_type(v) {}


	void show() const {
		return this->for_each([](auto v){ v.show(); });
	}

	Vector operator +() const {
		return this->template operate<_OpPlus>();
	}

	Vector operator -() const {
		return this->template operate<_OpMinus>();
	}

	Vector operator +(const Vector& vec) const {
		return this->template operate<_OpAdd>(vec);
	}

	Vector operator -(const Vector& vec) const {
		return this->template operate<_OpSub>(vec);
	}

	auto sum() const {
		return this->template reduce<_OpAdd>(double(0));
	}

};


template <class ValList>
void output_tmplval_list() {
	std::cout << ValList::template get<0>;
	if constexpr (ValList::size > 1) {
		std::cout << ",";
		output_tmplval_list<typename ValList::rests_list>();
	}
}

template <class ArrayType>
void output_array(std::string name, const ArrayType& arr) {
	std::cout << name << ": " << std::endl;
	arr.for_each([](auto v){
		std::cout << decltype(v)::index << ": " << v.value() << std::endl;
	});
	std::cout << std::endl;
}



int main(void) {


	Vector<4> vec1(  5.9, -21.3, 35.7, 42.6);
	Vector<4> vec2(  6.4,   8.5,- 2.5, 10.3);
 
	auto vec3 = - vec1 + vec2;

	std::cout << "vec3: " << std::endl;
	vec3.show();

	auto vec3_sum = vec3.sum();
	std::cout << "sum of vec3: " << vec3_sum << std::endl;


	auto arr1 = cype::make_array(10, -21, 32, -44, 58);
	auto arr2 = arr1.rearrange<2, 0, 4, 1, 3>();
	auto arr3 = arr2.extract<3, 0, 2>();
	auto arr3_r = arr3.reindex();

	output_array("arr1", arr1);
	output_array("arr2", arr2);
	output_array("arr3", arr3);
	output_array("arr3_r", arr3_r);



	cype::array<int, 10> arrn;
	arrn.fill(12);

	arrn.set<3>(34);
	arrn.set<8>(85);

	output_array("arrn", arrn);


	cype::array<std::string, 10> arr4;
	arr4.fill("empty");

	arr4.set<3>("Element 3");
	arr4.set<8>("Element 8");
	
	output_array("arr4", arr4);

	return 0;
}