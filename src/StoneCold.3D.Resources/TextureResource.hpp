
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
		TextureResource(std::string name, uint32 textureID, uint32 normalID, uint32 specularID)
			: Resource(name), _material(Material()), _textureDiffuse(textureID), _normalMap(normalID), _specularMap(specularID) { }

		//void BindTexture() const {
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, _textureDiffuse);
		//}

		//void UnbindTexture() const {
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, 0);
		//}

		//void BindNormalMap() const {
		//	if (_normalMap) {
		//		glActiveTexture(GL_TEXTURE1);
		//		glBindTexture(GL_TEXTURE_2D, _normalMap);
		//	}
		//}
		//void UnbindNormalMap() const {
		//	if (_normalMap) {
		//		glActiveTexture(GL_TEXTURE1);
		//		glBindTexture(GL_TEXTURE_2D, 0);
		//	}
		//}

		//void BindSpecularMap() const {
		//	if (_specularMap) {
		//		glActiveTexture(GL_TEXTURE2);
		//		glBindTexture(GL_TEXTURE_2D, _specularMap);
		//	}
		//}
		//void UnbindSpecularMap() const {
		//	if (_specularMap) {
		//		glActiveTexture(GL_TEXTURE2);
		//		glBindTexture(GL_TEXTURE_2D, 0);
		//	}
		//}

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
