#pragma once

#ifdef DEBUG
#include <iostream>
#include <iomanip>
#define TRACE(a) do {std::clog << __func__ << ' ' << a << std::endl;} while(false)
#else
#define TRACE(a)
#endif
