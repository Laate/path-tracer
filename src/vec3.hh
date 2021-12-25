#pragma once

#include <array>
#include <cmath>
#include <ostream>
#include <random>

class Vec3
{
public:
    float x{}, y{}, z{};

    constexpr Vec3() noexcept = default;
    constexpr Vec3(float x, float y, float z) noexcept : x(x), y(y), z(z) {}
    constexpr explicit Vec3(std::array<float, 3> coordinates) noexcept : x(coordinates[0]), y(coordinates[1]), z(coordinates[2]) {}

    constexpr auto operator+(const Vec3 &v) const noexcept -> Vec3
    {
        return {x + v.x, y + v.y, z + v.z};
    }

    constexpr auto operator+=(const Vec3 &v) noexcept -> Vec3 &
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    constexpr auto operator-(const Vec3 &v) const noexcept -> Vec3
    {
        return {x - v.x, y - v.y, z - v.z};
    }

    constexpr auto operator-=(const Vec3 &v) noexcept -> Vec3 &
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    friend auto operator*(float left, const Vec3 &right) -> Vec3
    {
        return {left * right.x, left * right.y, left * right.z};
    }

    constexpr auto operator*(float v) const noexcept -> Vec3
    {
        return {x * v, y * v, z * v};
    }

    constexpr auto operator*=(float v) noexcept -> Vec3 &
    {
        x *= v;
        y *= v;
        z *= v;
        return *this;
    }

    friend auto operator/(float left, const Vec3 &right) -> Vec3
    {
        return {left / right.x, left / right.y, left / right.z};
    }

    constexpr auto operator/(float v) const noexcept -> Vec3
    {
        const auto r = 1.0F / v;
        return {x * r, y * r, z * r};
    }

    constexpr auto operator/=(float v) noexcept -> Vec3 &
    {
        const auto r = 1.0 / v;
        x *= r;
        y *= r;
        z *= r;
        return *this;
    }

    constexpr auto operator*(const Vec3 &v) const noexcept -> Vec3
    {
        return {x * v.x, y * v.y, z * v.z};
    }

    constexpr auto operator*=(const Vec3 &v) noexcept -> Vec3 &
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    constexpr auto operator-() const noexcept -> Vec3 { return {-x, -y, -z}; }

    constexpr auto LengthSquared() const noexcept -> float
    {
        return dot(*this);
    }

    auto length() const noexcept -> float { return std::sqrt(LengthSquared()); }

    constexpr auto dot(const Vec3 &v) const noexcept -> float
    {
        return x * v.x + y * v.y + z * v.z;
    }

    constexpr auto cross(const Vec3 &v) const noexcept -> Vec3
    {
        auto cx = y * v.z - z * v.y;
        auto cy = z * v.x - x * v.z;
        auto cz = x * v.y - y * v.x;
        return {cx, cy, cz};
    }

    constexpr auto operator==(const Vec3 &v) const noexcept -> bool
    {
        return x == v.x && y == v.y && z == v.z;
    }

    constexpr auto operator!=(const Vec3 &v) const noexcept -> bool
    {
        return x != v.x || y != v.y || z != v.z;
    }

    inline auto normalized() const noexcept -> Vec3
    {

        return *this / length();
    }

    static auto random() noexcept -> Vec3
    {
        float inv_max = 1.0F / static_cast<float>(RAND_MAX);
        const std::array<float, 3> coordinates = {rand() * inv_max, rand() * inv_max, rand() * inv_max};
        return Vec3(coordinates);
    }

    friend auto operator<<(std::ostream &os, const Vec3 &v) -> std::ostream &
    {
        return os << '(' << v.x << ", " << v.y << ", " << v.z << ")";
    }
};
