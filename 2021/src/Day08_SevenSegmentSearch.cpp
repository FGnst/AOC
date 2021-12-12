#ifdef NDEBUG
#  define assert(condition) ((void)0)
#else
#  define assert(condition) /*implementation defined*/
#endif

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

void readData(std::string fileName, std::vector<std::vector<std::string>>& signals, std::vector<std::vector<std::string>>& outputs) {

    std::ifstream dataFile(fileName);
    std::string line;
    while (std::getline(dataFile, line)) {
        std::vector<std::string> signalVec;
        std::vector<std::string> outputVec;
        auto separator = line.find('|');
        auto signalString = line.substr(0, separator - 1);
        auto outputString = line.substr(separator + 1, line.size());
        std::string signal{};
        std::stringstream  ss(signalString);
        while (ss >> signal)
            signalVec.push_back(signal);

        ss = std::stringstream(outputString);
        while (ss >> signal)
            outputVec.push_back(signal);

        signals.push_back(signalVec);
        outputs.push_back(outputVec);
    }
}

std::vector<std::string> findStringsOfLength(const std::vector<std::string>& strings, size_t l)
{
    std::vector<std::string> foundStrings;
    auto it = strings.begin();
    while ((it = std::find_if(it, strings.end(), [&l](const std::string& str) {return str.length() == l; })) != strings.end()) {
        foundStrings.push_back(*it);
        ++it;
    }
    return foundStrings;
}

std::string logicalAnd(std::string a, std::string b) {
    std::string res;
    for (int i = 0; i < a.length(); ++i)
    {
        for (int j = 0; j < b.length(); ++j) {
            if (a[i] == b[j])
                res.push_back(a[i]);
        }
    }

    return res;
}

const int MAX_CHAR = 26;

// function to find the uncommon characters
// of the two strings
std::string findAndPrintUncommonChars(std::string str1, std::string str2)
{
    std::string res;
    // mark presence of each character as 0
    // in the hash table 'present[]'
    int present[MAX_CHAR];
    for (int i = 0; i < MAX_CHAR; i++)
        present[i] = 0;

    auto l1 = str1.size();
    auto l2 = str2.size();

    // for each character of str1, mark its
    // presence as 1 in 'present[]'
    for (unsigned i = 0; i < l1; i++)
        present[str1[i] - 'a'] = 1;

    // for each character of str2
    for (unsigned i = 0; i < l2; i++)
    {
        // if a character of str2 is also present
        // in str1, then mark its presence as -1
        if (present[str2[i] - 'a'] == 1
            || present[str2[i] - 'a'] == -1)
            present[str2[i] - 'a'] = -1;

        // else mark its presence as 2
        else
            present[str2[i] - 'a'] = 2;
    }

    // print all the uncommon characters
    for (int i = 0; i < MAX_CHAR; i++)
        if (present[i] == 1 || present[i] == 2)
            res.push_back(i + 'a');

    return res;
}

std::optional<unsigned> convertStrToDigit(const std::string& str, const std::map<unsigned, char>& segToChar, const std::map<std::vector<unsigned>, unsigned>& segToDigit)
{
    std::map<char, unsigned> charToSeg;
    for (const auto& m : segToChar)
        charToSeg.insert(std::make_pair(m.second, m.first));

    std::vector<unsigned> segs;
    for (auto i = 0; i < str.length(); ++i) {
        auto it = charToSeg.find(str[i]);
        assert(it != charToSeg.end());
        segs.push_back(it->second);
    }

    for (const auto& segsDigit : segToDigit)
    {
        if (segsDigit.first.size() != segs.size())
            continue;
        unsigned found = 0;
        for (const auto& seg : segsDigit.first) { // loop through vector in map
            if (std::find(segs.begin(), segs.end(), seg) != segs.end())
            {
                ++found;
            }
        }

        if (found == segs.size())
        {
            return segsDigit.second;
        }
    }

    return std::nullopt;
}

int main() {
    std::vector<std::vector<std::string>> signals;
    std::vector<std::vector<std::string>> outputs;
    readData("../data/day8.txt", signals, outputs);
//     std::vector<std::string> testSignal{ "acedgfb", "cdfbe" ,"gcdfa" ,"fbcad" ,"dab", "cefabd", "cdfgeb", "eafb" ,"cagedb" ,"ab" };
//     std::vector<std::string> testOutput{ "cdfeb", "fcadb", "cdfeb","cdbaf" };

    // Part 1
    //std::map<unsigned, unsigned> segmentsToDigits{ { 7, 8}, {4, 4}, {2, 1}, {3, 7} };
    //unsigned count = 0;
    //for (const auto& output : outputs) {
    //    for (auto& segments : output) {
    //        auto it = segmentsToDigits.find((unsigned)segments.size());
    //        if (it != segmentsToDigits.end())
    //            ++count;
    //    }
    //}

    //std::cout << "Count: " << count << std::endl;

    // Part 2
    //  _1_
    //2|   |0 
    //  -3- 
    //6|   |4
    //  -5-

    std::map<std::vector<unsigned>, unsigned> segToDigit;
    std::vector<unsigned> results;
    segToDigit.insert({ {0,1,2,4,5,6},0 });
    segToDigit.insert({ {0,4} ,1 });
    segToDigit.insert({ {0,1,3,5,6},2 });
    segToDigit.insert({ {0,1,3,4,5},3 });
    segToDigit.insert({ {0,2,3,4} ,4 });
    segToDigit.insert({ {1,2,3,4,5} ,5 });
    segToDigit.insert({ {1,2,3,4,5,6} ,6 });
    segToDigit.insert({ {4,0,1},7 });
    segToDigit.insert({ {0,1,2,3,4,5,6},8 });
    segToDigit.insert({ {0,1,2,3,4,5},9 });

    for (auto signal = signals.begin(); signal != signals.end(); ++signal)
    {
        std::map<unsigned, char> segToChar;

        auto twoStr = findStringsOfLength(*signal, 2); // 1
        assert(twoStr.size() == 1);
        auto threeStr = findStringsOfLength(*signal, 3); // 7
        auto fiveStr = findStringsOfLength(*signal, 5); // 2,3,6
        auto fourStr = findStringsOfLength(*signal, 4); // 4
        auto sixStr = findStringsOfLength(*signal, 6); // 0, 6, 9
        auto sevenStr = findStringsOfLength(*signal, 7); // 8
        assert(fiveStr.size() == 3);
        assert(sixStr.size() == 3);
        assert(fourStr.size() == 1);

        // and 1 and 7 and find uncommon
        //auto res = logicalAnd(twoStr.front(), threeStr.front());
        auto not = findAndPrintUncommonChars(threeStr.front(), twoStr.front());
        //auto xOr = segs1 ^ segs7;

        segToChar.insert({ 1, not[0] });

        // and the strings of length 5 with the twoStr (digit 1)
        auto a = logicalAnd(fiveStr[0], twoStr[0]);
        auto b = logicalAnd(fiveStr[1], twoStr[0]);
        auto c = logicalAnd(fiveStr[2], twoStr[0]);


        // find the string of length 2, that identifies the digit 3
        std::string three;
        if (a.length() == 2)
            three = fiveStr[0];
        else if (b.length() == 2)
            three = fiveStr[1];
        else if (c.length() == 2)
            three = fiveStr[2];

        auto threeCopy = three;
        auto threeCopy2 = three;
        three.erase(std::remove(three.begin(), three.end(), not[0]));
        three.erase(std::remove(three.begin(), three.end(), twoStr[0][0]));
        three.erase(std::remove(three.begin(), three.end(), twoStr[0][1]));
        auto threeAndFour = logicalAnd(three, fourStr[0]);
        assert(threeAndFour.size() == 1);
        segToChar.insert({ 3, threeAndFour[0] });

        // find 5th segment
        threeCopy.erase(std::remove(threeCopy.begin(), threeCopy.end(), twoStr[0][0]));
        threeCopy.erase(std::remove(threeCopy.begin(), threeCopy.end(), twoStr[0][1]));
        threeCopy.erase(std::remove(threeCopy.begin(), threeCopy.end(), threeAndFour[0]));
        threeCopy.erase(std::remove(threeCopy.begin(), threeCopy.end(), not[0]));
        assert(threeCopy.size() == 1);
        segToChar.insert({ 5, threeCopy[0] });

        std::string four = fourStr[0];
        std::string fourCopy = fourStr[0];
        four.erase(std::remove(four.begin(), four.end(), twoStr[0][0]));
        four.erase(std::remove(four.begin(), four.end(), twoStr[0][1]));
        four.erase(std::remove(four.begin(), four.end(), threeAndFour[0]));

        segToChar.insert({ 2, four[0] });

        std::string five;
        for (int i = 0; i < 3; ++i)
        {
            if (std::find(fiveStr[i].begin(), fiveStr[i].end(), segToChar.find(2)->second) != fiveStr[i].end())
                five = fiveStr[i];
        }
        auto seg4 = logicalAnd(five, twoStr[0]);
        segToChar.insert({ 4, seg4[0] });

        std::string one = twoStr[0];
        one.erase(std::remove(one.begin(), one.end(), segToChar.find(4)->second));
        segToChar.insert({ 0, one[0] });

        std::string eight = sevenStr[0];
        eight.erase(std::remove(eight.begin(), eight.end(), segToChar.find(0)->second));
        eight.erase(std::remove(eight.begin(), eight.end(), segToChar.find(1)->second));
        eight.erase(std::remove(eight.begin(), eight.end(), segToChar.find(2)->second));
        eight.erase(std::remove(eight.begin(), eight.end(), segToChar.find(3)->second));
        eight.erase(std::remove(eight.begin(), eight.end(), segToChar.find(4)->second));
        eight.erase(std::remove(eight.begin(), eight.end(), segToChar.find(5)->second));

        segToChar.insert({ 6, eight[0] });

        assert(segToChar.size() == 7);
        std::string outputString = "";
        for (unsigned i = 0; i < 4; ++i) {
            outputString += std::to_string(*convertStrToDigit(outputs[std::distance(signals.begin(), signal)][i], segToChar, segToDigit));
        }

        std::cout << "Res: " << outputString << std::endl;
        results.push_back(std::stoi(outputString));
    }

    std::cout << "Sum results: " << std::accumulate(results.cbegin(), results.cend(), 0) << std::endl;

    return 0;
}
