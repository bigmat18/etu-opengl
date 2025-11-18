#include "Buffers.hpp"
#include "VertexLayout.hpp"
#include "debug.hpp"
#include "logging.hpp"
#include <VertexArray.hpp>
#include <vector>

namespace etugl {

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
    LOG_INFO("VertexArray {} created", m_ID);
}

VerterArray::VerterArray(const std::vector<float>& vertices,
                         const std::vector<u32>& indices,
                         const VertexLayout layout) :
    m_VBO(vertices), 
    m_EBO(indices)
{
    glGenVertexArrays(1, &m_ID);
    glBindVertexArray(m_ID);

    m_VBO->bind();
    layout.bind();
    m_EBO->bind();

    glBindVertexArray(0);
    LOG_INFO("VertexArray {} created", m_ID);
}


VerterArray::~VerterArray() {
    LOG_INFO("VertexArray {} deleted", m_ID);
    glDeleteVertexArrays(1, &m_ID);
}

}
