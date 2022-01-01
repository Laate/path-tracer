#include <cassert>

#include "trace.hpp"
#include "brdf.hpp"
#include "hit.hpp"
#include "math/constants.hpp"
#include "math/vec3.hpp"
#include "ray.hpp"
#include "sampling.hpp"
#include "scene.hpp"

auto trace(const Scene &scene, const Ray &ray_from_camera, int bounces) -> Vec3
{
    Vec3 radiance = {0, 0, 0};
    Vec3 weight = {1, 1, 1};
    Ray ray = ray_from_camera;

    for (int bounce = 0; bounce < bounces; ++bounce)
    {
        Hit hit = Hit();
        if (!intersect(scene, ray, hit))
            break;

        radiance += hit.material.emission * weight;

        Vec3 outgoing = -ray.direction;
        Vec3 incoming = uniform_sample_hemisphere(hit.normal); // Todo: Mirrors will not work with uniform sampling.

        weight *= brdf(hit.material, hit.normal, incoming, outgoing) * math::TWO_PI;

        // Move origin along normal because the hit point might be
        // below the surface due to floating-point inaccuracies.
        ray = {hit.point + hit.normal * math::EPSILON * 5, incoming};
    };

    return radiance;
}
