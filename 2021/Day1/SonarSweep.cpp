// SonarSweep.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

bool readData(const std::string& fileName, std::vector<int>& data) {
    std::ifstream dataF(fileName);
    std::string dataS = "";
    if (!dataF.is_open())
        return false;

    while (std::getline(dataF, dataS))
    {
        if (dataS.empty())
            continue;

        data.push_back(std::stoi(dataS));
    }
    dataF.close();

    if (data.empty())
        return false;
    return true;
}

int main()
{
    std::vector<int> data;
    if (!readData("../data.txt", data)) {
        std::cout << "No data!" << std::endl;
        return 5;
    }

    // Part 1
    unsigned countIncreases = 0;
    for (auto it = data.cbegin(); it != (data.cend() - 1); ++it)
        if (*(it + 1) > *it)
            ++countIncreases;

    std::cout << "Count increases (1) " << countIncreases << std::endl;

    // Part 2
    unsigned countIncreasesWindows = 0;
    for (auto it = data.begin(); it != (data.end() - 3); ++it) {
        int sumWindow1 = std::accumulate(data.begin() + std::distance(data.begin(), it), data.begin() + std::distance(data.begin(), it + 3), 0);
        int sumWindow2 = std::accumulate(data.begin() + std::distance(data.begin(), it + 1), data.begin() + std::distance(data.begin(), it + 4), 0);
        if (sumWindow2 > sumWindow1)
            ++countIncreasesWindows;
    }

    std::cout << "Count increases (2) " << countIncreasesWindows << std::endl;

    return 0;
}
