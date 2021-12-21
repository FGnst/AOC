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

class RiskMap
{
public:
    RiskMap()
        : _risk{}
        , _unvisitedNodes{}
        , _shortestPath{}
        , _prevNodes{}
        , _rows{}
        , _cols{}
    {
    }

    void setData(unsigned i, unsigned j, unsigned val) {
        assert(j < _cols&& i < _rows&& i >= 0 && j >= 0);
        _risk[*getIndex(i, j)] = val;
    }

    void initRisk() {
        for (unsigned i = 0; i < _cols * _rows; ++i)
        {
            _risk.push_back(0);
        }
    }

    unsigned getRows() const {
        return _rows;
    }

    unsigned getCols() const {
        return _cols;
    }

    void setRowsCols(unsigned rows, unsigned cols) {
        _rows = rows;
        _cols = cols;
        for (unsigned i = 0; i < _rows * _cols; ++i)
        {
            _unvisitedNodes.push_back(i);
            _shortestPath.push_back(UINT_MAX);
            _prevNodes.push_back(0);
        }
    }

    void pushBack(unsigned val) {
        _risk.push_back(val);
    }

    unsigned getData(unsigned i, unsigned j) const {
        assert(j < (int)_cols&& i < (int)_rows&& i >= 0 && j >= 0);

        return _risk[*getIndex(i, j)];
    }

    std::optional<unsigned> getIndex(int i, int j) const {
        assert(_rows != 0 && _cols != 0);
        if (j >= (int)_cols || i >= (int)_rows || i < 0 || j < 0)
            return std::nullopt;

        return i * _cols + j;
    }

    std::vector<unsigned> getNeighsIndices(int i, int j) {
        std::vector<unsigned> neighs;
        auto up = getIndex(i - 1, j);
        auto down = getIndex(i + 1, j);
        auto left = getIndex(i, j - 1);
        auto right = getIndex(i, j + 1);
        if (up) {
            neighs.push_back(*up);
        }
        if (down) {
            neighs.push_back(*down);
        }
        if (left) {
            neighs.push_back(*left);
        }
        if (right) {
            neighs.push_back(*right);
        }

        return neighs;
    }

    unsigned findShortestPath(std::vector<unsigned>& shortestPath) {
        _shortestPath[0] = 0;
        while (!_unvisitedNodes.empty())
        {
            std::cout << "Size unvisited" << _unvisitedNodes.size() << std::endl;
            auto min_node = *std::min_element(_unvisitedNodes.begin(), _unvisitedNodes.end(), [this](const auto& a, const auto& b) {return _shortestPath[a] < _shortestPath[b]; });
            unsigned i = min_node / _rows, j = min_node % _cols;
            auto neighs = getNeighsIndices(i, j);
            for (auto neigh : neighs) {
                auto tentative = _risk[neigh] + _shortestPath[min_node];
                if (tentative < _shortestPath[neigh]) {
                    _prevNodes[neigh] = min_node;
                    _shortestPath[neigh] = tentative;
                }
            }
            _unvisitedNodes.erase(std::remove(_unvisitedNodes.begin(), _unvisitedNodes.end(), min_node));
        }

        unsigned totRisk = 0;
        unsigned thisNode = _cols * _rows - 1;
        while (thisNode != 0) {
            shortestPath.push_back(thisNode);
            thisNode = _prevNodes[thisNode];
            totRisk += _risk[thisNode];
        }
        return _shortestPath[_cols * _rows - 1];
    }


private:
    std::vector<unsigned> _risk;
    std::vector<unsigned> _unvisitedNodes;
    std::vector<unsigned> _shortestPath;
    std::vector<unsigned> _prevNodes;
    unsigned _rows;
    unsigned _cols;
};

RiskMap readData(std::string fileName) {
    RiskMap map;

    std::ifstream dataFile(fileName);
    std::string line;
    unsigned countLines = 0;
    size_t lineSize = 0;
    while (std::getline(dataFile, line)) {
        ++countLines;
        lineSize = line.length();
        for (const auto& val : line)
            map.pushBack(static_cast<unsigned>(val - '0'));
    }

    map.setRowsCols(countLines, (unsigned)lineSize);

    return map;
}

void printPath(const std::vector<unsigned>& path, const RiskMap& map) {
    for (unsigned i = 0; i < map.getRows(); ++i)
    {
        for (unsigned j = 0; j < map.getCols(); ++j)
        {
            auto ind = map.getIndex(i, j);
            assert(ind.has_value());

            if (std::find(path.begin(), path.end(), *ind) != path.end())
                std::cout << "*";
            else
                std::cout << map.getData(i, j);
        }
        std::cout << "\n";
    }
}

void printMap(const RiskMap& map) {
    for (unsigned i = 0; i < map.getRows(); ++i)
    {
        for (unsigned j = 0; j < map.getCols(); ++j)
        {
            auto ind = map.getIndex(i, j);
            assert(ind.has_value());
            std::cout << map.getData(i, j);
        }
        std::cout << "\n";
    }
}

RiskMap extendMap(const RiskMap& map) {
    RiskMap newMap;
    auto newRows = map.getRows() * 5;
    auto newCols = map.getCols() * 5;
    newMap.setRowsCols(newRows, newCols);
    newMap.initRisk();

    for (unsigned i = 0; i < map.getRows(); ++i)
    {
        for (unsigned j = 0; j < map.getCols(); ++j)
        {
            auto index = map.getIndex(i, j);
            for (unsigned k = 0; k < 5; ++k)
            {
                for (unsigned q = 0; q < 5; ++q)
                {
                    auto newIndex = map.getData(i, j) + q + k;
                    if (newIndex > 9)
                        newIndex = newIndex % 9;
                    newMap.setData(i + k * map.getRows(), j + q * map.getCols(), newIndex);
                }
            }
        }
    }

    return newMap;
}

int main() {

    auto map = readData("../data/day15.txt");
    //auto mapTest = readData("../data/day15_test2.txt");

    //printMap(map);

    auto newMap = extendMap(map);

    // printMap(newMap);


    std::vector<unsigned> shortestPath;

    auto totRisk = newMap.findShortestPath(shortestPath);

    // printPath(shortestPath, newMap);

    std::cout << "Tot risk: " << totRisk << std::endl;

    return 0;
}
