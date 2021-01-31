
#ifndef STONECOLD_TEXTURERESOURCES_H
#define STONECOLD_TEXTURERESOURCES_H

#include <string>
#include "Resource.hpp"
#include "HelperClasses.hpp"

namespace StoneCold::Resources {

	using namespace StoneCold::Base;

	//
	// TextureResource
	// 
	// Full Texture containing the OpenGL buffers of a Texture-Image, a NormalMap-Image, a SpecularMap-Image
	// and additionally all Surface/Material information needed to calculate lighting on the textured object
	//
	class TextureResource : public Resource {
	public:
		TextureResource(const std::string& name, const Material& material, uint32 textureID, uint32 normalID, uint32 specularID)
			: Resource(name), _material(material), _textureDiffuse(textureID), _normalMap(normalID), _specularMap(specularID) { }

		inline uint32 GetTextureId() const { return _textureDiffuse; }
		inline uint32 GetNormalMapId() const { return _normalMap; }
		inline uint32 GetSpecularMapId() const { return _specularMap; }

		//inline void SetAmbientReflectivity(float aRef) { this->_material.AmbientReflectivity = aRef; }
		//inline void SetDiffuseReflectivity(float dRef) { this->_material.DiffuseReflectivity = dRef; }
		//inline void SetSpectralReflectivity(float sRef) { this->_material.SpectralReflectivity = sRef; }
		//inline void SetShineDamper(float sDamp) { this->_material.ShineDamper = sDamp; }

		//inline Material GetMaterial() const { return this->_material; }

	private:
		Material _material;
		uint32 _textureDiffuse;
		uint32 _normalMap;
		uint32 _specularMap;
	};

}

#endif
