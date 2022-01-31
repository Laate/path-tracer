#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "parse-args.hpp"

auto parse_args(const std::vector<std::string> &arguments, Args &options) -> bool
{
    int N = arguments.size();
    int i = 0;
    while (i < N)
    {
        const auto &arg = arguments[i];
        if (arg == "--help" || arg == "-h")
        {
            std::string help_message =
                std::string("List of possible options:\n") +
                "-i, --input        path to glTF file.\n" +
                "-o, --output       path to save rendered image.\n" +
                "-r, --resolution   width and height of rendered image.\n" +
                "-s, --samples      samples per pixel.\n" +
                "-b, --max-bounces  maximum number of ray bounces.\n" +
                "-t, --threads      number of threads to use.\n"
                "-h, --help         display this message.";
            std::cout << help_message << std::endl;
            return false;
        }
        if (arg == "--input" || arg == "-i")
        {
            if (i >= N - 1)
            {
                std::cerr << "Missing parameter for " << arg << " <path to glTF file>" << std::endl;
                return false;
            }
            options.input_path = arguments[++i];
        }
        else if (arg == "--output" || arg == "-o")
        {
            if (i >= N - 1)
            {
                std::cerr << "Missing parameter for " << arg << " <path to save image>" << std::endl;
                return false;
            }
            options.output_path = arguments[++i];
        }
        else if (arg == "--resolution" || arg == "-r")
        {
            if (i >= N - 2)
            {
                std::cerr << "Expected two parameters for " << arg << " <width> <height>" << std::endl;
                return false;
            }
            options.width = std::max(stoi(arguments[++i]), 1);
            options.height = std::max(stoi(arguments[++i]), 1);
        }
        else if (arg == "--samples" || arg == "-s")
        {
            if (i >= N - 1)
            {
                std::cerr << "Missing parameter for " << arg << " <number of samples per pixel>" << std::endl;
                return false;
            }
            options.samples = std::max(stoi(arguments[++i]), 1);
        }
        else if (arg == "--max-bounces" || arg == "-b")
        {
            if (i >= N - 1)
            {
                std::cerr << "Missing parameter for " << arg << " <maximum number of bounces>" << std::endl;
                return false;
            }
            options.max_bounces = std::max(stoi(arguments[++i]), 1);
        }
        else if (arg == "--threads" || arg == "-t")
        {
            if (i >= N - 1)
            {
                std::cerr << "Missing parameter for " << arg << " <number of threads to use>" << std::endl;
                return false;
            }
            options.thread_count = std::max(stoi(arguments[++i]), 1);
        }
        else
        {
            std::cerr << "Unknown option: " << arg
                      << "\nTry --help for more information." << std::endl;
            return false;
        }
        ++i;
    }
    return true;
}
