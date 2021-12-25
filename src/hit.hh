#pragma once

#include <limits>

#include "vec3.hh"

struct Hit
{
    Hit() : t(std::numeric_limits<float>::infinity()){};
    float t;
    Vec3 normal;
};
