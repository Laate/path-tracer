#pragma once

#include <optional>
#include <ostream>

#include "math/constants.hpp"
#include "math/vec3.hpp"
#include "ray.hpp"

struct Triangle
{
    constexpr Triangle(Vec3 vertex0, Vec3 vertex1, Vec3 vertex2) : vertex0(vertex0),
                                                                   edge1(vertex1 - vertex0),
                                                                   edge2(vertex2 - vertex0){};
    Vec3 vertex0;
    Vec3 edge1;
    Vec3 edge2;
};

[[nodiscard]] inline auto intersect(const Triangle &triangle, const Ray &ray, float t_min, float t_max) -> std::optional<float>
{
    // Möller-Trumbore triangle intersection algorithm
    const Vec3 h = cross(ray.direction, triangle.edge2);
    float det = dot(triangle.edge1, h);
    if (det > -math::EPSILON && det < math::EPSILON)
        return {};
    float inv_det = 1.0F / det;
    const Vec3 s = ray.origin - triangle.vertex0;
    float u = inv_det * dot(s, h);
    if (u < 0.0F || u > 1.0F)
        return {};
    const Vec3 q = cross(s, triangle.edge1);
    float v = inv_det * dot(ray.direction, q);
    if (v < 0.0F || u + v > 1.0F)
        return {};
    float t = inv_det * dot(triangle.edge2, q);
    if (t < t_min || t > t_max)
        return {};

    return {t};
}

auto operator<<(std::ostream &os, const Triangle &t) -> std::ostream &;
