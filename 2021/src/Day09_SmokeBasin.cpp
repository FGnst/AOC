#ifdef NDEBUG
#  define assert(condition) ((void)0)
#else
#  define assert(condition) /*implementation defined*/
#endif

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

class HeightMap
{
public:
    HeightMap()
        : _heights{}
        , _rows{}
        , _cols{}
        , _labels{}
        , _equivalences{}
    {
    }

    void setRowsCols(unsigned rows, unsigned cols) {
        _rows = rows;
        _cols = cols;
    }

    void pushBack(unsigned val) {
        _heights.push_back(val);
    }

    unsigned getData(unsigned i, unsigned j) {
        assert(j < (int)_cols&& i < (int)_rows&& i >= 0 && j >= 0);

        return _heights[*getIndex(i, j)];
    }

    std::map<std::pair<unsigned, unsigned>, unsigned> getMinimaMap() const {
        std::map<std::pair<unsigned, unsigned>, unsigned> map;

        for (unsigned i = 0; i < _rows; ++i)
        {
            for (unsigned j = 0; j < _cols; ++j)
            {
                auto h = _heights[*getIndex(i, j)];
                auto neighs = getNeighborsMap(i, j);
                auto minIt = std::min_element(neighs.begin(), neighs.end(), [](const auto& a, const auto& b) {return a.second < b.second; });
                if (h < minIt->second)
                    map.insert({ { i, j }, h });
            }
        }

        return map;
    }

    void printLabels() {
        std::cout << "**************************\n";

        for (unsigned i = 0; i < _rows; ++i)
        {
            for (unsigned j = 0; j < _cols; ++j)
            {
                std::cout << std::setw(2) << _labels[*getIndex(i, j)] << " ";
            }
            std::cout << "\n";
        }
    }

    void part2() {
        assignLabels();
        updateWithEquivalences();
        auto prod = getProductThreeLargestBasinsSizes();
        std::cout << "Product: " << prod << std::endl;
    }

protected:
    //! return basin label given position of minimum
    unsigned getProductThreeLargestBasinsSizes() {
        auto minima = getMinimaMap();
        std::vector<unsigned> sizes;
        for (const auto& min : minima) {
            unsigned size = 0;
            for (const auto& label : _labels) {
                if (label == _labels[*getIndex(min.first.first, min.first.second)])
                    ++size;
            }

            sizes.push_back(size);
        }

        std::sort(sizes.begin(), sizes.end());

        return std::accumulate(sizes.end() - 3, sizes.end(), 1, std::multiplies<unsigned>());
    }

    void updateWithEquivalences() {
        for (auto mit = _equivalences.rbegin(); mit != _equivalences.rend(); ++mit) {
            auto it = _labels.begin();
            while ((it = std::find(it, _labels.end(), mit->first)) != _labels.end()) {
                auto dist = std::distance(_labels.begin(), it);
                _labels[dist] = mit->second;
                ++it;
            }
        }
    }

    std::optional<int> getWestLabel(unsigned i, unsigned j) {
        if (!getIndex(i, j - 1) || getData(i, j - 1) == 9)
            return std::nullopt;

        return _labels[*getIndex(i, j - 1)];
    }

    std::optional<int> getNorthLabel(unsigned i, unsigned j) {
        if (!getIndex(i - 1, j) || getData(i - 1, j) == 9)
            return std::nullopt;

        return _labels[*getIndex(i - 1, j)];
    }

    void assignLabels() {
        _labels = std::vector<int>(_rows * _cols);
        int label = 0;
        for (unsigned i = 0; i < _rows; ++i)
        {
            for (unsigned j = 0; j < _cols; ++j)
            {
                if (_heights[*getIndex(i, j)] == 9)
                    continue;
                auto north = getNorthLabel(i, j);
                auto west = getWestLabel(i, j);
                if (!north && !west) {
                    _labels[*getIndex(i, j)] = ++label;
                    //_equivalences.insert({ label, label });
                }
                else if (!west && north) {
                    _labels[*getIndex(i, j)] = *north;
                }
                else if (west && !north) {
                    _labels[*getIndex(i, j)] = *west;
                }
                else if (west && north) {
                    if (*west == *north)
                        _labels[*getIndex(i, j)] = *west;
                    else if (*west < *north)
                    {
                        _labels[*getIndex(i, j)] = *west;
                        _equivalences[*north] = *west;
                    }
                    else if (*north < *west) {
                        _labels[*getIndex(i, j)] = *north;
                        _equivalences[*west] = *north;
                    }
                }
            }
        }
    }

    //! returns vector of neighbor indices in _height
    std::map<std::pair<unsigned, unsigned>, unsigned> getNeighborsMap(unsigned i, unsigned j)  const {
        std::map<std::pair<unsigned, unsigned>, unsigned> neighMap;
        auto up = getIndex(i - 1, j);
        if (up)
            neighMap.insert({ {i - 1, j}, _heights[*up] });

        auto down = getIndex(i + 1, j);
        if (down)
            neighMap.insert({ {i + 1, j}, _heights[*down] });

        auto left = getIndex(i, j - 1);
        if (left)
            neighMap.insert({ {i, j - 1}, _heights[*left] });

        auto right = getIndex(i, j + 1);
        if (right)
            neighMap.insert({ {i, j + 1}, _heights[*right] });

        return neighMap;
    }

    std::optional<unsigned> getIndex(int i, int j) const {
        assert(_rows != 0 && _cols != 0);
        if (j >= (int)_cols || i >= (int)_rows || i < 0 || j < 0)
            return std::nullopt;
        return i * _cols + j;
    }

private:
    std::vector<unsigned> _heights;
    unsigned _rows;
    unsigned _cols;
    std::vector<int> _labels;
    std::map<int, int> _equivalences;
};

HeightMap readData(std::string fileName) {
    HeightMap map;

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



int main() {
    HeightMap map = readData("../data/day9.txt");

    auto minima = map.getMinimaMap();

    unsigned riskSum = 0;
    for (const auto& min : minima)
    {
        riskSum += min.second + 1;
    }

    std::cout << "Risk sum: " << riskSum << std::endl;

    map.part2();

    return 0;
}
