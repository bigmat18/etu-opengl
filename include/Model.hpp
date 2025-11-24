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
    static constexpr std::string_view AMBIENT       = "u_Ambient";
    static constexpr std::string_view DIFFUSE       = "u_Diffuse";
    static constexpr std::string_view SPECULAR      = "u_Specular";
    static constexpr std::string_view TRANSMITTANCE = "u_Transmittance";
    static constexpr std::string_view EMISSION      = "u_Emission";

    static constexpr std::string_view IOR           = "u_Ior";
    static constexpr std::string_view SHININESS     = "u_Shininess";
    static constexpr std::string_view DISSOLVE      = "u_Dissolve";
    static constexpr std::string_view ILLUM         = "u_Illum";

    static constexpr std::string_view DIFF_TEX      = "u_TexDiff";
    static constexpr std::string_view SPEC_TEX      = "u_TexSpec";
    static constexpr std::string_view NORM_TEX      = "u_TexNorm";
    static constexpr std::string_view AMBIENT_TEX   = "u_TexAmbient";
    static constexpr std::string_view ROUGHNESS_TEX = "u_TexRoughness";
    static constexpr std::string_view EMISSIVE_TEX  = "u_TexEmissive";

    struct Slots {
      static constexpr int Diffuse      = 0;
      static constexpr int Specular     = 1;
      static constexpr int Normal       = 2;
      static constexpr int Ambient      = 3;
      static constexpr int Roughness    = 4;
      static constexpr int Emissive     = 5;
    };

    std::string name = "unnamed_material";

    vec3f ambient{0.0f};
    vec3f diffuse{0.0f};
    vec3f specular{0.0f};
    vec3f transmittance{0.0f};
    vec3f emission{0.0f}; 

    float ior = 1.0f;       
    float shininess = 0.0f; 
    float dissolve = 1.0f; 
    int   illum = 2; 

    std::optional<Texture2D> diffuse_tex;
    std::optional<Texture2D> normal_tex;
    std::optional<Texture2D> specular_tex;
    std::optional<Texture2D> ambient_tex;
    std::optional<Texture2D> roughness_tex;
    std::optional<Texture2D> emissive_tex;

    inline void bind(const Program& program) const {
        program.set_vec3f(std::string(AMBIENT), ambient);
        program.set_vec3f(std::string(DIFFUSE), diffuse);
        program.set_vec3f(std::string(SPECULAR), specular);
        program.set_vec3f(std::string(TRANSMITTANCE), transmittance);
        program.set_vec3f(std::string(EMISSION), emission);

        program.set_float(std::string(IOR), ior);
        program.set_float(std::string(SHININESS), shininess);
        program.set_float(std::string(DISSOLVE), dissolve);
        program.set_float(std::string(ILLUM), illum);

        if (diffuse_tex) {
          program.set_int(std::string(DIFF_TEX), Slots::Diffuse);
          diffuse_tex->bind(Slots::Diffuse);
        }

        if (specular_tex) {
          program.set_int(std::string(SPEC_TEX), Slots::Specular);
          specular_tex->bind(Slots::Specular);
        }

        if (normal_tex) {
          program.set_int(std::string(NORM_TEX), Slots::Normal);
          normal_tex->bind(Slots::Normal);
        }

        if (ambient_tex) {
          program.set_int(std::string(AMBIENT_TEX), Slots::Ambient);
          ambient_tex->bind(Slots::Ambient);
        }

        if (roughness_tex) {
          program.set_int(std::string(ROUGHNESS_TEX), Slots::Roughness);
          roughness_tex->bind(Slots::Roughness);
        }

        if (emissive_tex) {
          program.set_int(std::string(EMISSIVE_TEX), Slots::Emissive);
          emissive_tex->bind(Slots::Emissive);
        }
    }
};


class Mesh {
    std::string m_Label = "unnamed_mesh";
    size_t m_MaterialIdx = -1;
    VertexArray m_VAO;

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
    bool m_HasMapsFilped;
    std::vector<Mesh> m_Meshes;
    std::vector<Material> m_Materials;
    
public:

    Model(const fs::path& path, const bool flip_maps = true);

    inline void draw(const Program& program) const {
        for (const auto& mesh : m_Meshes) {
            size_t mat_idx = mesh.material_idx();
            if (mat_idx != -1) m_Materials[mat_idx].bind(program);
            mesh.draw();
        }
    }

    inline void bind() const {
        for (const auto& mesh : m_Meshes) mesh.bind();
    }

private:
    void load_materials(const std::vector<tinyobj::material_t>& materials);

    void load_shapes(const std::vector<tinyobj::shape_t>& shapes,
                     const tinyobj::attrib_t& attrib);
};

}
