#include <cassert>

#include "trace.hpp"
#include "brdf.hpp"
#include "light.hpp"
#include "math/constants.hpp"
#include "math/rng.hpp"
#include "math/vec3.hpp"
#include "ray.hpp"
#include "sampling.hpp"
#include "scene.hpp"

auto trace(const Scene &scene, const Ray &ray_from_camera, int max_bounces, RNG &rng) -> Vec3
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
        const Vec3 outgoing = -ray.direction;
        // Moving the hit point along normal because it might be
        // below the surface due to floating-point inaccuracies.
        const Vec3 hit_point = hit.point + hit.normal * math::EPSILON * 5;

        // Sample every light directly.
        for (const auto &light_variant : scene.lights)
        {
            Vec3 to_light;
            Vec3 light_intensity;

            if (std::holds_alternative<DirectionalLight>(light_variant))
            {
                auto directional_light = std::get<DirectionalLight>(light_variant);
                to_light = normalize(-directional_light.direction);
                light_intensity = intensity(directional_light);
            }
            else if (std::holds_alternative<PointLight>(light_variant))
            {
                auto point_light = std::get<PointLight>(light_variant);
                to_light = normalize(point_light.position - hit_point);
                light_intensity = intensity(point_light, hit_point);
            }
            else if (std::holds_alternative<SpotLight>(light_variant))
            {
                auto spot_light = std::get<SpotLight>(light_variant);
                to_light = normalize(spot_light.position - hit_point);
                light_intensity = intensity(spot_light, hit_point);
            }

            Brdf brdf = eval_brdf(hit.material, hit.normal, outgoing, to_light);
            if (brdf.probability == 0.0F)
                break;

            Vec3 light_contribution = (light_intensity * brdf.value * std::abs(dot(to_light, hit.normal)) * weight) / brdf.probability;

            // Calculating the shadow ray intersection is expensive
            // so we don't bother if the contribution would be low anyway.
            if (max(light_contribution) < 0.01F)
                continue;

            const Ray shadow_ray = {hit_point, to_light};
            if (intersect_shadow(scene, shadow_ray))
                continue;

            radiance += light_contribution;
        }

        radiance += hit.material.emission * weight;

        if (max(hit.material.emission) > 0.0F)
            break;

        Vec3 incoming = sample_direction(hit.material, hit.normal, outgoing, rng);
        Brdf brdf = eval_brdf(hit.material, hit.normal, outgoing, incoming);

        if (brdf.probability == 0.0F)
            break;

        weight *= brdf.value * std::abs(dot(hit.normal, incoming)) / brdf.probability;

        if (max(weight) < 0.01F)
            break;

        ray = {hit_point, incoming};
    };

    return radiance;
}
