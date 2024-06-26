// Copyright 2024 James Chen

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <boost/test/unit_test.hpp>
#include "Universe.hpp"

constexpr float MASS_MAX_TOLERANCE = 0.1;
constexpr float STEP_TOLERANCE = 0.1;

// Tests if `Universe::getNumPlanets()` and `Universe::getRadius` work correcly.
BOOST_AUTO_TEST_CASE(testUniverseBasic) {
    const NB::Universe universe{ "assets/1body.txt" };

    constexpr auto EXPECTED_NUM_PLANETS = 1;
    constexpr auto EXPECTED_RADIUS = 100.0;
    BOOST_REQUIRE_EQUAL(universe.numPlanets(), EXPECTED_NUM_PLANETS);
    BOOST_REQUIRE_EQUAL(universe.radius(), EXPECTED_RADIUS);
}

// Tests if `Universe::getNumPlanets()` and `Universe::getRadius` work correcly.
BOOST_AUTO_TEST_CASE(testUniverseBasic2) {
    const NB::Universe universe{ "assets/binary.txt" };

    constexpr auto EXPECTED_NUM_PLANETS = 2;
    constexpr auto EXPECTED_RADIUS = 5.0e10;
    BOOST_REQUIRE_EQUAL(universe.numPlanets(), EXPECTED_NUM_PLANETS);
    BOOST_REQUIRE_EQUAL(universe.radius(), EXPECTED_RADIUS);
}

// Tests if `CelestialBody::getNumPlanets()`, `CelestialBody::getRadius()` and
// `CelestialBody::getMass()` work correcly.
BOOST_AUTO_TEST_CASE(testCelestialBodyBasic) {
    const NB::Universe universe{ "assets/planets.txt" };
    const auto celestialBody = universe[0];

    constexpr float EXPECTED_POSITION_X = 1.4960e+11;
    constexpr float EXPECTED_POSITION_Y = 0.0000e+00;
    constexpr float EXPECTED_VELOCITY_X = 0.0000e+00;
    constexpr float EXPECTED_VELOCITY_Y = 2.9800e+04;
    constexpr float EXPECTED_MASS = 5.9740e+24;
    BOOST_REQUIRE_EQUAL(celestialBody.position().x, EXPECTED_POSITION_X);
    BOOST_REQUIRE_EQUAL(celestialBody.position().y, EXPECTED_POSITION_Y);
    BOOST_REQUIRE_EQUAL(celestialBody.velocity().x, EXPECTED_VELOCITY_X);
    BOOST_REQUIRE_EQUAL(celestialBody.velocity().y, EXPECTED_VELOCITY_Y);
    BOOST_REQUIRE_CLOSE(celestialBody.mass(), EXPECTED_MASS, MASS_MAX_TOLERANCE);
}

// Tests if `Universe::operator[]` works for the non-first elements.
BOOST_AUTO_TEST_CASE(testUniverseBracketOperator1) {
    const NB::Universe universe{ "assets/3body.txt" };
    const auto celestialBody = universe[1];

    constexpr float EXPECTED_POSITION_X = 0.0;
    constexpr float EXPECTED_POSITION_Y = 4.50e10;
    constexpr float EXPECTED_VELOCITY_X = 3.00e04;
    constexpr float EXPECTED_VELOCITY_Y = 0.0e00;
    constexpr float EXPECTED_MASS = 1.989e30;
    BOOST_REQUIRE_EQUAL(celestialBody.position().x, EXPECTED_POSITION_X);
    BOOST_REQUIRE_EQUAL(celestialBody.position().y, EXPECTED_POSITION_Y);
    BOOST_REQUIRE_EQUAL(celestialBody.velocity().x, EXPECTED_VELOCITY_X);
    BOOST_REQUIRE_EQUAL(celestialBody.velocity().y, EXPECTED_VELOCITY_Y);
    BOOST_REQUIRE_CLOSE(celestialBody.mass(), EXPECTED_MASS, MASS_MAX_TOLERANCE);
}

// Tests if `Universe::operator[]` works for the last elements.
BOOST_AUTO_TEST_CASE(testUniverseBracketOperator3) {
    const NB::Universe universe{ "assets/8star-rotation.txt" };
    const auto celestialBody = universe[universe.numPlanets() - 1];

    constexpr float EXPECTED_POSITION_X = -13.125e10;
    constexpr float EXPECTED_POSITION_Y = 0;
    constexpr float EXPECTED_VELOCITY_X = 0;
    constexpr float EXPECTED_VELOCITY_Y = 81e3;
    constexpr float EXPECTED_MASS = 5e29;
    BOOST_REQUIRE_EQUAL(celestialBody.position().x, EXPECTED_POSITION_X);
    BOOST_REQUIRE_EQUAL(celestialBody.position().y, EXPECTED_POSITION_Y);
    BOOST_REQUIRE_EQUAL(celestialBody.velocity().x, EXPECTED_VELOCITY_X);
    BOOST_REQUIRE_EQUAL(celestialBody.velocity().y, EXPECTED_VELOCITY_Y);
    BOOST_REQUIRE_CLOSE(celestialBody.mass(), EXPECTED_MASS, MASS_MAX_TOLERANCE);
}

// Tests if "CelestialBody::operator>>" and "CelestialBody::operator<<" works correctly.
BOOST_AUTO_TEST_CASE(testCelestialBodyOutput) {
    std::ifstream fileStream{ "assets/uniform8.txt" };
    std::string line;
    getline(fileStream, line);
    getline(fileStream, line);
    getline(fileStream, line);
    getline(fileStream, line);
    std::istringstream istringstream{ line };
    NB::CelestialBody celestialBody;
    istringstream >> celestialBody;

    std::ostringstream ostringstream;
    ostringstream << celestialBody;

    // original line:
    // 3.535534e+08  3.535534e+08  1.934345e+02 -1.934345e+02 2.00e+23 earth.gif
    const std::string EXPECT_LINE = "3.53553e+08 3.53553e+08 193.435 -193.435 2e+23 earth.gif";
    BOOST_REQUIRE_EQUAL(ostringstream.str(), EXPECT_LINE);
}

// Tests if `Universe::step()` works correctly by performing one step.
BOOST_AUTO_TEST_CASE(testUniverseStep1) {
    NB::Universe universe{ "assets/planets.txt" };
    constexpr double DELTA_TIME = 25000.0;
    universe.step(DELTA_TIME);

    constexpr float EXPECTED_POSITION_X = 1.4960e+11F;
    constexpr float EXPECTED_POSITION_Y = 7.4500e+08F;
    constexpr float EXPECTED_VELOCITY_X = -1.4820e+02F;
    constexpr float EXPECTED_VELOCITY_Y = 2.9800e+04F;
    const auto celestialBody = universe[0];
    BOOST_REQUIRE_CLOSE(celestialBody.position().x, EXPECTED_POSITION_X, STEP_TOLERANCE);
    BOOST_REQUIRE_CLOSE(celestialBody.position().y, EXPECTED_POSITION_Y, STEP_TOLERANCE);
    BOOST_REQUIRE_CLOSE(celestialBody.velocity().x, EXPECTED_VELOCITY_X, STEP_TOLERANCE);
    BOOST_REQUIRE_CLOSE(celestialBody.velocity().y, EXPECTED_VELOCITY_Y, STEP_TOLERANCE);
}

// Tests if `Universe::step()` works correctly by performing two steps.
BOOST_AUTO_TEST_CASE(testUniverseStep2) {
    NB::Universe universe{ "assets/planets.txt" };
    constexpr double DELTA_TIME = 25000.0;
    universe.step(DELTA_TIME);
    universe.step(DELTA_TIME);

    constexpr float EXPECTED_POSITION_X = 2.2790e+11F;
    constexpr float EXPECTED_POSITION_Y = 1.2050e+09F;
    constexpr float EXPECTED_VELOCITY_X = -1.2772e+02F;
    constexpr float EXPECTED_VELOCITY_Y = 2.4100e+04F;
    const auto celestialBody = universe[1];
    BOOST_REQUIRE_CLOSE(celestialBody.position().x, EXPECTED_POSITION_X, STEP_TOLERANCE);
    BOOST_REQUIRE_CLOSE(celestialBody.position().y, EXPECTED_POSITION_Y, STEP_TOLERANCE);
    BOOST_REQUIRE_CLOSE(celestialBody.velocity().x, EXPECTED_VELOCITY_X, STEP_TOLERANCE);
    BOOST_REQUIRE_CLOSE(celestialBody.velocity().y, EXPECTED_VELOCITY_Y, STEP_TOLERANCE);
}

// Tests if `Universe::step()` works correctly by performing three steps.
BOOST_AUTO_TEST_CASE(testUniverseStep3) {
    NB::Universe universe{ "assets/planets.txt" };
    constexpr double DELTA_TIME = 25000.0;
    universe.step(DELTA_TIME);
    universe.step(DELTA_TIME);
    universe.step(DELTA_TIME);

    constexpr float EXPECTED_POSITION_X = 2.2789e+11F;
    constexpr float EXPECTED_POSITION_Y = 1.8075e+09F;
    constexpr float EXPECTED_VELOCITY_X = -1.9158e+02F;
    constexpr float EXPECTED_VELOCITY_Y = 2.4099e+04F;
    const auto celestialBody = universe[1];
    BOOST_REQUIRE_CLOSE(celestialBody.position().x, EXPECTED_POSITION_X, STEP_TOLERANCE);
    BOOST_REQUIRE_CLOSE(celestialBody.position().y, EXPECTED_POSITION_Y, STEP_TOLERANCE);
    BOOST_REQUIRE_CLOSE(celestialBody.velocity().x, EXPECTED_VELOCITY_X, STEP_TOLERANCE);
    BOOST_REQUIRE_CLOSE(celestialBody.velocity().y, EXPECTED_VELOCITY_Y, STEP_TOLERANCE);
}

// Tests if `Universe::step()` works correctly by performing multiple steps until the time reach one
// year.
BOOST_AUTO_TEST_CASE(testUniverseStepOneYear) {
    NB::Universe universe{ "assets/planets.txt" };
    constexpr double TOTAL_TIME = 31557600.0;
    double accumulatedTime = 0.0;
    while (accumulatedTime < TOTAL_TIME) {
        constexpr double DELTA_TIME = 25000.0;
        universe.step(DELTA_TIME);
        accumulatedTime += DELTA_TIME;
    }

    constexpr float EXPECTED_POSITION_X = -7.3731e+10F;
    constexpr float EXPECTED_POSITION_Y = -7.9391e+10F;
    constexpr float EXPECTED_VELOCITY_X = 2.5433e+04F;
    constexpr float EXPECTED_VELOCITY_Y = -2.3973e+04F;
    const auto celestialBody = universe[4];
    BOOST_REQUIRE_CLOSE(celestialBody.position().x, EXPECTED_POSITION_X, STEP_TOLERANCE);
    BOOST_REQUIRE_CLOSE(celestialBody.position().y, EXPECTED_POSITION_Y, STEP_TOLERANCE);
    BOOST_REQUIRE_CLOSE(celestialBody.velocity().x, EXPECTED_VELOCITY_X, STEP_TOLERANCE);
    BOOST_REQUIRE_CLOSE(celestialBody.velocity().y, EXPECTED_VELOCITY_Y, STEP_TOLERANCE);
}

// Tests if `Universe::step()` works correctly for negative delta time.
BOOST_AUTO_TEST_CASE(testUniverseStepNegativeDeltaTime) {
    NB::Universe universe{ "assets/planets.txt" };
    constexpr double DELTA_TIME = -25000.0;
    universe.step(DELTA_TIME);

    constexpr float EXPECTED_POSITION_X = 2.2789e11F;
    constexpr float EXPECTED_POSITION_Y = -602499968.F;
    constexpr float EXPECTED_VELOCITY_X = 63.8597F;
    constexpr float EXPECTED_VELOCITY_Y = 24100.F;
    const auto celestialBody = universe[1];
    BOOST_REQUIRE_CLOSE(celestialBody.position().x, EXPECTED_POSITION_X, STEP_TOLERANCE);
    BOOST_REQUIRE_CLOSE(celestialBody.position().y, EXPECTED_POSITION_Y, STEP_TOLERANCE);
    BOOST_REQUIRE_CLOSE(celestialBody.velocity().x, EXPECTED_VELOCITY_X, STEP_TOLERANCE);
    BOOST_REQUIRE_CLOSE(celestialBody.velocity().y, EXPECTED_VELOCITY_Y, STEP_TOLERANCE);
}

// Tests if `Universe::step()` works correctly by passing to different delta time consequently.
BOOST_AUTO_TEST_CASE(testUniverseStepDifferentDeltaTime) {
    NB::Universe universe{ "assets/planets.txt" };
    constexpr double DELTA_TIME_1 = 25000.0;
    constexpr double DELTA_TIME_2 = 45000.0;
    universe.step(DELTA_TIME_1);
    universe.step(DELTA_TIME_2);

    constexpr float EXPECTED_POSITION_X = 2.2789e11F;
    constexpr float EXPECTED_POSITION_Y = 1.68699e09F;
    constexpr float EXPECTED_VELOCITY_X = -178.808F;
    constexpr float EXPECTED_VELOCITY_Y = 24099.7F;
    const auto celestialBody = universe[1];
    BOOST_REQUIRE_CLOSE(celestialBody.position().x, EXPECTED_POSITION_X, STEP_TOLERANCE);
    BOOST_REQUIRE_CLOSE(celestialBody.position().y, EXPECTED_POSITION_Y, STEP_TOLERANCE);
    BOOST_REQUIRE_CLOSE(celestialBody.velocity().x, EXPECTED_VELOCITY_X, STEP_TOLERANCE);
    BOOST_REQUIRE_CLOSE(celestialBody.velocity().y, EXPECTED_VELOCITY_Y, STEP_TOLERANCE);
}
