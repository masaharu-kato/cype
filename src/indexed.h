#pragma once

namespace cype {

	template <class IType, class Type, IType Index>
	class Indexed {
	public:
		using original_type = Type;
		constexpr static IType index = Index;

	private:
		original_type value;

	public:
		Indexed(original_type value) noexcept
			: value(value) {}

		template <class _Type, IType _Index>
		explicit Indexed(Indexed<IType, _Type, _Index> v)
			: value(v.value) {}

		operator original_type() const noexcept {
			return value;
		}

	};


	template <class IType, class Type>
	struct indexed_of {
		template <IType Index>
		using type = Indexed<IType, Type, Index>;
	};

}