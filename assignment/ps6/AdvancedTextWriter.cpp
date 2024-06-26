// Copyright 2024 James Chen

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "WordWriter.hpp"

/**
 * @brief Starts the universe simulation.
 * @param argc The number of arguments.
 * @param argv The arguments vector. This program requires two arguments:
 * 1. k (int): The order of the Markov model.
 * 2. L (int): The length of the generated text.
 */
int main(const int argc, const char* argv[]) {
    if (argc != 3) {
        std::cerr << "Too many or too few arguments!" << std::endl;
        return 1;
    }

    // Arguments
    const auto orderK = std::stoi(argv[1]);
    const auto generatedTextLength = std::stoi(argv[2]);

    // Read the input text from standard input
    std::string line{ "0" };
    std::ostringstream textStream;
    while (!line.empty() && !std::cin.eof()) {
        getline(std::cin, line);
        textStream << line << " ";
    }
    const std::string text = textStream.str();

    // Get the first kgram
    std::vector<std::string> kgram;
    std::istringstream ss(text);
    std::string token;
    std::vector<std::string> token_vector;
    for (int i = 0; i < orderK; ++i) {
        ss >> token;
        kgram.push_back(token);
    }

    // Create a RandWriter instance and generate a text
    WordWriter word_writer{ text, static_cast<size_t>(orderK) };
    const auto generatedText = word_writer.generate(kgram, generatedTextLength);
    std::cout << generatedText;

    return 0;
}
