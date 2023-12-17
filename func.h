#include <ctime>
#include <random>
#include <unordered_map>

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

using namespace std;

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

