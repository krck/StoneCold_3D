
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
	using byte = std::uint8_t;
	using entityId = std::uint32_t;
	using hash = std::uint64_t;

	template<typename T>
	inline hash GetTypeHash() noexcept { return std::type_index(typeid(T)).hash_code(); }

	// Dawnbringer Palette (32 Colors) as SDL_Color sturcts
	// (The gray's differ a bit form the original and the Names are also custom)
	#define SC_COLOR_BLACK			glm::u8vec3(0,	 0,   0  )
	#define SC_COLOR_SLATE_GRAY		glm::u8vec3(34,  32,  52 )
	#define SC_COLOR_PLUM_PURPLE	glm::u8vec3(69,  40,  60 )
	#define SC_COLOR_DARK_BROWN		glm::u8vec3(102, 57,  49 )
	#define SC_COLOR_LIGHT_BROWN	glm::u8vec3(143, 86,  59 )
	#define SC_COLOR_ORANGE			glm::u8vec3(223, 113, 38 )
	#define SC_COLOR_DARK_SAND		glm::u8vec3(217, 160, 102)
	#define SC_COLOR_LIGHT_SAND		glm::u8vec3(238, 195, 154)
	#define SC_COLOR_YELLOW			glm::u8vec3(251, 242, 54 )
	#define SC_COLOR_LIGHT_GREEN	glm::u8vec3(153, 229, 80 )
	#define SC_COLOR_MEDIUM_GREEN	glm::u8vec3(106, 190, 48 )
	#define SC_COLOR_TURQUOISE 		glm::u8vec3(55,  148, 110)
	#define SC_COLOR_DARK_GREEN		glm::u8vec3(75,  105, 47 )
	#define SC_COLOR_MOSS_GREEN		glm::u8vec3(55,  148, 110)
	#define SC_COLOR_PINE_GREEN		glm::u8vec3(50,  60,  57 )
	#define SC_COLOR_DARK_PURPLE	glm::u8vec3(63,  63,  116)
	#define SC_COLOR_MARINE_BLUE	glm::u8vec3(48,  96,  130)
	#define SC_COLOR_DARK_BLUE		glm::u8vec3(91,  110, 225)
	#define SC_COLOR_MEDIUM_BLUE	glm::u8vec3(99,  155, 255)
	#define SC_COLOR_LIGHT_BLUE		glm::u8vec3(95,  205, 228)
	#define SC_COLOR_ICE_BLUE		glm::u8vec3(203, 219, 252)
	#define SC_COLOR_WHITE			glm::u8vec3(255, 255, 255)
	#define SC_COLOR_STONE_GRAY		glm::u8vec3(155, 173, 183)
	#define SC_COLOR_LIGHT_GRAY		glm::u8vec3(146, 146, 146)
	#define SC_COLOR_MEDIUM_GRAY	glm::u8vec3(115, 115, 115)
	#define SC_COLOR_DARK_GRAY		glm::u8vec3(89,  86,  82 )
	#define SC_COLOR_PURPLE			glm::u8vec3(118, 66,  138)
	#define SC_COLOR_DARK_RED		glm::u8vec3(172, 50,  50 )
	#define SC_COLOR_LIGHT_RED		glm::u8vec3(217, 87,  99 )
	#define SC_COLOR_PINK			glm::u8vec3(215, 123, 186)
	#define SC_COLOR_OLIVE_GREEN	glm::u8vec3(143, 151, 74 )
	#define SC_COLOR_MERIGOLD		glm::u8vec3(138, 111, 48 )
}

#endif
