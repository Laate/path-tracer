#pragma once

#include "math/rng.hpp"
#include "ray.hpp"
#include "scene.hpp"

auto trace(const Scene &scene, const Ray &ray, int max_bounces, RNG &rng) -> Vec3;
