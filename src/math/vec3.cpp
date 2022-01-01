#include <ostream>

#include "vec3.hpp"

auto operator<<(std::ostream &os, const Vec3 &a) -> std::ostream &
{
    return os << '(' << a.x << ", " << a.y << ", " << a.z << ")";
}
