#include "Texture.hpp"
#include "debug.hpp"
#include "logging.hpp"
#include <Model.hpp>
#include <cstddef>
#include <filesystem>
#include <optional>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

namespace etugl {

Model::Model(const fs::path& path, const bool flip_map) : 
    m_Path(path), m_HasMapsFilped(flip_map)
{
    const std::string path_str = m_Path.string();
    tinyobj::ObjReaderConfig config;
    config.triangulate = true; 

    tinyobj::ObjReader reader;

    bool result = reader.ParseFromFile(path.string(), config);
    if (!reader.Error().empty())
        LOG_ERROR("Error in parse {}: {}", path_str, reader.Error());

    if (!result) {
        LOG_ERROR("Error in mesh loading {}", path_str);
        return;
    }

    if (!reader.Warning().empty()) 
        LOG_WARN("Warn in parse {}: {}", path_str, reader.Warning());

    if (!reader.Valid()) {
        LOG_ERROR("Mesh {} not valid", path_str);
        return;
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    load_materials(materials);
    load_shapes(shapes, attrib);
}


void Model::load_materials(const std::vector<tinyobj::material_t>& materials) {
    const fs::path ppath = m_Path.parent_path();
    const Texture2D::Params tex_params{
        {GL_TEXTURE_WRAP_S, GL_REPEAT}, 
        {GL_TEXTURE_WRAP_T, GL_REPEAT},
        {GL_TEXTURE_MIN_FILTER, GL_LINEAR},
        {GL_TEXTURE_MAG_FILTER, GL_LINEAR}
    };

    auto has_tex = [&](const std::string& texname) {
        return texname.empty()
        ? std::optional<Texture2D>{std::nullopt}
        : std::optional<Texture2D>{Texture2D(ppath / texname, 
                                             tex_params, m_HasMapsFilped)};
    };

    m_Materials.reserve(materials.size());

    for (const auto& m : materials) {
        m_Materials.push_back({
            .name = m.name,

            .ambient = {m.ambient[0], m.ambient[1], m.ambient[2]},
            .diffuse = {m.diffuse[0], m.diffuse[1], m.diffuse[2]},
            .specular = {m.specular[0], m.specular[1], m.specular[2]},
            .transmittance = {m.transmittance[0], m.transmittance[1], m.transmittance[2]},
            .emission = {m.emission[0], m.emission[1], m.emission[2]},

            .ior = (m.ior != 0.0f ? m.ior : 1.0f),
            .shininess = m.shininess,
            .dissolve = (m.dissolve != 0.0f ? m.dissolve : 1.0f),
            .illum = m.illum,

            .diffuse_tex   = has_tex(m.diffuse_texname), 
            .normal_tex    = has_tex(!m.normal_texname.empty()
                                     ? m.normal_texname
                                     : m.bump_texname),    
            .specular_tex  = has_tex(m.specular_texname),   // map_Ks
            .ambient_tex   = has_tex(m.ambient_texname),    // map_Ka
            .roughness_tex = has_tex(m.roughness_texname),  // PBR roughness
            .emissive_tex  = has_tex(m.emissive_texname)    // map_Ke
        });

        LOG_INFO("Loaded material {}", m.name);
    }
}



void Model::load_shapes(const std::vector<tinyobj::shape_t>& shapes,
                        const tinyobj::attrib_t& attrib) 
{
    const std::string filename = m_Path.filename().string();
    m_Meshes.reserve(shapes.size());

    for (size_t s = 0; s < shapes.size(); s++) {
        const auto& shape = shapes[s];
        const auto& idxs = shape.mesh.indices;

        const size_t num_indices = idxs.size();
        const std::string label = shape.name;

        int material_idx = -1;
        if (!shapes[s].mesh.material_ids.empty())
            material_idx = shapes[s].mesh.material_ids[0];

        std::vector<float> vertices;
        vertices.reserve(num_indices * 12);

        std::vector<u32> indices;
        indices.reserve(num_indices);

        for (size_t i = 0; i + 2 < num_indices; i+=3) {
            const auto& i0 = idxs[i + 0];
            const auto& i1 = idxs[i + 1];
            const auto& i2 = idxs[i + 2];

            vec3f P0 = fetch_position(attrib, i0.vertex_index);
            vec3f P1 = fetch_position(attrib, i1.vertex_index);
            vec3f P2 = fetch_position(attrib, i2.vertex_index);

            vec3f N0 = fetch_normal(attrib, i0.normal_index);
            vec3f N1 = fetch_normal(attrib, i1.normal_index);
            vec3f N2 = fetch_normal(attrib, i2.normal_index);

            vec2f UV0 = fetch_uv(attrib, i0.texcoord_index);
            vec2f UV1 = fetch_uv(attrib, i1.texcoord_index);
            vec2f UV2 = fetch_uv(attrib, i2.texcoord_index);

            vec3f Ttri, Btri;
            compute_TB(P0, P1, P2, UV0, UV1, UV2, Ttri, Btri);

            auto add_vertex = [&](const vec3f& P, const vec3f& N, 
                                  const vec2f& UV, size_t base_offset) 
            {
                size_t offset = vertices.size();

                // Normal fallback [when we have normal to 0]
                glm::vec3 Nf(N.x, N.y, N.z);
                if (glm::length2(Nf) < 1e-12f) {
                    glm::vec3 E1(P1.x - P0.x, P1.y - P0.y, P1.z - P0.z);
                    glm::vec3 E2(P2.x - P0.x, P2.y - P0.y, P2.z - P0.z);
                    Nf = glm::normalize(glm::cross(E1, E2));
                    if (!std::isfinite(Nf.x)) 
                        Nf = glm::vec3(0, 0, 1);
                } else {
                    Nf = glm::normalize(Nf);
                }

                vec3f T = Ttri;
                vec3f B = Btri;

                // Tangent fallback [when we have tanget to 0 or NaN]
                if (glm::length2(T) < 1e-20f || !std::isfinite(T.x)) {
                    glm::vec3 a = (std::fabs(Nf.x) > 0.9f) ? glm::vec3(0,1,0) : glm::vec3(1,0,0);
                    T = glm::normalize(glm::cross(a, Nf));
                    B = glm::normalize(glm::cross(Nf, T));
                }

                T = glm::normalize(T - Nf * glm::dot(Nf, T));
                vec3f Brec = glm::normalize(glm::cross(Nf, T));

                float sign = 1.0f;
                // Bitangent fallback [when we have bitangent to 0 or NaN]
                if (glm::length2(B) > 1e-20f && std::isfinite(B.x)) {
                    sign = (glm::dot(Brec, glm::normalize(B)) < 0.0f) ? -1.0f : 1.0f;
                }

                vertices.push_back(P.x);
                vertices.push_back(P.y);
                vertices.push_back(P.z);

                vertices.push_back(Nf.x);
                vertices.push_back(Nf.y);
                vertices.push_back(Nf.z);

                vertices.push_back(T.x);
                vertices.push_back(T.y);
                vertices.push_back(T.z);
                vertices.push_back(sign);

                vertices.push_back(UV.x);
                vertices.push_back(UV.y);
            };

            uint32_t baseIndex = static_cast<uint32_t>(vertices.size() / 12);
            add_vertex(P0, N0, UV0, baseIndex);
            add_vertex(P1, N1, UV1, baseIndex);
            add_vertex(P2, N2, UV2, baseIndex);

            indices.push_back(baseIndex + 0);
            indices.push_back(baseIndex + 1);
            indices.push_back(baseIndex + 2);
        }

        u32 num_vertices = vertices.size() / 12;
        u32 num_faces = num_indices / 3;
        m_Meshes.emplace_back(label, vertices, indices, material_idx);
        LOG_INFO("Mesh {} Vertices loaded: {}", filename, num_vertices);
        LOG_INFO("Mesh {} Faces loaded: {}", filename, num_faces);
        LOG_INFO("Mesh {} material idx: {}", filename, material_idx);
        LOG_INFO("Mesh {} in {} loading: SUCCESS", label, filename);
    }
}

}
