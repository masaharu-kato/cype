#include <iostream>
#include "array.h"

//	sample class `Value`
template <size_t I>
struct Value {
public:
	using original_type = double;

	original_type value;

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
struct Vector : public cype::array_of_indexed<::Value, 1, N> {
private:

	struct _OpPlus  { template <class _T> static _T call(_T v) { return +v; } };
	struct _OpMinus { template <class _T> static _T call(_T v) { return -v; } };

	struct _OpAdd { template <class _T1, class _T2> static auto call(_T1 v1, _T2 v2) { return v1 + v2; } };
	struct _OpSub { template <class _T1, class _T2> static auto call(_T1 v1, _T2 v2) { return v1 - v2; } };

public:

	using _base_type = cype::array_of_indexed<::Value, 1, N>;

	using _base_type::_base_type;

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



int main(void) {

	Vector<4> vec1( 12.5, -21.3, 35.7, 42.6);
	Vector<4> vec2(  6.4,   8.5,- 2.5, 10.3);
 
	auto vec3 = - vec1 + vec2;

	std::cout << "vec3: " << std::endl;
	vec3.show();

	auto vec3_sum = vec3.sum();
	std::cout << "sum of vec3: " << vec3_sum << std::endl;


	auto arr1 = cype::make_array(10, -21, 32, -44, 58);
	auto arr2 = arr1.rearrange<2, 0, 4, 1, 3>();

	std::cout << "arr2:" << std::endl;
	arr2.for_each([](auto v){ std::cout << decltype(v)::index << ": " << v << std::endl; });

	auto arr3 = arr2.extract<3, 0, 2>();

	std::cout << "arr3:" << std::endl;
	arr3.for_each([](auto v){ std::cout << decltype(v)::index << ": " << v << std::endl; });

	return 0;
}