#pragma once

#include <Types.hpp>
#include <algorithm>
#include <cstddef>
#include <optional>
#include <vector>

#include <Buffers.hpp>
#include <VertexLayout.hpp>
 
namespace etugl {

class VertexArray {
    std::optional<VertexBuffer> m_VBO = std::nullopt;
    std::optional<IndexBuffer>  m_EBO = std::nullopt;

    size_t m_NumElements = 0;
    u32 m_ID = 0;

public:

    VertexArray() = default;

    VertexArray(const VertexBuffer& vbo, 
                const IndexBuffer& ebo,
                const VertexLayout layout,
                const size_t num_elements);

    VertexArray(const std::vector<float>& vertices,
                const std::vector<u32>& indices,
                const VertexLayout layout);

    VertexArray(const VertexArray& other) = delete;

    VertexArray& operator=(const VertexArray& other) = delete;

    VertexArray(VertexArray&& other) noexcept { swap(other); }

    VertexArray& operator=(VertexArray&& other) noexcept {swap(other);return *this;}

    ~VertexArray();

    void swap(VertexArray& other) { 
        using std::swap;
        swap(m_VBO, other.m_VBO);
        swap(m_EBO, other.m_EBO);
        swap(m_ID, other.m_ID);
        swap(m_NumElements, other.m_NumElements);
    }

    friend void swap(VertexArray& first, VertexArray& second) { first.swap(second); }

    inline void bind() const { glBindVertexArray(m_ID); }

    inline void unbind() const { glBindVertexArray(0); }

    inline void draw() const { draw(m_NumElements, 0); } 

    inline void draw(size_t count, size_t offset) const {
        glBindVertexArray(m_ID);
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)offset);
    } 
};
}

