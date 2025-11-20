#pragma once

#include "logging.hpp"
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
        glGenBuffers(1, &m_ID);

        if (data.empty())
            LOG_WARN("Buffer {} created with empty data", m_ID);

        glBindBuffer(type, m_ID);
        glBufferData(type, data.size() * sizeof(T), 
                     data.data(), GL_STATIC_DRAW);

        if(glGetError() != GL_NO_ERROR) {
            u32 id = m_ID;
            glDeleteBuffers(1, &m_ID);
            LOG_ERROR("Buffer {} creation: FAILED");
        }

        glBindBuffer(type, 0);
        if constexpr (type == GL_ARRAY_BUFFER)
            LOG_INFO("VertexBuffer {} create: SUCCESS", m_ID); 
        else                                    
            LOG_INFO("IndiexBuffer {} create: SUCCESS", m_ID); 
    };

    Buffer(const Buffer& other) = delete;
    Buffer& operator=(const Buffer& other) = delete;

    ~Buffer() {
        u32 id = m_ID;
        glDeleteBuffers(1, &m_ID);
        if constexpr (type == GL_ARRAY_BUFFER) 
            LOG_INFO("VertexBuffer {} destroy: SUCCESS", m_ID); 
        else                                   
            LOG_INFO("IndiexBuffer {} destroy: SUCCESS", m_ID); 
    }

    inline void bind() const { glBindBuffer(type, m_ID); };

    inline void unbind() const { glBindBuffer(type, 0); };
};

using VertexBuffer = Buffer<GL_ARRAY_BUFFER, float>;
using IndexBuffer = Buffer<GL_ELEMENT_ARRAY_BUFFER, u32>;

}
