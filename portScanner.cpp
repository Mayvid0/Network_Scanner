#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <SFML/Network.hpp>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

static bool isPortOpen(const std::string& address, int port)
{
    return (sf::TcpSocket().connect(address, port) == sf::Socket::Done);
}

static std::vector<std::string> splitString(const std::string& string,
                                      char delimiter = ' ',
                                      bool allowEmpty = false)
{
    std::vector<std::string> tokens;
    std::stringstream stream(string);
    std::string token;
    while (std::getline(stream, token, delimiter)) {
        if (allowEmpty || token.size() > 0)
            tokens.push_back(token);
    }
    return tokens;
}

static int stringToInt(const std::string& string)
{
    std::stringstream stream(string);
    int i;
    stream >> i;
    return i;
}

template <typename T>
static void swapValues(T& a, T& b)
{
    T temp = a;
    a = b;
    b = temp;
}

template <typename T>
static std::vector<T> generateRange(T min, T max)
{
    if (min > max)
        std::swap(min, max);
    if (min == max)
        return std::vector<T>(1, min);
    std::vector<T> values;
    for (; min <= max; ++min)
        values.push_back(min);
    return values;
}

static std::vector<int> parsePortsList(const std::string& list)
{
    std::vector<int> ports;
    for (const std::string& token : splitString(list, ',')) {
        std::vector<std::string> strRange = splitString(token, '-');
        switch (strRange.size()) {
            case 0: ports.push_back(stringToInt(token));       break;
            case 1: ports.push_back(stringToInt(strRange[0])); break;
            case 2:
            {
                int min = stringToInt(strRange[0]),
                    max = stringToInt(strRange[1]);
                for (int port : generateRange(min, max))
                    ports.push_back(port);
                break;
            }
            default:
                break;
        }
    }
    return ports;
}

template <typename T>
static T findMaximum(const std::vector<T>& values)
{
    T max = values[0];
    for (T value : values) {
        if (value > max)
            max = value;
    }
    return max;
}

template <typename T>
static size_t countDigits(T value)
{
    size_t count = (value < 0) ? 1 : 0;
    if (value == 0)
        return 0;
    while (value) {
        value /= 10;
        ++count;
    };
    return count;
}

int main(int argc, char* argv[])
{
    std::string ipAddress;
    std::vector<int> ports;
    if (argc == 3) {
        ipAddress = argv[1];
        ports = parsePortsList(std::string(argv[2]));
    } else {
        std::string portList;
        std::cout << "IP Address: " << std::flush;
        std::getline(std::cin, ipAddress);
        std::cout << "Port List: " << std::flush;
        std::getline(std::cin, portList);
        ports = parsePortsList(portList);
    }
    std::cout << "Showing open ports on " << ipAddress << "...\n";
    size_t width = countDigits(findMaximum(ports));
    for (int port : ports) {
        if (isPortOpen(ipAddress, port))
            std::cout << "Port " << std::setw(width) << port << " : OPEN\n";
    }
    std::cout << std::flush;
    return 0;
}
