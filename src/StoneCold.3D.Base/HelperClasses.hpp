
#ifndef STONECOLD_HELPERCLASSES_H
#define STONECOLD_HELPERCLASSES_H

#include "Types.hpp"
#include "StoneColdBase.hpp"

namespace StoneCold::Base {

	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec2 TextureCoords;
	};

	struct Material {
		float AmbientReflectivity = 0.0f;
		float DiffuseReflectivity = 0.0f;
		float SpectralReflectivity = 0.0f;
		float ShineDamper = 10.0f;
	};

}

#endif
