#include <algorithm>
#include <bitset>
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

struct Target {
    int minX;
    int minY;
    int maxX;
    int maxY;
};

struct Variables {
    void setVars(int x, int y, int vx, int vy) {
        _x = x;
        _y = y;
        _vX = vx;
        _vY = vy;
    }

    int _x;
    int _y;
    int _vX;
    int _vY;
};


void evolve(Variables& vars, int& Ymax) {
    vars._x += vars._vX;
    vars._y += vars._vY;
    if (vars._vX > 0)
        vars._vX -= 1;
    else if (vars._vX < 0)
        vars._vX += 1;
    vars._vY -= 1;

    if (vars._y > Ymax)
        Ymax = vars._y;
}

bool intersectsTarget(const Variables& var, const Target& target) {
    return var._x >= target.minX && var._x <= target.maxX && var._y >= target.minY && var._y <= target.maxY;
}

bool willIntersect(Variables var, const Target& target, int& yMax, std::pair<int,int>& velocity) {
    int ymax = 0;
    while (true) {
        evolve(var, ymax);

        if (intersectsTarget(var, target)) {
            yMax = ymax;
            velocity = {var._vX, var._vY};
            return true;
        }
        else if (var._x > target.maxX)
            return false; // overshoot
        else if (var._y < target.minY) // undershoot
            return false;
    }
}

int main() {
    Target target;

    target.minX = 206;
    target.maxX = 250;
    target.minY = -105;
    target.maxY = -57;

    int vYmin = 2*target.minY;
    int vYmax = target.maxX * 2;
    int vXmax = 20*target.maxX;
    int vXmin = 1;
    for (int i = 1; i < target.maxX; ++i)
    {
        if (i * i >= 2 * target.minX) {
            vXmin = i-1;
            break;
        }
    }

    std::vector<std::pair<int, int>> velocities;
    int yMaxTot = 0;
    for (int y = vYmin; y < vYmax ; ++y)
    {
    	for (int x = vXmin; x < vXmax ; ++x)
    	{
            std::pair<int, int> vel;
            int Ymax = 0;
            Variables var{};
            var.setVars(0, 0, x, y);
            if (willIntersect(var, target, Ymax, vel)) {
                velocities.push_back(vel);
                if (Ymax > yMaxTot)
                    yMaxTot = Ymax;
            }
               
    	}
    }

    std::cout << "YMax " << yMaxTot << std::endl;

    return 0;
}
