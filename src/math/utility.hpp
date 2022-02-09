#pragma once

#include <algorithm>

// Hermite interpolation between two values.
// start < end
inline auto smooth_step(float start, float end, float x) -> float
{
    float t = std::clamp((x - start) / (end - start), 0.0F, 1.0F);
    return t * t * (3.0F - 2.0F * t);
}
