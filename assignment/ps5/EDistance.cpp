// Copyright 2024 James Chen#include "EDistance.hpp"#include <algorithm>#include <iostream>#include <sstream>#include <string>#include "NeedlemanWunschEDistance.hpp"#include "OptimizedEDistance.hpp"int EDistance::penalty(const char& num1, const char& num2) {    return AbstractEDistance::penalty(num1, num2);}int EDistance::min3(const int& a, const int& b, const int& c) {    return AbstractEDistance::min3(a, b, c);}EDistance::EDistance(const std::string& geneX, const std::string& geneY) :    AbstractEDistance(geneX, geneY) {}int EDistance::optDistance() {    // Initialize arrowPath    const auto xLength = m_geneX.length();    const auto yLength = m_geneY.length();    for (size_t i = 0; i < xLength + yLength; ++i) {        arrowPath.emplace_back(0, 0);    }    arrowPath.emplace_back(xLength, yLength);    // Align the two genes    align(m_geneX, m_geneY, { 0, 0 });    return OptimizedEDistance(m_geneX, m_geneY).optDistance();}std::string EDistance::alignment() const {    std::ostringstream ostringstream;    for (const auto& [first, second] : arrowPath) {        ostringstream << "(" << first << ", " << second << ") ";    }    return ostringstream.str();}void EDistance::align(    const std::string& geneX, const std::string& geneY, const std::pair<size_t, size_t>& offset) {    const auto xLength = geneX.length();    const auto yLength = geneY.length();    const auto xHalfLength = xLength / 2;    std::cout << xLength << ", " << yLength << std::endl;    if (xLength <= 2 || yLength <= 2) {        // Use standard alignment        NeedlemanWunschEDistance eDistance{ geneX, geneY };        eDistance.optDistance();        const auto* const matrix = eDistance.getMatrix();        size_t i = 0;        size_t j = 0;        while (i + j < xLength + yLength) {            insertArrowPathCoordinate({ i + offset.first, j + offset.second });            const auto val = matrix[i][j];            const auto isFromBottom = i < xLength && val - 2 == matrix[i + 1][j];            const auto isFromRight = j < yLength && val - 2 == matrix[i][j + 1];            i += static_cast<int>(not isFromRight);            j += static_cast<int>(not isFromBottom);        }        return;    }    const auto xHalf1 = geneX.substr(0, xHalfLength);    const auto xHalf2 = geneX.substr(xHalfLength);    size_t bestCost = std::max(xLength, yLength) << 1;    size_t bestQ = 0;    for (size_t q = 0; q < yLength; ++q) {        const auto cost1 = OptimizedEDistance(xHalf1, geneY.substr(0, q)).optDistance();        const auto cost2 = OptimizedEDistance(xHalf2, geneY.substr(q)).optDistance();        const auto costSum = cost1 + cost2;        if (costSum < bestCost) {            bestQ = q;            bestCost = costSum;        }    }    // Add the coordinate to the arrow path    const std::pair coordinate = { xHalfLength + offset.first, bestQ + offset.second };    insertArrowPathCoordinate(coordinate);    // Recursively find the other coordinates of the arrow path    align(xHalf1, geneY.substr(0, bestQ), offset);    align(xHalf2, geneY.substr(bestQ), coordinate);}void EDistance::insertArrowPathCoordinate(const std::pair<size_t, size_t>& coordinate) {    arrowPath[coordinate.first + coordinate.second] = coordinate;}