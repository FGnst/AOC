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


struct Vector {
    Vector(int x, int y, int z) {
        _x = x;
        _y = y;
        _z = z;
    }

    int _x;
    int _y;
    int _z;

    Vector operator+(const Vector& right) {
        return Vector{ right._x + _x, right._y + _y, right._z + _z };
    }

    Vector operator-(const Vector& right) {
        return Vector{ _x - right._x,  _y - right._y, _z - right._z };
    }

};

struct Point {
    Point(int x, int y, int z) {
        _x = x;
        _y = y;
        _z = z;
    }

    int _x;
    int _y;
    int _z;

    Point operator+(const Point& right) {
        return Point{ right._x + _x, right._y + _y, right._z + _z };
    }

    Vector operator-(const Point& right) {
        return Vector{ _x - right._x,  _y - right._y, _z - right._z };
    }
};


struct Scanner {
    std::vector<Point> _points;
    std::vector<Vector> _vectorsBetweenPoints;
};

std::vector<std::function<Point(Point&)>> trafos{
    [](const Point& p) {return Point{p._x, p._y, p._z}; }, // 1 x 
    [](const Point& p) {return Point{p._x, -p._y, -p._z}; }, // 2 
    [](const Point& p) {return Point{p._x, p._z, -p._y}; }, // 3 
    [](const Point& p) {return Point{p._x, -p._z, p._y}; }, // 4 
    [](const Point& p) {return Point{-p._x, p._y, -p._z}; }, // 5 -x
    [](const Point& p) {return Point{-p._x, -p._y, p._z}; }, // 6 
    [](const Point& p) {return Point{-p._x, p._z, p._y}; }, // 7 
    [](const Point& p) {return Point{-p._x, -p._z, -p._y}; }, // 8 
    [](const Point& p) {return Point{p._y, -p._x, p._z}; }, // 9 y
    [](const Point& p) {return Point{p._y, p._z, p._x}; }, // 10 
    [](const Point& p) {return Point{p._y, -p._x, p._z}; }, // 11
    [](const Point& p) {return Point{p._y, p._x, -p._z}; }, // 12 
    [](const Point& p) {return Point{-p._y, -p._x, -p._z}; }, // 13 -y
    [](const Point& p) {return Point{-p._y, p._z, p._x}; }, // 14 
    [](const Point& p) {return Point{-p._y, -p._z, p._x}; }, // 15 
    [](const Point& p) {return Point{-p._y, p._x, p._z}; }, // 16 
    [](const Point& p) {return Point{p._z, p._y, -p._x}; }, // 17 z 
    [](const Point& p) {return Point{p._z, -p._x, -p._y}; }, // 18 
    [](const Point& p) {return Point{p._z, -p._y, p._x}; }, // 19 
    [](const Point& p) {return Point{p._z, p._x, -p._y}; }, // 20 
    [](const Point& p) {return Point{-p._z, p._y, p._x}; }, // 21 -z
    [](const Point& p) {return Point{-p._z, -p._y, -p._x}; }, // 22 
    [](const Point& p) {return Point{-p._z, p._x, -p._y}; }, // 23 
    [](const Point& p) {return Point{-p._z, -p._x, p._y}; }, // 24 
};

std::vector<std::function<Point(Point&)>> invTrafos{
    [](const Point& p) {return Point{p._x, p._y, p._z}; }, // 1 xyz 
    [](const Point& p) {return Point{p._x, -p._y, -p._z}; }, // 2 
    [](const Point& p) {return Point{p._x, -p._z, p._y}; }, // 3 
    [](const Point& p) {return Point{p._x, p._z, -p._y}; }, // 4 
    [](const Point& p) {return Point{-p._x, p._y, -p._z}; }, // 5 -xzy
    [](const Point& p) {return Point{-p._x, -p._y, p._z}; }, // 6 
    [](const Point& p) {return Point{-p._x, p._z, p._y}; }, // 7 
    [](const Point& p) {return Point{-p._x, -p._z, -p._y}; }, // 8 
    [](const Point& p) {return Point{-p._y, p._x, p._z}; }, 
    [](const Point& p) {return Point{p._z, p._x, p._y}; }, 
    [](const Point& p) {return Point{p._y, p._x, -p._z}; }, 
    [](const Point& p) {return Point{-p._z, p._x, -p._y}; }, 
    [](const Point& p) {return Point{-p._y, -p._x, -p._z}; }, // 9 yxz
    [](const Point& p) {return Point{-p._z, -p._x, p._y}; }, // 10 
    [](const Point& p) {return Point{p._z, -p._x, -p._y}; }, // 11
    [](const Point& p) {return Point{p._y, -p._x, p._z}; }, // 12 
    [](const Point& p) {return Point{-p._z, p._y, p._x}; }, // 13 yzx
    [](const Point& p) {return Point{-p._y, -p._z, p._x}; }, // 14 
    [](const Point& p) {return Point{p._z, -p._y, p._x}; }, // 15 
    [](const Point& p) {return Point{p._y, p._z, p._x}; }, // 16 
    [](const Point& p) {return Point{p._z, p._y, -p._x}; }, // 17 zxy 
    [](const Point& p) {return Point{-p._z, -p._y, -p._x}; }, // 18 
    [](const Point& p) {return Point{p._y, -p._z, -p._x}; }, // 19 
    [](const Point& p) {return Point{-p._y, p._z, -p._x}; }, // 20 
};

int main() {

    return 0;
}
