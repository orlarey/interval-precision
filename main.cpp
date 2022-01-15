#include <assert.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <list>
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
typedef interval<double> I;

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

// tronc un nombre d digits après le virgule
// propriété
double truncinf(double x, int d)
{
    double p = pow(2, d);
    double y = floor(x * p) / p;
    assert(y <= x);
    return y;
}

double truncsup(double x, int d)
{
    double p = pow(2, d);
    double y = floor(x * p + 1) / p;
    assert(x <= y);
    return y;
}

void testtrunc(double x, int d)
{
    double l = truncinf(x, d);
    double h = truncsup(x, d);
    std::cout << l << "<=" << x << "<=" << h << " truncature " << d << " digits après la virgule" << std::endl;
    std::cout << "l: " << decimalToBinary(l, d) << std::endl;
    std::cout << "x: " << decimalToBinary(x, 64) << std::endl;
    std::cout << "h: " << decimalToBinary(h, d) << std::endl;
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

void test(double x, double y)
{
    std::cout << "difference betwwen " << x << " and " << y << " at binary position " << lcb(x, y) << std::endl;
    std::cout << "  x: " << decimalToBinary(x, 64) << std::endl;
    std::cout << "  y: " << decimalToBinary(y, 64) << std::endl;
    // inspect(x);
    // inspect(y);
}

template <typename N>
inline std::ostream& operator<<(std::ostream& file, interval<N> i)
{
    return file << '[' << i.lower() << ',' << i.upper() << ']';
}

#define INSPECT(expr) std::cout << #expr << ": " << (expr) << "\n";

int main()
{
    std::cout << "Test difference" << std::endl;

    inspect(4);
    inspect(M_PI);
    inspect(2);
    inspect(1);
    inspect(0.5);
    inspect(0.25);

    test(3.14, 4.14);
    test(3.14, 3.14);
    test(3.14159, M_PI);
    test(1.0 / 2.0 + 1.0 / 4.0, 1.0 / 2.0 + 1.0 / 4.0 + 1.0 / 8.0);
    test(0.101, 0.102);

    testtrunc(0.1, 4);
    testtrunc(M_PI, 1);
    testtrunc(M_PI, 2);
    testtrunc(M_PI, 16);

    I x(-1, +1);
    INSPECT(x);
    INSPECT(square(x));
    INSPECT(x * x);
    I y = square(x);  // x ^ 2.0;
    // std::cout << "interval: " << y.lower() << "," << y.upper() << std::endl;
    std::cout << "interval: " << square(x) << std::endl;

    return 0;
}

// 1 =     printf("double %f as format %a\n", x, x);
// 0s01111111111:0000000000000000000000000000000000000000000000000000

/*


   binary format is : 0s01111111011:1010000111001010110000001000001100010010011011101001
 fixpoint format is : .0001101000011100101011000000100000110001001001101110100100000000



*/
