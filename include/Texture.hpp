#pragma once

#include <glad/glad.h>
#include <Types.hpp>
#include <filesystem>
#include <utils.hpp>

#include <stb_image.h>

namespace etugl {

namespace fs = std::filesystem;

template <GLenum T>
class Texture {
    u32 m_ID = 0;
    u32 m_Width;
    u32 m_Height;

public:
    Texture(const fs::path& path) {
        int channels, width, height;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* buffer = stbi_load(path.c_str(), &width, &height, &channels, 4);
        massert(buffer, "Failed to load image " + path.string());

        m_Height = height;
        m_Width = width;
        
        glGenTextures(1, &m_ID);
        glBindTexture(T, m_ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(T, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
        glBindTexture(T, 0);
        stbi_image_free(buffer);
        LOG_INFO("Texture {} creation: SUCCESS", m_ID);
    }

    ~Texture() {
        LOG_INFO("Texture {} delete: SUCCESS", m_ID);
        glDeleteTextures(1, &m_ID);
    }

    Texture(const Texture& other) = delete;
    Texture& operator=(const Texture& other) = delete;

    inline void bind(const u32 slot = 0) const { 
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(T, m_ID);
    }

    inline void unbind(const u32 slot) const { glBindTexture(T, 0); }

    inline void set_parameter(GLenum pname, GLfloat param) const {
        glTexParameteri(T, pname, param);
        LOG_INFO("In Texture {} set parameter: {} = {}", m_ID, pname, param);
    }

    [[nodiscard]] inline u32 width() const noexcept { return m_Width; }
    
    [[nodiscard]] inline u32 height() const noexcept { return m_Height; }
};

using Texture2D = Texture<GL_TEXTURE_2D>;

}
