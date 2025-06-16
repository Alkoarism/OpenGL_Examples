#ifndef ENTITY_H
#define ENTITY_H

#include "OpenGL/renderer.h"

//STORES THE DATA managed by a single entity on a currently running program
class Entity{
public:
    glm::vec3 position, size, color;
    float rotation;
    bool active;

    Entity();
    Entity(glm::vec3 pos, glm::vec3 size, glm::vec3 color);

    virtual ~Entity() = default;
    
    virtual void Draw();
            
    void SetShader(const std::string name);
    void SetTexture(const std::string name);
    void SetMesh(const std::string name);

    std::string GetShader() { return m_ShaderName; };
    std::string GetTexture() { return m_TextureName; };
    std::string GetMesh() { return m_MeshName; };

private:
	std::string m_ShaderName;
    std::string m_TextureName;
    std::string m_MeshName;

//    Shader& m_shader;
//    Texture& m_Texture;
//    Mesh& m_Mesh;
};

#endif