#include "../include/Day16_PacketDecoder.h"


int toInt(const std::vector<bool>& bits) {
    int res = 0;
    for (auto it = bits.rbegin(); it != bits.rend(); ++it)
    {
        res += (int)std::pow(2, std::distance(bits.rbegin(), it)) * (*it);
    }

    return res;
}

long long toLongLong(const std::vector<bool>& bits) {
    long long res = 0;
    for (auto it = bits.rbegin(); it != bits.rend(); ++it)
    {
        res += (long long)std::pow(2, std::distance(bits.rbegin(), it)) * (*it);
    }

    return res;
}

std::vector<std::string> readData(std::string fileName) {
    std::vector<std::string> data;
    std::ifstream dataFile(fileName);
    std::string line;
    while (std::getline(dataFile, line))
    {
        data.push_back(line);
    }

    return data;
}

int processLiteral(std::vector<bool> content, std::vector<bool>::iterator& begin, int& result, std::function<int(int, int)> op) {
    std::vector<bool> resultLocal;
    auto bitIt = begin;
    for (;; bitIt += 5) {
        if (*bitIt == 1) {
            resultLocal.push_back(*(bitIt + 1));
            resultLocal.push_back(*(bitIt + 2));
            resultLocal.push_back(*(bitIt + 3));
            resultLocal.push_back(*(bitIt + 4));
        }
        else if (*bitIt == 0) {
            resultLocal.push_back(*(bitIt + 1));
            resultLocal.push_back(*(bitIt + 2));
            resultLocal.push_back(*(bitIt + 3));
            resultLocal.push_back(*(bitIt + 4));
            break;
        }
    }

    result = toInt(resultLocal);
    std::cout << "Operation result: " << result << std::endl;

    return static_cast<int>(std::distance(begin, bitIt + 5));
}



int unpack(std::vector<bool>& bits, std::vector<bool>::iterator& begin, int& sumVersion, std::function<int(int, int)> op) {

    int result = 0;
    if (std::distance(begin, bits.end()) < 11)
        return result;

    std::vector<bool> version{ begin, begin + 3 };
    std::vector<bool> typeID{ begin + 3, begin + 6 };
    std::advance(begin, 6);
    std::cout << "************************" << std::endl;
    std::cout << "Version: " << toInt(version) << " typeID: " << toInt(typeID) << std::endl;

    sumVersion += toInt(version);
    if (toInt(typeID) == 4) { // literal
        int localResult = 0;
        std::advance(begin, processLiteral(bits, begin, localResult, op));
        result = op(localResult, unpack(bits, begin, sumVersion, op));
    }
    else
    {
        auto typeId = toInt(typeID);
        switch (typeId) {
        case 0: // sum
            result = processPacket(bits, begin, [](int a, int b) {return a + b; }, sumVersion);
            break;
        case 1: //prod
            result = processPacket(bits, begin, [](int a, int b, bool single = false) {
                return a * b; }, sumVersion);
            break;
        case 2: //min
            result = processPacket(bits, begin, [](int a, int b) {return std::min<int>(a, b); }, sumVersion);
            break;
        case 3: // max
            result = processPacket(bits, begin, [](int a, int b) {return std::max<int>(a, b); }, sumVersion);
            break;
        case 5: // greater than
            result = processPacket(bits, begin, [](int a, int b) {return a > b; }, sumVersion);
            break;
        case 6: // less than
            result = processPacket(bits, begin, [](int a, int b) {return a < b; }, sumVersion);
            break;
        case 7: // equal to
            result = processPacket(bits, begin, [](int a, int b) {return a == b; }, sumVersion);
            break;
        default:
            assert(false);
        }

    }
    return result;
}

int processPacket(std::vector<bool>& bits, std::vector<bool>::iterator& begin, std::function<int(int, int)> op, int& sumVersion) {
    // any other is an operator..
    if (*begin == 0) { // length type id
        std::advance(begin, 1);
        auto lengthInBits = std::vector<bool>(begin, begin + 15);
        int totalLengthInBits = toInt(lengthInBits); // next 15 are length
        std::advance(begin, 15);
        std::cout << " Total length (bits): " << totalLengthInBits << std::endl;
        int result = 0;
        result = op(result, unpack(bits, begin, sumVersion, op));
        return result;
    }
    else {
        std::advance(begin, 1);
        auto subpackets = std::vector<bool>(begin, begin + 11);
        int countSubPackets = toInt(subpackets); // next 11 are number of subpackets
        std::advance(begin, 11);
        std::cout << " Subpackets: " << countSubPackets << std::endl;
        int result = 0;
        for (int i = 0; i < countSubPackets; ++i)
        {
            result = op(result, unpack(bits, begin, sumVersion, op));
        }
        return result;
    }
}

std::pair<int, unsigned> parse(std::vector<bool>& bits) {
    int sumVersions = 0;
    auto result = unpack(bits, bits.begin(), sumVersions, nullptr);

    return { sumVersions, result };
}

std::vector<bool> stringToBool(const std::string& line) {

    std::string totStr;
    for (auto& ch : line) {
        totStr.append(hexToString[ch]);
    }

    std::vector<bool> bits;
    for (const auto ch : totStr) {
        if (ch == '1')
            bits.push_back(true);
        else if (ch == '0')
            bits.push_back(false);
        else
            assert(false);
    }

    return bits;
}


void attachPacket(unsigned& begin, std::vector<std::unique_ptr<Packet>>& packets, std::vector<bool>* data) {
    auto version = toInt(makeVec(begin, begin + 3, data));
    auto type = toInt(makeVec(begin + 3, begin + 6, data));
    begin += 6;
    if (type == 4) {
        auto literal = std::make_unique<LiteralPacket>(begin, data, version, type);
        packets.push_back(std::move(literal));
    }
    else {
        auto operatorPckt = std::make_unique<OperatorPacket>(begin, data, version, type);
        packets.push_back(std::move(operatorPckt));
    }
}

long long parsePackets(std::vector<bool> bits) {
    unsigned begin = 0;
    std::vector<std::unique_ptr<Packet>> packets;
    while (bits.size() - begin >= 11) {
        attachPacket(begin, packets, &bits);
    }

    std::vector<long long> results;
    for (auto& packet : packets) {
        auto* operatorPacket = dynamic_cast<OperatorPacket*>(packet.get());
        if (operatorPacket)
            operatorPacket->doOperation();
        auto res = packet->getResult();
        results.push_back(res);
    }

    assert(results.size() == 1);

    return results.front();
}

std::vector<bool> makeVec(unsigned begin, unsigned end, std::vector<bool>* data) {
    if (!data)
        return {};
    std::vector<bool> vec;
    for (unsigned i = begin; i < end; ++i)
    {
        vec.push_back(data->at(i));
    }

    return vec;
}

Packet::Packet(unsigned& begin, std::vector<bool>* data, int version, int type)
    : _subpackets{}
    , _result{}
    , _version{ version }
    , _begin{ begin }
    , _type{ type }
    , _data{ data }
{
}

long long Packet::getResult()
{
    return _result;
}

void Packet::setSubpacket(std::unique_ptr<Packet> packet)
{
    _subpackets.push_back(std::move(packet));
}

OperatorPacket::OperatorPacket(unsigned& begin, std::vector<bool>* data, int version, int type)
    : Packet(begin, data, version, type)
    , _op{}
{
    _type = type;
    switch (_type) {
    case 0: // sum
        _op = [](long long a, long long b) {return a + b; };
        break;
    case 1: //prod
        _op = [](long long a, long long b, bool single = false) { return a * b; };
        break;
    case 2: //min
        _op = [](long long a, long long b) {return std::min<long long>(a, b); };
        break;
    case 3: // max
        _op = [](long long a, long long b) {return std::max<long long>(a, b); };
        break;
    case 5: // greater than
        _op = [](long long a, long long b) {return a > b; };
        break;
    case 6: // less than
        _op = [](long long a, long long b) {return a < b; };
        break;
    case 7: // equal to
        _op = [](long long a, long long b) {return a == b; };
        break;
    default:
        assert(false);
    }


    if (_data->at(_begin) == 0) { // length type id
        _begin += 1;
        auto lengthInBits = makeVec(_begin, _begin + 15, _data);
        int totalLengthInBits = toInt(lengthInBits); // next 15 are length
        _begin += 15;
        std::cout << " Total length (bits): " << totalLengthInBits << std::endl;

        unsigned beginHere = _begin; // copy to be able to tell position in while loop
        while ((_begin - beginHere) < unsigned(totalLengthInBits)) {
            attachPacket(_begin, _subpackets, _data);
        }
    }
    else {
        begin += 1;
        auto subpackets = makeVec(_begin, _begin + 11, _data);
        int countSubPackets = toInt(subpackets); // next 11 are number of subpackets
        begin += 11;
        std::cout << " Subpackets: " << countSubPackets << std::endl;
        int result = 0;
        for (int i = 0; i < countSubPackets; ++i)
        {
            attachPacket(_begin, _subpackets, _data);
        }
    }
}

void OperatorPacket::doOperation()
{
    std::vector<long long> results;
    for (auto& child : _subpackets) {
        auto operat = dynamic_cast<OperatorPacket*>(child.get());
        if (operat) {
            operat->doOperation();
        }
        results.push_back(child->getResult());
    }

    assert(!results.empty() && results.size() == _subpackets.size());
    _result = results.front();
    if (results.size() == 1) // only one result
        return;
    else {
        for (auto it = results.begin() + 1; it != results.end(); ++it)
        {
            _result = _op(_result, *it);
        }
    }
}


LiteralPacket::LiteralPacket(unsigned& begin, std::vector<bool>* data, int version, int type)
    : Packet(begin, data, version, type)
{
    extractResult();
}

void LiteralPacket::extractResult()
{
    std::vector<bool> resultLocal{};

    for (;; _begin += 5) {
        if (_data->at(_begin) == 1) {
            resultLocal.push_back(_data->at(_begin + 1));
            resultLocal.push_back(_data->at(_begin + 2));
            resultLocal.push_back(_data->at(_begin + 3));
            resultLocal.push_back(_data->at(_begin + 4));
        }
        else if (_data->at(_begin) == 0) {
            resultLocal.push_back(_data->at(_begin + 1));
            resultLocal.push_back(_data->at(_begin + 2));
            resultLocal.push_back(_data->at(_begin + 3));
            resultLocal.push_back(_data->at(_begin + 4));
            break;
        }
    }

    _begin += 5;

    _result = toLongLong(resultLocal);
    std::cout << "Operation result: " << _result << std::endl;
}

void test1() {
    auto data = readData("../data/day16_test.txt");
    std::vector<int> testResults{ 9, 14, 16, 12, 23, 31 };
    for (unsigned i = 0; i < data.size(); ++i)
    {
        auto bits = stringToBool(data[i]);
        auto versionSumAndResult = parse(bits);
        assert(testResults[i] == versionSumAndResult.first);
    }
}

void part1() {
    auto data = readData("../data/day16.txt");
    int sumVersion{};
    for (unsigned i = 0; i < data.size(); ++i)
    {
        auto bits = stringToBool(data[i]);
        std::optional<int> packetResult = std::nullopt;
        sumVersion += parse(bits).first;
    }

    std::cout << "Sum of version = " << sumVersion << std::endl;
}

void test2() {
    auto data = readData("../data/day16_test2.txt");
    std::vector<int> testResults{ 3, 54, 7, 9, 1, 0, 0, 1 };
    for (unsigned i = 0; i < data.size(); ++i)
    {
        auto bits = stringToBool(data[i]);
        auto result = parsePackets(bits);
        assert(testResults[i] == result);
    }
}

void part2() {
    auto data = readData("../data/day16.txt");
    auto bits = stringToBool(data.front());
    auto result = parsePackets(bits);

    std::cout << "Result = " << result << std::endl;
}

int main() {
    //test1();

    //part1();

    //test2();

    part2();

    return 0;
}
