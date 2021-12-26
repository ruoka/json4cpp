#pragma once

#ifdef DEBUG
#include <iostream>
#include <iomanip>
#define TRACE(a) do {std::clog << __FILE__ << ' ' << __LINE__ << ' ' << __func__ << ' ' << a << std::endl;} while(false)
#else
#define TRACE(a)
#endif
