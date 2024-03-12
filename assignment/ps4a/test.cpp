// Copyright 2024 James Chen

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main

#include <fstream>
#include <iostream>
#include <sstream>
#include <boost/test/unit_test.hpp>
#include "../ps3b/SokobanConstants.hpp"
#include "Universe.hpp"

// Tests if `Universe::getNumPlanets()` and `Universe::getRadius` work correcly.
BOOST_AUTO_TEST_CASE(testUniverseBasic) {
    const NB::Universe universe{ SB::ASSETS_DIR.substr() + "1body.txt" };

    constexpr auto EXPECTED_NUM_PLANETS = 1;
    constexpr auto EXPECTED_RADIUS = 100.0;
    BOOST_REQUIRE_EQUAL(universe.numPlanets(), EXPECTED_NUM_PLANETS);
    BOOST_REQUIRE_EQUAL(universe.radius(), EXPECTED_RADIUS);
}

// Tests if `Universe::getNumPlanets()` and `Universe::getRadius` work correcly.
BOOST_AUTO_TEST_CASE(testUniverseBasic2) {
    const NB::Universe universe{ SB::ASSETS_DIR.substr() + "binary.txt" };

    constexpr auto EXPECTED_NUM_PLANETS = 2;
    constexpr auto EXPECTED_RADIUS = 5.0e10;
    BOOST_REQUIRE_EQUAL(universe.numPlanets(), EXPECTED_NUM_PLANETS);
    BOOST_REQUIRE_EQUAL(universe.radius(), EXPECTED_RADIUS);
}

// Tests if `CelestialBody::getNumPlanets()`, `CelestialBody::getRadius()` and
// `CelestialBody::getMass()` work correcly.
BOOST_AUTO_TEST_CASE(testCelestialBodyBasic) {
    const NB::Universe universe{ SB::ASSETS_DIR.substr() + "planets.txt" };
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
    BOOST_REQUIRE_EQUAL(celestialBody.mass(), EXPECTED_MASS);
}

// Tests if `Universe::operator[]` works for the non-first elements.
BOOST_AUTO_TEST_CASE(testUniverseBracketOperator1) {
    const NB::Universe universe{ SB::ASSETS_DIR.substr() + "3body.txt" };
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
    BOOST_REQUIRE_EQUAL(celestialBody.mass(), EXPECTED_MASS);
}

// Tests if `Universe::operator[]` works for the last elements.
BOOST_AUTO_TEST_CASE(testUniverseBracketOperator2) {
    const NB::Universe universe{ SB::ASSETS_DIR.substr() + "uniform8.txt" };
    const auto celestialBody = universe[universe.numPlanets() - 1];

    constexpr float EXPECTED_POSITION_X = 3.535534e+08;
    constexpr float EXPECTED_POSITION_Y = -3.535534e+08;
    constexpr float EXPECTED_VELOCITY_X = -1.934345e+02;
    constexpr float EXPECTED_VELOCITY_Y = -1.934345e+02;
    constexpr float EXPECTED_MASS = 2.00e+23;
    BOOST_REQUIRE_EQUAL(celestialBody.position().x, EXPECTED_POSITION_X);
    BOOST_REQUIRE_EQUAL(celestialBody.position().y, EXPECTED_POSITION_Y);
    BOOST_REQUIRE_EQUAL(celestialBody.velocity().x, EXPECTED_VELOCITY_X);
    BOOST_REQUIRE_EQUAL(celestialBody.velocity().y, EXPECTED_VELOCITY_Y);
    BOOST_REQUIRE_EQUAL(celestialBody.mass(), EXPECTED_MASS);
}

// Tests if `Universe::operator[]` works for the last elements.
BOOST_AUTO_TEST_CASE(testUniverseBracketOperator3) {
    const NB::Universe universe{ SB::ASSETS_DIR.substr() + "8star-rotation.txt" };
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
    BOOST_REQUIRE_EQUAL(celestialBody.mass(), EXPECTED_MASS);
}

// Tests if "CelestialBody::operator>>" and "CelestialBody::operator<<" works correctly.
BOOST_AUTO_TEST_CASE(testCelestialBodyOutput) {
    std::ifstream fileStream{ SB::ASSETS_DIR.substr() +"uniform8.txt" };
    std::string line;
    getline(fileStream, line);
    getline(fileStream, line);
    getline(fileStream, line);
    getline(fileStream, line);

    const std::string EXPECT_LINE =
        " 3.535534e+08  3.535534e+08  1.934345e+02 -1.934345e+02 2.00e+23 earth.gif";

    BOOST_REQUIRE_EQUAL(line, EXPECT_LINE);
}

// Tests if the program identify a broken implementation
BOOST_AUTO_TEST_CASE(testIdentifyBrokenImplementation) {
    NB::Universe universe;
    std::istringstream istringstream1("aaaaaaa");
    std::istringstream istringstream2("3\n1.25e11\naaaaaaa");

    BOOST_CHECK_THROW(istringstream1 >> universe, std::runtime_error);
    BOOST_CHECK_THROW(istringstream2 >> universe, std::runtime_error);
}
