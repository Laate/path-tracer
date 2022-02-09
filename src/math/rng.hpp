#pragma once

#include <random>

#include "../../libs/pcg/pcg_random.hpp"

struct RNG
{
    std::uniform_real_distribution<float> distribution = std::uniform_real_distribution<float>(0.0F, 1.0F);
    pcg32 engine;

    explicit RNG(int seed) : engine(pcg32(seed)) {}
};

inline auto zero_to_one(RNG &rng) -> float
{
    return rng.distribution(rng.engine);
}
