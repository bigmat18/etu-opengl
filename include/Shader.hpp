#pragma once
#include <cstddef>
#include <ios>
#include <regex>
#include <fstream>
#include <string>
#include <filesystem>

#include <glad/glad.h>
#include <Types.hpp>
#include <core/logging.hpp>
#include <core/common.hpp>

namespace etugl {

namespace fs = std::filesystem; 

template <GLenum T>
class Shader {
    u32 m_ID = 0;
    fs::path m_Path;

public:
    Shader(const fs::path& path) : m_Path(path)
    {
        m_ID = glCreateShader(T);

        const std::string shader_code = read_file(path);
        const std::string post_process_code = pre_process(shader_code);
        const char* final_code = post_process_code.c_str();

        glShaderSource(m_ID, 1, &final_code, NULL);
        glCompileShader(m_ID);

        if(is_valid()) 
            core::logging::info("Shader create: SUCCESS", VAR(m_ID));
    }


    ~Shader() {
        u32 id = m_ID;
        glDeleteShader(m_ID);
        core::logging::info("Shader destroy: SUCCESS", VAR(id));
    }

    Shader(const Shader& other) = delete;
    Shader& operator=(const Shader& other) = delete;


    bool is_valid() const {
        if (!m_ID) {
            core::logging::warn("Shader invalid", VAR(m_ID));
            return false;
        }

        GLint compiled = 0;
        glGetShaderiv(m_ID, GL_COMPILE_STATUS, &compiled);
        const char* status_str = (compiled ? "SUCCESS" : "FAILED");
        core::logging::info("Compile status of shader", VAR(m_ID), VAR(status_str));

        GLint logLength = 0;
        glGetShaderiv(m_ID, GL_INFO_LOG_LENGTH, &logLength);

        if (logLength > 1) {
          std::string log;
          log.resize(static_cast<size_t>(logLength));
          GLsizei written = 0;
          glGetShaderInfoLog(m_ID, logLength, &written, log.data());
          if (written > 0)
                log.resize(static_cast<size_t>(written));

            if (compiled == GL_TRUE)
                core::logging::warn("Shader compiled but has warnings", VAR(m_ID), VAR(log));
            else
                core::logging::error("Shader failed to compile", VAR(m_ID), VAR(log));
        }

        return compiled == GL_TRUE;
    }

    [[nodiscard]] inline u32 get_ID() const noexcept { return m_ID; }

private:
    std::string read_file(const fs::path& path) {
        std::ifstream file(path, std::ios::binary);
        if(!file) {
            std::string str = path.string();
            core::logging::error("Shader: cannot open file", VAR(m_ID), VAR(str));
            return "";
        }
        std::string out;
        file.seekg(0, std::ios::end);
        const auto file_size = file.tellg();
        out.resize(static_cast<size_t>(file_size));
        file.seekg(0, std::ios::beg);

        if (!out.empty())
          file.read(&out[0], static_cast<std::streamsize>(out.size()));
        
        return out;
    }

    std::string pre_process(const std::string& code) {
        std::string result;
        result.reserve(code.size());

        const std::regex re(R"(#include\s*<([^>\r\n]+)>)");

        u32 prev = 0;
        for (std::sregex_iterator it(code.begin(), code.end(), re), end; it != end; ++it) {
            const std::smatch& m = *it;

            const u32 start = static_cast<u32>(m.position());
            const u32 len = static_cast<u32>(m.length());
            const std::string header = m[1].str();

            result.append(code, prev, start - prev);

            fs::path header_path = m_Path.parent_path()/header;
            const std::string include_code = read_file(header_path);
            const std::string post_process_code = pre_process(include_code);

            result.append(post_process_code);

            prev = start + len;
        }
        result.append(code, prev, std::string::npos);
        return result;
    }
};


using VertexShader      = Shader<GL_VERTEX_SHADER>;
using FragmentShader    = Shader<GL_FRAGMENT_SHADER>;

}
