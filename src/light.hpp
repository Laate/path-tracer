#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>

#include "math/utility.hpp"
#include "math/vec3.hpp"

struct DirectionalLight
{
    Vec3 radiance;
    Vec3 direction;

    DirectionalLight(Vec3 rad, Vec3 dir) : radiance(rad), direction(dir){};
};

struct PointLight
{
    Vec3 radiance;
    Vec3 position;

    PointLight(Vec3 rad, Vec3 pos) : radiance(rad), position(pos){};
};

struct SpotLight
{
    Vec3 radiance;
    Vec3 position;
    Vec3 direction;
    float angle_scale;
    float angle_offset;

    SpotLight(Vec3 rad, Vec3 pos, Vec3 dir, float inner_cone_angle, float outer_cone_angle)
        : radiance(rad),
          position(pos),
          direction(dir),
          angle_scale(1.0f / std::max(0.001F, cosf(inner_cone_angle) - cosf(outer_cone_angle)))
    {
        angle_offset = -cosf(outer_cone_angle) * angle_scale;
    };
};

inline auto intensity(const DirectionalLight &light, const Vec3 & /*point*/) -> Vec3
{
    return light.radiance;
}

inline auto intensity(const PointLight &light, const Vec3 &point) -> Vec3
{
    return light.radiance * (1.0F / length_squared(light.position - point));
}

inline auto intensity(const SpotLight &light, const Vec3 &point) -> Vec3
{
    float cd = -dot(light.direction, normalize(light.position - point));
    return light.radiance * math::clamp(cd * light.angle_scale + light.angle_offset, 0.0F, 1.0F);
}

auto operator<<(std::ostream &os, const DirectionalLight &light) -> std::ostream &;
auto operator<<(std::ostream &os, const PointLight &light) -> std::ostream &;
auto operator<<(std::ostream &os, const SpotLight &light) -> std::ostream &;
