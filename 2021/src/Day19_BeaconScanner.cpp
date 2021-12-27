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

    bool operator==(const Vector& vec) {
        return vec._x == this->_x && vec._y == this->_y && vec._z == this->_z;
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

std::vector<std::function<Vector(Vector&)>> trafos{
    [](const Vector& p) {return Vector{p._x, p._y, p._z}; }, // 0
    [](const Vector& p) {return Vector{p._x, -p._y, -p._z}; }, // 1 
    [](const Vector& p) {return Vector{p._x, p._z, -p._y}; }, // 2
    [](const Vector& p) {return Vector{p._x, -p._z, p._y}; }, // 3 
    [](const Vector& p) {return Vector{-p._x, p._y, -p._z}; }, // 4 -x
    [](const Vector& p) {return Vector{-p._x, -p._y, p._z}; }, // 5 
    [](const Vector& p) {return Vector{-p._x, p._z, p._y}; }, // 6 
    [](const Vector& p) {return Vector{-p._x, -p._z, -p._y}; }, // 7 
    [](const Vector& p) {return Vector{p._y, -p._x, p._z}; }, // 8 y
    [](const Vector& p) {return Vector{p._y, p._z, p._x}; }, // 9 
    [](const Vector& p) {return Vector{p._y, p._x, -p._z}; }, // 10
    [](const Vector& p) {return Vector{p._y, -p._z, -p._x}; }, // 11 
    [](const Vector& p) {return Vector{-p._y, -p._x, -p._z}; }, // 12 -y
    [](const Vector& p) {return Vector{-p._y, p._z, -p._x}; }, // 13 
    [](const Vector& p) {return Vector{-p._y, -p._z, p._x}; }, // 14 
    [](const Vector& p) {return Vector{-p._y, p._x, p._z}; }, // 15 
    [](const Vector& p) {return Vector{p._z, p._y, -p._x}; }, // 16 z 
    [](const Vector& p) {return Vector{p._z, -p._x, -p._y}; }, // 17 
    [](const Vector& p) {return Vector{p._z, -p._y, p._x}; }, // 18 
    [](const Vector& p) {return Vector{p._z, p._x, p._y}; }, // 19 
    [](const Vector& p) {return Vector{-p._z, p._y, p._x}; }, // 20 -z
    [](const Vector& p) {return Vector{-p._z, -p._y, -p._x}; }, // 21 
    [](const Vector& p) {return Vector{-p._z, p._x, -p._y}; }, // 22
    [](const Vector& p) {return Vector{-p._z, -p._x, p._y}; }, // 23 
};

std::vector<std::function<Vector(Vector&)>> invTrafos{
    [](const Vector& p) {return Vector{p._x, p._y, p._z}; }, // 0
    [](const Vector& p) {return Vector{p._x, -p._y, -p._z}; }, // 1 
    [](const Vector& p) {return Vector{p._x, -p._z, p._y}; }, // 2
    [](const Vector& p) {return Vector{p._x, p._z, -p._y}; }, // 3 
    [](const Vector& p) {return Vector{-p._x, p._y, -p._z}; }, // 4 -xzy
    [](const Vector& p) {return Vector{-p._x, -p._y, p._z}; }, // 5 
    [](const Vector& p) {return Vector{-p._x, p._z, p._y}; }, // 6 
    [](const Vector& p) {return Vector{-p._x, -p._z, -p._y}; }, // 7 
    [](const Vector& p) {return Vector{-p._y, p._x, p._z}; }, // 8
    [](const Vector& p) {return Vector{p._z, p._x, p._y}; }, // 9
    [](const Vector& p) {return Vector{p._y, p._x, -p._z}; }, // 10
    [](const Vector& p) {return Vector{-p._z, p._x, -p._y}; }, //11
    [](const Vector& p) {return Vector{-p._y, -p._x, -p._z}; }, //12
    [](const Vector& p) {return Vector{-p._z, -p._x, p._y}; }, //13
    [](const Vector& p) {return Vector{p._z, -p._x, -p._y}; }, //14
    [](const Vector& p) {return Vector{p._y, -p._x, p._z}; }, //15
    [](const Vector& p) {return Vector{-p._z, p._y, p._x}; }, //16
    [](const Vector& p) {return Vector{-p._y, -p._z, p._x}; }, //17
    [](const Vector& p) {return Vector{p._z, -p._y, p._x}; }, //18
    [](const Vector& p) {return Vector{p._y, p._z, p._x}; }, //19
    [](const Vector& p) {return Vector{p._z, p._y, -p._x}; }, //20
    [](const Vector& p) {return Vector{-p._z, -p._y, -p._x}; }, //21
    [](const Vector& p) {return Vector{p._y, -p._z, -p._x}; }, //22
    [](const Vector& p) {return Vector{-p._y, p._z, -p._x}; }, //23
};

void testInverse() {
    Vector v{ 2, -3, 1 };
    for (unsigned i = 0; i < trafos.size(); ++i)
    {
        assert(invTrafos[i](trafos[i](v)) == v);
    }
}

std::vector<Scanner> readData(const std::string& fileName) {
    std::fstream f(fileName);
    std::vector<Scanner> scanners;
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(f, line)) {
        lines.push_back(line);
    }

    Scanner s;
    for (const auto& l : lines) {
        if (l.empty()) {
            scanners.push_back(s);
        }
        else if (l.substr(0, 3) == "---") {
            s._points.clear();
        }
        else {
            std::stringstream ss(l);
            int x = 0;
            int y = 0;
            int z = 0;
            char comma = 0;
            ss >> x;
            ss >> comma;
            ss >> y;
            ss >> comma;
            ss >> z;
            s._points.emplace_back(x, y, z);
        }
    }
    scanners.push_back(s);

    return scanners;
}

void setVectors(std::vector<Scanner>& scanners) {
    for (auto& s : scanners)
    {
        for (auto it = s._points.begin(); it != s._points.end() - 1; ++it) {
            for (auto it2 = it + 1; it2 != s._points.end(); ++it2)
                s._vectorsBetweenPoints.emplace_back(*(it2)-*it);
        }
    }
}

int main() {
    auto scanners = readData("../data/day19_test.txt");
    setVectors(scanners);

    testInverse();
    return 0;
}
