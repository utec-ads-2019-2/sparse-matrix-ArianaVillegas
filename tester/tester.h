#ifndef TESTER_H
#define TESTER_H

#include <stdexcept>
#include <iostream>
#include <assert.h>
#include <vector>
#include <algorithm>   

#include "../mocker/mocker.h"
#include "../matrix.h"

using namespace std;

#ifndef NDEBUG
#   define ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            std::terminate(); \
        } \
    } while (false)
#else
#   define ASSERT(condition, message) do { } while (false)
#endif

#define NUMBER_OF_TESTS 10

class Tester {
    private:
        template <typename T>
        static void testMatrix(unsigned int, unsigned int);

        template <typename T>
        static T** buildMatrix(unsigned int, unsigned int);

        template <typename T>
        static Matrix<T> setMatrix(T **&, unsigned int, unsigned int);

    public:
        static void execute();
};

#endif