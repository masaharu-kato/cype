#pragma once

namespace cype {

//	value with type `Type`
	template <class Type, class ValType>
	class typed {
	public:
		using type = Type;

	private:
		ValType value;

	public:
		typed(const ValType& value) noexcept
			: value(value) {}

		template <class _Type>
		explicit typed(const typed<_Type, ValType>& v)
			: value(v.value) {}

		template <class _ValType>
		typed(const typed<Type, _ValType>& v)
			: value(v.value) {}

		ValType get() const noexcept {
			return value;
		}

		operator ValType() const noexcept {
			return value;
		}

	};

	template <class Type, class ValType>
	auto make_typed(const ValType& value) {
		return typed<Type, ValType>(value);
	}


}