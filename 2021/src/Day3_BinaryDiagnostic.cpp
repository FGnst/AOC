#ifdef NDEBUG
#  define assert(condition) ((void)0)
#else
#  define assert(condition) /*implementation defined*/
#endif

#include <algorithm>
#include <bitset>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

bool readData(const std::string& fileName, std::vector<std::bitset<12>>& gammaBits) {
    std::ifstream dataF(fileName);
    std::string dataS = "";
    if (!dataF.is_open())
        return false;

    while (std::getline(dataF, dataS))
    {
        if (dataS.empty())
            continue;

        std::bitset<12> val(dataS);
        gammaBits.push_back(val);
    }
    dataF.close();

    return (!gammaBits.empty());
}

void computeGamma(std::bitset<12>& coeff, const std::vector<std::bitset<12>>& values, std::uint16_t bit){
    unsigned bitSum = 0;
    for (const auto& g : values){
        bitSum += g[bit];
    }
    assert(bitSum != values.size() / 2); // otherwise there is no majority
    if(bitSum >  values.size() / 2)
        coeff[bit] = true;
}

void filterByBit(std::vector<std::bitset<12>>& values, unsigned bitPos, const std::string& type){
    if(values.size() == 1)
        return;
    auto it = std::partition(values.begin(), values.end(), [&bitPos](auto n) { return n[bitPos] == 1; });
    assert(it != values.end());
    unsigned long long sizeOnes = std::distance(values.begin(), it);
    auto sizeZeros = values.size() - sizeOnes;
    if (type == "OX"){
        if(sizeOnes >= sizeZeros)
            values.erase(it, values.end());
        else
            values.erase(values.begin(),it);
    }
    else if (type == "CO2") {
        if (sizeOnes >= sizeZeros)
            values.erase(values.begin(), it);
        else
            values.erase(it, values.end());
    }
}

void filterByBit(std::vector<std::bitset<5>>& values, unsigned bitPos, const std::string& type){
    if(values.size() == 1)
        return;
    auto it = std::partition(values.begin(), values.end(), [&bitPos](auto n) { return n[bitPos] == 1; });
    assert(it != values.end());
    unsigned long long sizeOnes = std::distance(values.begin(), it);
    auto sizeZeros = values.size() - sizeOnes;
    if (type == "OX"){
        if(sizeOnes >= sizeZeros)
            values.erase(it, values.end());
        else
            values.erase(values.begin(),it);
    }
    else if (type == "CO2") {
        if (sizeOnes >= sizeZeros)
            values.erase(values.begin(), it);
        else
            values.erase(it, values.end());
    }
}

std::pair<int, int> extractGeneratorRating(const std::vector<std::bitset<5>>& data){
    std::vector<unsigned> bits;
    for(int i = int(data[0].size()) - 1; i >= 0; --i)
        bits.push_back(unsigned(i));

    auto oxygen(data);
    auto co2(data);
    for (auto bit : bits){
        filterByBit(oxygen, bit, "OX");
    }
    for (auto bit : bits) {
        filterByBit(co2, bit, "CO2");
    }
    assert(oxygen.size() == 1 && co2.size() == 1);
    return {oxygen.front().to_ulong(), co2.front().to_ulong()};
}


std::pair<int, int> extractGeneratorRating(const std::vector<std::bitset<12>>& data){
    std::vector<unsigned> bits;
    for(int i = int(data[0].size() - 1); i >= 0; --i)
        bits.push_back(unsigned(i));

    auto oxygen = data;
    auto co2 = data;
    for (auto bit : bits){
        filterByBit(oxygen, bit, "OX");
    }
    for (auto bit : bits) {
        filterByBit(co2, bit, "CO2");
    }
    assert(oxygen.size() == 1 && co2.size() == 1);
    return {oxygen.front().to_ulong(), co2.front().to_ulong()};
}

bool doTest(){
    std::vector<std::string> testDataString{"00100",
                                            "11110",
                                            "10110",
                                            "10111",
                                            "10101",
                                            "01111",
                                            "00111",
                                            "11100",
                                            "10000",
                                            "11001",
                                            "00010",
                                            "01010"};

    std::vector<std::bitset<5>> testData;
    for (const auto& s : testDataString)
        testData.push_back(std::bitset<5>(s));

    auto [ox, co2] = extractGeneratorRating(testData);
    return (ox == 23 && co2 == 10);
}

int main(){

    // Part 1
    std::vector<std::bitset<12>> data;
    if(!readData("../data/day3.txt", data)) {
        return 5;
    }

 //   std::vector<std::uint16_t> bits{0,1,2,3,4,5,6,7,8,9,10,11};
/*    std::bitset<12> gamma{};
    for (auto bit : bits){
        computeGamma(gamma, gammaBits, bit);
    }

    std::cout << "Gamma*Epsilon: " << gamma.to_ulong() * (~gamma).to_ulong() << std::endl;*/

    // Part 2
    assert(doTest());

    auto [ox, co2] = extractGeneratorRating(data);

    std::cout << "Ox*Co2 " << ox * co2 << std::endl;


    return 0;
}
