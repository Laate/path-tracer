#pragma once

#include <cassert>
#include <cmath>
#include <ostream>

struct Vec3
{
    float x{}, y{}, z{};
    constexpr Vec3() noexcept = default;
    constexpr Vec3(float x, float y, float z) noexcept : x(x), y(y), z(z) {}
    constexpr auto operator[](size_t i) -> float &;
    constexpr auto operator[](size_t i) const -> const float &;
};

constexpr auto Vec3::operator[](size_t i) -> float &
{
    assert(i < 3);
    return (&x)[i];
}
constexpr auto Vec3::operator[](size_t i) const -> const float &
{
    assert(i < 3);
    return (&x)[i];
}
constexpr auto operator+(const Vec3 &a, const Vec3 &b) noexcept -> Vec3 { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
constexpr auto operator+=(Vec3 &a, const Vec3 &b) noexcept -> Vec3 & { return a = a + b; }
constexpr auto operator-(const Vec3 &a, const Vec3 &b) noexcept -> Vec3 { return {a.x - b.x, a.y - b.y, a.z - b.z}; }
constexpr auto operator-(float a, const Vec3 &b) noexcept -> Vec3 { return {a - b.x, a - b.y, a - b.z}; }
constexpr auto operator-=(Vec3 &a, const Vec3 &b) noexcept -> Vec3 & { return a = a - b; }
constexpr auto operator*(float a, const Vec3 &b) noexcept -> Vec3 { return {a * b.x, a * b.y, a * b.z}; }
constexpr auto operator*(const Vec3 &a, float b) noexcept -> Vec3 { return {a.x * b, a.y * b, a.z * b}; }
constexpr auto operator*=(Vec3 &a, float b) noexcept -> Vec3 & { return a = a * b; }
constexpr auto operator/(float a, const Vec3 &b) -> Vec3 { return {a / b.x, a / b.y, a / b.z}; }
constexpr auto operator/(const Vec3 &a, float b) noexcept -> Vec3 { return a * (1.0F / b); }
constexpr auto operator/=(Vec3 &a, float b) noexcept -> Vec3 & { return a = a / b; }
constexpr auto operator*(const Vec3 &a, const Vec3 &b) noexcept -> Vec3 { return {a.x * b.x, a.y * b.y, a.z * b.z}; }
constexpr auto operator*=(Vec3 &a, const Vec3 &b) noexcept -> Vec3 & { return a = a * b; }
constexpr auto operator-(const Vec3 &a) noexcept -> Vec3 { return {-a.x, -a.y, -a.z}; }
constexpr auto dot(const Vec3 &a, const Vec3 &b) noexcept -> float { return a.x * b.x + a.y * b.y + a.z * b.z; }
constexpr auto length_squared(const Vec3 &a) noexcept -> float { return dot(a, a); }
constexpr auto length(const Vec3 &a) noexcept -> float { return std::sqrt(length_squared(a)); }
constexpr auto cross(const Vec3 &a, const Vec3 &b) noexcept -> Vec3 { return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x}; }
constexpr auto hadamard(const Vec3 &a, const Vec3 &b) noexcept -> Vec3 { return {a.x * b.x, a.y * b.y, a.z * b.z}; }
constexpr auto operator==(const Vec3 &a, const Vec3 &b) noexcept -> bool { return a.x == b.x && a.y == b.y && a.z == b.z; }
constexpr auto operator!=(const Vec3 &a, const Vec3 &b) noexcept -> bool { return a.x != b.x || a.y != b.y || a.z != b.z; }
constexpr auto normalize(const Vec3 &a) noexcept -> Vec3 { return a / length(a); }
constexpr auto lerp(const Vec3 &a, const Vec3 &b, float u) noexcept -> Vec3 { return a * (1.0F - u) + b * u; }
auto operator<<(std::ostream &os, const Vec3 &a) -> std::ostream &;
