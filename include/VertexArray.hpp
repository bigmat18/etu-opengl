#pragma once

#include <Types.hpp>
#include <cstddef>
#include <optional>
#include <vector>

#include <Buffers.hpp>
#include <VertexLayout.hpp>
 
namespace etugl {

class VerterArray {
    std::optional<VertexBuffer> m_VBO;
    std::optional<IndexBuffer>  m_EBO;

    size_t m_NumElements = 0;
    u32 m_ID = 0;

public:
    VerterArray(const VertexBuffer& vbo, 
                const IndexBuffer& ebo,
                const VertexLayout layout,
                const size_t num_elements);

    VerterArray(const std::vector<float>& vertices,
                const std::vector<u32>& indices,
                const VertexLayout layout);


    VerterArray(const VerterArray& other) = delete;
    VerterArray& operator=(const VerterArray& other) = delete;

    ~VerterArray();

    inline void bind() const { glBindVertexArray(m_ID); }

    inline void unbind() const { glBindVertexArray(0); }

    inline void draw() const {
        glBindVertexArray(m_ID);
        glDrawElements(GL_TRIANGLES, m_NumElements, GL_UNSIGNED_INT, 0);
    } 
};

}
