#include "Shader.hpp"
#include "logging.hpp"
#include <Program.hpp>
#include <glad/glad.h>
#include <string>

namespace etugl {

Program::Program(const VertexShader& vert_shader, const FragmentShader& frag_shader) {
    m_ID = glCreateProgram();
    glAttachShader(m_ID, vert_shader.get_ID());
    glAttachShader(m_ID, frag_shader.get_ID());
    glLinkProgram(m_ID);

    if (is_valid()) 
        LOG_INFO("Program {} created", m_ID);
}

Program::Program(const fs::path& vs_path, const fs::path& fs_path) {    
    const VertexShader vert(vs_path);
    const FragmentShader frag(fs_path);
    Program(vert, frag);
}

Program::~Program() {
    LOG_INFO("Program {} deleted", m_ID);
    glDeleteProgram(m_ID);
}
    
bool Program::is_valid() const {
    if (!m_ID) {
        LOG_WARN("Program ID {} invalid", m_ID);
        return false;
    }

    glValidateProgram(m_ID);
    GLint validate_status = 0;
    glGetProgramiv(m_ID, GL_VALIDATE_STATUS, &validate_status);
    LOG_INFO("Validation status of program " + std::to_string(m_ID) + ": " + 
             (validate_status ? "SUCCESS" : "FAILED"));

    GLint link_status;
    glGetProgramiv(m_ID, GL_LINK_STATUS, &link_status);
    LOG_INFO("Link status of program " + std::to_string(m_ID) + ": " + 
             (link_status ? "SUCCESS" : "FAILED"));

    if (link_status != GL_TRUE || validate_status != GL_TRUE) {
        GLint log_length = 0;
        glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &log_length);
        if (log_length > 1) {
            std::string log(static_cast<size_t>(log_length), '\0');
            GLsizei written = 0;
            glGetProgramInfoLog(m_ID, log_length, &written, log.data());
            log.resize(static_cast<size_t>(written));
            LOG_ERROR("Program {} failed: {}", m_ID, log);
        }
        return false;
    }

    GLint active_attribs = 0;
    glGetProgramiv(m_ID, GL_ACTIVE_ATTRIBUTES, &active_attribs);
    LOG_INFO("Active attributes in program {}: {}", m_ID, active_attribs);

    GLint active_uniforms = 0;
    glGetProgramiv(m_ID, GL_ACTIVE_UNIFORMS, &active_uniforms);
    LOG_INFO("Active uniforms in program {}: {}", m_ID, active_uniforms);

    GLint uniform_max_len = 0;
    glGetProgramiv(m_ID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniform_max_len);
    LOG_INFO("Max uniform name length in program {}: {}", m_ID, uniform_max_len);

    GLint attached = 0;
    glGetProgramiv(m_ID, GL_ATTACHED_SHADERS, &attached);
    LOG_INFO("Attached shaders in program {}: {}", m_ID, attached);

    return true;
}

}
