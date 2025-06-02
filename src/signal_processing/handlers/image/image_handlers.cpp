#include <iostream>

#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

#include "image_handlers.hpp"

namespace sp::img
{
    unsigned char* loadImage(const std::string& path, int& width, int& height, int& channels) {
        unsigned char* data = stbi_load(
            path.c_str(),
            &width,
            &height,
            &channels,
            0
        );
        if (!data) {
            throw std::runtime_error("Failed to load image: " + path);
        }
        return data;
    }

    std::vector<unsigned char> toGrayscale(const unsigned char* data, int width, int height, int channels) {
        std::vector<unsigned char> gray(width * height);

        if (channels == 1) {
            // Already grayscale
            std::copy(data, data + width * height, gray.begin());
        } else if (channels >= 3) {
            for (int i = 0; i < width * height; ++i) {
                const int idx = i * channels;
                const unsigned char r = data[idx];
                const unsigned char g = data[idx + 1];
                const unsigned char b = data[idx + 2];
                gray[i] = static_cast<unsigned char>(0.299 * r + 0.587 * g + 0.114 * b);
            }
        } else {
            throw std::runtime_error("Unsupported number of channels: " + std::to_string(channels));
        }

        return gray;
    }

    bool saveGrayscaleImage(const std::string& path, const std::vector<unsigned char>& gray, int width, int height) {
        const int success = stbi_write_png(
            path.c_str(),
            width,
            height,
            1,
            gray.data(),
            width
        );
        if (!success) {
            throw std::runtime_error("Failed to save image: " + path);
        }
        return success;
    }

    bool saveRGBImage(const std::string& path, const std::vector<unsigned char>& rgb, int width, int height) {
        const int success = stbi_write_png(
            path.c_str(),
            width,
            height,
            3,
            rgb.data(),
            width * 3
        );
        if (!success) {
            throw std::runtime_error("Failed to save image: " + path);
        }
        return success;
    }
}

