#include <iostream>
#include <map>

#include "fptester.h"
#include "truncate.h"

/**
 * @brief fptester : fixpoint tester
 *
 * @param fun
 * @param i
 * @param l
 * @param hist
 */
void fptester(const std::string& msg, const std::function<I(I)>& fun, I i, int l, bool hist)
{
    I      j     = truncInterval(i, l);
    double delta = pow(2, l);

    std::cout << "\nTEST: lambda(i).(" << msg << "); interval: " << i << "; lsb: " << l << "; truncated interval: " << j
              << "; resulting interval: " << fun(j) << std::endl;

    std::map<int, int> R;

    int cases = 0;
    for (double x = j.lower(); x < j.upper(); x += delta) {
        cases++;
        I   z(x, x + delta);
        I   w  = fun(z);
        int lr = lcb(w);
        R[lr]++;
    }

    if (hist) std::cout << "Histogram " << i << ", " << j << ", lsb " << l << std::endl;
    int sum  = 0;
    int g999 = 0;
    int g99  = 0;
    int best = 0;
    for (auto r : R) {
        double w = double(cases - sum) / cases;
        if (w >= 1) {
            best = r.first;
        }
        if (w >= 0.999) {
            g999 = r.first;
        }
        if (w >= 0.99) {
            g99 = r.first;
        }
        if (hist) std::cout << r.first << ':' << r.second << " RATIO= " << w << std::endl;
        sum += r.second;
    }
    std::cout << "Number of cases: " << cases << ", best precision: " << best << ", very good 0.999 precision: " << g999
              << ", good 0.99 precision: " << g99 << std::endl;
}
