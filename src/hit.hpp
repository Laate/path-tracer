#pragma once

#include <limits>

#include "material.hpp"
#include "math/constants.hpp"
#include "math/vec3.hpp"

struct Hit
{
    Hit() : t(math::FLOAT_MAX){};
    float t;
    Vec3 normal;
    Vec3 point;
    Material material;
};
