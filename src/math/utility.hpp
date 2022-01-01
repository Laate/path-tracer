#pragma once

#include <algorithm>

namespace math
{
    inline auto clamp(float n, float lower, float upper) -> float
    {
        return std::max(lower, std::min(n, upper));
    }
} // namespace math
