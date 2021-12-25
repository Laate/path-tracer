#pragma once
#include <utility>
#include <vector>

#include "camera.hh"
#include "math.hh"
#include "triangle.hh"

struct Scene
{
    std::vector<Triangle> triangles;
    explicit Scene(std::vector<Triangle> triangles) : triangles(std::move(triangles)){};

    inline auto Intersect(const Ray &ray, Hit &hit,
                          const float t_min = Math::EPSILON,
                          const float t_max = Math::INF) const -> bool
    {
        float closest_t = t_max;
        size_t closest_i = -1;
        for (size_t i = 0; i < triangles.size(); ++i)
        {
            // Möller-Trumbore intersection algorithm
            const auto &tri = triangles[i];
            const Vec3 h = ray.direction.cross(tri.edge2);
            float a = tri.edge1.dot(h);
            if (a > -Math::EPSILON && a < Math::EPSILON)
                continue;
            float f = 1.0F / a;
            const Vec3 s = ray.origin - tri.vertex0;
            float u = f * s.dot(h);
            if (u < 0.0F || u > 1.0F)
                continue;
            const Vec3 q = s.cross(tri.edge1);
            float v = f * ray.direction.dot(q);
            if (v < 0.0F || u + v > 1.0F)
                continue;
            float t = f * tri.edge2.dot(q);
            if (t < closest_t && t > t_min)
            {
                closest_t = t;
                closest_i = i;
            }
        }
        if (closest_t == t_max)
            return false;

        hit.t = closest_t;
        const Triangle &tri = triangles[closest_i];
        hit.normal = tri.edge1.cross(tri.edge2).normalized();
        return true;
    }
};

namespace Scenes
{
    inline auto SingleTriangle() -> Scene
    {
        Triangle triangle1 = Triangle(Vec3(-10, -5, 0), Vec3(0, 7, 0), Vec3(10, -5, 0));
        return Scene({triangle1});
    }

    inline auto RandomTriangles(int N) -> Scene
    {
        std::vector<Triangle> random_triangles;
        random_triangles.reserve(N);

        for (int i = 0; i < N; ++i)
        {
            random_triangles.emplace_back(Vec3::random(), Vec3::random(), Vec3::random());
        }

        return Scene(random_triangles);
    }
} // namespace Scenes
