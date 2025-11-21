#pragma once

#include "logging.hpp"
#include <utility>
#include <vector>
#include <glad/glad.h>

#include <VertexLayout.hpp>
#include <Types.hpp>
#include <utils.hpp>

namespace etugl {

template <GLenum type, typename T>
class Buffer {
    static constexpr std::string name = 
        (type == GL_ARRAY_BUFFER ? "VertexBuffer" : "IndexBuffer"); 
    u32 m_ID = 0;

public:

    Buffer(const std::vector<T>& data) {
        glGenBuffers(1, &m_ID);

        if (data.empty())
            LOG_WARN("{} {} created with empty data", name, m_ID);
        glBindBuffer(type, m_ID);
        glBufferData(type, data.size() * sizeof(T), 
                     data.data(), GL_STATIC_DRAW);

        if(glGetError() != GL_NO_ERROR) {
            u32 id = m_ID;
            glDeleteBuffers(1, &m_ID);
            LOG_ERROR("{} {} creation: FAILED", name, m_ID);
        }

        glBindBuffer(type, 0);
        LOG_INFO("{} {} create: SUCCESS", name, m_ID); 
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
        LOG_INFO("{} {} destroy: SUCCESS", name, m_ID); 
    }

    inline void bind() const { glBindBuffer(type, m_ID); };

    inline void unbind() const { glBindBuffer(type, 0); };
};

using VertexBuffer = Buffer<GL_ARRAY_BUFFER, float>;
using IndexBuffer = Buffer<GL_ELEMENT_ARRAY_BUFFER, u32>;

}
