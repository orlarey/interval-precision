#include <algorithm>
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

#include <boost/numeric/interval.hpp>

using namespace boost::numeric;
using namespace interval_lib;

typedef interval<double, policies<save_state<rounded_transc_std<double>>, checking_base<double>>> I;

inline std::ostream& operator<<(std::ostream& file, I i)
{
    return file << '[' << i.lower() << ';' << i.upper() << ']';
}

// http://www.binaryconvert.com/result_double.html?decimal=049

// Function to convert decimal to binary upto
// k-precision after decimal point
std::string decimalToBinary(double num, int k_prec)
{
    std::string binary = "";

    // Fetch the integral part of decimal number
    int Integral = num;

    // Fetch the fractional part decimal number
    double fractional = num - Integral;

    // Conversion of integral part to
    // binary equivalent
    while (Integral) {
        int rem = Integral % 2;

        // Append 0 in binary
        binary.push_back(rem + '0');

        Integral /= 2;
    }

    // Reverse string to get original binary
    // equivalent
    reverse(binary.begin(), binary.end());

    // Append point before conversion of
    // fractional part
    binary.push_back('.');

    // Conversion of fractional part to
    // binary equivalent
    while (k_prec--) {
        // Find next bit in fraction
        fractional *= 2;
        int fract_bit = fractional;

        if (fract_bit == 1) {
            fractional -= fract_bit;
            binary.push_back(1 + '0');
        } else
            binary.push_back(0 + '0');
    }

    return binary;
}

// int and decimal parts

double ipart(double x)
{
    return std::floor(x);
}

double dpart(double x)
{
    return x - ipart(x);
}

// tronc un nombre après le bit 2^l
double truncinf(double x, int l)
{
    double p = pow(2, l);
    double y = floor(x / p) * p;
    assert(y <= x);
    return y;
}

double truncsup(double x, int l)
{
    double p = pow(2, l);
    double y = floor(x / p + 1) * p;
    assert(x <= y);
    return y;
}

I truncInterval(I i, int l)
{
    return I(truncinf(i.lower(), l), truncsup(i.upper(), l));
}

void testtrunc(double x, int d)
{
    double l = truncinf(x, d);
    double h = truncsup(x, d);
    std::cout << l << "<=" << x << "<=" << h << " truncature après le bit " << d << std::endl;
    std::cout << "l: " << decimalToBinary(l, -d) << std::endl;
    std::cout << "x: " << decimalToBinary(x, 64) << std::endl;
    std::cout << "h: " << decimalToBinary(h, -d) << std::endl;
}

void d2binstr(double f, std::string& str)
{
    union {
        double   f;
        uint64_t i;
    } u;
    u.f = f;

    str.clear();

    for (int i = 0; i < 64; i++) {
        if (u.i & 1)
            str.push_back('1');
        else
            str.push_back('0');

        if (i == 51) str.push_back(':');
        if (i == 62) str.push_back('s');
        u.i >>= 1;
    }

    // Reverse the string since now it's backwards
    std::string temp(str.rbegin(), str.rend());
    str = temp;
}

void inspect(double x)
{
    std::string s;
    d2binstr(x, s);
    printf("double %f as format %a\n", x, x);
    std::cout << "   binary format is : " << s << std::endl;
    std::cout << " fixpoint format is : " << decimalToBinary(x, 64) << std::endl;
}

// last common bit. on recherche la première décimale binaire
// ou x et y dieviennent différents
// return : 0, les parties entières sont différentes (int(x)!=int(y))
// return -1 : les parties entières sont identiques, mais la premiere décimale binaire est différente
// etc.

int lcb(double x, double y)
{
    int dn = 0;
    while ((ipart(x) == ipart(y)) && (dn > -53)) {
        x = dpart(x) * 2;
        y = dpart(y) * 2;
        dn--;
    }
    return dn;
}

int lcb(I i)
{
    return lcb(i.lower(), i.upper());
}

void test(double x, double y)
{
    std::cout << "difference betwwen " << x << " and " << y << " at binary position " << lcb(x, y) << std::endl;
    std::cout << "  x: " << decimalToBinary(x, 64) << std::endl;
    std::cout << "  y: " << decimalToBinary(y, 64) << std::endl;
    // inspect(x);
    // inspect(y);
}

#define INSPECT(expr) std::cout << #expr << ": " << (expr) << "\n";

// l: least significant bit
// c: coverage
void job(I i, int l, double c)
{
    std::cout << "\nJob " << std::endl;

    I                  j = truncInterval(i, l);
    std::map<int, int> R;
    INSPECT(i);
    INSPECT(j);
    double delta = pow(2, l);

    int cases = 0;
    for (double x = j.lower(); x < j.upper(); x += delta) {
        cases++;
        // std::cout << "...." << std::endl;
        I z(x, x + delta);
        // INSPECT(z);
        // I w = square(z);
        I w = z / 4.0;
        // INSPECT(square(z));
        int lr = lcb(w);
        // INSPECT(lcb(square(z)));
        R[lr]++;
    }

    std::cout << "Histogram " << i << ", " << j << ", lsb " << l << ", coverage: " << c << std::endl;
    int sum  = 0;
    int ropt = 0;
    for (auto r : R) {
        double w = double(cases - sum) / cases;
        if (w >= c) {
            ropt = r.first;
        }
        std::cout << r.first << ':' << r.second << " RATIO= " << w << std::endl;
        sum += r.second;
    }
    std::cout << "Optimal lsb: " << ropt << std::endl;
}

// l: least significant bit
// c: coverage
void jobf(const std::function<I(I)>& fun, I i, int l, double c, bool hist)
{
    I      j     = truncInterval(i, l);
    double delta = pow(2, l);

    std::cout << "\nJobf -- Interval " << i << " lsb: " << l << " truncated interval: " << j
              << " number of points: " << (j.upper() - j.lower()) / delta << " coverage: " << c << std::endl;

    std::map<int, int> R;

    int cases = 0;
    for (double x = j.lower(); x < j.upper(); x += delta) {
        cases++;
        I   z(x, x + delta);
        I   w  = fun(z);
        int lr = lcb(w);
        R[lr]++;
    }

    if (hist) std::cout << "Histogram " << i << ", " << j << ", lsb " << l << ", coverage: " << c << std::endl;
    int sum  = 0;
    int ropt = 0;
    for (auto r : R) {
        double w = double(cases - sum) / cases;
        if (w >= c) {
            ropt = r.first;
        }
        if (hist) std::cout << r.first << ':' << r.second << " RATIO= " << w << std::endl;
        sum += r.second;
    }
    std::cout << "Number of cases: " << cases << " Optimal output lsb: " << ropt << std::endl;
}

int main()
{
    /*    std::cout << "Tests A" << std::endl;

        inspect(4);
        inspect(M_PI);
        inspect(2);
        inspect(1);
        inspect(0.5);
        inspect(0.25);

        std::cout << "Tests B" << std::endl;

        test(3.14, 4.14);
        test(3.14, 3.14);
        test(3.14159, M_PI);
        test(1.0 / 2.0 + 1.0 / 4.0, 1.0 / 2.0 + 1.0 / 4.0 + 1.0 / 8.0);
        test(0.101, 0.102);

        std::cout << "Tests C" << std::endl;

        testtrunc(0.1, -4);
        testtrunc(M_PI, -1);
        testtrunc(M_PI, -2);
        testtrunc(M_PI, -16);

        I x(-1, +1);
        INSPECT(x);
        INSPECT(square(x));
        INSPECT(x * x);
        I z(-M_PI, M_PI);
        INSPECT(z);
        INSPECT(truncInterval(z, -2));
        INSPECT(truncInterval(z, -3));
        INSPECT(truncInterval(z, -4));
    */
    // std::cout << "interval: " << y.lower() << "," << y.upper() << std::endl;
    // std::cout << "interval: " << square(x) << std::endl;
    std::cout << "JOB" << std::endl;
    I w(0, M_PI / 2);
    jobf([](I i) { return i / 4.0; }, w, -16, 0.99, true);
    jobf([](I i) { return i * 4.0; }, w, -16, 0.99, true);
    jobf([](I i) { return sin(i); }, w, -16, 0.99, true);
    //
    {
        std::cout << "\nSearch for [](I i) { return square(sin(i)); }" << std::endl;
        auto foo = [](I i) { return square(sin(i)); };
        jobf(foo, I(0.1, M_PI), -16, 0.99, false);
        jobf(foo, I(0.1, M_PI), -15, 0.99, false);
        jobf(foo, I(0.1, M_PI), -14, 0.99, false);
        jobf(foo, I(0.1, M_PI), -13, 0.99, false);
        jobf(foo, I(0.1, M_PI), -12, 0.99, false);
        jobf(foo, I(0.1, M_PI), -11, 0.99, false);
        jobf(foo, I(0.1, M_PI), -10, 0.99, false);
    }
    //
    {
        std::cout << "\nSearch for [](I i) { return 1.0/sin(i); }" << std::endl;
        auto foo = [](I i) { return 1.0 / sin(i); };
        jobf(foo, I(0.1, M_PI), -16, 0.99, false);
        jobf(foo, I(0.1, M_PI), -15, 0.99, false);
        jobf(foo, I(0.1, M_PI), -14, 0.99, false);
        jobf(foo, I(0.1, M_PI), -13, 0.99, false);
        jobf(foo, I(0.1, M_PI), -12, 0.99, false);
        jobf(foo, I(0.1, M_PI), -11, 0.99, false);
        jobf(foo, I(0.1, M_PI), -10, 0.99, false);
    }
    //
    {
        std::cout << "\n> Search for [](I i) { return arctan(i*10.0); }" << std::endl;
        auto foo = [](I i) { return atan(i * 10.0); };
        jobf(foo, I(-1, 1), -16, 0.99, false);
        jobf(foo, I(-1, 1), -15, 0.99, false);
        jobf(foo, I(-1, 1), -14, 0.99, false);
        jobf(foo, I(-1, 1), -13, 0.99, false);
        jobf(foo, I(-1, 1), -12, 0.99, false);
        jobf(foo, I(-1, 1), -11, 0.99, false);
        jobf(foo, I(-1, 1), -10, 0.99, false);
    }
    //
    {
        std::cout << "\n> Search for [](I i) { return i-pow(i,3.0); }" << std::endl;
        auto foo = [](I i) { return i - pow(i, 3.0); };
        jobf(foo, I(-1, 1), -16, 0.99, false);
        jobf(foo, I(-1, 1), -15, 0.99, false);
        jobf(foo, I(-1, 1), -14, 0.99, false);
        jobf(foo, I(-1, 1), -13, 0.99, false);
        jobf(foo, I(-1, 1), -12, 0.99, false);
        jobf(foo, I(-1, 1), -11, 0.99, false);
        jobf(foo, I(-1, 1), -10, 0.99, false);
    }
    return 0;
}

// 1 =     printf("double %f as format %a\n", x, x);
// 0s01111111111:0000000000000000000000000000000000000000000000000000

/*


   binary format is : 0s01111111011:1010000111001010110000001000001100010010011011101001
 fixpoint format is : .0001101000011100101011000000100000110001001001101110100100000000



*/
