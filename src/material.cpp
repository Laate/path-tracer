#include <ostream>

#include "material.hpp"

auto operator<<(std::ostream &os, const Material &mat) -> std::ostream &
{
    return os << "Material {emission = " << mat.emission
              << "\n\tbase_color = " << mat.base_color
              << "\n\troughness = " << mat.roughness
              << "\n\tmetallic = " << mat.metallic
              << " }";
}
