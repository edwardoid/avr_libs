#ifndef RC522_CONF_H
#define RC522_CONF_H

#if defined(F_RC522) && defined(F_SPI)

#include <my_types.h>

typedef struct
{
	pin_cfg_t ss;
} rc522_cfg_t;


#endif // F_RC522 && F_SPI

#endif // RC522_CONF_H