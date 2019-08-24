#include <iostream>
#include "data.h"

template <size_t I>
struct Value {
public:
	double value;

	Value(double value)
		: value(value) {}

	void show() const {
		std::cout << "[" << I << "] " << value << std::endl;
	};

	Value operator +() const { return +value; }
	Value operator -() const { return -value; }

	Value operator +(Value v) const { return value + v.value; }
	Value operator -(Value v) const { return value - v.value; }

};

template <size_t... I>
struct Vector : public cype::Data<Value<I>...> {

	using _BaseType = cype::Data<Value<I>...>;

//	using _BaseType::_BaseType;

	Vector(const _BaseType& base)
		: _BaseType(base) {}

	Vector(Value<I>... vals)
		: _BaseType(vals...) {}

	void show() const {
		return visit_each([](auto v){ v.show(); });
	}

	Vector operator +() const {
		return operate<_OpPlus>();
	}

	Vector operator -() const {
		return operate<_OpMinus>();
	}

	Vector operator +(const Vector& vec) const {
		return operate<_OpAdd>(vec);
	}

	Vector operator -(const Vector& vec) const {
		return operate<_OpSub>(vec);
	}


private:

	struct _OpPlus  { template <class _T> static _T call(_T v) { return +v; } };
	struct _OpMinus { template <class _T> static _T call(_T v) { return -v; } };

	struct _OpAdd { template <class _T> static _T call(_T v1, _T v2) { return v1 + v2; } };
	struct _OpSub { template <class _T> static _T call(_T v1, _T v2) { return v1 - v2; } };

};


template <size_t... I>
struct _ISeqHelper {
	template <template <size_t...> class _Target> using apply_to = _Target<I...>;
	template <size_t _I> using push_back = _ISeqHelper<I..., _I>;
};


template <size_t FIRST, size_t LAST>
struct _ISeq {
	using Type = typename _ISeq<FIRST, LAST-1>::Type::template push_back<LAST>;
};

template <size_t FIRST>
struct _ISeq<FIRST, FIRST> {
	using Type = _ISeqHelper<FIRST>;
};


template <template <size_t...> class _Target, size_t FIRST, size_t LAST>
using ApplyISeq = typename _ISeq<FIRST, LAST>::Type::template apply_to<_Target>;


template <size_t N>
using VectorND = ApplyISeq<Vector, 1, N>;




int main(void) {

	VectorND<4> vec1( 12.5, -21.3, 35.7, 42.6);
	VectorND<4> vec2(  6.4,   8.5,- 2.5, 10.3);
 
	auto vec3 = - vec1 + vec2;

	vec3.show();

	return 0;
}