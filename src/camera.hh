#pragma once

#include <cmath>
#include <iostream>

#include "math.hh"
#include "ray.hh"
#include "vec3.hh"

class Camera
{
private:
    Vec3 origin_, x_axis_, y_axis_, z_axis_;
    const float img_plane_distance;

public:
    Camera(Vec3 position, Vec3 up, Vec3 look_at, float fov) : origin_(position),
                                                              y_axis_(up.normalized()),
                                                              z_axis_(look_at.normalized()),
                                                              img_plane_distance(1.0F / tanf(fov * (Math::PI / 180.0F) / 2.0F))

    {
        x_axis_ = z_axis_.cross(y_axis_).normalized();
    }

    inline auto GetRay(float x, float y) const -> Ray
    {
        Vec3 direction = (x * x_axis_ + y * y_axis_ + img_plane_distance * z_axis_).normalized();
        return {origin_, direction};
    }
};
