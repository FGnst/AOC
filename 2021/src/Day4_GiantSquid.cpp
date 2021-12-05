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

class Board{
public:
    Board() = default;
    Board(unsigned rows, unsigned cols)
    : _data{}
    , _rows{rows}
    , _cols{cols}
    , _markedNumbers{}
    {
        init(0);
    };

    void init(unsigned initValue) {
        for (auto i = 0; i < _rows * _cols; ++i) {
            _data.push_back(initValue);
        }
    }

    void setData(unsigned i, unsigned j, unsigned val){
        assert( i < _rows && j < _cols);
        _data[getIndex(i,j)] = val;
    }

    unsigned getData(unsigned i, unsigned j) const{
        return _data[getIndex(i,j)];
    }

    bool markNumber(unsigned numberToMark){
        if(_hasWon)
            return false;

        auto copiedData = _data;
        auto it = std::partition(copiedData.begin(), copiedData.end(), [&numberToMark](const auto & d){return d == numberToMark;});
        if (it == copiedData.end())
          return  false;
        for(auto i = copiedData.cbegin(); i != it; ++i)
        {
            _markedNumbers.push_back(*i);
        }

        return true;
    }

    unsigned getSumUnmarked(){
        std::vector<unsigned> markedNums;
        for(const auto& d : _data){
            if(!isMarked(d))
                markedNums.emplace_back(d);
        }

        return std::accumulate(markedNums.begin(), markedNums.end(), 0);
    }

    bool hasWon() {
        if(_hasWon)
            return  true;

        // loop columns
        for (unsigned i = 0; i < _rows; ++i) {
            unsigned countMarkedColumn = 0;
            for (unsigned j = 0; j < _cols; ++j) {
                if(isMarked(getData(i,j)))
                    countMarkedColumn++;
            }
            if(countMarkedColumn == _rows) {
                _hasWon = true;
                return true;
            }
        }

        // loop rows
        for (unsigned j = 0; j < _cols; ++j) {
            unsigned countMarkedRow = 0;
            for (unsigned i = 0; i < _rows; ++i) {
                if(isMarked(getData(i, j)))
                    countMarkedRow++;
            }
            if(countMarkedRow == _cols) {
                _hasWon = true;
                return true;
            }
        }

        return false;
    }

    bool isMarked(unsigned i) const{
        return std::find(_markedNumbers.cbegin(), _markedNumbers.cend(), i) != _markedNumbers.cend();
    }
protected:


    unsigned getIndex(unsigned i, unsigned j) const{
        return _cols*i + j;
    }

private:
    std::vector<unsigned> _data; // row x cols
    unsigned _rows;
    unsigned _cols;
    std::vector<unsigned> _markedNumbers; // stores true at marked Positions, false otherwise
    bool _hasWon;
};

bool readData(const std::string& fileName, std::vector<Board>& boards) {
    const unsigned rows = 5;
    const unsigned cols = 5;
    std::ifstream dataF(fileName);
    std::string instructionString = "";
    if (!dataF.is_open())
        return false;

    Board b{rows,cols};
    unsigned rowCount = 0;
    while (std::getline(dataF, instructionString))
    {
        if (instructionString.empty()) {
            assert(rowCount == 5);
            rowCount = 0;
            boards.push_back(b);
            continue;
        }

        std::stringstream  ss(instructionString);
        for (unsigned j = 0; j < cols; j++){
            unsigned val;
            ss >> val;
            b.setData(rowCount, j, val);
        }
        ++rowCount;

    }
    boards.push_back(b);
    dataF.close();

    if (boards.empty())
        return false;

    return true;
}

std::optional<unsigned> getWinningBoard(std::vector<Board>& boards, unsigned num) {
    for (auto boardIt = boards.begin(); boardIt != boards.end(); ++boardIt) {
        if(boardIt->hasWon())
            continue;

        boardIt->markNumber(num);
        if (boardIt->hasWon()) {
            auto sumUnmarked = boardIt->getSumUnmarked();
            return std::distance(boards.begin(), boardIt);
        }
    }
    return std::nullopt;
}

void updateWinningBoards(std::vector<Board>& boards, std::vector<unsigned>& winningBoardsIndices,std::vector<unsigned>& winningNums, unsigned num) {
    for (auto boardIt = boards.begin(); boardIt != boards.end(); ++boardIt) {
        if(boardIt->hasWon())
            continue;

        boardIt->markNumber(num);
    }
    for (auto boardIt = boards.begin(); boardIt != boards.end(); ++boardIt) {
        if (boardIt->hasWon() && std::find(winningBoardsIndices.begin(), winningBoardsIndices.end(), std::distance(boards.begin(), boardIt)) == winningBoardsIndices.end()) {
            winningBoardsIndices.push_back(std::distance(boards.begin(), boardIt));
            winningNums.push_back(num);
        }
    }
}

void doTestPart1(){
    std::vector<unsigned> numbers{7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1};
    std::vector<Board> boards;
    assert(readData("../data/day4_testdata.txt", boards));
    assert(boards.size() == 3);
    assert(boards[0].getData(0,0) == 22 && boards[1].getData(0,0) == 3 && boards[2].getData(0,0) == 14);

    for(const auto& num : numbers) {
        auto winningBoard = getWinningBoard(boards, num);
        if (winningBoard.has_value()) {
            auto sumUnmarked = boards[winningBoard.value()].getSumUnmarked();
            std::cout << "Num " << num << ", sum unmarked " << sumUnmarked << ", product " << num * sumUnmarked
                      << std::endl;
            assert(num == 24);
            assert(num * sumUnmarked == 4512);
            break;
        }
    }
}

void doTestPart2(){
    std::vector<unsigned> numbers{7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1};
    std::vector<Board> boards;
    assert(readData("../data/day4_testdata.txt", boards));
    assert(boards.size() == 3);
    assert(boards[0].getData(0,0) == 22 && boards[1].getData(0,0) == 3 && boards[2].getData(0,0) == 14);

    std::vector<unsigned> winningBoardsIndices;
    std::vector<unsigned> winningNums;
    for(const auto& num : numbers) {
        updateWinningBoards(boards, winningBoardsIndices,winningNums, num);
    }

    auto sumUnmarked = boards[winningBoardsIndices.back()].getSumUnmarked();
    assert(sumUnmarked*(winningNums.back()) == 1924);
}

int main(){
    // Part 1
    // doTestPart1();
    std::vector<unsigned> extractedNums{46,79,77,45,57,34,44,13,32,88,86,82,91,97,89,1,48,31,18,10,55,74,24,11,80,78,28,37,47,17,21,61,26,85,99,96,23,70,3,54,5,41,50,63,14,64,42,36,95,52,76,68,29,9,98,35,84,83,71,49,73,58,56,66,92,30,51,20,81,69,65,15,6,16,39,43,67,7,59,40,60,4,90,72,22,0,93,94,38,53,87,27,12,2,25,19,8,62,33,75};
    std::vector<Board> boards;
    if(!readData("../data/day4.txt", boards))
        return 5;

    for(const auto& num : extractedNums) {
        auto winningBoard = getWinningBoard(boards, num);
        if (winningBoard.has_value()) {
            auto sumUnmarked = boards[winningBoard.value()].getSumUnmarked();
            std:: cout << "Winning board: " << *winningBoard << std::endl;
            std:: cout << "Score: " << num*sumUnmarked << std::endl;
            break;
        }
    }

    // Part 2
    doTestPart2();
    std::optional<std::pair<Board, unsigned>> lastWinningBoardNum{std::nullopt};
    std::vector<unsigned> winningBoardsIndices;
    std::vector<unsigned> winningNums;
    for(const auto& num : extractedNums) {
        updateWinningBoards(boards, winningBoardsIndices,winningNums, num);
    }

    auto sumUnmarked = boards[winningBoardsIndices.back()].getSumUnmarked();

    std::cout << "Last Winning board is: " << winningBoardsIndices.back() << std::endl;
    std:: cout << "Score: " << winningNums.back()*sumUnmarked << std::endl;

    return 0;
}



