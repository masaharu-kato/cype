#pragma once

namespace cype {

//	wrapper for primitive (built-in) types
	template <class Type>
	class primitive_wrapper {
	private:
		Type value;

	public:
		primitive_wrapper(Type value) noexcept
			: value(value) {}

		operator Type() const noexcept {
			return value;
		}

		operator Type&() noexcept {
			return value;
		}
	};

}