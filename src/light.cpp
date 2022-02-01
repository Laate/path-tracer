#include <ostream>

#include "light.hpp"

auto operator<<(std::ostream &os, const DirectionalLight &light) -> std::ostream &
{
    return os << "DirectionalLight {"
              << "\n\tradiance = " << light.radiance
              << "\n\tdirection = " << light.direction << "}";
}

auto operator<<(std::ostream &os, const PointLight &light) -> std::ostream &
{
    return os << "PointLight {"
              << "\n\tradiance = " << light.radiance
              << "\n\tposition = " << light.position << "}";
}

auto operator<<(std::ostream &os, const SpotLight &light) -> std::ostream &
{
    return os << "SpotLight {"
              << "\n\tradiance = " << light.radiance
              << "\n\tposition = " << light.position
              << "\n\tdirection = " << light.direction
              << "\n\tangle_scale = " << light.angle_scale
              << "\n\tangle_offset = " << light.angle_offset << "}";
}
