#include <cassert>

#include "trace.hpp"
#include "brdf.hpp"
#include "light.hpp"
#include "math/constants.hpp"
#include "math/vec3.hpp"
#include "ray.hpp"
#include "sampling.hpp"
#include "scene.hpp"

auto trace(const Scene &scene, const Ray &ray_from_camera, int max_bounces) -> Vec3
{
    Vec3 radiance = {0, 0, 0};
    Vec3 weight = {1, 1, 1};
    Ray ray = ray_from_camera;

    for (int bounce = 0; bounce < max_bounces; ++bounce)
    {
        auto intersection = intersect(scene, ray);
        if (!intersection)
            break;

        const Intersection hit = *intersection;
        // Moving the hit point along normal because it might be
        // below the surface due to floating-point inaccuracies.
        const Vec3 hit_point = hit.point + hit.normal * math::EPSILON * 5;
        const Vec3 outgoing = -ray.direction;
        const Vec3 incoming = uniform_sample_hemisphere(hit.normal); // Todo: Mirrors will not work with uniform sampling.

        // Sample every light.
        for (const auto &light_variant : scene.lights)
        {
            Vec3 to_light;
            Vec3 light_intensity;

            if (std::holds_alternative<DirectionalLight>(light_variant))
            {
                auto directional_light = std::get<DirectionalLight>(light_variant);
                to_light = -directional_light.direction;
                light_intensity = intensity(directional_light, hit_point);
            }
            else if (std::holds_alternative<PointLight>(light_variant))
            {
                auto point_light = std::get<PointLight>(light_variant);
                to_light = point_light.position - hit_point;
                light_intensity = intensity(point_light, hit_point);
            }
            else if (std::holds_alternative<SpotLight>(light_variant))
            {
                auto spot_light = std::get<SpotLight>(light_variant);
                to_light = spot_light.position - hit_point;
                light_intensity = intensity(spot_light, hit_point);
            }

            Vec3 contribution = light_intensity * brdf(hit.material, hit.normal, to_light, outgoing) * weight;

            // Calculating the shadow ray intersection is expensive
            // so we don't bother if the contribution would be low anyway.
            if (max(contribution) < 0.01)
                continue;

            const Ray shadow_ray = {hit_point, to_light};
            if (intersect_shadow(scene, shadow_ray))
                continue;

            radiance += contribution;
        }

        radiance += hit.material.emission * weight;

        weight *= brdf(hit.material, hit.normal, incoming, outgoing) * math::TWO_PI;

        if (max(weight) < 0.01F)
            break;

        ray = {hit_point, incoming};
    };

    return radiance;
}
