#include "mocker.h"

int Mocker::generateRandomInt(int min, int max) {
    uniform_int_distribution<mt19937::result_type> distribution(min, max);
    return distribution(rng);
}

char Mocker::generateRandomChar() {
    int increment = generateRandomInt() % 26;
    return 'a' + increment;
}
