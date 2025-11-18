#pragma once

#include "debug.hpp"
#include <vector>
#include <glad/glad.h>

#include <VertexLayout.hpp>
#include <Types.hpp>
#include <utils.hpp>

namespace etugl {

template <GLenum type, typename T>
class Buffer {
    u32 m_ID = 0;

public:
    Buffer(const std::vector<T>& data) {
        massert(!data.empty(), "VertexBuffer can not be empty");
        glGenBuffers(1, &m_ID);
        glBindBuffer(type, m_ID);
        glBufferData(type, data.size() * sizeof(T), 
                     data.data(), GL_STATIC_DRAW);

        if(glGetError() != GL_NO_ERROR) {
            glDeleteBuffers(1, &m_ID);
            massert(false, "OpenGL error during buffer creation");
        }

        glBindBuffer(type, 0);
        if constexpr (type == GL_ARRAY_BUFFER) {
            LOG_INFO("VertexBuffer with ID {} created", m_ID); 
        } else {
            LOG_INFO("IndiexBuffer with ID {} created", m_ID); 
        }
    };

    Buffer(const Buffer& other) = delete;
    Buffer& operator=(const Buffer& other) = delete;

    ~Buffer() {
        if constexpr (type == GL_ARRAY_BUFFER) {
            LOG_INFO("VertexBuffer with ID {} deleted", m_ID); 
        } else {
            LOG_INFO("IndiexBuffer with ID {} deleted", m_ID); 
        }
        glDeleteBuffers(1, &m_ID);
    }

    inline void bind() const {
        glBindBuffer(type, m_ID);
    };

    inline void unbind() const {
        glBindBuffer(type, 0);
    };
};

using VertexBuffer = Buffer<GL_ARRAY_BUFFER, float>;
using IndexBuffer = Buffer<GL_ELEMENT_ARRAY_BUFFER, u32>;

}
