
#ifndef STONECOLD_RENDERSYSTEMMAP_H
#define STONECOLD_RENDERSYSTEMMAP_H

#include "StoneColdBase.hpp"
#include "EntityComponentSystem.hpp"
#include "ShaderMap.hpp"

namespace StoneCold::Engine {

    class RenderSystemMap : public System {
    public:
        //
        // Hardcoded System Component-Mask:
        // Only Entities with a TransformationComponent, a ShaderComponent and a MeshComponent will be rendered with this System
        //
        RenderSystemMap(EntityComponentSystem& ecs)
            : System(MASK_SHADER_MAP | GetComponentMask<TransformationComponent>()  | GetComponentMask<MeshComponent>() | GetComponentMask<TextureComponent>())
            , _ecs(ecs)
            , _shader(ShaderMap())
        { }

        RenderSystemMap(const RenderSystemMap&) = delete;
        RenderSystemMap& operator=(const RenderSystemMap&) = delete;

        virtual void Render(const glm::mat4& projection, const glm::mat4& view) override {
            auto& transformationComponents = *_ecs.GetComponentArray<TransformationComponent>();
            auto& textureComponents = *_ecs.GetComponentArray<TextureComponent>();
            auto& meshComponents = *_ecs.GetComponentArray<MeshComponent>();

            _shader.Bind();
            _shader.SetUniformMat4("u_ViewMatrix", view);
            _shader.SetUniformMat4("u_ProjectionMatrix", projection);

            for (const auto& entityId : _entities) {
                auto& t = transformationComponents[entityId];
                auto& x = textureComponents[entityId];
                auto& m = meshComponents[entityId];

                _shader.SetUniformMat4("u_TransformMatrix", t.Transformation);
                _shader.SetUniformInt("u_DiffuseTextureSampler", 0);

                // Bind the Vertex Array + Index/Element Buffer and draw the Mesh
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, x.TextureId);
                glBindVertexArray(m.VAO);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.EBO);
                glDrawElements(GL_TRIANGLES, (int)m.Size, GL_UNSIGNED_INT, nullptr);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
            _shader.Unbind();
        }

    private:
        EntityComponentSystem& _ecs;
        ShaderMap _shader;
    };

}

#endif
