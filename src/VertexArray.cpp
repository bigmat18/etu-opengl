#include "Buffer.hpp"
#include "VertexLayout.hpp"
#include <VertexArray.hpp>
#include <vector>

namespace etugl {

VerterArray::VerterArray(const std::vector<float>& vertices,
                         const std::vector<u32>& indices,
                         const VertexLayout layout) :
    m_VBO(std::in_place, vertices), 
    m_EBO(std::in_place, indices)
{
    glGenVertexArrays(1, &m_ID);
    glBindVertexArray(m_ID);
    m_VBO->bind();
    layout.bind();
    m_EBO->bind();
    glBindVertexArray(0);
}

VerterArray::VerterArray(const VertexBuffer& vbo,
                         const IndexBuffer& ebo,
                         const VertexLayout layout) 
{
    
    glGenVertexArrays(1, &m_ID);
    glBindVertexArray(m_ID);
    vbo.bind();
    layout.bind();
    ebo.bind();
    glBindVertexArray(0);
}

VerterArray::~VerterArray() {
    glDeleteVertexArrays(1, &m_ID);
}

inline void VerterArray::bind() const {
    glBindVertexArray(m_ID);
}

inline void VerterArray::unbind() const {
    glBindVertexArray(0);
}

}
