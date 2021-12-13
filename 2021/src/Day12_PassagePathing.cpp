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

bool islowerCase(const std::string& str) {
    for (const auto& s : str)
        if (std::isupper(static_cast<unsigned char>(s)))
            return false;
    return true;
}

bool hasLowerDuplicates(const std::vector<std::string>& path, const std::string& key) {
    unsigned counter = 0;
    for (const auto& p : path)
    {
        if (islowerCase(p) && p == key)
            ++counter;
        if (counter > 1)
            return true;
    }
    return false;
}

struct Node {
    Node(std::string value) {
        _value = value;
    }

    void addConnection(Node* cn) {
        _connections.push_back(cn);
    }

    std::string _value;
    std::vector<Node*> _connections;
};
struct Network {
    void expandPath(Node& n, std::vector<std::string> path) {
        if (n._value == "start")
            return;

        if (n._value == "end") {
            _paths.push_back(path);
            return;
        }

        bool lowerDuplicates = false;
        bool hasVisited = false;
        for (const auto& p : path) {
            if (hasLowerDuplicates(path, p)) {
                lowerDuplicates = true;
                break;
            }
        }
        for (const auto& p : path) {
            if (islowerCase(p) && n._value == p)
            {
                hasVisited = true;
                break;
            }
        }
        if (hasVisited  && lowerDuplicates) // already visited and lowercase
            return;

        path.push_back(n._value);
        for (auto& conn : n._connections) {
            expandPath(*conn, path);
        }
    }

    void computePaths() {
        auto start = std::find_if(_nodes.begin(), _nodes.end(), [](const auto& node) {return node->_value == "start"; });

        std::vector<std::string> path;
        path.push_back((*start)->_value);
        for (auto& conn : (*start)->_connections) {
            expandPath(*conn, path);
        }
    }

    void setNodes() {
        for (auto& connection : _connections) {
            auto nodeItFirst = std::find_if(_nodes.begin(), _nodes.end(), [&connection](const std::unique_ptr<Node>& node) {return node->_value == connection.first; });

            if (nodeItFirst == _nodes.end()) {
                _nodes.push_back(std::make_unique<Node>(connection.first));
            }
        }

        for (auto& connection : _connections) {
            auto nodeItSec = std::find_if(_nodes.begin(), _nodes.end(), [&connection](const std::unique_ptr<Node>& node) {return node->_value == connection.second; });
            if (nodeItSec == _nodes.end()) {
                _nodes.push_back(std::make_unique<Node>(connection.second));
            }
        }

        for (const auto& conn : _connections)
        {
            auto nodeItFirst = std::find_if(_nodes.begin(), _nodes.end(), [&conn](const std::unique_ptr<Node>& node) {return node->_value == conn.first; });
            if (nodeItFirst != _nodes.end()) {
                for (auto& node : _nodes)
                {
                    if (node->_value == conn.second)
                        (*nodeItFirst)->addConnection(node.get());
                }
            }
        }


        for (const auto& conn : _connections)
        {
            auto nodeItSecond = std::find_if(_nodes.begin(), _nodes.end(), [&conn](const std::unique_ptr<Node>& node) {return node->_value == conn.second; });
            if (nodeItSecond != _nodes.end()) {
                for (auto& node : _nodes)
                {
                    if (node->_value == conn.first)
                        (*nodeItSecond)->addConnection(node.get());
                }
            }
        }
    }

    std::vector<std::unique_ptr<Node>>  _nodes;
    std::vector<std::pair<std::string, std::string>> _connections;
    std::vector<std::vector<std::string>> _paths;
};

void readData(const std::string& fileName, Network& net) {

    std::ifstream dataFile(fileName, std::ios_base::binary);
    std::string line;
    while (std::getline(dataFile, line)) {
        std::string begin;
        std::string end;
        auto ch = line.begin();
        while (*ch != '-' && ch != line.end()) {
            begin.push_back(*ch);
            ch++;
        }
        ++ch;
        while (ch != line.end() && *ch != '\r') {
            end.push_back(*ch);
            ++ch;
        }
        net._connections.push_back(std::make_pair(begin, end));
    }
}

int main() {
    Network net;
    readData("../data/day12.txt", net);

    net.setNodes();
    net.computePaths();

    std::cout << "Count paths: " << net._paths.size() << std::endl;

    return 0;
}
