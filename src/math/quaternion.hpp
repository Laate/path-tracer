#pragma once

#include "vec3.hpp"

struct Quaternion
{
    float x{}, y{}, z{}, w{};

    constexpr Quaternion() noexcept = default;
    constexpr Quaternion(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w){};
};

constexpr auto conjugate(const Quaternion &a) noexcept -> Quaternion
{
    return {-a.x, -a.y, -a.z, a.w};
};

constexpr auto operator*(const Quaternion a, const Quaternion &b) noexcept -> Quaternion
{
    return {
        a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
        a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
        a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z};
}

constexpr auto rotate(const Quaternion a, const Vec3 b) noexcept -> Vec3
{
    Quaternion p = {b.x, b.y, b.z, 0.0F};
    Quaternion r = a * p * conjugate(a);
    return {r.x, r.y, r.z};
}
