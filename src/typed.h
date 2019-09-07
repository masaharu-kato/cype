#pragma once

namespace cype {

//	value with type `Type`
	template <class Type, class ValType>
	class typed {
	public:
		using qualify_type = Type;
		using original_type = ValType;

	private:
		ValType _value;

	public:
	//	default constructor
		typed() = default;

	//	construct with original value type
		typed(const ValType& _value) noexcept
			: _value(_value) {}

		template <class _Type>
		explicit typed(const typed<_Type, ValType>& v)
			: _value(v._value) {}

		template <class _ValType>
		typed(const typed<Type, _ValType>& v)
			: _value(v.value()) {}

	//	get value
		ValType value() const noexcept {
			return _value;
		}

	//	get value with reference
		ValType& value() noexcept {
			return _value;
		}

	//	get value
		operator ValType() const noexcept {
			return _value;
		}

	//	get value with reference
		operator ValType&() noexcept {
			return _value;
		}

	};

//	make typed
	template <class Type, class ValType>
	auto make_typed(const ValType& value) {
		return typed<Type, ValType>(value);
	}


}