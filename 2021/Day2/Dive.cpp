/**
 * - forward X increases the horizontal position by X units.
 * - down X increases the depth by X units.
 * - up X decreases the depth by X units.
 */
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
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

    // Part 2
    /*    In addition to horizontal position and depth, you'll also need to track a third value, aim, which also starts at 0. The commands also mean something entirely different than you first thought:

    down X increases your aim by X units.
    up X decreases your aim by X units.
    forward X does two things:
        It increases your horizontal position by X units.
        It increases your depth by your aim multiplied by X.
    Again note that since you're on a submarine, down and up do the opposite of what you might expect: "down" means aiming in the positive direction.

    Now, the above example does something different:

    forward 5 adds 5 to your horizontal position, a total of 5. Because your aim is 0, your depth does not change.
            down 5 adds 5 to your aim, resulting in a value of 5.
    forward 8 adds 8 to your horizontal position, a total of 13. Because your aim is 5, your depth increases by 8*5=40.
    up 3 decreases your aim by 3, resulting in a value of 2.
    down 8 adds 8 to your aim, resulting in a value of 10.
    forward 2 adds 2 to your horizontal position, a total of 15. Because your aim is 10, your depth increases by 2*10=20 to a total of 60.
    After following these new instructions, you would have a horizontal position of 15 and a depth of 60. (Multiplying these produces 900.)

    Using this new interpretation of the commands, calculate the horizontal position and depth you would have after following the planned course. What do you get if you multiply your final horizontal position by your final depth?

     */

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