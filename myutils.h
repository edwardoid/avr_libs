#ifndef MYUTILS_H
#define MYUTILS_H

#include <inttypes.h>
#define _CONCAT(a,b) a##b
#define PORT(x) _CONCAT(PORT,x)
#define PIN(x) _CONCAT(PIN,x)
#define DDR(x) _CONCAT(DDR,x)

#define forever while(1)

#endif
