#ifndef TEXTURE_H
#define TEXTURE_H

#include "headers.h"

class Texture {
public:
	GLenum type;
	GLenum format;

	Texture() : Texture(GL_TEXTURE_2D, GL_RGB) { }
	Texture(GLenum type, GLenum format);
	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;

	void Load(const void*, const int&, const int&);
	void SetPar(const GLenum& first, const GLenum& second);
	void Bind() const;
	void Unbind() const;

private:
	GLuint m_TextureID;

	void Release() noexcept {
		glDeleteTextures(1, &m_TextureID);
		m_TextureID = 0;
	}
};

#endif