#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../libs/stb_image_write.h"

#include "camera.hpp"
#include "math/utility.hpp"
#include "scene.hpp"
#include "trace.hpp"

auto main(int argc, char **argv) -> int
{
    std::vector<std::string> arguments(argv + 1, argv + argc);

    constexpr int samples_per_pixel = 64;
    constexpr float reciprocal = 1.0F / samples_per_pixel;
    constexpr int color_channels = 3;
    constexpr int width = 720;
    constexpr int height = 576;
    constexpr float aspect_ratio = static_cast<float>(width) / height;

    std::vector<uint8_t> pixels(height * width * color_channels);
    Scene test_scene = scenes::indirect_illumination();

    auto start_time = std::chrono::system_clock::now();
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
                Ray ray = test_scene.camera.get_ray(img_plane_x, img_plane_y);
                pixel_color += trace(test_scene, ray, 10) * reciprocal;
            }
            for (int c = 0; c < 3; ++c)
            {
                pixels[y * width * color_channels + x * color_channels + c] += math::clamp(pixel_color[c], 0.0F, 1.0F) * 255;
            }
        }
        // Progress bar
        int percent = ceil(static_cast<float>(100 * y) / height);
        std::cout << "\033[2K\r"
                  << "[" << std::string(percent / 2, '=')
                  << std::string(50 - (percent) / 2, ' ') << "] " << percent
                  << "%" << std::flush;
    }
    auto end_time = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    std::cout
        << "\nRendering complete\n"
        << "Width " << width << ", "
        << "Height " << height << "\n"
        << samples_per_pixel << " samples per pixel\n"
        << "Time: " << elapsed << " ms\n"
        << static_cast<float>(elapsed) / (height * width * samples_per_pixel)
        << " ms per iteration"
        << std::endl;

    stbi_write_png("output.png", width, height, color_channels, pixels.data(), width * color_channels);
}
