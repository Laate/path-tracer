#pragma once

#include "math/vec3.hpp"

struct Material
{
    float roughness{};
    float metallic{};
    Vec3 emission{};
    Vec3 base_color{};
};

auto operator<<(std::ostream &os, const Material &mat) -> std::ostream &;
