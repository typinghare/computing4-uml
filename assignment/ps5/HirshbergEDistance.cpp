// Copyright 2024 James Chen

#include "HirshbergEDistance.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include "NeedlemanWunschEDistance.hpp"
#include "OptimizedEDistance.hpp"

HirshbergEDistance::HirshbergEDistance(const std::string& geneX, const std::string& geneY) :
    AbstractEDistance(geneX, geneY) {}

std::vector<int>
HirshbergEDistance::allYPrefixCosts(const std::string& geneX, const std::string& geneY) {
    auto reversedGeneX = geneX;
    auto reversedGeneY = geneY;
    std::reverse(reversedGeneX.begin(), reversedGeneX.end());
    std::reverse(reversedGeneY.begin(), reversedGeneY.end());
    OptimizedEDistance optimizedEDistance{ reversedGeneX, reversedGeneY };
    optimizedEDistance.optDistance();
    auto row = optimizedEDistance.row();

    // Also reverse the row
    std::reverse(row.begin(), row.end());

    return row;
}

std::vector<int>
HirshbergEDistance::allYSuffixCosts(const std::string& geneX, const std::string& geneY) {
    OptimizedEDistance optimizedEDistance{ geneX, geneY };
    optimizedEDistance.optDistance();

    return optimizedEDistance.row();
}

void HirshbergEDistance::align(
    const std::string& geneX, const std::string& geneY, const std::pair<size_t, size_t>& offset) {
    const auto xLength = geneX.length();
    const auto yLength = geneY.length();
    const auto xHalfLength = xLength / 2;

    if (xLength <= 2 || yLength <= 2) {
        // Use standard alignment
        NeedlemanWunschEDistance eDistance{ geneX, geneY };
        eDistance.optDistance();
        const auto* const matrix = eDistance.matrix();

        size_t i = 0;
        size_t j = 0;
        while (i + j < xLength + yLength) {
            insertArrowPathCoordinate({ i + offset.first, j + offset.second });
            const auto val = matrix[i][j];
            const auto isFromBottom = i < xLength && val - 2 == matrix[i + 1][j];
            const auto isFromRight = j < yLength && val - 2 == matrix[i][j + 1];
            i += static_cast<int>(!isFromRight);
            j += static_cast<int>(!isFromBottom);
        }

        return;
    }

    const auto xHalf1 = geneX.substr(0, xHalfLength);
    const auto xHalf2 = geneX.substr(xHalfLength);
    const auto yPrefix = allYPrefixCosts(xHalf1, geneY);
    const auto ySuffix = allYSuffixCosts(xHalf2, geneY);

    int bestCost = static_cast<int>(xLength + yLength) << 1;
    size_t bestQ = 0;
    for (size_t q = 0; q < yLength; ++q) {
        const auto costSum = yPrefix[q] + ySuffix[q];
        if (costSum < bestCost) {
            bestCost = costSum;
            bestQ = q;
        }
    }

    // Add the coordinate to the arrow path
    const std::pair coordinate = { xHalfLength + offset.first, bestQ + offset.second };
    insertArrowPathCoordinate(coordinate);

    // Recursively find the other coordinates of the arrow path
    align(xHalf1, geneY.substr(0, bestQ), offset);
    align(xHalf2, geneY.substr(bestQ), coordinate);
}

int HirshbergEDistance::optDistance() {
    static constexpr std::pair<size_t, size_t> ZERO_PAIR(0, 0);

    // Initialize arrowPath
    const auto xLength = m_geneX.length();
    const auto yLength = m_geneY.length();
    for (size_t i = 0; i < xLength + yLength; ++i) {
        arrowPath.emplace_back(0, 0);
    }
    arrowPath.emplace_back(xLength, yLength);

    // Align the two genes
    align(m_geneX, m_geneY, { 0, 0 });

    // Find the total cost according to the arrow path
    const auto length = m_geneX.length() + m_geneY.length();
    int totalCost = 0;
    for (size_t i = 0; i < length; ++i) {
        if (arrowPath[i] == ZERO_PAIR && i != 0) {
            continue;
        }

        if (arrowPath[i + 1] != ZERO_PAIR) {
            // Either from right or from bottom
            totalCost += 2;
            // From diagonal
        } else {
            const auto xChar = m_geneX.at(arrowPath[i].first);
            const auto yChar = m_geneY.at(arrowPath[i].second);
            totalCost += penalty(xChar, yChar);
        }
    }

    return totalCost;
}

std::string HirshbergEDistance::alignment() const {
    static constexpr auto CHAR_GAP = '-';
    static constexpr auto CHAR_SPACE = ' ';
    static constexpr std::pair<size_t, size_t> ZERO_PAIR(0, 0);

    const auto xLength = m_geneX.length();
    const auto yLength = m_geneY.length();
    std::ostringstream ostringstream;
    for (size_t i = 0; i < m_geneX.length() + m_geneY.length(); ++i) {
        if (arrowPath[i] == ZERO_PAIR && i != 0) {
            continue;
        }

        const auto [xIndex, yIndex] = arrowPath[i];
        const auto isFromRightOrBottom = arrowPath[i + 1] != ZERO_PAIR;
        const auto isFromRight =
            isFromRightOrBottom && arrowPath[i].first == arrowPath[i + 1].first;
        const auto isFromBottom = isFromRightOrBottom && !isFromRight;
        const auto xChar = xIndex < xLength && !isFromRight ? m_geneX.at(xIndex) : CHAR_GAP;
        const auto yChar = yIndex < yLength && !isFromBottom ? m_geneY.at(yIndex) : CHAR_GAP;
        const auto cost = isFromRightOrBottom ? 2 : penalty(xChar, yChar);

        // Output: "<xChar> <yChar> <cost>\n"
        ostringstream << xChar << CHAR_SPACE << yChar << CHAR_SPACE << cost << std::endl;
    }

    return ostringstream.str();
}

void HirshbergEDistance::insertArrowPathCoordinate(const std::pair<size_t, size_t>& coordinate) {
    arrowPath[coordinate.first + coordinate.second] = coordinate;
}
