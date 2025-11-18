#pragma once

#include <Shader.hpp>
#include <filesystem>

namespace etugl {

class Program {
    u32 m_ID;

public:
    Program(const VertexShader& vert_shader, const FragmentShader& frag_shader);
    Program(const fs::path& vs_path, const fs::path& fs_path);

    ~Program();

    Program(const Program& other) = delete;
    Program& operator=(const Program& other) = delete;

    inline void bind() const { glUseProgram(m_ID); };

    inline void unbind() const { glUseProgram(0); };

    bool is_valid() const;
};

}
