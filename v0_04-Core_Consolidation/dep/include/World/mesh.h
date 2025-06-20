#ifndef MESH_H
#define MESH_H

#include "OpenGL/vertex_array.h"
#include "OpenGL/index_buffer.h"

#include "headers.h"

class Mesh{
public:
    Mesh(const std::vector<float>& vData,
         const std::vector<unsigned int>& bufferLayout,
         const std::vector<unsigned int>& indices);
    ~Mesh();
    
    const VertexBuffer& GetVertexData(){ return *m_VBO; }
    const IndexBuffer& GetIndexData() { return *m_IBO; }
    const VertexBufferLayout& GetVertexLayout() { return m_VBL; }
    
private:
    VertexBufferLayout m_VBL;
    std::unique_ptr<VertexBuffer> m_VBO;
    std::unique_ptr<IndexBuffer> m_IBO;
};

#endif