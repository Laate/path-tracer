#pragma once

#include <cmath>
#include <iostream>

#include "math/constants.hpp"
#include "math/vec3.hpp"
#include "ray.hpp"

class Camera
{
private:
    Vec3 origin_, x_axis_, y_axis_, z_axis_;
    float img_plane_distance_;

public:
    Camera(Vec3 position, Vec3 up, Vec3 look_at, float fov_degrees) : origin_(position),
                                                                      y_axis_(normalize(up)),
                                                                      z_axis_(normalize(look_at)),
                                                                      img_plane_distance_(1.0F / tanf(fov_degrees * (math::PI / 180.0F) / 2.0F))

    {
        x_axis_ = normalize(cross(z_axis_, y_axis_));
    }

    inline auto get_ray(float x, float y) const -> Ray
    {
        return {origin_, normalize(x * x_axis_ + y * y_axis_ + img_plane_distance_ * z_axis_)};
    }
};
