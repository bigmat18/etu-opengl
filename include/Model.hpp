#pragma once

#include "Program.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "VertexLayout.hpp"
#include <cstddef>
#include <filesystem>
#include <optional>
#include <string>
#include <utility>
#include <vector>
#include <Types.hpp>
#include <utils.hpp>

#include <tiny_obj_loader.h>

namespace etugl {

namespace fs = std::filesystem;

struct Material {
    std::string name;

    vec3f ambient;
    vec3f diffuse;
    vec3f specular;
    float shiniess;

    std::optional<Texture2D> diffuse_tex = std::nullopt;

    inline void bind(const Program& program) const {
        program.set_vec3f("u_Ambient", ambient);
        program.set_vec3f("u_Diffuse", diffuse);
        program.set_vec3f("u_Specular", specular);
        program.set_float("u_Shiniess", shiniess);

        if (diffuse_tex) {
            program.set_int("u_Texture", 0);
            diffuse_tex->bind(0);
        }
    }
};


class Mesh {
    std::string m_Label;
    VertexArray m_VAO;
    size_t m_MaterialIdx;

public: 

    Mesh(const std::string& label, 
         const std::vector<float>& vertices, 
         const std::vector<u32>& indices,
         const size_t material_id = -1) : 
        m_Label(label), m_MaterialIdx(material_id)
    {
        m_VAO = VertexArray(
            vertices, indices,
            VertexLayout()
             .add<LayoutType::Float3>(0)
             .add<LayoutType::Float3>(1)
             .add<LayoutType::Float2>(2)
        );
    }

    Mesh(const Mesh& other) = delete;

    Mesh& operator=(const Mesh& other) = delete;

    Mesh(Mesh&& other) noexcept { swap(other); }

    Mesh& operator=(Mesh&& other) noexcept { swap(other); return *this; }

    void swap(Mesh& other) {
        using std::swap;
        swap(m_Label, other.m_Label);
        swap(m_VAO, other.m_VAO);
        swap(m_MaterialIdx, other.m_MaterialIdx);
    }

    friend void swap(Mesh& first, Mesh& second) { first.swap(second); }

    inline void bind() const noexcept { m_VAO.bind(); }

    inline void draw() const noexcept { m_VAO.draw(); }

    [[nodiscard]] inline size_t material_idx() const { return m_MaterialIdx; }
};

class Model {
    fs::path m_Path;
    std::vector<Mesh> m_Meshes;
    std::vector<Material> m_Materials;
    
public:

    Model(const fs::path& path);

    inline void draw(const Program& program) const {
        for (const auto& mesh : m_Meshes) {
            size_t mat_idx = mesh.material_idx();
            if (mat_idx != -1)
                m_Materials[mat_idx].bind(program);
            mesh.draw();
        }
    }

    inline void bind() const {
        for (const auto& mesh : m_Meshes)
            mesh.bind();
    }

private:
    void load_materials(const std::vector<tinyobj::material_t>& materials);

    void load_shapes(const std::vector<tinyobj::shape_t>& shapes,
                     const tinyobj::attrib_t& attrib);
};

}
