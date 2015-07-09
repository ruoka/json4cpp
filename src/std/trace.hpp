#pragma once

#ifdef DEBUG
#include <iostream>
#include <iomanip>
#define TRACE(a) do {using namespace std; clog << __func__ << ' ' << a << endl;} while(false)
#else
#define TRACE(a)
#endif
