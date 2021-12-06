#ifdef NDEBUG
#  define assert(condition) ((void)0)
#else
#  define assert(condition) /*implementation defined*/
#endif

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

using Line = std::vector<std::pair<unsigned, unsigned>>;

Line createLine(const std::pair<unsigned, unsigned> start, const std::pair<unsigned, unsigned> end) {
    assert(start != end);

    Line l{};

    int dX = (end.first - start.first);
    dX = dX >= 0 ? ((dX == 0) ? 0 : 1) : -1;
    int dY = end.second - start.second;
    dY = dY >= 0 ? ((dY == 0) ? 0 : 1) : -1;

    l.emplace_back(start);
    std::pair<unsigned, unsigned> newStart{ start.first + dX, start.second + dY };
    while (newStart != end) {
        l.emplace_back(newStart);
        newStart.first = newStart.first + dX;
        newStart.second = newStart.second + dY;
    }

    l.emplace_back(end);

    assert(l.size() > 1);

    return l;
}

class Diagram
{
public:
    Diagram(unsigned rows, unsigned cols)
        : _data{}
        , _rows{ rows }
        , _cols{ cols }
    {
        for (unsigned i = 0; i < _rows * _cols; ++i)
            _data.emplace_back(0);
    }

    void addLine(const Line& l, bool print = false) {
        assert(l.size() > 1);
        // Part 1
        // if (!isVerticalOrHorizontal(l))
        // return;

        for (const auto& point : l) {


            ++_data[getIndex(point.first, point.second)];
        }

        if (print)
            printDiagram();
    }

    void setData(unsigned i, unsigned j, unsigned val) {
        assert(i < _rows&& j < _cols);
        _data[getIndex(i, j)] = val;
    }

    unsigned getData(unsigned i, unsigned j) const {
        return _data[getIndex(i, j)];
    }

    void printDiagram() {
        std::cout << "**************" << std::endl;
        for (unsigned i = 0; i < _rows; ++i)
        {

            for (unsigned j = 0; j < _cols; ++j)
            {
                std::cout << _data[getIndex(i, j)] << " ";
            }
            std::cout << "\n";
        }
    }

protected:
    unsigned getIndex(unsigned i, unsigned j) const {
        return _cols * i + j;
    }

    bool isVerticalOrHorizontal(const Line& l) {
        return l.front().first == l.back().first || l.front().second == l.back().second;
    }

private:
    std::vector<unsigned> _data;
    unsigned _rows;
    unsigned _cols;
};

bool readData(const std::string& fileName, std::vector<Line>& lines) {
    std::ifstream dataF(fileName);
    std::string instructionString = "";
    if (!dataF.is_open())
        return false;

    while (std::getline(dataF, instructionString))
    {
        if (instructionString.empty()) {
            continue;
        }

        char ch;
        std::stringstream  ss(instructionString);
        std::pair<unsigned, unsigned> start{};
        std::pair<unsigned, unsigned> end{};
        ss >> start.first;
        ss >> ch; //,
        ss >> start.second;
        ss >> ch; // -
        ss >> ch; // >
        ss >> end.first;
        ss >> ch; //,
        ss >> end.second;

        auto line = createLine(start, end);
        if (!line.empty())
            lines.push_back(line);
    }

    dataF.close();

    return true;
}

int main() {
    std::vector<Line> lines{};
    readData("../data/day5.txt", lines);

    unsigned maxRow = 0;
    unsigned maxCol = 0;
    for (const auto& line : lines) {
        if (line.front().first > maxRow)
            maxRow = line.front().first;
        if (line.front().second > maxCol)
            maxCol = line.front().second;
        if (line.back().second > maxCol)
            maxCol = line.back().second;
        if (line.back().first > maxRow)
            maxRow = line.back().first;
    }


    Diagram diag{ maxRow + 1, maxCol + 1 };
    for (const auto& l : lines)
        diag.addLine(l);

    unsigned countOverlaps = 0;
    for (unsigned i = 0; i <= maxRow; ++i)
    {
        for (unsigned j = 0; j <= maxCol; ++j) {
            if (diag.getData(i, j) >= 2)
                countOverlaps++;
        }
    }

    std::cout << "Overlaps: " << countOverlaps << std::endl;

    return 0;
}