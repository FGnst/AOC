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

using charData = std::vector<std::vector<wchar_t>>;
charData readData(const std::string& fileName) {
    std::vector<std::vector<wchar_t>> input;

    std::ifstream dataFile(fileName, std::ios_base::binary);
    std::string line;
    while (std::getline(dataFile, line)) {
        std::vector<wchar_t> lineData;
        for (const auto& ch : line)
            lineData.push_back(ch);
        input.push_back(lineData);
    }


    return input;
}

std::map<wchar_t, unsigned> scoresCorrupt{ {')', 3},{']', 57},{'}', 1197},{'>', 25137} };
std::map<wchar_t, unsigned> scoresCompletion{ {')', 1},{']', 2},{'}', 3},{'>', 4} };

std::vector<std::pair<wchar_t, wchar_t>> symbols{ {'(', ')'},{'[',']'},{'{', '}'},{'<','>'} };

enum class Type {
    Corrupt = 0,
    Incomplete = 1,
    Normal = 2
};

std::vector<Type> findCorruptedOrIncompleteLines(const charData& charData, std::vector<wchar_t>& illegalSymbols) {
    std::vector<Type> lineTypes;
    illegalSymbols.clear();
    for (const auto line : charData)
    {
        std::list<wchar_t> openingChars;
        bool isCorrupt = false;
        for (const auto symbol : line) {

            auto foundOpening = std::find_if(symbols.begin(), symbols.end(), [&symbol](const auto& symbolPair) { return symbolPair.first == symbol; });
            auto foundClosing = std::find_if(symbols.begin(), symbols.end(), [&symbol](const auto& symbolPair) { return symbolPair.second == symbol; });
            if (foundOpening != symbols.end())
                openingChars.push_back(symbol);
            else if (foundClosing != symbols.end()) {
                if (*(--openingChars.end()) != foundClosing->first) {
                    isCorrupt = true;
                    illegalSymbols.push_back(foundClosing->second);
                    break;
                }
                openingChars.pop_back();
            }
        }

        if (isCorrupt)
            lineTypes.push_back(Type::Corrupt);
        else if (!openingChars.empty())
            lineTypes.push_back(Type::Incomplete);
        else
            lineTypes.push_back(Type::Normal);
    }

    return lineTypes;
}

unsigned computeScoreCorrupted(const std::vector<wchar_t>& illegalSymbols) {
    unsigned total = 0;
    for (const auto& s : illegalSymbols)
    {
        total += scoresCorrupt[s];
    }

    return total;
}

std::vector<std::list<wchar_t>> completeLines(charData& incompleteLines) {
    std::vector<std::list<wchar_t>> completionStrings;
    for (auto& line : incompleteLines) {

        std::list<wchar_t> completionString;
        std::list<wchar_t> openingChars;
        bool isCorrupt = false;
        for (auto itSymbol = line.begin(); itSymbol != line.end(); ++itSymbol) {

            auto foundOpening = std::find_if(symbols.begin(), symbols.end(), [&itSymbol](const auto& symbolPair) { return symbolPair.first == *itSymbol; });
            auto foundClosing = std::find_if(symbols.begin(), symbols.end(), [&itSymbol](const auto& symbolPair) { return symbolPair.second == *itSymbol; });
            if (foundOpening != symbols.end()) {
                openingChars.push_back(*itSymbol);
                completionString.push_back(foundOpening->second);
            }
            else if (foundClosing != symbols.end()) {
                if (*(--openingChars.end()) == foundClosing->first) {
                    openingChars.pop_back();
                    completionString.pop_back();
                }
            }
        }
        completionString.reverse();
        completionStrings.push_back(completionString);
    }

    return completionStrings;
}

long long computeScoreCompletion(const std::list<wchar_t>& completionStrings) {
    long long total = 0;
    for (const auto& s : completionStrings)
    {
        total = total*5 + scoresCompletion[s];
    }

    return total;
}

int main() {

    auto data = readData("../data/day10.txt");
    std::vector<wchar_t> illegalSymbols;
    auto corruptedOrIncomplete = findCorruptedOrIncompleteLines(data, illegalSymbols);
    charData incomplete;
    for (auto it = corruptedOrIncomplete.begin(); it != corruptedOrIncomplete.end(); ++it) {
        if (*it == Type::Incomplete)
            incomplete.push_back(data[std::distance(corruptedOrIncomplete.begin(), it)]);
    }
    assert(corruptedOrIncomplete.size() == data.size());
    auto score = computeScoreCorrupted(illegalSymbols);
    std::cout << "Corrupted score: " << score << std::endl;

    auto completionStrings = completeLines(incomplete);
    std::vector<long long> scores;
    for (const auto& string : completionStrings) {
        scores.push_back(computeScoreCompletion(string));
    }

    std::sort(scores.begin(), scores.end());

    std::cout << "Middle score: " << scores[scores.size() / 2] << std::endl;

    return 0;
}
