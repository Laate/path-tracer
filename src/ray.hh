#pragma once

#include "vec3.hh"

struct Ray
{
    Ray(Vec3 origin, Vec3 direction) : origin(origin), direction(direction){};
    Vec3 origin;
    Vec3 direction;
};
