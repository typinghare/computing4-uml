// Copyright 2024 James Chen

#ifndef NBODYCONSTANT_HPP
#define NBODYCONSTANT_HPP

#include <filesystem>
#include <string>

namespace NB {

// Use string literals directly for constexpr variables:
constexpr std::string_view WINDOW_TITLE = "N-Body Problem Simulation by James Chen";

// Window fixed height
constexpr unsigned WINDOW_WIDTH = 720;
constexpr unsigned WINDOW_HEIGHT = WINDOW_WIDTH;

// Use std::filesystem for path handling:
const std::filesystem::path ASSETS_DIR = "assets/";

// Images
const std::filesystem::path ASSETS_IMAGE_DIR = ASSETS_DIR;
const std::filesystem::path IMAGE_BACKGRROUND = ASSETS_IMAGE_DIR / "background.jpg";

// Sounds
const std::filesystem::path ASSETS_SOUND_DIR = ASSETS_DIR;
const std::filesystem::path SOUND_BACKGROUND_MUSIC = ASSETS_SOUND_DIR / "2001.wav";

}  // namespace NB

#endif
