#pragma once

namespace cype {

	template <class Type>
	class PrimitiveWrapper {
	public:
		PrimitiveWrapper(Type value) noexcept
			: value(value) {}

		operator Type() const noexcept {
			return value;
		}

		operator Type&() noexcept {
			return value;
		}

	private:
		Type value;
	};

}