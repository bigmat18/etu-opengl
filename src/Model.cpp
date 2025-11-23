#include "Texture.hpp"
#include <Model.hpp>
#include <filesystem>
#include <optional>

namespace etugl {

Model::Model(const fs::path& path) : m_Path(path)
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

    auto has_tex = [](bool has, fs::path p) {
        return has ? std::optional<Texture2D>(p) : std::nullopt;
    };

    m_Materials.reserve(materials.size());
    for (const auto& m : materials) {
        const bool has_diff = !m.diffuse_texname.empty();

        m_Materials.push_back({
            .name = m.name,
            .ambient = {m.ambient[0], m.ambient[1], m.ambient[2]},
            .diffuse = {m.diffuse[0], m.diffuse[1], m.diffuse[2]},
            .specular = {m.specular[0], m.specular[1], m.specular[2]},
            .shiniess = m.shininess,
            .diffuse_tex = has_tex(has_diff, ppath/m.diffuse_texname) 
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
        const size_t num_indices = shapes[s].mesh.indices.size();
        const std::string label = shapes[s].name;

        int material_idx = -1;
        if (!shapes[s].mesh.material_ids.empty())
            material_idx = shapes[s].mesh.material_ids[0];

        std::vector<float> vertices;
        vertices.resize(num_indices * 8, 0.0f);

        std::vector<u32> indices;
        indices.reserve(num_indices);

        size_t vi = 0;
        size_t ii = 0;

        for (const auto& idx : shapes[s].mesh.indices) {

            if (idx.vertex_index >= 0) {
                size_t vidx = 3 * static_cast<size_t>(idx.vertex_index);
                vertices[vi++] = attrib.vertices[vidx + 0];
                vertices[vi++] = attrib.vertices[vidx + 1];
                vertices[vi++] = attrib.vertices[vidx + 2];
            } else vi+=3;

            if (idx.normal_index >= 0) {
                size_t nidx = 3 * static_cast<size_t>(idx.normal_index);
                vertices[vi++] = attrib.normals[nidx + 0];
                vertices[vi++] = attrib.normals[nidx + 1];
                vertices[vi++] = attrib.normals[nidx + 2];
            } else vi+=3;

            if (idx.texcoord_index >= 0) {
                size_t tidx = 2 * static_cast<size_t>(idx.texcoord_index);
                vertices[vi++] = attrib.texcoords[tidx + 0];
                vertices[vi++] = attrib.texcoords[tidx + 1];
            } else vi+=2;

            indices.push_back(static_cast<u32>(ii++));
        }

        u32 num_vertices = vertices.size() / 8;
        m_Meshes.emplace_back(label, vertices, indices, material_idx);
        LOG_INFO("Mesh {} Vertices loaded: {}", filename, num_vertices);
        LOG_INFO("Mesh {} Indices loaded: {}", filename, num_indices);
        LOG_INFO("Mesh {} material idx: {}", filename, material_idx);
        LOG_INFO("Mesh {} in {} loading: SUCCESS", label, filename);
    }
}

}
