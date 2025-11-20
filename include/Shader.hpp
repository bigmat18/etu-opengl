#pragma once
#include <cstddef>
#include <ios>
#include <regex>
#include <fstream>
#include <string>
#include <filesystem>

#include <glad/glad.h>
#include <Types.hpp>
#include <utils.hpp>

namespace etugl {

namespace fs = std::filesystem; 

template <GLenum T>
class Shader {
    u32 m_ID = 0;

public:
    Shader(const fs::path& path) {
        m_ID = glCreateShader(T);

        const std::string shader_code = read_file(path);
        const std::string post_process_code = pre_process(shader_code);
        const char* final_code = post_process_code.c_str();

        glShaderSource(m_ID, 1, &final_code, NULL);
        glCompileShader(m_ID);

        if(is_valid()) LOG_INFO("Shader {} create: SUCCESS", m_ID);
    }


    ~Shader() {
        u32 id = m_ID;
        glDeleteShader(m_ID);
        LOG_INFO("Shader {} destroy: SUCCESS", id);
    }

    Shader(const Shader& other) = delete;
    Shader& operator=(const Shader& other) = delete;


    bool is_valid() const {
        if (!m_ID) {
            LOG_WARN("Shader {} invalid", m_ID);
            return false;
        }

        GLint compiled = 0;
        glGetShaderiv(m_ID, GL_COMPILE_STATUS, &compiled);
        LOG_INFO("Compile status of shader " + std::to_string(m_ID) + ": " + 
                 (compiled ? "SUCCESS" : "FAILED"));

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
                LOG_WARN("Shader {} compiled but has warnings: {}", m_ID, log);
            else
                LOG_ERROR("Shader {} failed to compile: {}", m_ID, log);
        }

        return compiled == GL_TRUE;
    }

    inline u32 get_ID() const noexcept { return m_ID; }
private:
    std::string read_file(const fs::path& path) {
        std::ifstream file(path, std::ios::binary);
        if(!file) {
            std::string str = path.string();
            LOG_ERROR("Shader {}: cannot open file {}", m_ID, str);
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

            const std::string include_code = read_file(header);
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
