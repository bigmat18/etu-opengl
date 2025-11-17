#pragma once

#include <vector>
#include <glad/glad.h>

#include <VertexLayout.hpp>
#include <Types.hpp>
#include <utils.hpp>

namespace etugl {

template <GLenum type, typename T>
class Buffer {
    u32 m_ID;

public:
    Buffer(const std::vector<T>& vertices) {

        massert(!vertices.empty(), "VertexBuffer can not be empty");
        glGenBuffers(1, &m_ID);
        glBindBuffer(type, m_ID);
        glBufferData(type, vertices.size() * sizeof(T), 
                     vertices.data(), GL_STATIC_DRAW);

        if(glGetError() != GL_NO_ERROR) {
            glDeleteBuffers(1, &m_ID);
            massert(false, "OpenGL error during buffer creation");
        }

        glBindBuffer(type, 0);
        LOG_INFO("VertexBuffer with ID {} created", m_ID);
    };

    Buffer(const Buffer& other) = delete;
    Buffer& operator=(const Buffer& other) = delete;

    ~Buffer();

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
