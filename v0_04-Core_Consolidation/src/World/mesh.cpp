#include "World/mesh.h"

Mesh::Mesh( const std::vector<float>& vData,
            const std::vector<unsigned int>& bufferLayout,
            const std::vector<unsigned int>& indices) 
{
    for (auto i = bufferLayout.begin(); i != bufferLayout.end(); i++){
        m_VBL.Push<float>(*i);
    }
    m_VBO.reset(new VertexBuffer(vData.data(), sizeof(decltype(vData[0])) * vData.size()));
    m_IBO.reset(new IndexBuffer(&indices[0], indices.size()));
    //m_VBO->Unbind();    //Usually unnecessary
    //m_IBO->Unbind();    //Usually unnecessary
}

Mesh::~Mesh(){

}