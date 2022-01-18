#pragma once

#include <functional>
#include <string>

#include "interval.h"

void fptester(const std::string& msg, const std::function<I(I)>& fun, I i, int l, bool hist);
