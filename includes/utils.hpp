#ifndef _UTILS_HPP
#define _UTILS_HPP

#include <iostream>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <iomanip>

#include <functional>

#include <vector>
#include <list>
#include <chrono>
#include <thread>
#include <condition_variable>

#define BIT(a, n) ((a & (1 << n)) ? 1 : 0)
#define BITSET(a, n, on) {if (on) a |= (1 << n); else a &= ~(1 << n);}
#define BETWEEN(a, b, c) ((a >= b) && (a <= c))

#endif