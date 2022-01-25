#include <random>
#include <vector>

#include "material.hpp"
#include "math/vec3.hpp"
#include "scene.hpp"
#include "triangle.hpp"

namespace scenes
{
    constexpr Material white_emissive_material = {0.5F, 0.5F, Vec3(1.0F, 1.0F, 1.0F), Vec3(1.0F, 1.0F, 1.0F)};
    constexpr Material red_material = {0.5F, 0.5F, Vec3(0.0F, 0.0F, 0.0F), Vec3(1.0F, 0.0F, 0.0F)};
    constexpr Material green_material = {0.5F, 0.5F, Vec3(0.0F, 0.0F, 0.0F), Vec3(0.2F, 1.0F, 0.0001F)};
    constexpr Material blue_material = {0.5F, 0.5F, Vec3(0.0F, 0.0F, 0.0F), Vec3(0.0F, 0.0F, 1.0F)};

    auto a_triangle() -> Scene
    {
        Triangle triangle1 = Triangle(Vec3(-10, -5, 0), Vec3(0, 7, 0), Vec3(10, -5, 0));
        return Scene({triangle1}, {white_emissive_material}, {0});
    }

    auto random_triangles(int N) -> Scene
    {
        auto random_vec = []() -> Vec3
        {
            constexpr float inv_max = 1.0F / static_cast<float>(RAND_MAX);
            return {rand() * inv_max, rand() * inv_max, rand() * inv_max};
        };
        std::vector<Triangle> random_triangles;
        random_triangles.reserve(N);
        std::vector<int> material_indices(N, 0);

        for (int i = 0; i < N; ++i)
        {
            random_triangles.emplace_back(random_vec(), random_vec(), random_vec());
        }

        return Scene(random_triangles, {white_emissive_material}, material_indices);
    }

    auto indirect_illumination() -> Scene
    {
        Triangle emissive_triangle = Triangle(Vec3(-5, -2.5, 0), Vec3(0, 3.5, 0), Vec3(5, -2.5, 0));
        Triangle red_triangle = Triangle(Vec3(-5, -5.5, -1), Vec3(0, 0.5, -1), Vec3(5, -5.5, -1));
        Triangle green_triangle = Triangle(Vec3(-8, -1.5, -1), Vec3(-3, 4.5, -1), Vec3(2, -1.5, -1));
        Triangle blue_triangle = Triangle(Vec3(-2, -1.5, -1), Vec3(3, 4.5, -1), Vec3(8, -1.5, -1));
        return Scene({emissive_triangle,
                      red_triangle, green_triangle, blue_triangle},
                     {white_emissive_material, red_material, green_material, blue_material},
                     {0, 1, 2, 3});
    }

} // namespace scenes
