#pragma once

#include <Types.hpp>
#include <optional>
#include <vector>

#include <Buffers.hpp>
#include <VertexLayout.hpp>

namespace etugl {

class VerterArray {
    std::optional<VertexBuffer> m_VBO;
    std::optional<IndexBuffer>  m_EBO;
    u32 m_ID;

public:
    VerterArray(const VertexBuffer& vbo, 
                const IndexBuffer& ebo,
                const VertexLayout layout);

    VerterArray(const std::vector<float>& vertices,
                const std::vector<u32>& indices,
                const VertexLayout layout);


    VerterArray(const VerterArray& other) = delete;
    VerterArray& operator=(const VerterArray& other) = delete;

    ~VerterArray();

    inline void bind() const { glBindVertexArray(m_ID); }

    inline void unbind() const { glBindVertexArray(0); }
};

}
