#include "Buffers.hpp"
#include "VertexLayout.hpp"
#include "logging.hpp"
#include <VertexArray.hpp>
#include <cstddef>
#include <vector>

namespace etugl {

VertexArray::VertexArray(const VertexBuffer& vbo,
                         const IndexBuffer& ebo,
                         const VertexLayout layout,
                         const size_t num_elements) : m_NumElements(num_elements)
{
    glGenVertexArrays(1, &m_ID);
    glBindVertexArray(m_ID);

    vbo.bind();
    layout.bind();
    ebo.bind();

    glBindVertexArray(0);
    LOG_INFO("VertexArray {} create: SUCCESS", m_ID);
}

VertexArray::VertexArray(const std::vector<float>& vertices,
                         const std::vector<u32>& indices,
                         const VertexLayout layout) :
    m_VBO(vertices), 
    m_EBO(indices),
    m_NumElements(indices.size())
{
    glGenVertexArrays(1, &m_ID);
    glBindVertexArray(m_ID);

    m_VBO->bind();
    layout.bind();
    m_EBO->bind();

    glBindVertexArray(0);
    LOG_INFO("VertexArray {} create: SUCCESS", m_ID);
}


VertexArray::~VertexArray() {
    u32 id = m_ID;
    glDeleteVertexArrays(1, &m_ID);
    LOG_INFO("VertexArray {} destroy: SUCCESS", id);
}

}
