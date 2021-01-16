
#ifndef STONECOLD_RENDERSYSTEMINSTANCED_H
#define STONECOLD_RENDERSYSTEMINSTANCED_H

#include "StoneColdBase.hpp"
#include "EntityComponentSystem.hpp"
#include "DefaultShaderInstanced.hpp"

namespace StoneCold::Engine {

    class RenderSystemInstanced : public System {
    public:
        //
        // Hardcoded System Component-Mask:
        // Only Entities with a InstanceComponent, a ShaderComponent and a MeshComponent will be rendered with this System
        //
        RenderSystemInstanced(EntityComponentSystem& ecs)
            : System(MASK_SHADER_DEFAULTINSTANCED | GetComponentMask<MeshComponent>() | GetComponentMask<TextureComponent>()  | GetComponentMask<InstanceComponent>())
            , _ecs(ecs)
            , _shader(DefaultShaderInstanced())
        { }

        RenderSystemInstanced(const RenderSystemInstanced&) = delete;
        RenderSystemInstanced& operator=(const RenderSystemInstanced&) = delete;

        virtual void Render(const glm::mat4& projection, const glm::mat4& view) override {
            auto& instanceComponents = *_ecs.GetComponentArray<InstanceComponent>();
            auto& textureComponents = *_ecs.GetComponentArray<TextureComponent>();
            auto& meshComponents = *_ecs.GetComponentArray<MeshComponent>();

            _shader.Bind();
            _shader.SetUniformMat4("u_projection", projection);
            _shader.SetUniformMat4("u_view", view);

            for (const auto& entityId : _entities) {
                auto& i = instanceComponents[entityId];
                auto& x = textureComponents[entityId];
                auto& m = meshComponents[entityId];

                _shader.SetUniformInt("u_textureDiffuse", 0);

                glBindVertexArray(m.VAO);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.EBO);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, x.TextureId);
                glDrawElementsInstanced(GL_TRIANGLES, (int)m.Size, GL_UNSIGNED_INT, 0, (int)i.InstanceCount);
                glBindTexture(GL_TEXTURE_2D, 0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                glBindVertexArray(0);
            }
            _shader.Unbind();
        }

    private:
        //SDL_Renderer* _renderer;
        EntityComponentSystem& _ecs;
        DefaultShaderInstanced _shader;
    };

}

#endif
