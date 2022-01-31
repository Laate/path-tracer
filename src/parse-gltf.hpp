#pragma once

#include <string>

#include "scene.hpp"

namespace gltf
{
    auto load_gltf(const std::string &file_path, Scene &scene) -> bool;
} // namespace gltf
