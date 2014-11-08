#ifndef INT_UTILS_H
#define INT_UTILS_H

#include "utils.h"

#define fast_swap(x, y)	{						\
							(x) ^=	(y);		\
							(y)	^=	(x);		\
							(x) ^=	(y);		\
						}

#define my_abs(v) ((v > 0) ? (v) : -(v))

#endif // INT_UTILS_H