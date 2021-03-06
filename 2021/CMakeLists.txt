cmake_minimum_required(VERSION 3.10)

set (CMAKE_CXX_STANDARD 17)

# set the project name
project(AdventOfCode)

SET(INCLUDE_DIR "./include")

add_executable(Day01_SonarSweep src/Day01_SonarSweep.cpp)
add_executable(Day02_Dive src/Day02_Dive.cpp)
add_executable(Day03_BinaryDiagnostic src/Day03_BinaryDiagnostic.cpp)
add_executable(Day04_GiantSquid src/Day04_GiantSquid.cpp)
add_executable(Day05_HydrothermalVenture src/Day05_HydrothermalVenture.cpp)
add_executable(Day06_LanternFish src/Day06_LanternFish.cpp)
add_executable(Day07_TheTreacheryOfWales src/Day07_TheTreacheryOfWales.cpp)
add_executable(Day08_SevenSegmentSearch src/Day08_SevenSegmentSearch.cpp)
add_executable(Day09_SmokeBasin src/Day09_SmokeBasin.cpp)
add_executable(Day10_SyntaxScoring src/Day10_SyntaxScoring.cpp)
add_executable(Day11_DumboOctopus src/Day11_DumboOctopus.cpp)
add_executable(Day12_PassagePathing src/Day12_PassagePathing.cpp)
add_executable(Day13_TransparentOrigami src/Day13_TransparentOrigami.cpp)
add_executable(Day14_ExtendedPolymerization src/Day14_ExtendedPolymerization.cpp)
add_executable(Day15_Chiton src/Day15_Chiton.cpp)
add_executable(Day16_PacketDecoder src/Day16_PacketDecoder.cpp ${INCLUDE_DIR}/Day16_PacketDecoder.h)
add_executable(Day17_TrickShot src/Day17_TrickShot.cpp)
add_executable(Day18_SnailFish src/Day18_SnailFish.cpp)
add_executable(Day19_BeaconScanner src/Day19_BeaconScanner.cpp)
add_executable(Day20_TrenchMap src/Day20_TrenchMap.cpp)

# copy data to cmake dir
add_custom_target(data ALL)
add_custom_command(TARGET data POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/data
    ${CMAKE_CURRENT_BINARY_DIR}/data
    COMMENT "Copying 'data'")
