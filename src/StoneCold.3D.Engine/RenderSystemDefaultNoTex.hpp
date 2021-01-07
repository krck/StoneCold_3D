
#ifndef STONECOLD_MOTIONRENDERSYSTEM_H
#define STONECOLD_MOTIONRENDERSYSTEM_H

#include "StoneColdBase.hpp"
#include "EntityComponentSystem.hpp"
#include "DefaultShaderNoTex.hpp"

namespace StoneCold::Engine {

	class RenderSystemDefaultNoTex : public System {
	public:
		//
		// Hardcoded System Component-Mask: 
		// Only Entities with a TransformationComponent, a ShaderComponent and a MeshComponent will be rendered with this System
		//
		RenderSystemDefaultNoTex(EntityComponentSystem& ecs)
			: System(MASK_SHADER_DEFAULTNOTEX | GetComponentMask<TransformationComponent>() | GetComponentMask<MeshComponent>())
			, _ecs(ecs)
			, _shader(DefaultShaderNoTex())
		{ }

		RenderSystemDefaultNoTex(const RenderSystemDefaultNoTex&) = delete;
		RenderSystemDefaultNoTex& operator=(const RenderSystemDefaultNoTex&) = delete;

		virtual void Render(const glm::mat4& projection, const glm::mat4& view) override {
			auto& transformationComponents = *_ecs.GetComponentArray<TransformationComponent>();
			auto& meshComponents = *_ecs.GetComponentArray<MeshComponent>();

			_shader.Bind();
			_shader.SetUniformMat4("u_projection", projection);
			_shader.SetUniformMat4("u_view", view);

			for (const auto& entityId : _entities) {
				auto& t = transformationComponents[entityId];
				auto& m = meshComponents[entityId];

				_shader.SetUniformMat4("u_transform", t.Transformation);

				glBindVertexArray(m.VAO);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.EBO);
				glDrawElements(GL_TRIANGLES, (int)m.Size, GL_UNSIGNED_INT, nullptr);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			}
			_shader.Unbind();
		}

	private:
		//SDL_Renderer* _renderer;
		EntityComponentSystem& _ecs;
		DefaultShaderNoTex _shader;
	};

}

#endif
