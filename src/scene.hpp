#pragma once

#include <optional>
#include <utility>
#include <variant>
#include <vector>

#include "camera.hpp"
#include "light.hpp"
#include "material.hpp"
#include "math/constants.hpp"
#include "triangle.hpp"

struct Intersection
{
    Vec3 point;
    Vec3 normal;
    Material material;
};

struct Scene
{
    std::vector<Triangle> triangles;
    std::vector<Material> materials;
    std::vector<int> material_indices;
    std::vector<std::variant<DirectionalLight, PointLight, SpotLight>> lights;
    // Todo: normals
    // std::vector<Vec3> normals;
    // Todo: uv coordinates
    // std::vector<Vec2> uvs;
    Camera camera = Camera(Vec3(0, 0, 15), Vec3(0, 1, 0), Vec3(0, 0, -1), 1.3F);

    Scene() = default;
    Scene(std::vector<Triangle> triangles, std::vector<Material> materials, std::vector<int> material_indices)
        : triangles(std::move(triangles)),
          materials(std::move(materials)),
          material_indices(std::move(material_indices)){};
};

inline auto intersect(const Scene &scene,
                      const Ray &ray,
                      const float t_min = math::EPSILON,
                      const float t_max = math::FLOAT_MAX) -> std::optional<Intersection>
{
    float closest_t = t_max;
    int closest_index = -1;

    for (size_t i = 0; i < scene.triangles.size(); ++i)
    {
        std::optional<float> t = intersect(scene.triangles[i], ray, t_min, t_max);
        if (t && *t < closest_t)
        {
            closest_t = *t;
            closest_index = i;
        }
    }
    if (closest_index == -1)
        return {};

    Triangle closest_triangle = scene.triangles[closest_index];
    Vec3 normal = normalize(cross(closest_triangle.edge1, closest_triangle.edge2));

    return {{ray.origin + ray.direction * closest_t,
             dot(-ray.direction, normal) < 0 ? -normal : normal,
             scene.materials[scene.material_indices[closest_index]]}};
}

// Shadow rays don't care about closest intersection.
// Can exit early if any intersection is found.
inline auto intersect_shadow(const Scene &scene,
                             const Ray &shadow_ray,
                             const float t_min = math::EPSILON,
                             const float t_max = math::FLOAT_MAX) -> bool
{
    for (const auto &triangle : scene.triangles)
    {
        std::optional<float> t = intersect(triangle, shadow_ray, t_min, t_max);
        if (t)
            return true;
    }
    return false;
}

namespace scenes
{
    auto a_triangle() -> Scene;
    auto random_triangles(int N) -> Scene;
    auto indirect_illumination() -> Scene;
    auto spot_light() -> Scene;
    auto point_light() -> Scene;
} // namespace scenes
