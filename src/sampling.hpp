#pragma once

#include <cassert>
#include <cmath>

#include "material.hpp"
#include "math/constants.hpp"
#include "math/rng.hpp"
#include "math/vec3.hpp"

// Transforms the input vector into a coordinate system where the normal is the z-axis.
// Assumes normal is unit vector.
inline auto to_orthonormal_basis(const Vec3 &input, const Vec3 &normal) -> Vec3
{
    // Building an Orthonormal Basis, Revisited. 2017.
    // https://graphics.pixar.com/library/OrthonormalB/paper.pdf
    auto sign = copysignf(1.0F, normal.z);
    const auto a = -1.0F / (sign + normal.z);
    const auto b = normal.x * normal.y * a;
    Vec3 basis_x = {1.0F + sign * normal.x * normal.x * a, sign * b, -sign * normal.x};
    Vec3 basis_y = {b, sign + normal.y * normal.y * a, -normal.y};

    return basis_x * input.x + basis_y * input.y + normal * input.z;
}

// Sample a direction with uniform distribution.
inline auto uniform_sample_hemisphere(const Vec3 &normal, RNG &rng) -> Vec3
{
    // Generate a random direction in a hemisphere oriented around the z-axis.
    auto r1 = zero_to_one(rng);
    auto r2 = zero_to_one(rng);
    auto z = r1;
    auto r = std::sqrt(1.0F - z * z);
    auto phi = 2 * math::PI * r2;
    auto random_direction = Vec3(r * std::cos(phi), r * std::sin(phi), z);

    // Transform the direction so that it's in a hemisphere oriented around the normal.
    return to_orthonormal_basis(random_direction, normal);
}

// Sample a direction with cosine distribution.
inline auto sample_hemisphere_cos(const Vec3 &normal, RNG &rng) -> Vec3
{
    auto r1 = zero_to_one(rng);
    auto r2 = zero_to_one(rng);
    auto z = std::sqrt(r1); // This sqrt is the only difference between this and uniform sample.
    auto r = std::sqrt(1.0F - z * z);
    auto phi = 2 * math::PI * r2;
    auto random_direction = Vec3(r * std::cos(phi), r * std::sin(phi), z);

    return to_orthonormal_basis(random_direction, normal);
}
