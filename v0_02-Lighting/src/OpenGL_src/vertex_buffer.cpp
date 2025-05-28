#include "OpenGL/vertex_buffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
	glGenBuffers(1, &m_vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
	Release();
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept 
	: m_vertexBufferID(other.m_vertexBufferID) {
	other.m_vertexBufferID = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept {
	if (this != &other) {
		Release();
		std::swap(m_vertexBufferID, other.m_vertexBufferID);
	}
	return *this;
}

void VertexBuffer::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
}

void VertexBuffer::Unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}