#include <ctime>
#include <random>
#include <unordered_map>
#include <utility>
#include <typeinfo>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <iomanip> 
#include <iomanip> // Include the iomanip library
#include <algorithm> 

// Define colors
#define RED_TEXT "\033[31m"
#define GREEN_TEXT "\033[32m"
#define YELLOW_TEXT "\033[33m"
#define BLUE_TEXT "\033[34m"
#define MAGENTA_TEXT "\033[35m"
#define CYAN_TEXT "\033[36m"
#define WHITE_TEXT "\033[37m"

// #define BOLD_BLACK_TEXT "\033[1;30m"
#define BOLD_RED_TEXT "\033[1;31m"
#define BOLD_GREEN_TEXT "\033[1;32m"
#define BOLD_YELLOW_TEXT "\033[1;33m"
#define BOLD_BLUE_TEXT "\033[1;34m"
#define BOLD_MAGENTA_TEXT "\033[1;35m"
#define BOLD_CYAN_TEXT "\033[1;36m"
#define BOLD_WHITE_TEXT "\033[1;37m"
#define RESET_TEXT "\033[0m"

#define maybe (std::rand() % 2 == 0)

using namespace std;

int count3 = 0;
int count4 = 0;
int count5 = 0;

// Define an enum named Operation
enum class Action {
    UP,
    LEFT,
    RIGHT,
    DOWN,
    STAY,
    KILL
};

unordered_map<string, string> colorMap = {
        {"Red", RED_TEXT},
        {"Green", GREEN_TEXT},
        {"Yellow", YELLOW_TEXT},
        {"Blue", BLUE_TEXT},
        {"Magenta", MAGENTA_TEXT},
        {"Cyan", CYAN_TEXT},
        {"White", WHITE_TEXT},
        {"Bold Red", BOLD_RED_TEXT},
        {"Bold Green", BOLD_GREEN_TEXT},
        {"Bold Yellow", BOLD_YELLOW_TEXT},
        {"Bold Blue", BOLD_BLUE_TEXT},
        {"Bold Magenta", BOLD_MAGENTA_TEXT},
        {"Bold Cyan", BOLD_CYAN_TEXT},
        {"Bold White", BOLD_WHITE_TEXT},
        {"Reset", RESET_TEXT}
    };

int getRandom(int minBound, int maxBound) {
    if (maxBound < minBound) {swap(minBound, maxBound);}
    int range = maxBound - minBound + 1;    
    return minBound + (rand() % range);;
}

double getRandomDouble(double lowerBound, double upperBound) {
    int randomInt = rand();
    double randomFraction = static_cast<double>(randomInt) / RAND_MAX;
    double randomDouble = lowerBound + randomFraction * (upperBound - lowerBound);
    return randomDouble;
}

bool charToBool(char c) {return (c == '1');}

unsigned int boolArrayToUnsigned(const bool* boolArray, const int size) {
    unsigned int result = 0;
    if(boolArray == nullptr) {throw invalid_argument("Cannot convert boolArray, it is nullptr!");}
    for (int i = 0; i < size; i++) {
        if (boolArray[i]) {         // !!! SEGMENTATION FAULT !!!
            result += static_cast<unsigned int>(pow(2, size - 1 - i));
        }
    }
    return result;
}

unsigned int stringToUnsigned(const string& str, int start, int size) {
    unsigned int result = 0;
    if (start < 0) {throw invalid_argument("Cannot convert string to unsigned, start is negative!");}
    if (size < 0) {throw invalid_argument("Cannot convert string to unsigned, size is negative!");}
    if (start + size > str.size()) {throw invalid_argument("Cannot convert string to unsigned, start + size is greater than string size!");}
    for (int i = start; i < start + size; i++) {
        if (str[i] == '1') {
            result += static_cast<unsigned int>(pow(2, start + size - 1 - i));
        }
        else if(str[i] != '0') {throw invalid_argument("Cannot convert string to unsigned, string contains invalid characters!");}
    }
    return result;    
}

/*

// First

unsigned int boolArrayToUnsigned(const bool* boolArray, const int size) {
    unsigned int result = 0;
    if(boolArray == nullptr) {throw invalid_argument("Cannot convert boolArray, it is nullptr!");}
    for (int i = size - 1; i >= 0; --i) {
        if (boolArray[i]) {
            result += static_cast<unsigned int>(pow(2, size - 1 - i));
        }
    }
    return result;
}

POOSIBLY MORE EFFICIENT IMPLEMENTATION OF boolArrayToUnsigned

unsigned int boolArrayToUnsigned(const bool* boolArray, int size) {
    unsigned int result = 0;
    for (int i = size - 1; i >= 0; --i) {
        if (boolArray[i]) {
            result |= (1U << (size - 1 - i));
        }
    }
    return result;
}


*/

struct activationFunctions {
    // Activation functions are used to determine the output of a neuron
    static double sigmoid(double x) {
        return 1.0 / (1.0 + exp(-x));
    }

    static double relu(double x) {
        return max(0.0, x);
    }
};
