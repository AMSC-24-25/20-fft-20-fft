#ifndef IMAGE_HANDLERS_HPP
#define IMAGE_HANDLERS_HPP

#include <string>
#include <vector>

/**
 * Image handling utilities for loading, converting to grayscale, and saving images.
 */
namespace sp::img
{
    /**
     * Load an image from the specified path.
     * @param path The file path to the image.
     * @param width Output parameter for the image width.
     * @param height Output parameter for the image height.
     * @param channels Output parameter for the number of channels in the image.
     * @return Pointer to the loaded image data (RGB or grayscale).
     * @throws std::runtime_error if the image cannot be loaded.
     */
    unsigned char* loadImage(const std::string& path, int& width, int& height, int& channels);

    /**
     * Convert image data to grayscale.
     * @param data Pointer to the image data.
     * @param width Width of the image.
     * @param height Height of the image.
     * @param channels Number of channels in the image (1 for grayscale, 3 for RGB).
     * @return Vector containing the grayscale pixel values.
     * @throws std::runtime_error if the number of channels is unsupported.
     */
    std::vector<unsigned char> toGrayscale(const unsigned char* data, int width, int height, int channels);

    /**
     * Save a grayscale image to the specified path.
     * @param path The file path to save the image.
     * @param gray Vector containing the grayscale pixel values.
     * @param width Width of the image.
     * @param height Height of the image.
     * @return True if the image was saved successfully, false otherwise.
     * @throws std::runtime_error if the image cannot be saved.
     */
    bool saveGrayscaleImage(const std::string& path, const std::vector<unsigned char>& gray, int width, int height);

    /**
     * Save an RGB image to the specified path.
     * @param path The file path to save the image.
     * @param rgb Vector containing the RGB pixel values.
     * @param width Width of the image.
     * @param height Height of the image.
     * @return True if the image was saved successfully, false otherwise.
     * @throws std::runtime_error if the image cannot be saved.
     */
    bool saveRGBImage(const std::string& path, const std::vector<unsigned char>& rgb, int width, int height);
}

#endif //IMAGE_HANDLERS_HPP
