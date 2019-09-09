#pragma once

namespace cype {

//	binary type of specified type
	class binary {
	private:
		using byte = char;
		size_t size = 0;
		byte* data = nullptr;

	public:
		binary() = default;

		binary(byte* data, size_t size)
			: size(size), data(new data[size])
		{}

		template <class T>
		T cast_to() const;

		~binary() {
			delete[] data;
		}
	};

}