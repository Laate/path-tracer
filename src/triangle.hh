#pragma once

#include "vec3.hh"
#include "hit.hh"
#include "math.hh"
#include "ray.hh"
#include <array>

struct Triangle
{
    Triangle(Vec3 vertex0, Vec3 vertex1, Vec3 vertex2) : vertex0(vertex0),
                                                         edge1(vertex1 - vertex0),
                                                         edge2(vertex2 - vertex0){};
    Vec3 vertex0;
    Vec3 edge1;
    Vec3 edge2;
};
