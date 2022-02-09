#include <random>
#include <vector>

#include "../libs/pcg/pcg_random.hpp"

#include "light.hpp"
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
    constexpr Material grey_material = {0.5F, 0.5F, Vec3(0.0F, 0.0F, 0.0F), Vec3(0.5F, 0.5F, 0.5F)};

    auto a_triangle() -> Scene
    {
        Triangle triangle1 = {Vec3(-10, -5, 0), Vec3(0, 7, 0), Vec3(10, -5, 0)};
        return {{triangle1}, {white_emissive_material}, {0}};
    }

    auto random_triangles(int N) -> Scene
    {
        auto rng = pcg32(1729);
        std::uniform_real_distribution<float> zero_to_one(0.0F, 1.0F);
        auto random_vec = [&]() -> Vec3
        {
            return {zero_to_one(rng), zero_to_one(rng), zero_to_one(rng)};
        };
        std::vector<Triangle> random_triangles;
        random_triangles.reserve(N);
        std::vector<int> material_indices(N, 0);

        for (int i = 0; i < N; ++i)
        {
            random_triangles.emplace_back(random_vec(), random_vec(), random_vec());
        }

        return {random_triangles, {white_emissive_material}, material_indices};
    }

    auto indirect_illumination() -> Scene
    {
        Triangle emissive_triangle = {Vec3(-5, -2.5, 0), Vec3(0, 3.5, 0), Vec3(5, -2.5, 0)};
        Triangle red_triangle = {Vec3(-5, -5.5, -1), Vec3(0, 0.5, -1), Vec3(5, -5.5, -1)};
        Triangle green_triangle = {Vec3(-8, -1.5, -1), Vec3(-3, 4.5, -1), Vec3(2, -1.5, -1)};
        Triangle blue_triangle = {Vec3(-2, -1.5, -1), Vec3(3, 4.5, -1), Vec3(8, -1.5, -1)};

        return {{emissive_triangle,
                 red_triangle, green_triangle, blue_triangle},
                {white_emissive_material, red_material, green_material, blue_material},
                {0, 1, 2, 3}};
    }

    auto spot_light() -> Scene
    {
        auto scene = Scene();

        SpotLight spot_light = {{20, 20, 5}, {11, -1, 2}, {-1, 0, -1}, 0.0F, math::PI / 4.0F};

        Triangle red_triangle = {Vec3(-5, -5.5, 0), Vec3(0, 0.5, 0), Vec3(5, -5.5, 0)};
        Triangle green_triangle = {Vec3(-8, -1.5, 0), Vec3(-3, 4.5, 0), Vec3(2, -1.5, 0)};
        Triangle blue_triangle = {Vec3(-2, -1.5, 0), Vec3(3, 4.5, 0), Vec3(8, -1.5, 0)};
        Triangle backgroud = {Vec3(-50, -25, -1), Vec3(0, 35, -1), Vec3(50, -25, -1)};

        scene.triangles = {red_triangle, green_triangle, blue_triangle, backgroud};
        scene.materials = {red_material, green_material, blue_material, grey_material};
        scene.material_indices = {0, 1, 2, 3};
        scene.lights.emplace_back(spot_light);

        return scene;
    }

    auto point_light() -> Scene
    {
        auto scene = Scene();

        PointLight point_light = {{20, 20, 5}, {11, -1, 5}};

        Triangle red_triangle = {Vec3(-5, -5.5, 0), Vec3(0, 0.5, 0), Vec3(5, -5.5, 0)};
        Triangle green_triangle = {Vec3(-8, -1.5, 0), Vec3(-3, 4.5, 0), Vec3(2, -1.5, 0)};
        Triangle blue_triangle = {Vec3(-2, -1.5, 0), Vec3(3, 4.5, 0), Vec3(8, -1.5, 0)};
        Triangle backgroud = {Vec3(-50, -25, -1), Vec3(0, 35, -1), Vec3(50, -25, -1)};

        scene.triangles = {red_triangle, green_triangle, blue_triangle, backgroud};
        scene.materials = {red_material, green_material, blue_material, grey_material};
        scene.material_indices = {0, 1, 2, 3};
        scene.lights.emplace_back(point_light);

        return scene;
    }

} // namespace scenes
