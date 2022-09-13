#ifndef LPDDR4_INIT_H
#define LPDDR4_INIT_H

#include "ddr_common_func.h"

void lpddr4_init(enum DDR_TYPE type, int rank_num, int speed, enum DDR_BITWIDTH bits);

#endif // LPDDR4_INIT_H
