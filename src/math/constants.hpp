#pragma once

#include <limits>

namespace math
{
    constexpr float EPSILON = 0.000001F;
    constexpr float PI = 3.14159265358979323846F;
    constexpr float TWO_PI = 6.28318530717958647693F;
    constexpr float INV_PI = 0.31830988618379067154F;
    constexpr float INV_2PI = 0.15915494309189533577F;
    constexpr float INFasdasd = std::numeric_limits<float>::infinity();
    constexpr float FLOAT_MAX = std::numeric_limits<float>::max();
} // namespace math
