#ifdef NDEBUG
#  define assert(condition) ((void)0)
#else
#  define assert(condition) /*implementation defined*/
#endif

#include <bitset>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

const int sizeOfRow = 12;

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

int main(){

    // Part 1
    std::vector<std::bitset<12>> gammaBits;
    if(!readData("../data/day3.txt", gammaBits)) {
        return 5;
    }

    std::vector<std::uint16_t> bits{0,1,2,3,4,5,6,7,8,9,10,11};
    std::bitset<12> gamma{};
    for (auto bit : bits){
        computeGamma(gamma, gammaBits, bit);
    }

    std::cout << "Gamma*Epsilon: " << gamma.to_ulong() * (~gamma).to_ulong() << std::endl;

    return 0;
}