#ifndef RC522_H
#define RB522_H

#include "lib_ex_config.h"

#if defined(F_RC522) && defined(F_SPI)

#include "rc522_conf.h"

void rc522_init(rc522_cfg_t* cfg);

#endif // F_RC522 && F_SPI

#endif // RC522_H