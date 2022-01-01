#pragma once

#include <cassert>
#include <cmath>
#include <iostream>
#include <random>

#include "material.hpp"
#include "math/constants.hpp"
#include "math/utility.hpp"
#include "math/vec3.hpp"

// Schlick's approximation of the Fresnel term
inline auto fresnel(const Vec3 &F0, const Vec3 &outgoing, const Vec3 &halfway) -> Vec3
{
    return F0 + (1.0F - F0) * std::pow(math::clamp(1.0F - dot(outgoing, halfway), 0.0F, 1.0F), 5);
}

// Trowbridge-Reitz distribution function
inline auto microfacet_distribution(float roughness, const Vec3 &normal, const Vec3 &halfway) -> float
{
    const auto N_dot_H = dot(normal, halfway);
    const auto roughness_squared = roughness * roughness;
    const auto f = (N_dot_H * N_dot_H) * (roughness_squared - 1.0F) + 1.0F;
    return roughness_squared / (math::PI * f * f);
}

// Smith joint masking-shadowing function
inline auto microfacet_shadowing(float roughness, const Vec3 &normal, const Vec3 &incoming, const Vec3 &outgoing) -> float
{
    const auto N_dot_V = dot(normal, outgoing);
    const auto N_dot_L = dot(normal, incoming);
    const auto roughness_squared = roughness * roughness;

    const auto GGXL = N_dot_V * std::sqrt(N_dot_L * N_dot_L * (1.0F - roughness_squared) + roughness_squared);
    const auto GGXV = N_dot_L * std::sqrt(N_dot_V * N_dot_V * (1.0F - roughness_squared) + roughness_squared);
    const auto GGX = GGXL + GGXV;

    return std::max(0.5F / GGX, 0.0F);
}

inline auto brdf(const Material &material, const Vec3 &normal, const Vec3 &incoming, const Vec3 &outgoing) -> Vec3
{
    assert(dot(normal, outgoing) > 0.0F);
    assert(dot(normal, incoming) > 0.0F);
    const auto dielectric_specular = Vec3(0.04F, 0.04F, 0.04F);
    const auto diffuse_color = lerp(material.base_color * (1.0F - dielectric_specular), Vec3(0.0F, 0.0F, 0.0F), material.metallic);
    const auto F0 = lerp(dielectric_specular, material.base_color, material.metallic);
    const auto up_normal = dot(normal, incoming) > 0.0F ? normal : -normal;
    const auto halfway = normalize(incoming + outgoing);

    const auto F = fresnel(F0, outgoing, halfway);
    const auto G = microfacet_distribution(material.roughness, up_normal, halfway);
    const auto D = microfacet_shadowing(material.roughness, up_normal, incoming, outgoing);

    const auto diffuse = (1.0F - F) * diffuse_color * math::INV_PI;
    const auto specular = (F * G * D) / (4 * dot(up_normal, incoming) * dot(up_normal, outgoing));

    return diffuse + specular;
}
