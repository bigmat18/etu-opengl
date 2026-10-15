#pragma once

#include <utility>
#include <vector>
#include <string_view>
#include <glad/glad.h>

#include <VertexLayout.hpp>
#include <Types.hpp>
#include <core/logging.hpp>
#include <core/common.hpp>

namespace etugl {

template <GLenum type, typename T>
class Buffer {
    static constexpr std::string_view name = 
        (type == GL_ARRAY_BUFFER ? "VertexBuffer" : "IndexBuffer"); 
    u32 m_ID = 0;

public:

    Buffer(const std::vector<T>& data) {
        glGenBuffers(1, &m_ID);

        if (data.empty())
            core::logging::warn("Buffer created with empty data", VAR(name), VAR(m_ID));
        glBindBuffer(type, m_ID);
        glBufferData(type, data.size() * sizeof(T), 
                     data.data(), GL_STATIC_DRAW);

        if(glGetError() != GL_NO_ERROR) {
            u32 id = m_ID;
            glDeleteBuffers(1, &m_ID);
            core::logging::error("Buffer creation: FAILED", VAR(name), VAR(id));
        }

        glBindBuffer(type, 0);
        core::logging::info("Buffer create: SUCCESS", VAR(name), VAR(m_ID)); 
    };

    Buffer(const Buffer& other) = delete;

    Buffer& operator=(const Buffer& other) = delete;

    Buffer(Buffer&& other) noexcept { swap(other); }

    Buffer& operator= (Buffer&& other) noexcept { swap(other);return *this; }

    void swap(Buffer& other) {
        using std::swap;
        swap(m_ID, other.m_ID);
    }

    friend void swap(Buffer& first, Buffer& second) { first.swap(second); }

    ~Buffer() {
        u32 id = m_ID;
        glDeleteBuffers(1, &m_ID);
        core::logging::info("Buffer destroy: SUCCESS", VAR(name), VAR(id)); 
    }

    inline void bind() const { glBindBuffer(type, m_ID); };

    inline void unbind() const { glBindBuffer(type, 0); };
};

using VertexBuffer = Buffer<GL_ARRAY_BUFFER, float>;
using IndexBuffer = Buffer<GL_ELEMENT_ARRAY_BUFFER, u32>;

}
