#include "../include/common_lib.h"
#include "../include/ddr_common_func.h"
#include "../include/lpddr4_init.h"

const char board_name[] = "FM";

void init_ddr(void)
{
    enum DDR_TYPE type = DDR_TYPE_LPDDR4X;
    int freq = 3733;
    int rank_num = 1;
    enum DDR_BITWIDTH bits = DDR_BITWIDTH_64;
    bool dbi_off = false;
    int board_ver = 1;
#ifdef CONFIG_DDR_MSG
    printf("enter init_ddr\n");
#endif
#ifdef CONFIG_DDR_H32_MODE
    bits = DDR_BITWIDTH_32;
#elif CONFIG_DDR_H16_MODE
    bits = DDR_BITWIDTH_16;
#else
    bits = DDR_BITWIDTH_64;
#endif

#ifdef CONFIG_DDR_DUAL_RANK
    rank_num = 2;
#endif

#ifdef CONFIG_LPDDR4X
    type = DDR_TYPE_LPDDR4X;
#elif defined CONFIG_LPDDR4
    type = DDR_TYPE_LPDDR4;
#else
    printf("unsupport lpddr4 type!!!\n");
    return;
#endif // #ifdef CONFIG_LPDDR4X

#ifdef CONFIG_DDR_4266
    freq = 4266;
#elif defined CONFIG_DDR_3733
    freq = 3733;
#elif defined CONFIG_DDR_3200
    freq = 3200;
#elif defined CONFIG_DDR_2133
    freq = 2133;
#else
    printf("unsupport ddr freq config!!!\n");
    return;
#endif // #ifdef CONFIG_DDR4_4266

#ifdef CONFIG_DDR_DBI_OFF
    dbi_off = true;
#endif

    printf("%s[%d] lpddr4%c %s freq=%d %dbit dbi_off=%c sdram init\n", board_name, board_ver, (type==DDR_TYPE_LPDDR4X?'x':' '), (rank_num==1?"singlerank":"dualrank"), freq, bits, (dbi_off==true?'y':'n'));
#ifdef CONFIG_LPDDR_EYE
    printf("lpddr diag eye test\n");
    lp4_diag_eye(type, rank_num, freq, bits);
#else
    lpddr4_init(type, rank_num, freq, bits);
#endif

#ifdef CONFIG_DDR_MSG
    printf("exit init_ddr\n");
#endif
}
