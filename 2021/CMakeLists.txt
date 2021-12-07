cmake_minimum_required(VERSION 3.10)

set (CMAKE_CXX_STANDARD 17)

# set the project name
project(AdventOfCode)

add_executable(Day1_SonarSweep src/Day1_SonarSweep.cpp)
add_executable(Day2_Dive src/Day2_Dive.cpp)
add_executable(Day3_BinaryDiagnostic src/Day3_BinaryDiagnostic.cpp)
add_executable(Day4_GiantSquid src/Day4_GiantSquid.cpp)
add_executable(Day5_HydrothermalVenture src/Day5_HydrothermalVenture.cpp)
add_executable(Day6_LanternFish src/Day6_LanternFish.cpp)
add_executable(Day7_TheTreacheryOfWales.cpp)

message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS ${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(data ALL)
add_custom_command(TARGET data POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/data
    ${CMAKE_CURRENT_BINARY_DIR}/data
    COMMENT "Copying 'data'")