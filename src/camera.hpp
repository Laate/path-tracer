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
    Camera(const Vec3 &position, const Vec3 &up, const Vec3 &look_at, float fov_radians) : origin_(position),
                                                                                           y_axis_(normalize(up)),
                                                                                           z_axis_(normalize(look_at)),
                                                                                           img_plane_distance_(1.0F / tanf(fov_radians / 2.0F))

    {
        x_axis_ = normalize(cross(z_axis_, y_axis_));
    }

    [[nodiscard]] inline auto get_ray(float x, float y) const -> Ray
    {
        return {origin_, normalize(x * x_axis_ + y * y_axis_ + img_plane_distance_ * z_axis_)};
    }

    friend auto operator<<(std::ostream &os, const Camera &camera) -> std::ostream &
    {
        return os << "Camera {origin = " << camera.origin_
                  << "\n\tx-axis = " << camera.x_axis_
                  << "\n\ty-axis = " << camera.y_axis_
                  << "\n\tz-axis = " << camera.z_axis_
                  << "}";
    }
};
