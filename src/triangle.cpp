#include <ostream>

#include "triangle.hpp"

auto operator<<(std::ostream &os, const Triangle &t) -> std::ostream &
{
    return os << "Triangle {" << t.vertex0 << ", " << t.vertex0 + t.edge1 << ", " << t.vertex0 + t.edge2 << "}";
}
