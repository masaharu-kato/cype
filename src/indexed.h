#pragma once

namespace cype {

	template <class Type, size_t Index>
	class Indexed {
	public:
		using original_type = Type;
		constexpr static size_t index = Index;

	private:
		original_type value;

	public:
		Indexed(original_type value) noexcept
			: value(value) {}

		template <class _Type, size_t _Index>
		explicit Indexed(Indexed<_Type, _Index> v)
			: value(v.value) {}

		operator original_type() const noexcept {
			return value;
		}

	};


	template <class Type>
	struct indexed_of {
		template <size_t Index>
		using type = Indexed<Type, Index>;
	};

}