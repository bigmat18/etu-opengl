#pragma once

#include <vector>
#include <filesystem>
#include <Types.hpp>
#include <nlohmann/json.hpp>

namespace nh = nlohmann;

struct Primitive {
    etugl::mat4f matrix;
    etugl::vec4f color;
    int type;
    int id_node;

    static etugl::mat4f json_to_mat4(const nh::json& json_matrix);

    static etugl::vec4f json_to_vec4(const nh::json& json_vector);
};

// Importer for CSG models formatted as JSON file
bool format_json(const std::filesystem::path& file_path,
                 std::vector<Primitive>& primitives,
                 std::vector<int>& nodes,
                 std::vector<int>& parents,
                 std::vector<std::vector<int>>& children);
