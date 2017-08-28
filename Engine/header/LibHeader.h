#pragma once
#define _WIN32_WINNT 0x0600
#include <math.h>
#include <vector>
#include <list>

#include <windows.h>
#include <winspool.h>
#include <Threadpoolapiset.h>
#include <Sysinfoapi.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <strings.h>
#include <cstring>
#include <ctime>
#include <cmath>
#include <tchar.h>
#include <cstdio>
#include <stdexcept>
#include <algorithm>
#include<initializer_list>
#include<limits>



#include "lib\glew.h"
#include <GL\gl.h>
#include <GL\glu.h>
#ifdef WIN32
    #include "lib\wglew.h"
#else
    #include "lib\glxew.h"
#endif // WIN32

#define string std::string
