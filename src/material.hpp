#pragma once

#include "math/vec3.hpp"

// Values should be in the range [0, 1]
struct Material
{
    float roughness{};
    float metallic{};
    Vec3 emission{};
    Vec3 base_color{};
};

auto operator<<(std::ostream &os, const Material &mat) -> std::ostream &;
