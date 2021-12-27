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

class Image {
public:
    Image(int rows, int cols)
        : _data{}
        , _rows{ rows }
        , _cols{ cols }
    {
        _data.resize(_rows* _cols);
    };

    Image(const std::vector<bool>& data, int rows, int cols)
        : _data{ data }
        , _rows{ rows }
        , _cols{ cols }
    {
        _data.resize(_rows* _cols);
    };
    Image(const Image& img)
        : _data{ img._data }
        , _rows{ img._rows }
        , _cols{ img._cols }
    {
    }

    void setData(int i, int j, int val) {
        assert(i >= 0 && i < _rows&& j >= 0 && j < _cols);
        _data[*getIndex(i, j)] = val;
    }

    std::vector<bool> getData() const {
        return _data;
    }

    bool getData(int i, int j) const {
        assert(i >= 0 && i < _rows&& j >= 0 && j < _cols);

        return _data[*getIndex(i, j)];
    }

    bool getDataAlsoOutside(int i, int j) const {
        if (i < 0 || i >= _rows || j < 0 || j >= _cols)
            return false;

        return _data[*getIndex(i, j)];
    }

    inline int getRows() const { return _rows; };
    inline int getCols() const { return _cols; };

    bool operator()(int i, int j) const {
        return getDataAlsoOutside(i, j);
    }

protected:

    std::optional<int> getIndex(int i, int j) const {
        assert(_rows != 0 && _cols != 0);
        if (j >= (int)_cols || i >= (int)_rows || i < 0 || j < 0)
            return std::nullopt;

        return i * _cols + j;
    }

private:
    std::vector<bool> _data;
    int _rows;
    int _cols;

};

Image popBorder(const Image& img) {
    Image newImg(img.getRows() - 2, img.getCols() - 2);
    for (int i = 0; i < img.getRows() ; ++i)
    {
        for (int j = 0; j < img.getCols() ; ++j)
        {
            if ((i > 1 && i < img.getRows() - 1) && (j > 1 && j < img.getCols() -1))
                newImg.setData(i - 1, j - 1, img(i,j));
        }
    }

    return newImg;
}

void addBorderToImage(Image& img, int border, bool lit) {
    Image newImg{ img.getRows() + 2 * border, img.getCols() + 2 * border };
    for (int i = 0; i < newImg.getRows(); ++i)
    {
        for (int j = 0; j < newImg.getCols(); ++j)
        {
            if (i < border || j < border || j - border > img.getCols() - 1 || i - border > img.getRows() - 1) {
                newImg.setData(i, j, lit);
            }
            else
                newImg.setData(i, j, img.getData(i - border, j - border));
        }
    }

    img = newImg;
}


void printImg(const Image& img) {

    std::cout << "*******************************" << std::endl;

    for (int i = 0; i < img.getRows(); ++i)
    {
        for (int j = 0; j < img.getCols(); ++j)
        {
            if (img.getData(i, j))
                std::cout << "#";
            else {
                std::cout << ".";
            }
        }
        std::cout << "\n";
    }
}

Image readData(const std::string& fileName, std::vector<bool>& algo) {
    std::fstream file(fileName);
    std::vector<std::string> inputLines;
    std::string line;
    while (std::getline(file, line)) {
        inputLines.push_back(line);
    }

    for (const char val : inputLines.front()) {
        if (val == '#')
            algo.push_back(true);
        else
            algo.push_back(false);
    }

    Image img{ (int)inputLines.size() - 2, (int)inputLines[2].size() };
    for (int i = 2; i < inputLines.size(); ++i)
    {
        for (int j = 0; j < inputLines[i].size(); ++j)
        {
            if (inputLines[i][j] == '#')
                img.setData(i - 2, j, true);
            else
                img.setData(i - 2, j, false);
        }
    }

    return img;
}

int toInt(const std::vector<bool>& bits) {
    int res = 0;
    for (auto it = bits.rbegin(); it != bits.rend(); ++it)
    {
        res += (int)std::pow(2, std::distance(bits.rbegin(), it)) * (*it);
    }

    return res;
}

Image getPixelWindow(const Image& img, int i, int j) {
    std::vector<bool> windowData{
            img(i - 1, j - 1),
            img(i - 1, j),
            img(i - 1, j + 1),
            img(i, j - 1),
            img(i, j),
            img(i, j + 1),
            img(i + 1, j - 1),
            img(i + 1, j),
            img(i + 1, j + 1)
    };

    return Image{ windowData, 3, 3 };
}

void applyAlgo(const std::vector<bool>& algo, Image& img) {
    Image newImg(img.getRows(), img.getCols());
    for (int i = 1; i < img.getRows(); ++i)
    {
        for (int j = 1; j < img.getCols(); ++j)
        {
            auto window = getPixelWindow(img, i, j);
            assert(window.getRows() * window.getCols() == 9);
            int res = toInt(window.getData());
            newImg.setData(i, j, algo[res]);
        }
    }

    img = popBorder(newImg); // reduce border by one
}


int main() {
    std::vector<bool> algo;

    auto image = readData("../data/day20.txt", algo);
    printImg(image);

    // border is 2*algo_runs + 1
    addBorderToImage(image, 101, false);

    for (unsigned i = 0; i < 50; ++i)
    {
        std::cout << i << std::endl;
        applyAlgo(algo, image);
    }

    unsigned count = 0;
    for (int i = 0; i < image.getRows(); ++i)
    {
        for (int j = 0; j < image.getCols(); ++j)
        {
            if (image(i, j))
                ++count;
        }
    }

    std::cout << count << std::endl;
    return 0;
}
