#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "headers.h"

class VertexBuffer {
public:
	VertexBuffer(const void*, unsigned int);
	~VertexBuffer();

	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;
	VertexBuffer(VertexBuffer&& other) noexcept;
	VertexBuffer& operator=(VertexBuffer&& other) noexcept;

	void Bind() const;
	void Unbind() const;

private:
	GLuint m_vertexBufferID = 0;
	void Release() noexcept {
		glDeleteBuffers(1, &m_vertexBufferID);
		m_vertexBufferID = 0;
	}
};

#endif