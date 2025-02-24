#include "headers.hpp"

using namespace std;
using ll = long long;

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "sub.h"

TEST_CASE("Testing add function") {
    CHECK(add(2, 3) == 5);
    CHECK(add(-1, 1) == 0);
    CHECK(add(0, 0) == 0);
    CHECK(add(40, 2) == 42);
}

