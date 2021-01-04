
#ifndef STONECOLD_SETTINGS_H
#define STONECOLD_SETTINGS_H

#include <glm/vec3.hpp>

#define WINDOW_WIDTH			2560
#define WINDOW_HEIGHT			1440

#define MAX_LIGHTS              4

#define KEY_CODES               512
#define KEY_BUFFER_SZ           4

#define MIP_MAPPING             1
#define ANISOTROPIC_FILTERING   1

#define MSAA                    8       // 0 - 2 - 4 - 8

#define FRAME_SAMPLES           10
#define FPS_MAX                 60		// Handled by V-Sync

#define RNG_SEED				12345
#define MAP_SIZE				70

#define KEY_CODES               512
#define KEY_BUFFER_SIZE			4

#define FONT_SIZE_SMALL			10
#define FONT_SIZE_NORMAL		18
#define FONT_SIZE_BIG			24

// Dawnbringer Palette (32 Colors) as SDL_Color sturcts
// (The gray's differ a bit form the original and the Names are also custom)
#define CL_BLACK			glm::u8vec3(0,	 0,   0  )
#define CL_SLATE_GRAY		glm::u8vec3(34,  32,  52 )
#define CL_PLUM_PURPLE		glm::u8vec3(69,  40,  60 )
#define CL_DARK_BROWN		glm::u8vec3(102, 57,  49 )
#define CL_LIGHT_BROWN		glm::u8vec3(143, 86,  59 )
#define CL_ORANGE			glm::u8vec3(223, 113, 38 )
#define CL_DARK_SAND		glm::u8vec3(217, 160, 102)
#define CL_LIGHT_SAND		glm::u8vec3(238, 195, 154)
#define CL_YELLOW			glm::u8vec3(251, 242, 54 )
#define CL_LIGHT_GREEN		glm::u8vec3(153, 229, 80 )
#define CL_MEDIUM_GREEN		glm::u8vec3(106, 190, 48 )
#define CL_TURQUOISE 		glm::u8vec3(55,  148, 110)
#define CL_DARK_GREEN		glm::u8vec3(75,  105, 47 )
#define CL_MOSS_GREEN		glm::u8vec3(55,  148, 110)
#define CL_PINE_GREEN		glm::u8vec3(50,  60,  57 )
#define CL_DARK_PURPLE		glm::u8vec3(63,  63,  116)
#define CL_MARINE_BLUE		glm::u8vec3(48,  96,  130)
#define CL_DARK_BLUE		glm::u8vec3(91,  110, 225)
#define CL_MEDIUM_BLUE		glm::u8vec3(99,  155, 255)
#define CL_LIGHT_BLUE		glm::u8vec3(95,  205, 228)
#define CL_ICE_BLUE			glm::u8vec3(203, 219, 252)
#define CL_WHITE			glm::u8vec3(255, 255, 255)
#define CL_STONE_GRAY		glm::u8vec3(155, 173, 183)
#define CL_LIGHT_GRAY		glm::u8vec3(146, 146, 146)
#define CL_MEDIUM_GRAY		glm::u8vec3(115, 115, 115)
#define CL_DARK_GRAY		glm::u8vec3(89,  86,  82 )
#define CL_PURPLE			glm::u8vec3(118, 66,  138)
#define CL_DARK_RED			glm::u8vec3(172, 50,  50 )
#define CL_LIGHT_RED		glm::u8vec3(217, 87,  99 )
#define CL_PINK				glm::u8vec3(215, 123, 186)
#define CL_OLIVE_GREEN		glm::u8vec3(143, 151, 74 )
#define CL_MERIGOLD			glm::u8vec3(138, 111, 48 )

#endif
