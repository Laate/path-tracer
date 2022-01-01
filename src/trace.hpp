#pragma once

#include "scene.hpp"
#include "ray.hpp"

auto trace(const Scene &scene, const Ray &ray, int bounces) -> Vec3;
