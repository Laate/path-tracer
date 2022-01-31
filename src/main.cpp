#include <chrono>
#include <cstdint>
#include <iostream>
#include <omp.h>
#include <valarray>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../libs/stb_image_write.h"

#include "camera.hpp"
#include "math/utility.hpp"
#include "parse-args.hpp"
#include "parse-gltf.hpp"
#include "scene.hpp"
#include "trace.hpp"

auto main(int argc, char **argv) -> int
{
    std::vector<std::string> input_arguments(argv + 1, argv + argc);
    auto options = Args{};
    if (!parse_args(input_arguments, options))
    {
        exit(1);
    }
    omp_set_num_threads(options.thread_count);
    int samples_per_pixel = options.samples;
    float sample_weight = 1.0F / samples_per_pixel;
    int max_bounces = options.max_bounces;
    int width = options.width;
    int height = options.height;
    constexpr int color_channels = 3;
    float aspect_ratio = static_cast<float>(width) / height;

    std::vector<uint8_t> pixels(height * width * color_channels);

    auto scene = Scene();
    if (!gltf::load_gltf(options.input_path, scene))
    {
        std::cerr << "Loading scene from " << options.input_path << " failed." << std::endl;
        exit(1);
    }

    // Progress bar
    std::valarray<unsigned> progress_counter(omp_get_max_threads());
    auto print_progress = [&progress_counter, height]()
    {
        int percent = 100.0f * progress_counter.sum() / height;
        std::cout << "\033[2K\r"
                  << "[" << std::string(percent / 2, '=')
                  << std::string(50 - (percent) / 2, ' ') << "] " << percent
                  << "%, " << omp_get_max_threads() << " threads" << std::flush;
    };

    auto start_time = std::chrono::system_clock::now();
#pragma omp parallel for schedule(static, 1)
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            // Transform the pixel coordinates to image plane [-1, 1]^2 coordinates
            float img_plane_x = (2 * (static_cast<float>(x) / width) - 1) * aspect_ratio;
            float img_plane_y = -2 * (static_cast<float>(y) / height) + 1;
            Vec3 pixel_color = Vec3(0.0F, 0.0F, 0.0F);
            for (int i = 0; i < samples_per_pixel; ++i)
            {
                Ray ray = scene.camera.get_ray(img_plane_x, img_plane_y);
                pixel_color += trace(scene, ray, max_bounces) * sample_weight;
            }
            for (int c = 0; c < 3; ++c)
            {
                pixels[y * width * color_channels + x * color_channels + c] += math::clamp(pixel_color[c], 0.0F, 1.0F) * 255;
            }
        }
        ++progress_counter[omp_get_thread_num()];
        if (omp_get_thread_num() == 0)
            print_progress();
    }
    print_progress();

    auto end_time = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    std::cout << "\nRendering complete\n"
              << "Width: " << width << ", Height: " << height << "\n"
              << samples_per_pixel << " samples per pixel\n"
              << "Time: " << static_cast<float>(elapsed) / 1e6 << " s\n"
              << "Mean: " << static_cast<float>(elapsed) / (height * width * samples_per_pixel) << " µs per iteration"
              << std::endl;

    if (stbi_write_png("output.png", width, height, color_channels, pixels.data(), width * color_channels))
        std::cout << "Image saved to " << options.output_path << std::endl;
    else
        std::cerr << "Failed to save image to " << options.output_path << std::endl;
}
