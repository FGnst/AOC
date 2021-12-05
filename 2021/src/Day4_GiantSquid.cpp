#ifdef NDEBUG
#  define assert(condition) ((void)0)
#else
#  define assert(condition) /*implementation defined*/
#endif

#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

class Board{
public:
    Board() = default;
    Board(unsigned rows, unsigned cols)
    : _data{}
    , _rows{rows}
    , _cols{cols}
    , _markedNumbers{}
    {
    };

    void init(unsigned initValue) {
        for (auto i = 0; i < _rows * _cols; ++i) {
            _data[i] = initValue;
        }
    }

    void setData(unsigned i, unsigned j, unsigned val){
        _data[getIndex(i,j)] = val;
    }

    unsigned getData(unsigned i, unsigned j){
        return _data[getIndex(i,j)];
    }

    bool markNumber(unsigned numberToMark){
        auto it = std::partition(_data.begin(), _data.end(), [&numberToMark](const auto & d){return d == numberToMark;});
        if (it == _data.end())
          return  false;
        for(auto i = _data.cbegin(); i != it; ++it)
        {
            _markedNumbers.push_back(*i);
        }

        return true;
    }

    bool hasWon() {
        for (unsigned i = 0; i < _rows; ++i) {
            for (unsigned j = 0; i < _cols; ++j) {
            }

        }

        return false;
    }

protected:
    bool isMarked(unsigned i){
        return std::find(_markedNumbers.cbegin(), _markedNumbers.cend(), i) != _markedNumbers.cend();
    }

    unsigned getIndex(unsigned i, unsigned j){
        return _cols*i + j;
    }

    std::pair<unsigned, unsigned > getIndices(unsigned index){
        return {index / _rows, index % _rows}; // i, j
    }

private:
    std::vector<unsigned> _data; // row x cols
    unsigned _rows;
    unsigned _cols;
    std::vector<unsigned> _markedNumbers; // stores true at marked Positions
};

int main(){
    return 0;
}