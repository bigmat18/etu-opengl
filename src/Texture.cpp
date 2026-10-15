#include <Texture.hpp>
#include <filesystem>
#include <core/logging.hpp>
#ifdef assert
#undef assert
#endif
#include <core/massert.hpp>
#include <core/common.hpp>

namespace etugl {
   
template <GLenum T>
Texture<T>::Texture(const fs::path& path, const Params params, const bool flip) : 
    m_IsFillped(flip)
{
    int channels, width, height;
    stbi_set_flip_vertically_on_load(flip);
    stbi_uc* buffer = stbi_load(path.c_str(), &width, &height, &channels, 0);
    std::string fail_msg = "Failed to load image " + path.string();
    massert(buffer, fail_msg);

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

    for (const auto& [pname, pval] : params)
        glTexParameteri(T, pname, pval);

    glTexImage2D(T, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, buffer);
    glBindTexture(T, 0);
    stbi_image_free(buffer);
    stbi_set_flip_vertically_on_load(false);
    core::logging::info("Texture create: SUCCESS", VAR(m_ID));
}

template <GLenum T>
Texture<T>::~Texture() {
    core::logging::info("Texture destroy: SUCCESS", VAR(m_ID));
    glDeleteTextures(1, &m_ID);
}

template class Texture<GL_TEXTURE_2D>;

}
