#include "OpenGL/texture.h"

Texture::Texture(GLenum type, GLenum format) : type(type), format(format) {
	glGenTextures(1, &m_TextureID);
	Bind();
	SetPar(GL_TEXTURE_WRAP_S, GL_REPEAT);
	SetPar(GL_TEXTURE_WRAP_T, GL_REPEAT);
	SetPar(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	SetPar(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::Texture(Texture&& other) noexcept
	: m_TextureID(other.m_TextureID), type(other.type), format(other.format) {
	other.m_TextureID = 0;
}

Texture::~Texture() {
	Release();
}

Texture& Texture::operator=(Texture&& other) noexcept {
	if (this != &other) {
		Release();
		std::swap(m_TextureID, other.m_TextureID);
		std::swap(this->type, other.type);
		std::swap(this->format, other.format);
	}
	return *this;
}

void Texture::Load(const void* texture, const int& width,const int& height) {
	glTexImage2D(this->type, 0, this->format, width, height,
		0, this->format, GL_UNSIGNED_BYTE, texture);
	glGenerateMipmap(this->format);
	
	if (texture == nullptr)
		std::cout << "ERROR::TEXTURE::UNDEFINED_TEXT_PTR" << std::endl;
}

void Texture::SetPar(const GLenum& pName, const GLenum& param) {
	glTexParameteri(this->type, pName, param);
}

void Texture::Bind() const {
	glBindTexture(this->type, m_TextureID);
}

void Texture::Unbind() const {
	glBindTexture(this->type, 0);
}