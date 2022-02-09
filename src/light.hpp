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
    float inner_cone_cos;
    float outer_cone_cos;

    SpotLight(Vec3 rad, Vec3 pos, Vec3 dir, float inner_cone_angle, float outer_cone_angle)
        : radiance(rad),
          position(pos),
          direction(dir),
          inner_cone_cos(cosf(inner_cone_angle)),
          outer_cone_cos(cosf(outer_cone_angle)){};
};

inline auto range_attenuation(const Vec3 &light_position, const Vec3 &point) -> float
{
    return 1.0F / length_squared(light_position - point);
}

inline auto intensity(const DirectionalLight &light) -> Vec3
{
    return light.radiance;
}

inline auto intensity(const PointLight &light, const Vec3 &point) -> Vec3
{
    return light.radiance * range_attenuation(light.position, point);
}

inline auto intensity(const SpotLight &light, const Vec3 &point) -> Vec3
{
    float actual_cos = dot(light.direction, normalize(point - light.position));
    float spot_attenuation = 0.0F;
    if (actual_cos > light.outer_cone_cos)
    {
        spot_attenuation = (actual_cos < light.inner_cone_cos) ? smooth_step(light.outer_cone_cos, light.inner_cone_cos, actual_cos)
                                                               : 1.0F;
    }

    return light.radiance * range_attenuation(light.position, point) * spot_attenuation;
}

auto operator<<(std::ostream &os, const DirectionalLight &light) -> std::ostream &;
auto operator<<(std::ostream &os, const PointLight &light) -> std::ostream &;
auto operator<<(std::ostream &os, const SpotLight &light) -> std::ostream &;
