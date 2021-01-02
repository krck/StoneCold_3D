
#ifndef STONECOLD_TYPES_H
#define STONECOLD_TYPES_H

#include <cstdint>
#include <typeindex>
#include <typeinfo>

// Macro to get the name of a variable
#define GET_VARIABLE_NAME(Variable) (#Variable)

namespace StoneCold::Base {

	// StoneCold signed integer typedefs (fastest option, with a width of at least (!) 8, 16, 32 and 64 bits) 
	using int8 = std::int_fast8_t;
	using int16 = std::int_fast16_t;
	using int32 = std::int_fast32_t;
	using int64 = std::int_fast64_t;
	// StoneCold unsigned integer typedefs (fastest option, with a width of at least (!) 8, 16, 32 and 64 bits) 
	using uint8 = std::uint_fast8_t;
	using uint16 = std::uint_fast16_t;
	using uint32 = std::uint_fast32_t;
	using uint64 = std::uint_fast64_t;
	// StoneCold bit-mask typedefs (normal int option, with a width of exactly (!) 32 or 64 bits) 
	using bitMask32 = std::uint32_t;
	using bitMask64 = std::uint64_t;
	// StoneCold custom integer typedefs
	using byte = std::uint_fast8_t;
	using entityId = std::uint_fast32_t;
	using hash = std::uint_fast64_t;

	template<typename T>
	inline hash GetTypeHash() noexcept { return std::type_index(typeid(T)).hash_code(); }

}

#endif
