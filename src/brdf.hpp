#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>

#include "material.hpp"
#include "math/constants.hpp"
#include "math/rng.hpp"
#include "math/vec3.hpp"
#include "sampling.hpp"

struct Brdf
{
    Vec3 value{};
    float probability{};
};

inline auto reflect(const Vec3 &incident, const Vec3 &normal) -> Vec3
{
    return incident - 2.0F * dot(normal, incident) * normal;
}

// Schlick's approximation of the Fresnel term.
inline auto fresnel(const Vec3 &F0, float V_dot_H) -> Vec3
{
    return F0 + (1.0F - F0) * std::pow(std::clamp(1.0F - V_dot_H, 0.0F, 1.0F), 5);
}

// Trowbridge-Reitz distribution function.
// Models the distribution of microfacet normals.
inline auto microfacet_distribution(float roughness, float N_dot_H) -> float
{
    const auto roughness_squared = roughness * roughness;
    const auto f = (N_dot_H * N_dot_H) * (roughness_squared - 1.0F) + 1.0F;
    return roughness_squared / (math::PI * f * f);
}

// Smith joint masking-shadowing function.
// See Eric Heitz. 2014. Understanding the Masking-Shadowing Function in Microfacet-Based BRDFs.
inline auto microfacet_shadowing(float roughness, float N_dot_L, float N_dot_V) -> float
{
    const auto roughness_squared = roughness * roughness;

    const auto GGXL = N_dot_V * std::sqrt(N_dot_L * N_dot_L * (1.0F - roughness_squared) + roughness_squared);
    const auto GGXV = N_dot_L * std::sqrt(N_dot_V * N_dot_V * (1.0F - roughness_squared) + roughness_squared);
    const auto GGX = GGXL + GGXV;

    return std::max(0.5F / GGX, 0.0F);
}

inline auto sample_microfacet(float roughness, const Vec3 &normal, RNG &rng) -> Vec3
{
    const auto r1 = zero_to_one(rng);
    const auto r2 = zero_to_one(rng);

    const auto phi = r1 * math::TWO_PI;
    const auto cos_theta = std::sqrt((1.0F - r2) / (1.0F + (roughness * roughness - 1.0F) * r2));
    const auto sin_theta = std::clamp(std::sqrt(1.0F - (cos_theta * cos_theta)), 0.0F, 1.0F);
    const auto sin_phi = sinf(phi);
    const auto cos_phi = cosf(phi);
    Vec3 local_half_vector = {sin_theta * cos_phi, sin_theta * sin_phi, cos_theta};

    return to_orthonormal_basis(local_half_vector, normal);
}

inline auto specular_brdf(const Material &material, const Vec3 &normal, const Vec3 &outgoing, const Vec3 &incoming, const Vec3 &halfway) -> Brdf
{
    const auto N_dot_V = std::clamp(dot(normal, outgoing), 0.001F, 1.0F);
    const auto N_dot_L = std::clamp(dot(normal, incoming), 0.001F, 1.0F);
    const auto N_dot_H = std::clamp(dot(normal, halfway), 0.0F, 1.0F);
    const auto L_dot_H = std::clamp(dot(incoming, halfway), 0.0F, 1.0F);
    const auto V_dot_H = std::clamp(dot(outgoing, halfway), 0.0F, 1.0F);
    const auto roughness = std::clamp(material.roughness, 0.001F, 1.0F);

    const auto dielectric_specular = Vec3(0.04F, 0.04F, 0.04F);
    const auto F0 = lerp(dielectric_specular, material.base_color, material.metallic);

    const auto F = fresnel(F0, V_dot_H);
    const auto G = microfacet_distribution(roughness, N_dot_H);
    const auto D = microfacet_shadowing(roughness, N_dot_L, N_dot_V);

    const auto probability = microfacet_distribution(roughness, N_dot_H) * N_dot_H / (4.0F * L_dot_H);

    return {F * G * D,
            probability};
}

inline auto eval_brdf(const Material &material, const Vec3 &normal, const Vec3 &outgoing, const Vec3 &incoming) -> Brdf
{
    const auto N_dot_V = dot(normal, outgoing);
    const auto N_dot_L = dot(normal, incoming);

    if (N_dot_V < 0.0F || N_dot_L < 0.0F)
    {
        return {{0, 0, 0}, 0.0F};
    }

    Vec3 value = {0, 0, 0};
    float probability = 0.0F;

    const auto halfway = normalize(incoming + outgoing);
    const auto dielectric_specular = Vec3(0.04F, 0.04F, 0.04F);
    const auto diffuse_color = lerp(material.base_color * (1.0F - dielectric_specular), Vec3(0.0F, 0.0F, 0.0F), material.metallic);
    const auto F0 = lerp(dielectric_specular, material.base_color, material.metallic);
    const auto F = fresnel(F0, dot(halfway, outgoing));
    const auto specular_probability = mean(F);
    const auto diffuse_probability = 1.0F - specular_probability;

    // Diffuse
    value += (1.0F - F) * diffuse_color * math::INV_PI;
    probability += math::INV_PI * diffuse_probability;

    // Specular
    Brdf spec_brdf = specular_brdf(material, normal, outgoing, incoming, normalize(incoming + outgoing));
    value += spec_brdf.value;
    probability += spec_brdf.probability * specular_probability;

    return {value, probability};
}

// Importance sampling i.e., more likely to return directions where the BRDF has high values.
// Reduces variance compared to uniform sampling a direction.
inline auto sample_direction(const Material &material, const Vec3 &normal, const Vec3 &outgoing, RNG &rng) -> Vec3
{
    const auto dielectric_specular = Vec3(0.04F, 0.04F, 0.04F);
    const auto F0 = lerp(dielectric_specular, material.base_color, material.metallic);
    const auto F = fresnel(F0, dot(normal, outgoing));
    const auto diffuse_probability = 1.0F - mean(F);

    if (material.roughness == 0.0F && material.metallic == 1.0F)
    {
        // Perfectly specular material will only reflect to one direction.
        return reflect(-outgoing, normal);
    }

    if (zero_to_one(rng) < diffuse_probability)
    {
        // Sample diffuse.
        return sample_hemisphere_cos(normal, rng);
    }

    // Sample specular
    Vec3 halfway = sample_microfacet(material.roughness, normal, rng);
    return reflect(-outgoing, halfway);
}
