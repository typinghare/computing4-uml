// Copyright 2024 James Chen
#include "PhotoMagic.hpp"
#include <climits>
#include <string>

namespace PhotoMagic {

void transform(sf::Image& image, FibLFSR* fibLfsr) {
    const sf::Vector2u size = image.getSize();
    for (unsigned int row = 0; row < size.y; ++row) {
        for (unsigned int col = 0; col < size.x; ++col) {
            const int t_r = fibLfsr->generate(8);
            const int t_g = fibLfsr->generate(8);
            const int t_b = fibLfsr->generate(8);

            sf::Color pixel = image.getPixel(col, row);
            pixel.r ^= t_r;
            pixel.g ^= t_g;
            pixel.b ^= t_b;

            image.setPixel(col, row, pixel);
        }
    }
}

std::string convertPasswordToSeed(const std::string& password) {
    constexpr size_t SEED_LENGTH = 16;
    unsigned int ans = 0;

    for (size_t i = 0; i < password.length(); ++i) {
        unsigned int t = static_cast<unsigned char>(password[i]);
        t <<= i % SEED_LENGTH;
        ans ^= t;
    }

    std::string seed;
    seed.reserve(SEED_LENGTH);

    for (size_t i = 0; i < SEED_LENGTH; ++i) {
        const bool isBitSet =
            (ans & (1u << i)) != 0 && (ans & (1u << (i + SEED_LENGTH)));
        seed.push_back(isBitSet ? '1' : '0');
    }

    return seed;
}

SpriteTexture::SpriteTexture(const sf::Image& image) {
    texture = std::make_shared<sf::Texture>();
    texture->loadFromImage(image);
    sprite = std::make_shared<sf::Sprite>(*texture);
}

void displayImages(sf::Image& inputImage, sf::Image& outputImage) {
    static constexpr unsigned WINDOW_FPS = 60;

    // Get the size of two images
    const auto inputImageSize = inputImage.getSize();
    const auto outputImageSize = outputImage.getSize();

    // Create two windows for two images respectively
    sf::RenderWindow inputImageWindow(
        sf::VideoMode(inputImageSize.x, inputImageSize.y), "Input Image");
    sf::RenderWindow outputImageWindow(
        sf::VideoMode(outputImageSize.x, outputImageSize.y), "Output Image");
    inputImageWindow.setFramerateLimit(WINDOW_FPS);
    outputImageWindow.setFramerateLimit(WINDOW_FPS);

    const SpriteTexture inputSpriteTexture(inputImage);
    const SpriteTexture outputSpriteTexture(outputImage);

    // Render images and display windows
    inputImageWindow.clear();
    inputImageWindow.draw(*inputSpriteTexture.sprite);
    inputImageWindow.display();
    outputImageWindow.clear();
    outputImageWindow.draw(*outputSpriteTexture.sprite);
    outputImageWindow.display();

    while (inputImageWindow.isOpen() && outputImageWindow.isOpen()) {
        sf::Event event{};
        while (inputImageWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                inputImageWindow.close();
        }

        while (outputImageWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                outputImageWindow.close();
        }
    }
}

}  // namespace PhotoMagic