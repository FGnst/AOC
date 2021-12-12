#ifdef NDEBUG
#  define assert(condition) ((void)0)
#else
#  define assert(condition) /*implementation defined*/
#endif

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <optional>
#include <sstream>
#include <string>
#include <vector>
#include <map>

class Fish
{
public:
    Fish(int initTimer, unsigned maxTimer)
        :_maxTimer{ maxTimer }
        , _timer{ initTimer }
    {}
    bool createsNewFish() {
        if (_timer == 0) {
            _timer = _maxTimer;
            return true;
        }
        _timer--;
        return false;
    }

    int getTimer() const {
        return _timer;
    }
private:
    unsigned _maxTimer;
    int _timer;
};

int main() {
    std::vector<int> testTimers = { 3,4,3,1,2 };
    std::vector<int> times = { 1,1,5,2,1,1,5,5,3,1,1,1,1,1,1,3,4,5,2,1,2,1,1,1,1,1,1,1,1,3,1,1,5,4,5,1,5,3,1,3,2,1,1,1,1,2,4,1,5,1,1,1,4,4,1,1,1,1,1,1,3,4,5,1,1,2,1,1,5,1,1,4,1,4,4,2,4,4,2,2,1,2,3,1,1,2,5,3,1,1,1,4,1,2,2,1,4,1,1,2,5,1,3,2,5,2,5,1,1,1,5,3,1,3,1,5,3,3,4,1,1,4,4,1,3,3,2,5,5,1,1,1,1,3,1,5,2,1,3,5,1,4,3,1,3,1,1,3,1,1,1,1,1,1,5,1,1,5,5,2,1,5,1,4,1,1,5,1,1,1,5,5,5,1,4,5,1,3,1,2,5,1,1,1,5,1,1,4,1,1,2,3,1,3,4,1,2,1,4,3,1,2,4,1,5,1,1,1,1,1,3,4,1,1,5,1,1,3,1,1,2,1,3,1,2,1,1,3,3,4,5,3,5,1,1,1,1,1,1,1,1,1,5,4,1,5,1,3,1,1,2,5,1,1,4,1,1,4,4,3,1,2,1,2,4,4,4,1,2,1,3,2,4,4,1,1,1,1,4,1,1,1,1,1,4,1,5,4,1,5,4,1,1,2,5,5,1,1,1,5};
    
    std::sort(times.begin(), times.end());

    std::map<std::string, long long> timeGroups;
    for (int i = 0; i < 9; ++i)
        timeGroups.insert({std::to_string(i),0});

    for (const auto& t : times) {
        auto it = timeGroups.find(std::to_string(t));
        if (it != timeGroups.end())
           it->second++;
    }
    
    int totDays = 256;
    for (int i =0; i < totDays; ++i)
    {
        auto zeros = timeGroups["0"];
        for (int j = 0; j < 8; ++j)
        {
            timeGroups[std::to_string(j)] = timeGroups[std::to_string(j + 1)];
        }
        timeGroups["8"] = zeros;
        timeGroups["6"] += zeros;
    }

    long long tot = 0;
    for (const auto& t : timeGroups)
    {
        tot += t.second;
    }

    std::cout << "tot " << tot << std::endl;
    return 0;
}