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

struct Digit {
    Digit(int begin, int length) {
        _begin = begin;
        _length = length;
    }
    int _begin;
    int _length;
};

std::optional<std::pair<Digit, Digit>> findFirstNested4Pair(const std::string& input) {
    int lvl = -1;
    for (int i = 0; i < input.size() - 2; ++i)
    {

        if (input[i] == '[')
        {
            ++lvl;
        }
        else if (input[i] == ']')
        {
            --lvl;
        }
        else if (std::isdigit(input[i]) && lvl >= 4) {
            auto it = std::find_if(input.begin() + i, input.end(), [](char ch) {return ch == ','; });
            assert(it != input.end());
            Digit first{ i, (int)std::distance(input.begin(), it) - i };
            auto endIt = std::find_if(it, input.end(), [](char ch) {return ch == ']'; });
            assert(endIt != input.end());
            Digit second{ (int)std::distance(input.begin(), it + 1) , (int)std::distance(it + 1, endIt) };
            return std::make_pair(first, second);
        }
    }

    return std::nullopt;
}

std::optional<std::pair<int, int>> findFirstLeftNumber(const std::string& input, int left) {
    for (int i = left - 1; i >= 0; --i)
    {
        if (input[i] != '[' && input[i] != ']' && input[i] != ',')
        {
            if (std::isdigit(input[i])) {
                auto it = std::find_if(input.rbegin() + (input.size() - (i + 1)), input.rend(), [](char ch) {return ch == '[' || ch == ','; });
                if (it != input.rend())
                    return std::make_pair((int)std::distance(it, input.rend()), i + 1 - (int)std::distance(it, input.rend()));
            }
        }
    }

    return std::nullopt;
}

std::optional<std::pair<int, int>> findFirstRightNumber(const std::string& input, int right) {
    for (int i = right; i < input.size(); ++i)
    {
        if (input[i] != '[' && input[i] != ']' && input[i] != ',') //could be a range if > 10!!
        {
            if (isdigit(input[i])) {
                auto it = std::find_if(input.begin() + i, input.end(), [](char ch) {return ch == ']' || ch == ','; });
                if (it != input.end())
                    return std::make_pair(i, (int)std::distance(input.begin() + i, it));
            }
        }
    }

    return std::nullopt;
}

std::optional<std::pair<int, int>> findFirstBiggerThan10(const std::string& input) {
    int first = 0;
    for (int i = 0; i < input.size() - 1; ++i)
    {
        if (std::isdigit(input[i])) {
            auto it = std::find_if(input.begin() + i, input.end(), [](char c) {return c == ',' || c == ']'; });
            if (it != input.end() && std::stoi(input.substr(i, std::distance(input.begin(), it) - 1)) >= 10)
                return std::make_pair(i, (int)std::distance(input.begin(), it) - 1);
        }

    }

    return std::nullopt;
}


bool split(std::string& input) {

    auto newString = input;
    auto first10 = findFirstBiggerThan10(input);
    if (!first10) {
        std::cout << "Nothing to split" << std::endl;
        return false;
    }

    int numberToSplit = std::stoi(input.substr(first10->first, first10->second + 1));
    int left = (int)std::floor(static_cast<double>(numberToSplit) / 2.0);
    int right = (int)std::ceil(static_cast<double>(numberToSplit) / 2.0);

    int numberOfDigits = first10->second - first10->first + 1;
    std::string replace = "[";
    replace += std::to_string(left);
    replace.push_back(',');
    replace += std::to_string(right);
    replace.push_back(']');
    newString.replace(first10->first, numberOfDigits, replace);
    //newString.insert(first10->first + numberOfDigits, replace.substr(numberOfDigits, replace.size() - numberOfDigits));

    input = newString;
    return true;
}

bool explode(std::string& input) {
    std::string newString = input;
    auto nested4 = findFirstNested4Pair(input);
    if (!nested4.has_value()) {
        std::cout << "No nested 4 pair!" << std::endl;
        return false;
    }
    std::cout << "nested 4: " << input.substr(nested4->first._begin, nested4->second._begin + nested4->second._length - nested4->first._begin) << "\n";
    std::cout << std::stoi(input.substr(nested4->first._begin, nested4->first._length)) << std::endl;
    std::cout << std::stoi(input.substr(nested4->second._begin, nested4->second._length)) << std::endl;

    auto leftNeigh = findFirstLeftNumber(newString, nested4->first._begin);
    int offset = 0;
    if (leftNeigh) {
        std::cout << "Left neigh: " << input.substr(leftNeigh->first, leftNeigh->second) << std::endl;
        auto leftResult = std::stoi(newString.substr(nested4->first._begin, nested4->first._length)) + std::stoi(newString.substr(leftNeigh->first, leftNeigh->second));
        newString.replace(leftNeigh->first, leftNeigh->second, std::to_string(leftResult));
        offset = (int)(newString.size() - input.size());
    }


    auto rightNeigh = findFirstRightNumber(input, nested4->second._begin + nested4->second._length);
    if (rightNeigh) {
        std::cout << "Right neigh: " << input.substr(rightNeigh->first, rightNeigh->second) << std::endl;
        int rightResult = std::stoi(input.substr(nested4->second._begin, nested4->second._length)) + std::stoi(input.substr(rightNeigh->first, rightNeigh->second));
        newString.replace(rightNeigh->first + offset, rightNeigh->second, std::to_string(rightResult));
    }

    newString.replace(nested4->first._begin - 1 + offset, nested4->second._length + nested4->first._length + 3, "0"); //replace pair with 0

    input = newString;

    return true;
}

std::string add(const std::string& left, const std::string& right) {
    std::string result = "[";
    result += left;
    result.push_back(',');
    result += right;
    result.push_back(']');
    return result;
}



void reduce(std::string& input) {
    while (true) {
        if (explode(input)) {
            std::cout << "Exploded: " << input << std::endl;
        }
        else if (split(input)) {
            std::cout << "Split: " << input << std::endl;
        }
        else
        {
            std::cout << "Nothing to reduce, break" << std::endl;
            break;
        }
    }
}

int computeMagnitude(const std::string& input) {
    int sum = 0, i = 0;
    std::stack<std::vector<int>> stack;
    do {

        if (input[i] == '[')
        {
            stack.push({});
        }
        else if (input[i] == ']')
        {
            // auto it = std::find_if(input.rbegin() + (input.size() - (i + 1)), input.rend(), [](char ch) {return ch == '[' || ch == ','; });

            auto& top = stack.top();
            // top.push_back(std::stoi(input.substr(std::distance(input.rend(), it) - 1, i)));

            if (top.size() > 1) {
                std::cout << "Pair: " << top.front() << " " << top.back() << std::endl;
                assert(top.size() == 2);
                sum = 3 * top.front() + 2 * top.back();
            }
            else if (top.size() == 1) {
                std::cout << "Single number: " << top.front() << std::endl;
                sum = top.front();
            }

            stack.pop();
            if (!stack.empty())
                stack.top().push_back(sum);
        }
        else if (std::isdigit(input[i])) {
            auto it = std::find_if(input.begin() + i, input.end(), [](char ch) {return ch == ']' || ch == ','; });
            stack.top().push_back(std::stoi(input.substr(i, std::distance(input.begin(), it) - 1)));
        }

        ++i;
    } while (!stack.empty());

    std::cout << "Sum: " << sum << std::endl;
    return sum;
}

std::vector<std::string> readData(const std::string& fileName) {
    std::vector<std::string> data;
    std::fstream file(fileName);
    std::string line;
    while (std::getline(file, line)) {
        data.push_back(line);
    }

    return data;
}

void testExplode() {
    std::vector<std::pair<std::string, std::string>> inputOutput{ {"[[[[[9,8],1],2],3],4]", "[[[[0,9],2],3],4]"},{"[7,[6,[5,[4,[3,2]]]]]", "[7,[6,[5,[7,0]]]]"},{"[[6,[5,[4,[3,2]]]],1]", "[[6,[5,[7,0]]],3]"},{"[[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]]", "[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]"},{"[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]", "[[3,[2,[8,0]]],[9,[5,[7,0]]]]"} };

    for (const auto& pair : inputOutput)
    {
        std::string input = pair.first;
        explode(input);
        assert(input == pair.second);
    }

}

void testSplit() {
    std::string input("[10,]");
    std::string output = "[[5,5],]";
    auto splitted = split(input);
    assert(input == output);
}

void testData() {
    auto data = readData("../data/day18_test.txt");

    std::string addition = data[0];
    for (unsigned i = 1; i < data.size(); i += 1)
    {
        addition = add(addition, data[i]);
        reduce(addition);
        std::cout << "Addition result: " << addition << std::endl;
    }

    assert(addition == "[[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]]");
}

void testData2() {
    auto data = readData("../data/day18_test2.txt");

    std::string addition = data[0];
    for (unsigned i = 1; i < data.size(); i += 1)
    {
        addition = add(addition, data[i]);
        reduce(addition);
        std::cout << "Addition result: " << addition << std::endl;
    }

    assert(addition == "[[[[6,6],[7,6]],[[7,7],[7,0]]],[[[7,7],[7,7]],[[7,8],[9,9]]]]");
    assert(computeMagnitude(addition) == 4140);
}

void testAddition() {
    const std::string left = "[[[[4,3],4],4],[7,[[8,4],9]]]";
    const std::string right = "[1,1]";

    auto addition = add(left, right);
    reduce(addition);

    assert(addition == "[[[[0,7],4],[[7,8],[6,0]]],[8,1]]");
}

void testComputeMagnitude() {
    std::map<std::string, int> magnitudes;
    magnitudes.insert({ "[[1,2],[[3,4],5]]", 143 });
    magnitudes.insert({ "[[[[0,7],4],[[7,8],[6,0]]],[8,1]]", 1384 });
    magnitudes.insert({ "[[[[1,1],[2,2]],[3,3]],[4,4]]", 445 });
    magnitudes.insert({ "[[[[3,0],[5,3]],[4,4]],[5,5]]", 791 });
    magnitudes.insert({ "[[[[5,0],[7,4]],[5,5]],[6,6]]", 1137 });
    magnitudes.insert({ "[[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]]", 3488 });

    for (const auto& magn : magnitudes)
    {
        auto res = computeMagnitude(magn.first);
        assert(res == magn.second);
    }
}

void part1() {
    auto data = readData("../data/day18.txt");

    std::string addition = data[0];
    for (unsigned i = 1; i < data.size(); i += 1)
    {
        addition = add(addition, data[i]);
        reduce(addition);
        std::cout << "Addition result: " << addition << std::endl;
    }
    auto magnitude = computeMagnitude(addition);

    std::cout << "Magnitude: " << magnitude << std::endl;
}

void testLargestMagnitude() {
    auto data = readData("../data/day18_test2.txt");
    std::vector<int> magnitudes;
    for (unsigned i = 0; i < data.size(); i += 1) {
        std::string addition = data[i];
        for (unsigned j = 0; j < data.size(); j += 1)
        {
            if (i == j)
                continue;
            auto additionInternal = add(addition, data[j]);
            reduce(additionInternal);
            std::cout << "Addition result: " << additionInternal << std::endl;
            auto magn = computeMagnitude(additionInternal);
            std::cout << "Magnitude : " << magn << std::endl;
            magnitudes.push_back(magn);
        }
    }

    assert(3993 == *std::max_element(magnitudes.begin(), magnitudes.end()));
}

void part2() {
    auto data = readData("../data/day18.txt");
    std::vector<int> magnitudes;
    for (unsigned i = 0; i < data.size(); i += 1) {
        std::string addition = data[i];
        for (unsigned j = 0; j < data.size(); j += 1)
        {
            if (i == j)
                continue;
            auto additionInternal = add(addition, data[j]);
            reduce(additionInternal);
            std::cout << "Addition result: " << additionInternal << std::endl;
            auto magn = computeMagnitude(additionInternal);
            std::cout << "Magnitude : " << magn << std::endl;
            magnitudes.push_back(magn);
        }
    }

    std::cout << "Max magnitude: " << *std::max_element(magnitudes.begin(), magnitudes.end()) << std::endl;
}

int main() {

    testExplode();
    testSplit();
    testAddition();
    testData();
    testComputeMagnitude();
    testData2();
    part1();
    testLargestMagnitude();
    part2();

    return 0;
}
