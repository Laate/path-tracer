#include <array>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#define CGLTF_IMPLEMENTATION
#include "../libs/cgltf.h"

#include "material.hpp"
#include "parse-gltf.hpp"
#include "scene.hpp"
#include "triangle.hpp"

namespace
{
    // Used if no other material is defined.
    constexpr Material default_material = {0.5F, 0.5F, Vec3(1.0F, 0.5F, 0.25F), Vec3(0.0F, 0.0F, 0.0F)};

    struct primitive_data
    {
        std::vector<Vec3> points;
        std::vector<size_t> indices;
        std::optional<int> material_index;
    };

    // Apply translation, rotation and scaling to a vector.
    auto transform(const std::array<float, 16> &mat4, const Vec3 &p) -> Vec3
    {
        return {mat4[0] * p.x + mat4[4] * p.y + mat4[8] * p.z + mat4[12],
                mat4[1] * p.x + mat4[5] * p.y + mat4[9] * p.z + mat4[13],
                mat4[2] * p.x + mat4[6] * p.y + mat4[10] * p.z + mat4[14]};
    }

    auto transform(const std::array<float, 16> &mat4, std::vector<Vec3> &points) -> void
    {
        for (auto &point : points)
            point = transform(mat4, point);
    }

    // Apply only the rotation part of the transformation matrix.
    auto rotate(const std::array<float, 16> &mat4, const Vec3 &p) -> Vec3
    {
        return {mat4[0] * p.x + mat4[4] * p.y + mat4[8] * p.z,
                mat4[1] * p.x + mat4[5] * p.y + mat4[9] * p.z,
                mat4[2] * p.x + mat4[6] * p.y + mat4[10] * p.z};
    }
} // namespace

// See the glTF documentation for explanation of the format.
// https://github.com/KhronosGroup/glTF/blob/main/specification/2.0/figures/gltfOverview-2.0.0b.png
auto gltf::load_gltf(const std::string &filename, Scene &scene) -> bool
{
    auto start_time = std::chrono::system_clock::now();
    auto res_to_str = [](cgltf_result res) -> std::string
    {
        switch (res)
        {
        case cgltf_result_success:
            return "success";
        case cgltf_result_data_too_short:
            return "data too short";
        case cgltf_result_unknown_format:
            return "unknown format";
        case cgltf_result_invalid_json:
            return "invalid JSON";
        case cgltf_result_invalid_gltf:
            return "invalid glTF";
        case cgltf_result_file_not_found:
            return "file not found";
        case cgltf_result_io_error:
            return "IO error";
        case cgltf_result_out_of_memory:
            return "out of memory";
        case cgltf_result_legacy_gltf:
            return "legacy glTF";
        default:
            return "unknown parse result";
        }
    };
    cgltf_options options = {};
    memset(&options, 0, sizeof(options));
    // This needs to be freed using the cgltf_free function if cgltf_parse_file is successful.
    cgltf_data *data_ptr = nullptr;
    cgltf_result parse_result = cgltf_parse_file(&options, filename.c_str(), &data_ptr);

    if (parse_result != cgltf_result_success)
    {
        std::cerr << "Error parsing " << filename << ": " << res_to_str(parse_result) << std::endl;
        return false;
    }

    cgltf_result load_result = cgltf_load_buffers(&options, data_ptr, filename.c_str());

    if (load_result != cgltf_result_success)
    {
        std::cerr << "Error loading buffers: " << res_to_str(load_result) << std::endl;
        cgltf_free(data_ptr);
        return false;
    }

    cgltf_data &data = *data_ptr;

    // Add materials to scene;
    scene.materials.reserve(data.materials_count);

    if (data.materials_count == 0)
        scene.materials.emplace_back(default_material);

    for (size_t i = 0; i < data.materials_count; ++i)
    {
        Material material = Material();
        cgltf_material &gltf_material = data.materials[i];

        material.emission = {gltf_material.emissive_factor[0],
                             gltf_material.emissive_factor[1],
                             gltf_material.emissive_factor[2]};

        if (gltf_material.has_pbr_metallic_roughness)
        {
            auto gltf_pbr = gltf_material.pbr_metallic_roughness;

            material.base_color = {gltf_pbr.base_color_factor[0],
                                   gltf_pbr.base_color_factor[1],
                                   gltf_pbr.base_color_factor[2]};

            material.metallic = gltf_pbr.metallic_factor;
            material.roughness = gltf_pbr.roughness_factor;
            // Metallic and roughness textures not currently implemented.
        }
        scene.materials.emplace_back(material);
    }

    // Read mesh data. Meshes consist of one or more primitives.
    // The primitives contain information about vertices, material, etc.
    auto mesh_data = std::vector<std::vector<primitive_data>>{};
    mesh_data.resize(data.meshes_count);

    for (size_t i = 0; i < data.meshes_count; ++i)
    {
        auto &gltf_mesh = data.meshes[i];
        auto &primitives = mesh_data[i];
        primitives.resize(gltf_mesh.primitives_count);
        for (size_t j = 0; j < gltf_mesh.primitives_count; ++j)
        {
            auto &gltf_primitive = gltf_mesh.primitives[j];
            auto &primitive = primitives[j];
            // Supporting only triangles.
            if (gltf_primitive.type != cgltf_primitive_type_triangles)
            {
                std::cerr << "unsupported primitive type: " << gltf_primitive.type << std::endl;
                continue;
            }
            if (gltf_primitive.material != nullptr)
            {
                primitive.material_index = static_cast<int>(gltf_primitive.material - data.materials);
            }

            for (size_t k = 0; k < gltf_primitive.attributes_count; ++k)
            {
                auto &attribute = gltf_primitive.attributes[k];
                auto &accessor = *attribute.data;
                // Todo: normal and texture attributes
                if (attribute.type != cgltf_attribute_type_position || accessor.type != cgltf_type_vec3)
                    continue;

                static_assert(sizeof(Vec3) == sizeof(float) * 3, "Expecting Vec3 to contain three floats");
                primitive.points.resize(accessor.count);
                cgltf_accessor_unpack_floats(&accessor, reinterpret_cast<float *>(primitive.points.data()), 3 * accessor.count);

                primitive.indices.reserve(gltf_primitive.indices->count);
                for (size_t idx = 0; idx < gltf_primitive.indices->count; ++idx)
                    primitive.indices.emplace_back(cgltf_accessor_read_index(gltf_primitive.indices, idx));
            }
        }
    }

    // Read camera attributes. Currently only fov.
    float yfov = 75;
    if (data.cameras_count > 0)
    {
        // There can be multiple cameras, but we use only the first one.
        auto &gltf_camera = data.cameras[0];
        if (gltf_camera.type == cgltf_camera_type_perspective)
            yfov = gltf_camera.data.perspective.yfov;
        else
            std::cerr << "Unsupported camera type" << std::endl;
    }

    // To make sure we only read data from one camera.
    bool found_camera = false;

    // A scene can contain the same mesh multiple times with different transforms and materials applied to it.
    // World space transform of a node is the product of the node's parents transform and a node's local transform.
    for (size_t i = 0; i < data.nodes_count; ++i)
    {
        cgltf_node &node = data.nodes[i];
        std::array<float, 16> to_world{};

        cgltf_node_transform_world(&node, to_world.data());

        if (node.camera != nullptr && !found_camera)
        {
            found_camera = true;
            Vec3 position = transform(to_world, {0, 0, 0});
            Vec3 up = rotate(to_world, {0, 1, 0});
            Vec3 look_at = rotate(to_world, {0, 0, -1});
            scene.camera = Camera(position, up, look_at, yfov);
        }
        if (node.mesh != nullptr)
        {
            for (auto &primitive : mesh_data.at(node.mesh - data.meshes))
            {
                std::vector<Vec3> points = primitive.points;
                transform(to_world, points);

                for (size_t idx = 0; idx < primitive.indices.size(); idx += 3)
                {
                    Vec3 vertex0 = points[primitive.indices[idx]];
                    Vec3 vertex1 = points[primitive.indices[idx + 1]];
                    Vec3 vertex2 = points[primitive.indices[idx + 2]];
                    scene.triangles.emplace_back(vertex0, vertex1, vertex2);
                    scene.material_indices.emplace_back(primitive.material_index.value_or(0));
                }
            }
        }
    }
    assert(scene.triangles.size() == scene.material_indices.size());
    assert(!scene.materials.empty());

    auto end_time = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    std::cout << "Loaded scene in " << elapsed << " µs\n- "
              << scene.materials.size() << " materials\n- "
              << scene.triangles.size() << " triangles\n"
              << scene.camera << "\nyfov = " << yfov << std::endl;

    for (auto &mat : scene.materials)
    {
        std::cout << mat << std::endl;
    }

    cgltf_free(data_ptr);
    return true;
}
