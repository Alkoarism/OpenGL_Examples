#include "sprite_renderer.h"

SpriteRenderer::SpriteRenderer(std::string& shaderName) : m_ShaderName(shaderName) {
	float vertices[] = {
		// pos      // tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 1, 3
	};

	VertexBufferLayout vbl;
	vbl.Push<float>(4);
	m_QuadVAO.Bind();
	m_IBO.reset(new IndexBuffer(indices, 6));
	m_VBO.reset(new VertexBuffer(vertices, sizeof(vertices)));
	m_QuadVAO.AddBuffer(*m_VBO, vbl);
	//m_QuadVAO.Unbind();		//--> Usually unnecessary
}

SpriteRenderer::~SpriteRenderer() {

}

void SpriteRenderer::DrawSprite(std::string& textureName, 
	glm::vec2 position, 
	glm::vec2 size, float rotate, 
	glm::vec3 color) {

	Renderer::GetShader(m_ShaderName).Use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));

	if (rotate != 0) {
		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
		model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
	}

	model = glm::scale(model, glm::vec3(size, 1.0f));

	Renderer::GetShader(m_ShaderName).SetUniform("model", model);
	Renderer::GetShader(m_ShaderName).SetUniform("textColor", color);

	glActiveTexture(GL_TEXTURE0);
	Renderer::GetTexture(textureName).Bind();

	Renderer::Render(m_QuadVAO, *m_IBO, Renderer::GetShader(m_ShaderName));
}
