#pragma once

#include "../libs/pcg/pcg_random.hpp"

#include "scene.hpp"
#include "ray.hpp"

auto trace(const Scene &scene, const Ray &ray, int max_bounces, pcg32 &rng) -> Vec3;
