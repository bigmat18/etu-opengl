#pragma once

#include "VertexArray.hpp"
#include "VertexLayout.hpp"
#include "logging.hpp"
#include <cstddef>
#include <filesystem>
#include <string>
#include <utility>
#include <vector>
#include <Types.hpp>
#include <utils.hpp>

#include <tiny_obj_loader.h>

namespace etugl {

namespace fs = std::filesystem;

class Mesh {
    std::string m_Label;
    VerterArray m_VAO;

public: 

    Mesh(const std::string label, 
         const std::vector<float>& vertices, 
         const std::vector<u32>& indices) : m_Label(label) 
    {
        m_VAO = VerterArray(
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

    Mesh& operator=(Mesh&& other) noexcept { swap(other);return *this; }

    void swap(Mesh& other) {
        using std::swap;
        swap(m_Label, other.m_Label);
        swap(m_VAO, other.m_VAO);
    }

    friend void swap(Mesh& first, Mesh& second) { first.swap(second); }

    inline void draw() const noexcept { m_VAO.draw(); }
};

class Model {
    std::vector<Mesh> m_Meshes;
public:

    Model(const fs::path path) {
        const std::string path_str = path.string();
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

        m_Meshes.reserve(shapes.size());

        u32 vertex_offset = 0;
        for (size_t s = 0; s < shapes.size(); s++) {

            const size_t num_indices = shapes[s].mesh.indices.size();
            const std::string label = shapes[s].name;

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
                    size_t tidx = 3 * static_cast<size_t>(idx.texcoord_index);
                    vertices[vi++] = attrib.texcoords[tidx + 0];
                    vertices[vi++] = attrib.texcoords[tidx + 1];
                } else vi+=2;

                indices.push_back(vertex_offset + static_cast<u32>(ii++));
            }

            vertex_offset += static_cast<u32>(num_indices);
            m_Meshes.emplace_back(label, vertices, indices);
            LOG_INFO("Mesh {} in {} loading: SUCCESS", vertex_offset, path_str);
            vertex_offset++;
        }
    }

    inline void draw() const noexcept {
        for (const auto& m : m_Meshes) 
            m.draw();
    }
};

}
