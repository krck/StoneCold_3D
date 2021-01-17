
#ifndef STONECOLD_RENDERSYSTEMDEFAULT_H
#define STONECOLD_RENDERSYSTEMDEFAULT_H

#include "StoneColdBase.hpp"
#include "EntityComponentSystem.hpp"
#include "ShaderDefault.hpp"

namespace StoneCold::Engine {

	class RenderSystemDefault : public System {
	public:
		//
		// Hardcoded System Component-Mask: 
		// Only Entities with a TransformationComponent, a ShaderComponent and a MeshComponent will be rendered with this System
		//
		RenderSystemDefault(EntityComponentSystem& ecs)
			: System(MASK_SHADER_DEFAULT | GetComponentMask<TransformationComponent>() | GetComponentMask<MeshComponent>() | GetComponentMask<TextureComponent>())
			, _ecs(ecs)
			, _shader(ShaderDefault())
		{ }

		RenderSystemDefault(const RenderSystemDefault&) = delete;
		RenderSystemDefault& operator=(const RenderSystemDefault&) = delete;

		virtual void Render(const glm::mat4& projection, const glm::mat4& view) override {
			auto& transformationComponents = *_ecs.GetComponentArray<TransformationComponent>();
			auto& textureComponents = *_ecs.GetComponentArray<TextureComponent>();
			auto& meshComponents = *_ecs.GetComponentArray<MeshComponent>();

			_shader.Bind();
            _shader.SetUniformMat4("u_ViewMatrix", view);
            _shader.SetUniformMat4("u_ProjectionMatrix", projection);
            // Set the Lighting uniforms
            //_shader.SetUniformVec3("u_lightColor", glm::vec3(1.0f, 0.9f, 0.6f));
            _shader.SetUniformVec3("u_LightPosition_worldspace", glm::vec3(100.0f, -50.0f, 100.0f));

			for (const auto& entityId : _entities) {
				auto& t = transformationComponents[entityId];
				auto& x = textureComponents[entityId];
				auto& m = meshComponents[entityId];

				_shader.SetUniformMat4("u_TransformMatrix", t.Transformation);
				//_shader.SetUniformVec3("u_objectColor", SC_COLOR_WHITE);
                _shader.SetUniformInt("u_DiffuseTextureSampler", 0);
                _shader.SetUniformInt("u_NormalTextureSampler", 1);

                // Bind the Textures
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, x.TextureId);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, x.NormalMapId);
                // Bind the Vertex Array + Index/Element Buffer and draw the Mesh
                glBindVertexArray(m.VAO);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.EBO);
                glDrawElements(GL_TRIANGLES, (int)m.Size, GL_UNSIGNED_INT, nullptr);
                // Unbind the Textures in reverse order (Unbinding the Buffers is not necessary)
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, 0);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, 0);
			}
			_shader.Unbind();
		}

	private:
		EntityComponentSystem& _ecs;
		ShaderDefault _shader;
	};

}

#endif
