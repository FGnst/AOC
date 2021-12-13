#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <sstream>
#include <string>
#include <vector>
#include <stack>

std::vector<std::vector<unsigned>> readData(const std::string& fileName) {
    std::vector<std::vector<unsigned>> input;

    std::ifstream dataFile(fileName, std::ios_base::binary);
    std::string line;
    while (std::getline(dataFile, line)) {
        std::vector<unsigned> lineData;
        for (auto ch : line) {
            if (ch != '\r')
                lineData.push_back(unsigned(ch - '0'));
        }

        input.push_back(lineData);
    }

    return input;
}
bool isWithinLimits(unsigned i, unsigned j, size_t energySize) {
    return !(i < 0 || i >= energySize || j < 0 || j >= energySize);
}

unsigned* getNeighborEnergy(unsigned i, unsigned j, std::vector<std::vector<unsigned>>& energies) {
    if (i < 0 || i >= energies.size() || j < 0 || j >= energies[0].size())
        return nullptr;

    return &energies[i][j];
}

void expandEnergyToNeighbors(unsigned i, unsigned j, std::vector<std::vector<unsigned>>& energies) {
    if (i < 0 || i >= energies.size() || j < 0 || j >= energies[0].size())
        return;

    energies[i][j] = 0;
    auto* upRight = getNeighborEnergy(i - 1, j + 1, energies);
    auto* upLeft = getNeighborEnergy(i - 1, j - 1, energies);
    auto* upMiddle = getNeighborEnergy(i - 1, j, energies);
    auto* right = getNeighborEnergy(i, j + 1, energies);
    auto* left = getNeighborEnergy(i, j - 1, energies);
    auto* downRight = getNeighborEnergy(i + 1, j + 1, energies);
    auto* downLeft = getNeighborEnergy(i + 1, j - 1, energies);
    auto* downMiddle = getNeighborEnergy(i + 1, j, energies);

    if (upRight && *upRight != 0) {
        *upRight += 1;
        if (*upRight > 9) {
            expandEnergyToNeighbors(i - 1, j + 1, energies);
        }
    }
    if (upLeft && *upLeft != 0) {
        *upLeft += 1;
        if (*upLeft > 9) {
            *upLeft = 0;
            expandEnergyToNeighbors(i - 1, j - 1, energies);
        }
    }
    if (upMiddle && *upMiddle != 0) {
        *upMiddle += 1;
        if (*upMiddle > 9) {
            expandEnergyToNeighbors(i - 1, j, energies);
        }
    }
    if (right && *right != 0) {
        *right += 1;
        if (*right > 9) {
            if (isWithinLimits(i, j + 1, energies.size()))
                expandEnergyToNeighbors(i, j + 1, energies);
        }
    }
    if (left && *left != 0) {
        *left += 1;
        if (*left > 9) {
            expandEnergyToNeighbors(i, j - 1, energies);
        }

    }
    if (downRight && *downRight != 0) {
        *downRight += 1;
        if (*downRight > 9) {
            expandEnergyToNeighbors(i + 1, j + 1, energies);
            *downRight = 0;
        }
    }
    if (downLeft && *downLeft != 0) {
        *downLeft += 1;
        if (*downLeft > 9) {
            expandEnergyToNeighbors(i + 1, j - 1, energies);
        }
    }
    if (downMiddle && *downMiddle != 0) {
        *downMiddle += 1;
        if (*downMiddle > 9) {
            expandEnergyToNeighbors(i + 1, j, energies);
        }
    }
}

void printStatus(std::vector<std::vector<unsigned>>& energies) {

    for (int i = 0; i < energies.size(); ++i)
    {

        for (int j = 0; j < energies[0].size(); ++j)
        {
            std::cout << energies[i][j] << " ";
        }
        std::cout << '\n';
    }

}
void increaseEnergies(std::vector<std::vector<unsigned>>& energies) {
    for (auto& line : energies)
    {
        for (auto& energy : line)
        {
            ++energy;
        }
    }
}

void updateEnergies(std::vector<std::vector<unsigned>>& energies) {
    for (auto& line : energies)
    {
        for (auto& energy : line)
        {
            if (energy > 9)
                energy = 0;
        }
    }
}
void evolve(std::vector<std::vector<unsigned>>& energies) {
    increaseEnergies(energies);
    for (int i = 0; i < energies.size(); ++i)
    {

        for (int j = 0; j < energies[0].size(); ++j)
        {
            if (energies[i][j] > 9) {
                expandEnergyToNeighbors(i, j, energies);
            }
        }
    }
    // updateEnergies(energies);

}

void updateFlashCount(std::vector<std::vector<unsigned>>& energies, int& count) {
    for (int i = 0; i < energies.size(); ++i)
    {

        for (int j = 0; j < energies[0].size(); ++j)
        {
            if (energies[i][j] == 0) {
                ++count;
            }
        }
    }
}

int getFlashCount(std::vector<std::vector<unsigned>>& energies) {
    int count = 0;
    for (int i = 0; i < energies.size(); ++i)
    {

        for (int j = 0; j < energies[0].size(); ++j)
        {
            if (energies[i][j] == 0) {
                ++count;
            }
        }
    }
    return count;
}

int main() {

    auto energies = readData("../data/day11.txt");

    std::cout << "Time: t0 " << std::endl;
    printStatus(energies);

    // unsigned totSteps = 100;
    bool synchronized = false;
    //int flashCount = 0;
    unsigned t = 0;
    while (!synchronized)
    {
        evolve(energies);
        //updateFlashCount(energies, flashCount);
        auto flashCount = getFlashCount(energies);
        if (flashCount == energies.size() * energies.size())
            synchronized = true;
        ++t;
        std::cout << "Time: " << t << std::endl;

        // printStatus(energies);
    }

    printStatus(energies);

    //std::cout << "Flash count: " << flashCount << std::endl;

    return 0;
}
