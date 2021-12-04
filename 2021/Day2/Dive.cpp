#include <fstream>
#include <iosfwd>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

enum Instructions {
    Forward = 0,
    Up = -1,
    Down = 1,
    None= 3
};

std::unordered_map<std::string, Instructions> stringEnum{{"forward", Instructions::Forward},{"up", Instructions::Up},{"down", Instructions::Down}};

bool readData(const std::string& fileName, std::vector<std::pair<Instructions, unsigned>>& data) {
    std::ifstream dataF(fileName);
    std::string instructionString = "";
    if (!dataF.is_open())
        return false;

    while (std::getline(dataF, instructionString))
    {
        if (instructionString.empty())
            continue;
        std::stringstream  lineStream(instructionString);
        Instructions inst{};
        unsigned val = 0;
        std::string s = "";
        lineStream >> s;
        lineStream >> val;

        auto valEnum = stringEnum.find(s);
        if(valEnum == stringEnum.end())
            return false;
        data.push_back({valEnum->second, val});
    }
    dataF.close();

    if (data.empty())
        return false;
    return true;
}

int main(){
    std::vector<std::pair<Instructions, unsigned>> data{};
    if(!readData("../../Day2/data.txt", data)) {
        std::cout << "Failed to read data" << std::endl;
        return 5;
    }

    // Part 1
    int horizontal = 0, depth = 0;

    for(auto& d : data){
        switch(d.first){
            case Instructions::Up: {
                depth -= d.second;
                break;
            }
            case Instructions::Down:{
                depth += d.second;
                break;
            }
            case Instructions::Forward:{
                horizontal += d.second;
                break;
            }
            default:
                return -1;
        }
    }
    assert(horizontal >= 0 && depth >= 0);
    std::cout << "H*D: " << horizontal * depth << std::endl;

    horizontal = depth = 0;
    int aim = 0;

    for(auto& d : data){
        switch(d.first){
            case Instructions::Up: {
                aim -= d.second;
                break;
            }
            case Instructions::Down:{
                aim += d.second;
                break;
            }
            case Instructions::Forward:{
                horizontal += d.second;
                depth += aim * d.second;
                break;
            }
            default:
                return -1;
        }
    }

    std::cout << "H*D: " << horizontal * depth << std::endl;

    return 0;
}