#include <algorithm>
#include <cassert>
#include <chrono>
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
#include <thread>
#include <vector>
#include <stack>
#include <unordered_set>
#include <queue>

struct Vector {
    Vector(int x, int y, int z) {
        _x = x;
        _y = y;
        _z = z;
    }

    int _x;
    int _y;
    int _z;

    Vector operator+(const Vector& right) const {
        return Vector{ right._x + _x, right._y + _y, right._z + _z };
    }

    Vector operator-(const Vector& right) const {
        return Vector{ _x - right._x,  _y - right._y, _z - right._z };
    }

    bool operator==(const Vector& vec) const {
        return vec._x == this->_x && vec._y == this->_y && vec._z == this->_z;
    }

    bool operator!=(const Vector& vec) const {
        return !(*this == vec);
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector& p);
};

using Vectors = std::vector<Vector>;

struct Point {
    Point(int x, int y, int z) {
        _x = x;
        _y = y;
        _z = z;
    }

    Point(const Vector& v) {
        _x = v._x;
        _y = v._y;
        _z = v._z;
    }

    int _x;
    int _y;
    int _z;

    Point operator+(const Point& right) const {
        return Point{ right._x + _x, right._y + _y, right._z + _z };
    }


    Point operator+(const Vector& vec) const {
        return Point{ vec._x + _x, vec._y + _y, vec._z + _z };
    }

    bool operator==(const Point& other) const {
        return this->_x == other._x && this->_y == other._y && this->_z == other._z;
    }

    bool operator!=(const Point& p) const {
        return !(*this == p);
    }

    Vector operator-(const Point& right) const {
        return Vector{ _x - right._x,  _y - right._y, _z - right._z };
    }

    friend std::ostream& operator<<(std::ostream& os, const Point& p);
};

std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << p._x << " " << p._y << " " << p._z;
    return os;
}

std::ostream& operator<<(std::ostream& os, const Vector& v) {
    os << v._x << " " << v._y << " " << v._z;
    return os;
}

struct Hash {
    unsigned operator()(const Point& p) const
    {
        return p._x + 1000 * p._y + 1000 * p._z;
    }
};

using Points = std::vector<Point>;

Point origin() {
    return Point(0, 0, 0);
}


std::vector<std::function<Vector(const Vector&)>> trafos{
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

std::vector<std::function<Vector(const Vector&)>> invTrafos{
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

struct Connection {
    unsigned sInd1;
    unsigned sInd2;
    unsigned trInd;
    std::unordered_map<Point, Point, Hash> pointsMap;

    Points transform() {
        Points res;
        for (const auto& pointPair : pointsMap)
            res.push_back(trafos[trInd](pointPair.second - origin()));

        return res;
    }

    Points invTransform() {
        Points res;
        for (const auto& pointPair : pointsMap)
            res.push_back(invTrafos[trInd](pointPair.first - origin()));

        return res;
    }
};

struct Scanner {
    Points _points;
    std::unordered_map<Point, std::vector<Vector>, Hash> _pointVectorMap;
    Point _positionGlobalFrame{ 0,0,0 };
};

void testInverse() {
    Vector v{ std::rand(), std::rand(), std::rand() };
    for (unsigned i = 0; i < trafos.size(); ++i)
    {
        assert(invTrafos[i](trafos[i](v)) == v);
    }
}

void testUnique() {
    Vector v{ std::rand(), std::rand(), std::rand() };
    for (unsigned i = 0; i < trafos.size(); ++i)
    {
        for (unsigned j = 0; j < trafos.size(); ++j) {
            if (i == j)
                continue;
            assert(trafos[i](v) != trafos[j](v));
        }

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

    Scanner s{};
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
        for (auto it1 = s._points.begin(); it1 != s._points.end(); ++it1) {
            for (auto it2 = s._points.begin(); it2 != s._points.end(); ++it2) {
                if (it1 == it2)
                    continue;
                auto it = s._pointVectorMap.find(*it1);
                if (it != s._pointVectorMap.end())
                    it->second.emplace_back(*it2 - *it1);
                else
                    s._pointVectorMap.insert({ *it1, {*it2 - *it1} });
            }
        }
    }
}

bool checkTrafo(const Vectors& vectors1, const Vectors& vectors2, unsigned& trafoIndex) {
    for (auto trIt = trafos.begin(); trIt != trafos.end(); ++trIt) {
        unsigned matches = 0;
        for (const auto& v1 : vectors1) {
            for (const auto& v2 : vectors2)
            {
                if ((*trIt)(v2) == v1)
                    ++matches;
            }
        }
        if (matches >= 11) {
            //std::cout << "      Matches " << matches << " with trafo " << std::distance(trafos.begin(), trIt) << std::endl;
            trafoIndex = (unsigned)std::distance(trafos.begin(), trIt);
            return true;
        }
    }

    return false;
}

std::unordered_map<Point, Point, Hash> findCommonPoints(Scanner& s1, Scanner& s2, unsigned& trafoIndex) {

    std::unordered_map<Point, Point, Hash> res;
    for (const auto& v1 : s1._pointVectorMap)
    {
        auto pointS1 = v1.first;
        //std::cout << "Point1 " << pointS1 << std::endl;
        for (const auto& v2 : s2._pointVectorMap) {
            auto pointS2 = v2.first;
            //std::cout << "  Point2 " << pointS2 << std::endl;
            if (checkTrafo(v1.second, v2.second, trafoIndex)) {
                res.insert({ pointS1, pointS2 });
            }
        }
    }

    return res;
}

void transformPoints(Scanner& s, int trafoIndex) {
    auto trafo = trafos[trafoIndex];
    std::unordered_map<Point, std::vector<Vector>, Hash> pointVectorMap;
    Points points;
    for (auto mapIt = s._pointVectorMap.begin(); mapIt != s._pointVectorMap.end(); ++mapIt) {
        auto newPoint = s._positionGlobalFrame + trafo(mapIt->first - origin());
        points.push_back(newPoint);
        Vectors vecs;
        for (Vector& v : mapIt->second) {
            vecs.emplace_back(trafo(v));
        }

        pointVectorMap.insert({ newPoint, vecs });
    }
    s._points = points;
    s._pointVectorMap = pointVectorMap;
}

void computeScannerCoordinateInGlobalFrame(Scanner& scanner2, const Point& p1, const Point& p2, int trIndex) {
    auto positionOfScanner2RelativeToScanner1 = (p1 - origin()) - trafos[trIndex](p2 - origin());
    std::cout << "Position " << positionOfScanner2RelativeToScanner1 << std::endl;
    scanner2._positionGlobalFrame = positionOfScanner2RelativeToScanner1;
}

int manhattanDistance(const Point& p1, const Point& p2) {
    return std::abs(p1._x - p2._x) + std::abs(p1._y - p2._y) + std::abs(p1._z - p2._z);
}


std::unordered_set<Point, Hash> matchScanners(std::vector<Scanner>& scanners) {
    std::unordered_set<Point, Hash> allBeacons; // set of all beacons (a set because we do not want duplicates)
    std::unordered_set<unsigned> matched; // set of all scanner indices that have been matched
    std::queue<unsigned> toMatchAgainst; // queue containing scanner indices that have been matched in the last loop cycle and need to be matched against
    toMatchAgainst.push(0);
    matched.insert(0);
    // scanner 0 is the reference, insert all its beacons
    for (const auto& p : scanners[0]._points)
    {
        allBeacons.insert(p);
    }
    while (!toMatchAgainst.empty()) {
        auto indToMatch = toMatchAgainst.front();
        auto& toMatch = scanners[indToMatch];
        toMatchAgainst.pop();
        for (unsigned scannerId = 0; scannerId < scanners.size(); ++scannerId)
        {
            if (scannerId == indToMatch || std::find(matched.begin(), matched.end(), scannerId) != matched.end())
                continue;

            unsigned trIndex = 0;
            auto commonMap = findCommonPoints(toMatch, scanners[scannerId], trIndex);
            if (!commonMap.empty()) {
                matched.insert(scannerId);
                std::cout << "Link: " << indToMatch << ", " << scannerId << ", # el: " << commonMap.size() << std::endl;

                computeScannerCoordinateInGlobalFrame(scanners[scannerId], commonMap.begin()->first, commonMap.begin()->second, trIndex);
                transformPoints(scanners[scannerId], trIndex);

                // insert tranformed points in allBeacons set, log if already inserted for debugging
                for (const auto& p : scanners[scannerId]._points) {
                    if (allBeacons.insert(p).second == false) {
                        std::cout << p << " already inserted. \n";
                    }
                }

                toMatchAgainst.push(scannerId);
            }

        }
    }

    return allBeacons;
}

void run(std::vector<Scanner>& scanners) {
    using namespace std::chrono_literals;

    setVectors(scanners);

    //Part 1
    auto allBeacons = matchScanners(scanners);
    std::cout << "Number of beacons: " << allBeacons.size() << std::endl;

    std::this_thread::sleep_for(4000ms);

    // Part 2
    std::vector<int> distances;
    for (unsigned i = 0; i < scanners.size() - 1; ++i)
    {
        for (unsigned j = i + 1; j < scanners.size(); ++j)
        {
            auto dist = manhattanDistance(scanners[i]._positionGlobalFrame, scanners[j]._positionGlobalFrame);
            
            std::cout << "Distance " << i << ", " << j << ": " << dist << std::endl;
            
            distances.push_back(dist);
        }
    }

    std::cout << "Max dist: " << *std::max_element(distances.cbegin(), distances.cend()) << std::endl;
}

int main() {
    auto scanners = readData("../data/day19.txt");
    run(scanners);
    return 0;
}
