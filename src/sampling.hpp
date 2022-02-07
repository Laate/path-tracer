#pragma once

#include <cassert>
#include <cmath>
#include <random>

#include "../libs/pcg/pcg_random.hpp"

#include "material.hpp"
#include "math/constants.hpp"
#include "math/vec3.hpp"

// Create a coordinate system with the normal as z-axis.
// Assumes normal is a unit vector.
inline auto orthonormal_basis(const Vec3 &normal, Vec3 &basis_x, Vec3 &basis_y) -> void
{
    // Building an Orthonormal Basis, Revisited. 2017.
    // https://graphics.pixar.com/library/OrthonormalB/paper.pdf
    auto sign = copysignf(1.0F, normal.z);
    const auto a = -1.0F / (sign + normal.z);
    const auto b = normal.x * normal.y * a;
    basis_x = Vec3(1.0F + sign * normal.x * normal.x * a, sign * b, -sign * normal.x);
    basis_y = Vec3(b, sign + normal.y * normal.y * a, -normal.y);
}

// Todo: importance sampling

inline auto uniform_sample_hemisphere(const Vec3 &normal, pcg32 &rng) -> Vec3
{
    // Generate a random direction in a hemisphere oriented around the z-axis.
    std::uniform_real_distribution<float> zero_to_one(0.0F, 1.0F);
    auto r1 = zero_to_one(rng);
    auto r2 = zero_to_one(rng);
    auto z = r1;
    auto r = std::sqrt(1.0F - z * z);
    auto phi = 2 * math::PI * r2;
    auto random_direction = Vec3(r * std::cos(phi), r * std::sin(phi), z);

    // Transform the direction so that it's in a hemisphere oriented around the normal.
    Vec3 basis_x;
    Vec3 basis_y;
    orthonormal_basis(normal, basis_x, basis_y);

    return basis_x * random_direction.x + basis_y * random_direction.y + normal * random_direction.z;
}
