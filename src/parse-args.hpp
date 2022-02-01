#pragma once

#include <omp.h>

#include <string>
#include <vector>

struct Args
{
    std::string input_path = "../models/cornell-box.gltf";
    std::string output_path = "output.png";
    int width = 640;
    int height = 480;
    int samples = 10;
    int max_bounces = 10;
    int thread_count = omp_get_max_threads();
};

auto parse_args(const std::vector<std::string> &arguments, Args &options) -> bool;
