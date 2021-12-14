#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using VectorPairs = std::vector<std::pair<int, int>>;

void readData(const std::string& fileName, std::vector<std::pair<int, int>>& dots, std::vector<std::pair<unsigned, unsigned>>& foldInstructions) {

    std::ifstream dataFile(fileName, std::ios_base::binary);
    std::string line;
    bool emptyLine = false;
    while (std::getline(dataFile, line)) {
        if (line == "\r") {
            emptyLine = true;
            continue;
        }
        std::stringstream ss(line);
        if (!emptyLine) {
            int val1{}, val2{};
            char comma{};
            ss >> val1;
            ss >> comma;
            ss >> val2;
            dots.push_back({ val1, val2 });
        }
        else {
            std::string fold{};
            std::string along{};
            char axis{};
            char equalsign{};
            unsigned val;
            ss >> fold;
            ss >> along;
            ss >> axis;
            ss >> equalsign;
            ss >> val;
            foldInstructions.push_back({ axis == 'x' ? 0 : 1, val });
        }
    }
}

void printSheet(const std::vector<std::pair<int, int>>& dots) {
    std::cout << "*******************************" << std::endl;
    auto maxX = std::max_element(dots.begin(), dots.end(), [](const auto& a, const auto& b) {return a.first < b.first; })->first;
    auto maxY = std::max_element(dots.begin(), dots.end(), [](const auto& a, const auto& b) {return a.second < b.second; })->second;

    for (int i = 0; i <= maxY; ++i)
    {
        for (int j = 0; j <= maxX; ++j)
        {
            auto foundIt = std::find_if(dots.begin(), dots.end(), [&i, &j](const auto& point) {return point.first == j && point.second == i; });
            if (foundIt == dots.end())
                std::cout << ".";
            else {
                std::cout << "#";
            }
        }
        std::cout << "\n";
    }
}

std::pair<unsigned, unsigned> getMappedPoint(const std::pair<int, int>& dot, const std::pair<unsigned, unsigned>& instruction) {
    if (instruction.first == 0) { // line is along y 
        int dist = dot.first - instruction.second;
        if (dist < 0)
            return dot;
        else if (dist == 0)
            return std::make_pair(dot.first - 1, dot.second);
        auto newX = dot.first - 2 * dist;
        assert(newX >= 0);
        return std::make_pair(newX, dot.second);
    }
    // (instruction.second == 1) ,line is along x
    int dist = dot.second - instruction.second;
    if (dist < 0)
        return dot;
    else if (dist == 0)
        return std::make_pair(dot.first, dot.second - 1);
    auto newY = dot.second - 2 * dist;
    assert(newY >= 0);
    return std::make_pair(dot.first, newY);

}
void removeDuplicates(std::vector<std::pair<int, int>>& dots) {
    std::map<std::pair<int, int>, std::vector<long long>> duplicatesMap;
    for (auto it = dots.begin(); it != dots.end(); ++it) {
        std::vector<long long> itVector;
        for (auto it2 = dots.begin(); it2 != dots.end(); ++it2)
        {
            if (it == it2)
                continue;
            else if (it->first == it2->first && it->second == it2->second) {
                if (duplicatesMap.find(*it) != duplicatesMap.end()) {
                    if (std::find(duplicatesMap[*it].begin(), duplicatesMap[*it].end(), std::distance(dots.begin(), it2)) == duplicatesMap[*it].end()) {
                        duplicatesMap[*it].push_back(std::distance(dots.begin(), it2));
                    }
                }
                else // not found
                    duplicatesMap.insert({ *it, {std::distance(dots.begin(), it2)} });
            }
        }
    }

    std::vector<long long> toBeDeleted;
    for (auto it = duplicatesMap.begin(); it != duplicatesMap.end(); ++it) {
        toBeDeleted.push_back(it->second.front());
    }

    std::sort(toBeDeleted.begin(), toBeDeleted.end());
    std::vector<std::pair<int, int>> newDots;
    for (auto it = dots.begin(); it != dots.end(); ++it) {
        auto dist = std::distance(dots.begin(), it);
        if (std::find(toBeDeleted.begin(), toBeDeleted.end(), dist) == toBeDeleted.end())
            newDots.push_back(*it);
    }

    dots = newDots;
}

void foldSheet(std::vector<std::pair<int, int>>& dots, const std::pair<unsigned, unsigned>& instruction) {
    std::vector<std::pair<int, int>> newDots;
    for (auto& dot : dots) {
        newDots.push_back(getMappedPoint(dot, instruction));
    }
    removeDuplicates(newDots);
    dots = newDots;
}

int main() {
    std::vector<std::pair<int, int>> dots;
    std::vector<std::pair<unsigned, unsigned>> foldInstructions;
    readData("../data/day13.txt", dots, foldInstructions);
    std::cout << dots.size() << std::endl;

    for (const auto& inst : foldInstructions) {
        foldSheet(dots, inst);
    }

    printSheet(dots);
    std::cout << dots.size() << std::endl;
    return 0;
}
