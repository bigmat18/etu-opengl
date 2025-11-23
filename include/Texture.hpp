#pragma once

#include <glad/glad.h>
#include <Types.hpp>
#include <filesystem>
#include <utility>
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
        stbi_set_flip_vertically_on_load(true);
        stbi_uc* buffer = stbi_load(path.c_str(), &width, &height, &channels, 0);
        massert(buffer, "Failed to load image " + path.string());

        if (!buffer)
            return;

        m_Height = height;
        m_Width = width;
 
        GLenum format;
        if (channels == 1)       format = GL_RED;
        else if (channels == 3)  format = GL_RGB;
        else if (channels == 4)  format = GL_RGBA;
        
        glGenTextures(1, &m_ID);
        glBindTexture(T, m_ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(T, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, buffer);
        glBindTexture(T, 0);
        stbi_image_free(buffer);
        LOG_INFO("Texture {} create: SUCCESS", m_ID);
    }

    ~Texture() {
        u32 id = m_ID;
        glDeleteTextures(1, &m_ID);
        LOG_INFO("Texture {} destroy: SUCCESS", id);
    }

    Texture(Texture&& other) noexcept { swap(other); }

    Texture& operator= (Texture&& other) noexcept {
        swap(other);
        return *this;
    }

    Texture(const Texture& other) = delete;

    Texture& operator=(const Texture& other) = delete;

    void swap(Texture& other) {
        using std::swap;
        swap(m_ID, other.m_ID);
        swap(m_Width, other.m_Width);
        swap(m_Height, other.m_Height);
    }

    friend void swap(Texture& first, Texture& second) { first.swap(second); }

    inline void bind(const u32 slot = 0) const { 
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(T, m_ID);
    }

    inline void unbind(const u32 slot) const { glBindTexture(T, 0); }

    inline void set_parameter(GLenum pname, GLfloat param) const {
        glTexParameteri(T, pname, param);
        LOG_INFO("Texture {} set parameter: {} = {}", m_ID, pname, param);
    }

    [[nodiscard]] inline u32 width() const noexcept { return m_Width; }
    
    [[nodiscard]] inline u32 height() const noexcept { return m_Height; }
};

using Texture2D = Texture<GL_TEXTURE_2D>;

}
