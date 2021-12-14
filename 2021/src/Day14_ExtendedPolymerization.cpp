#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using VectorPairs = std::vector<std::pair<int, int>>;

void readData(const std::string& fileName, std::string& poly, std::unordered_map<std::string, char>& rules) {

    std::ifstream dataFile(fileName, std::ios_base::binary);
    std::string line;
    std::vector<std::string> lines;
    bool emptyLine = false;
    while (std::getline(dataFile, line)) {
        if (line == "\r") {
            continue;
        }
        if(line.back() == '\r')
            line.erase(line.end() -1);
        lines.push_back(line);
    }

    poly = lines.front();

    for (auto it = lines.begin() + 1; it != lines.end(); ++it)
    {
        std::stringstream ss(*it);
        std::string in;
        char arrow;
        char out;
        ss >> in >> std::skipws >> arrow >> arrow >> std::skipws >> out;
        rules.insert({in, out});
    }
}

void printFreqs(const std::unordered_map<char, long long>& freqs) {
    for (auto it = freqs.begin(); it != freqs.end(); ++it)
    {
        std::cout << " " << it->first << ": " << it->second << std::endl;
    }
}
void part1(const std::unordered_map<std::string, char> rules, std::string& poly) {
    unsigned maxT = 40;
    std::unordered_map<char, long long> freqs;
    for (unsigned t = 0; t < maxT; t++) {
        freqs.clear();
        for (auto it = poly.begin(); it != (poly.end() - 1); ++it) {
            const auto d = std::distance(poly.begin(), it);
            std::string pair{ *it };
            pair.push_back(*(it + 1));
            it = poly.insert(it+1, rules.find(pair)->second);
        }

        for (auto it = poly.begin(); it != poly.end(); ++it) {
            if (freqs.find(*it) != freqs.end())
                ++freqs[*it];
            else
                freqs.insert({*it, 1});
        }
        std::cout << "t = " << t << std::endl;
        std::cout << " length = " << poly.size() << std::endl;
        // printFreqs(freqs);
    }


    auto [min, max] = std::minmax_element(freqs.begin(), freqs.end(), [](auto& pair1, auto& pair2) {return pair1.second < pair2.second; });
    std::cout << "Min: " << min->first << ", freq = " << min->second << std::endl;
    std::cout << "Max: " << max->first << ", freq = " << max->second << std::endl;
    std::cout << "Max - Min: " <<  max->second - min->second << std::endl;

}

std::unordered_map<char, long long> initFreqs(const std::string& poly) {
    std::unordered_map<char, long long> freqs;
    for (auto it = poly.begin(); it != poly.end(); ++it) {
        if (freqs.find(*it) != freqs.end())
            ++freqs[*it];
        else
            freqs.insert({ *it, 1 });
    }

    return freqs;
}

std::unordered_map<std::string, long long> initGroups(const std::unordered_map<std::string, char>& rules, const std::string& poly) {
    std::unordered_map<std::string, long long> groups;
    for (auto it = rules.begin(); it != rules.end(); ++it) {
        groups.insert({it->first, 0});
    }

    for (auto it = poly.begin(); it != (poly.end() - 1); ++it) {
        std::string pair{ *it };
        pair.push_back(*(it + 1));
        ++groups[pair];
    }

    return groups;
}

void updateGroupsAndFreqs(std::unordered_map<std::string, long long>& groups, std::unordered_map<char, long long>& freqs, const std::unordered_map<std::string, char>& rules) {
    std::unordered_map<std::string, long long> newGroups;
    for (auto it = groups.begin(); it != groups.end(); ++it) {
        if(it->second == 0)
            continue;
        auto rule = rules.at(it->first);
        if (freqs.find(rule) != freqs.end())
            freqs[rule] += it->second;
        else
            freqs.insert({ rule , it->second });

        std::string strA = *(it->first.cbegin()) + std::string(1, rule);
        std::string strB = std::string(1, rule) + *(it->first.cend()-1);
        if (newGroups.find(strA) != newGroups.end())
            newGroups[strA] += it->second;
        else
            newGroups.insert({strA, it->second});
        if (newGroups.find(strB) != newGroups.end())
            newGroups[strB] += it->second;
        else
            newGroups.insert({strB, it->second});
    }

    groups = newGroups;
}

void part2(const std::unordered_map<std::string, char>& rules, std::string& poly) {
    auto groups = initGroups(rules, poly);
    auto freqs = initFreqs(poly);
    printFreqs(freqs);

    unsigned maxT = 40;
    for (unsigned t = 0; t < maxT; t++) {
        updateGroupsAndFreqs(groups, freqs, rules);

        std::cout << "t = " << t << std::endl;
        printFreqs(freqs);
    }


    auto [min, max] = std::minmax_element(freqs.begin(), freqs.end(), [](auto& pair1, auto& pair2) {return pair1.second < pair2.second; });
    std::cout << "Min: " << min->first << ", freq = " << min->second << std::endl;
    std::cout << "Max: " << max->first << ", freq = " << max->second << std::endl;
    std::cout << "Max - Min: " << max->second - min->second << std::endl;
}

int main() {
    std::string poly;
    std::unordered_map<std::string, char> rules;
    readData("../data/day14.txt", poly, rules);

    part1(rules, poly);
    part2(rules, poly);

    return 0;
}
