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

class Packet
{
public:
    Packet();
    Packet(unsigned& begin, std::vector<bool>* data, int version, int type);
    virtual ~Packet() = default;
    long long getResult();
    inline int getVersion() { return _version; };
    void setSubpacket(std::unique_ptr<Packet> packet);

protected:
    std::vector<std::unique_ptr<Packet>> _subpackets;
    long long _result; //result of this packet
    int _version;
    unsigned& _begin;
    int _type;
    std::vector<bool>* _data;
};

class LiteralPacket : public Packet {
public:    
    LiteralPacket(unsigned& begin, std::vector<bool>* data, int version, int type);
    void extractResult();
};

class OperatorPacket : public Packet {
public:
    OperatorPacket(unsigned& begin, std::vector<bool>* data, int version, int type);
    void doOperation();
private:
    std::function<long long(long long, long long)> _op; // operation
};

std::map<char, std::string> hexToString{{'0', "0000"},
                                        {'1', "0001"},
                                        {'2', "0010"},
                                        {'3', "0011"},
                                        {'4', "0100"},
                                        {'5', "0101"},
                                        {'6', "0110"},
                                        {'7', "0111"},
                                        {'8', "1000"},
                                        {'9', "1001"},
                                        {'A', "1010"},
                                        {'B', "1011"},
                                        {'C', "1100"},
                                        {'D', "1101"},
                                        {'E', "1110"},
                                        {'F', "1111"}};

int toInt(const std::vector<bool> &bits);

std::vector<std::string> readData(std::string fileName);

int processLiteral(std::vector<bool> content, std::vector<bool>::iterator &begin, int &resultInt, std::function<int(int, int)> op);

int unpack(std::vector<bool>& bits, std::vector<bool>::iterator& begin, int& sumVersion, std::function<int(int, int)> op = nullptr);

int processPacket(std::vector<bool> &bits, std::vector<bool>::iterator &begin, std::function<int(int, int)> op, int &sumVersion);

std::pair<int, unsigned> parse(std::vector<bool> &bits);

std::vector<bool> stringToBool(const std::string &line);

void parsePacket(std::vector<bool>& bits, std::vector<bool>::iterator& begin);

std::vector<bool> makeVec(unsigned begin, unsigned end, std::vector<bool>* data);