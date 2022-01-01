#pragma once
#include <utility>
#include <vector>

#include "camera.hpp"
#include "hit.hpp"
#include "material.hpp"
#include "math/constants.hpp"
#include "triangle.hpp"

struct Scene
{
    std::vector<Triangle> triangles;
    std::vector<Material> materials;
    std::vector<int> material_indices;
    // Todo: normals
    // std::vector<Vec3> normals;
    // Todo: uv coordinates
    // std::vector<Vec2> uvs;
    Camera camera = Camera(Vec3(0, 0, 15), Vec3(0, 1, 0), Vec3(0, 0, -1), 75);

    Scene() = default;
    Scene(std::vector<Triangle> triangles, std::vector<Material> materials, std::vector<int> material_indices)
        : triangles(std::move(triangles)),
          materials(std::move(materials)),
          material_indices(std::move(material_indices)){};
};

inline auto intersect(const Scene &scene,
                      const Ray &ray, Hit &hit,
                      const float t_min = math::EPSILON,
                      const float t_max = math::FLOAT_MAX) -> bool
{
    float closest_t = t_max;
    int closest_index = -1;

    for (size_t i = 0; i < scene.triangles.size(); ++i)
    {
        // Möller-Trumbore triangle intersection algorithm
        const auto &tri = scene.triangles[i];
        const Vec3 h = cross(ray.direction, tri.edge2);
        float det = dot(tri.edge1, h);
        if (det > -math::EPSILON && det < math::EPSILON)
            continue;
        float inv_det = 1.0F / det;
        const Vec3 s = ray.origin - tri.vertex0;
        float u = inv_det * dot(s, h);
        if (u < 0.0F || u > 1.0F)
            continue;
        const Vec3 q = cross(s, tri.edge1);
        float v = inv_det * dot(ray.direction, q);
        if (v < 0.0F || u + v > 1.0F)
            continue;
        float t = inv_det * dot(tri.edge2, q);
        if (t < closest_t && t > t_min)
        {
            closest_t = t;
            closest_index = i;
        }
    }
    if (closest_index == -1)
        return false;

    // Found an intersection
    hit.t = closest_t;
    hit.point = ray.origin + ray.direction * closest_t;
    hit.material = scene.materials[scene.material_indices[closest_index]];
    Triangle closest_triangle = scene.triangles[closest_index];
    Vec3 normal = normalize(cross(closest_triangle.edge1, closest_triangle.edge2));
    hit.normal = dot(-ray.direction, normal) < 0 ? -normal : normal;
    return true;
}

namespace scenes
{
    auto a_triangle() -> Scene;
    auto random_triangles(int N) -> Scene;
    auto indirect_illumination() -> Scene;
} // namespace scenes
