#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STBI_FAILURE_USERMSG
#define STBI_ONLYJPEG

#include "OpenGL/texture.h"

Texture::Texture(const TextureLayout& layout, const char* location) : m_Layout(layout) {
	Load(location);
}

Texture::Texture(Texture&& other) noexcept
	: m_TextureID(other.m_TextureID), m_Layout(other.m_Layout) {
	other.m_TextureID = 0;
}

Texture::~Texture() {
	Release();
}


Texture& Texture::operator=(Texture&& other) noexcept {
	if (this != &other) {
		Release();
		std::swap(m_TextureID, other.m_TextureID);
		std::swap(m_Layout, other.m_Layout);
	}
	return *this;
}

void Texture::SetLayout(const TextureLayout& layout) {
	m_Layout = layout;
}

void Texture::Load(const char* location) {

	stbi_set_flip_vertically_on_load(true);
	int imgWidth, imgHeight, nrChannels;
	unsigned char* data
		= stbi_load(location, &imgWidth, &imgHeight, &nrChannels, 0);
	if (data) {
		DirectLoad(data, imgWidth, imgHeight);
	}
	else {
		const char* failLog = stbi_failure_reason();
		std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD_TEXTURE\n" << failLog << std::endl;
	}
	stbi_image_free(data);

}

void Texture::DirectLoad(const void* texture, const int& width,const int& height) {
	glGenTextures(1, &m_TextureID);
	Bind();

	if (texture) {
		glTexImage2D(m_Layout.GetType(), 0, m_Layout.GetFormat(),
			width, height, 0, m_Layout.GetFormat(), GL_UNSIGNED_BYTE, texture);
		glGenerateMipmap(m_Layout.GetType());
	}
	else {
		std::cout << "ERROR::TEXTURE::FAILED_TO_DIRECT_LOAD_TEXTURE" << std::endl;
	}

	m_Layout.Run();
}

void Texture::Bind() const {
	glBindTexture(m_Layout.GetType(), m_TextureID);
}

void Texture::Unbind() const {
	glBindTexture(m_Layout.GetType(), 0);
}