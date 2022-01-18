#include <algorithm>
#include <boost/numeric/interval.hpp>
#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>

#include "fptester.h"
#include "truncate.h"

int main()
{
    int lsb = -18;
    {
        auto        fun = [](I i) { return square(sin(i)); };
        std::string msg = "square(sin(i))";
        fptester(msg, fun, I(0.1, M_PI), lsb, false);
    }
    //
    {
        auto        fun = [](I i) { return 1.0 / sin(i); };
        std::string msg = "1.0 / sin(i)";
        fptester(msg, fun, I(0.1, M_PI), lsb, false);
    }
    //
    {
        auto        fun = [](I i) { return atan(i * 10.0); };
        std::string msg = "atan(i * 10.0)";
        fptester(msg, fun, I(-1, 1), lsb, false);
    }
    //
    {
        auto        fun = [](I i) { return i - pow(i, 3.0); };
        std::string msg = "i - pow(i, 3.0)";
        fptester(msg, fun, I(-1, 1), lsb, false);
    }
    //
    {
        auto        fun = [](I i) { return sin(i); };
        std::string msg = "sin(i)";
        I           i(0, 2.0 * M_PI);
        fptester(msg, fun, i, lsb, false);
    }
    {
        auto        fun = [](I i) { return cos(i); };
        std::string msg = "cos(i)";
        I           i(0, 2.0 * M_PI);
        fptester(msg, fun, i, lsb, false);
    }
    {
        auto        fun = [](I i) { return cos(i - M_PI_2); };
        std::string msg = "cos(i-PI/2)";
        I           i(0, 2.0 * M_PI);
        fptester(msg, fun, i, lsb, false);
    }
    {
        auto        fun = [](I i) { return sin(i + M_PI_2); };
        std::string msg = "sin(i + M_PI_2)";
        I           i(0, 2.0 * M_PI);
        fptester(msg, fun, i, lsb, false);
    }
    return 0;
}
