#include "Shader.hpp"
#include <core/logging.hpp>
#include <core/common.hpp>
#include <Program.hpp>
#include <glad/glad.h>
#include <string>

namespace etugl {

Program::Program(const VertexShader& vert_shader, const FragmentShader& frag_shader) {
    m_ID = glCreateProgram();
    glAttachShader(m_ID, vert_shader.get_ID());
    glAttachShader(m_ID, frag_shader.get_ID());
    glLinkProgram(m_ID);

    if (is_valid()) core::logging::info("Program create: SUCCESS", VAR(m_ID));
}

Program::Program(const fs::path& vs_path, const fs::path& fs_path) {    
    const VertexShader vert(vs_path);
    const FragmentShader frag(fs_path);
    
    m_ID = glCreateProgram();
    glAttachShader(m_ID, vert.get_ID());
    glAttachShader(m_ID, frag.get_ID());
    glLinkProgram(m_ID);

    if (is_valid()) core::logging::info("Program create: SUCCESS", VAR(m_ID));
}

Program::~Program() {
    core::logging::info("Program deleted", VAR(m_ID));
    glDeleteProgram(m_ID);
}
    
bool Program::is_valid() const {
    if (!m_ID) {
        core::logging::warn("Program ID invalid", VAR(m_ID));
        return false;
    }

    glValidateProgram(m_ID);
    GLint validate_status = 0;
    glGetProgramiv(m_ID, GL_VALIDATE_STATUS, &validate_status);
    const char* val_status = (validate_status ? "SUCCESS" : "FAILED");
    core::logging::info("Validation status of program", VAR(m_ID), VAR(val_status));

    GLint link_status;
    glGetProgramiv(m_ID, GL_LINK_STATUS, &link_status);
    const char* lnk_status = (link_status ? "SUCCESS" : "FAILED");
    core::logging::info("Link status of program", VAR(m_ID), VAR(lnk_status));

    if (link_status != GL_TRUE || validate_status != GL_TRUE) {
        GLint log_length = 0;
        glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &log_length);
        if (log_length > 1) {
            std::string log(static_cast<size_t>(log_length), '\0');
            GLsizei written = 0;
            glGetProgramInfoLog(m_ID, log_length, &written, log.data());
            log.resize(static_cast<size_t>(written));
            core::logging::error("Program failed", VAR(m_ID), VAR(log));
        }
        return false;
    }

    GLint active_attribs = 0;
    glGetProgramiv(m_ID, GL_ACTIVE_ATTRIBUTES, &active_attribs);
    core::logging::info("Active attributes in program", VAR(m_ID), VAR(active_attribs));

    GLint active_uniforms = 0;
    glGetProgramiv(m_ID, GL_ACTIVE_UNIFORMS, &active_uniforms);
    core::logging::info("Active uniforms in program", VAR(m_ID), VAR(active_uniforms));

    GLint uniform_max_len = 0;
    glGetProgramiv(m_ID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniform_max_len);
    core::logging::info("Max uniform name length in program", VAR(m_ID), VAR(uniform_max_len));

    GLint attached = 0;
    glGetProgramiv(m_ID, GL_ATTACHED_SHADERS, &attached);
    core::logging::info("Attached shaders in program", VAR(m_ID), VAR(attached));

    return true;
}

}
