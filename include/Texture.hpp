#pragma once

#include <glad/glad.h>
#include <Types.hpp>
#include <filesystem>
#include <utility>
#include <utils.hpp>

#include <stb_image.h>
#include <vector>

namespace etugl {

namespace fs = std::filesystem;

template <GLenum T>
class Texture {
    u32 m_ID = 0;
    u32 m_Width = 0;
    u32 m_Height = 0;
    bool m_IsFillped = false;

public:
    using Params = std::vector<std::pair<GLenum, GLenum>>;

    Texture(const fs::path& path, 
            const Params params, 
            const bool flip = true);

    ~Texture();

    Texture(Texture&& other) noexcept { swap(other); }

    Texture& operator=(Texture&& other) noexcept {
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

    inline void unbind() const { glBindTexture(T, 0); }

    [[nodiscard("Close the parameter adding")]] 
    inline Texture<T>& begin() { 
        glBindTexture(T, m_ID); 
        return *this;
    }

    [[nodiscard("Close the parameter adding")]] 
    inline Texture<T>& add(GLenum pname, GLfloat param) {
        glTexParameteri(T, pname, param);
        LOG_INFO("Texture {} set parameter: {} = {}", m_ID, pname, param);
        return *this;
    }

    inline Texture<T>& end() { unbind(); return *this; }

    [[nodiscard]] inline u32 width() const noexcept { return m_Width; }
    
    [[nodiscard]] inline u32 height() const noexcept { return m_Height; }

    [[nodiscard]] inline bool is_fliped() const noexcept { return m_IsFillped; }
};

using Texture2D = Texture<GL_TEXTURE_2D>;

}
