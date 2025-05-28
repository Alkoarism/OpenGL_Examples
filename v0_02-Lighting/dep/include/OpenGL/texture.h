#ifndef TEXTURE_H
#define TEXTURE_H

#include "OpenGL\texture_layout.h"

//The glTexParameteri on the texture constructor needs to be
//further analyzed and improved in order to make the class
//more corret relating to it´s configuration

class Texture {
public:
	Texture() { }
	Texture(const TextureLayout&, const char*);
	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;

	void SetLayout(const TextureLayout&);

	void Load(const char*);
	void DirectLoad(const void*, const int&, const int&);
	void Bind() const;
	void Unbind() const;

private:
	GLuint m_TextureID;
	TextureLayout m_Layout;

	void Release() noexcept {
		glDeleteTextures(1, &m_TextureID);
		m_TextureID = 0;
	}
};

#endif