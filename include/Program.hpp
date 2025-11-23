#pragma once

#include "Texture.hpp"
#include "Types.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <Shader.hpp>
#include <filesystem>

namespace etugl {

class Program {
    u32 m_ID = 0;

public:
    Program(const VertexShader& vert_shader, const FragmentShader& frag_shader);
    Program(const fs::path& vs_path, const fs::path& fs_path);

    ~Program();

    Program(const Program& other) = delete;
    Program& operator=(const Program& other) = delete;

    inline void bind() const { glUseProgram(m_ID); };

    inline void unbind() const { glUseProgram(0); };

    bool is_valid() const;


    inline void set_bool(const std::string& name, const bool value) const {
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
    }

    inline void set_int(const std::string& name, const int value) const {
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
    }

    inline void set_float(const std::string& name, const float value) const {
        glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
    }

    inline void set_vec2f(const std::string& name, const vec2f& vec) const {
        glUniform2f(glGetUniformLocation(m_ID, name.c_str()), vec.x, vec.y);
    }

    inline void set_vec3f(const std::string& name, const vec3f& vec) const {
        glUniform3f(glGetUniformLocation(m_ID, name.c_str()), vec.x, vec.y, vec.z);
    }

    inline void set_vec4f(const std::string& name, const vec4f& vec) const {
        glUniform4f(glGetUniformLocation(m_ID, name.c_str()), vec.x, vec.y, vec.z, vec.w);
    }
    
    inline void set_vec2i(const std::string& name, const vec2i& vec) const {
        glUniform2i(glGetUniformLocation(m_ID, name.c_str()), vec.x, vec.y);
    }

    inline void set_vec3i(const std::string& name, const vec3i& vec) const {
        glUniform3f(glGetUniformLocation(m_ID, name.c_str()), vec.x, vec.y, vec.z);
    }

    inline void set_vec4i(const std::string& name, const vec4i& vec) const {
        glUniform4i(glGetUniformLocation(m_ID, name.c_str()), vec.x, vec.y, vec.z, vec.w);
    }

    inline void set_vec2u(const std::string& name, const vec2u& vec) const {
        glUniform2ui(glGetUniformLocation(m_ID, name.c_str()), vec.x, vec.y);
    }

    inline void set_vec3u(const std::string& name, const vec3u& vec) const {
        glUniform3ui(glGetUniformLocation(m_ID, name.c_str()), vec.x, vec.y, vec.z);
    }

    inline void set_vec4u(const std::string& name, const vec4u& vec) const {
        glUniform4ui(glGetUniformLocation(m_ID, name.c_str()), vec.x, vec.y, vec.z, vec.w);
    }

    inline void set_mat2f(const std::string& name, const mat2f& mat) const {
        glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    inline void set_mat3f(const std::string& name, const mat3f& mat) const {
        glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    inline void set_mat4f(const std::string& name, const mat4f& mat) const {
        glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }
};

}
