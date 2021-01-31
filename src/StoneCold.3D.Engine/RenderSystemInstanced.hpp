
#ifndef STONECOLD_RENDERSYSTEMINSTANCED_H
#define STONECOLD_RENDERSYSTEMINSTANCED_H

#include "StoneColdBase.hpp" 
#include "EntityComponentSystem.hpp"
#include "ShaderInstanced.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace StoneCold::Engine {

    class RenderSystemInstanced : public System {
    public:
        //
        // Hardcoded System Component-Mask:
        // Only Entities with a InstanceComponent, a ShaderComponent and a MeshComponent will be rendered with this System
        //
        RenderSystemInstanced(EntityComponentSystem& ecs)
            : System(MASK_SHADER_INSTANCED | GetComponentMask<MeshComponent>() | GetComponentMask<TextureComponent>()  | GetComponentMask<InstanceComponent>())
            , _ecs(ecs)
            , _shader(ShaderInstanced()) 
        { }

        RenderSystemInstanced(const RenderSystemInstanced&) = delete;
        RenderSystemInstanced& operator=(const RenderSystemInstanced&) = delete;

        virtual void Render(const glm::mat4& projection, const glm::mat4& view) override {
            auto& instanceComponents = *_ecs.GetComponentArray<InstanceComponent>();
            auto& textureComponents = *_ecs.GetComponentArray<TextureComponent>();
            auto& meshComponents = *_ecs.GetComponentArray<MeshComponent>();

            _shader.Bind();
            _shader.SetUniformMat4("view", view);
            _shader.SetUniformMat4("projection", projection);
            // Set the Lighting uniforms
            //_shader.SetUniformVec3("u_lightColor", glm::vec3(1.0f, 0.9f, 0.6f));
            _shader.SetUniformVec3("lightPos", glm::vec3(100.0f, 20.0f, 100.0f));

            for (const auto& entityId : _entities) {
                auto& i = instanceComponents[entityId];
                auto& x = textureComponents[entityId];
                auto& m = meshComponents[entityId];

                // Also set the Texture samplers / Material data
                //_shader.SetUniformFloat("u_spectralReflectivity", 2.0f);
                _shader.SetUniformInt("diffuseMap", 0);
                _shader.SetUniformInt("normalMap", 1);
                
                // Bind the Textures
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, x.TextureId);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, x.NormalMapId);
                // Bind the Vertex Array + Index/Element Buffer and draw the Mesh
                glBindVertexArray(m.VAO);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.EBO);
                glDrawElementsInstanced(GL_TRIANGLES, (int)m.Size, GL_UNSIGNED_INT, 0, (int)i.InstanceCount);
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
        ShaderInstanced _shader;
    };

}

#endif
