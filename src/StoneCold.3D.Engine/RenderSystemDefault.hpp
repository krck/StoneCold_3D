
#ifndef STONECOLD_RENDERSYSTEMDEFAULT_H
#define STONECOLD_RENDERSYSTEMDEFAULT_H

#include "StoneColdBase.hpp"
#include "EntityComponentSystem.hpp"
#include "DefaultShader.hpp"

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
			, _shader(DefaultShader())
		{ }

		RenderSystemDefault(const RenderSystemDefault&) = delete;
		RenderSystemDefault& operator=(const RenderSystemDefault&) = delete;

		virtual void Render(const glm::mat4& projection, const glm::mat4& view) override {
			auto& transformationComponents = *_ecs.GetComponentArray<TransformationComponent>();
			auto& textureComponents = *_ecs.GetComponentArray<TextureComponent>();
			auto& meshComponents = *_ecs.GetComponentArray<MeshComponent>();

			_shader.Bind();
			_shader.SetUniformMat4("u_projection", projection);
			_shader.SetUniformMat4("u_view", view);

			for (const auto& entityId : _entities) {
				auto& t = transformationComponents[entityId];
				auto& x = textureComponents[entityId];
				auto& m = meshComponents[entityId];

				_shader.SetUniformMat4("u_transform", t.Transformation);
				//_shader.SetUniformVec3("u_objectColor", SC_COLOR_WHITE);
				_shader.SetUniformInt("u_textureDiffuse", 0);

				glBindVertexArray(m.VAO);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.EBO);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, x.TextureId);
				glDrawElements(GL_TRIANGLES, (int)m.Size, GL_UNSIGNED_INT, nullptr);
				glBindTexture(GL_TEXTURE_2D, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			}
			_shader.Unbind();
		}

	private:
		//SDL_Renderer* _renderer;
		EntityComponentSystem& _ecs;
		DefaultShader _shader;
	};

}

#endif
