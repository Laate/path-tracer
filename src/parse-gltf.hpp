#pragma once

#include <string>

#include "scene.hpp"

namespace gltf
{
    auto load_gltf(const std::string &filename, Scene &scene) -> bool;
} // namespace gltf
