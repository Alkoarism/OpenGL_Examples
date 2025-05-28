#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "OpenGL\renderer.h"

class SpriteRenderer {
public:
	SpriteRenderer(std::string& shader);
	~SpriteRenderer();

	void DrawSprite(std::string& textureName, glm::vec2 position,
		glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f));
	
private:
	std::string m_ShaderName;
	std::unique_ptr<IndexBuffer> m_IBO;
	std::unique_ptr<VertexBuffer> m_VBO;
	VertexArray m_QuadVAO;
};

#endif