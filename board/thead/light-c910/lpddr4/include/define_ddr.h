#define DDR_SYSREG_BADDR 0xffff005000
#define _DDR_PHY_BADDR   0xfffd000000
#define _DDR_PHY1_BADDR   _DDR_PHY_BADDR+0x1000000
#define _DDR_CTRL_BADDR  _DDR_PHY_BADDR+0x2000000
#define _LPDDR4_BADDR DDR_AXI4S0_BADDR
#define _LPDDR4_EADDR DDR_AXI4S0_EADDR
/* fixed in ddr_reg_define.h or c_header.h
#define  FREQ1_DERATEEN            _DDR_CTRL_BADDR+0x00002020 
#define  FREQ1_DERATEINT           _DDR_CTRL_BADDR+0x00002024 
#define  FREQ1_DFITMG0             _DDR_CTRL_BADDR+0x00002190 
#define  FREQ1_DFITMG1             _DDR_CTRL_BADDR+0x00002194 
#define  FREQ1_DFITMG2             _DDR_CTRL_BADDR+0x000021b4 
#define  FREQ1_DFITMG3             _DDR_CTRL_BADDR+0x000021b8 
#define  FREQ1_DRAMTMG0            _DDR_CTRL_BADDR+0x00002100 
#define  FREQ1_DRAMTMG1            _DDR_CTRL_BADDR+0x00002104 
#define  FREQ1_DRAMTMG10           _DDR_CTRL_BADDR+0x00002128 
#define  FREQ1_DRAMTMG11           _DDR_CTRL_BADDR+0x0000212c 
#define  FREQ1_DRAMTMG12           _DDR_CTRL_BADDR+0x00002130 
#define  FREQ1_DRAMTMG13           _DDR_CTRL_BADDR+0x00002134 
#define  FREQ1_DRAMTMG14           _DDR_CTRL_BADDR+0x00002138 
#define  FREQ1_DRAMTMG15           _DDR_CTRL_BADDR+0x0000213c 
#define  FREQ1_DRAMTMG2            _DDR_CTRL_BADDR+0x00002108 
#define  FREQ1_DRAMTMG3            _DDR_CTRL_BADDR+0x0000210c 
#define  FREQ1_DRAMTMG4            _DDR_CTRL_BADDR+0x00002110 
#define  FREQ1_DRAMTMG5            _DDR_CTRL_BADDR+0x00002114 
#define  FREQ1_DRAMTMG6            _DDR_CTRL_BADDR+0x00002118 
#define  FREQ1_DRAMTMG7            _DDR_CTRL_BADDR+0x0000211c 
#define  FREQ1_DRAMTMG8            _DDR_CTRL_BADDR+0x00002120 
#define  FREQ1_DRAMTMG9            _DDR_CTRL_BADDR+0x00002124 
#define  FREQ1_INIT3               _DDR_CTRL_BADDR+0x000020dc 
#define  FREQ1_INIT4               _DDR_CTRL_BADDR+0x000020e0 
#define  FREQ1_INIT6               _DDR_CTRL_BADDR+0x000020e8 
#define  FREQ1_INIT7               _DDR_CTRL_BADDR+0x000020ec 
#define  FREQ1_ODTCFG              _DDR_CTRL_BADDR+0x00002240 
#define  FREQ1_PWRTMG              _DDR_CTRL_BADDR+0x00002034 
#define  FREQ1_RANKCTL             _DDR_CTRL_BADDR+0x000020f4 
#define  FREQ1_RFSHCTL0            _DDR_CTRL_BADDR+0x00002050 
#define  FREQ1_RFSHTMG             _DDR_CTRL_BADDR+0x00002064 
#define  FREQ1_RFSHTMG1            _DDR_CTRL_BADDR+0x00002068 
#define  FREQ1_ZQCTL0              _DDR_CTRL_BADDR+0x00002180
*/
#define  PUB_RDIMMCR1              0x00000055 

#include "ddr_reg_define.h"

#ifdef USE_LPDDR4
#define DDR_ADDRMAP0      0x0000001f//only one active rank,cs_bit0 not used    
#define DDR_ADDRMAP1      0x00080808//b2 b1 b0                                 
#define DDR_ADDRMAP2      0x00000000//c5 c4 c3 c2                              
#define DDR_ADDRMAP3      0x00000000//c9 c8 c7 c6                              
#define DDR_ADDRMAP4      0x00000f0f//c11 c10(not used,so config f)            
#define DDR_ADDRMAP5      0x07070707//r11 r10-r2 r1 r0                         
#define DDR_ADDRMAP6      0x0f070707//r15(not used) r14 r13 r12                
#define DDR_ADDRMAP7      0xffffffff//not used
#define DDR_ADDRMAP8      0xffffffff//not used
#define DDR_ADDRMAP9      0xffffffff//not used
#define DDR_ADDRMAP10     0xffffffff//not used
#define DDR_ADDRMAP11     0xffffffff//not used
#endif

#ifdef USE_DDR4
#define DDR_ADDRMAP0     0x0000001f
#define DDR_ADDRMAP1     0x003f0909
#define DDR_ADDRMAP2     0x01010100
#define DDR_ADDRMAP3     0x01010101
#define DDR_ADDRMAP4     0x00001f1f
#define DDR_ADDRMAP5     0x070f0707
#define DDR_ADDRMAP6     0x07070707
#define DDR_ADDRMAP7     0x00000f0f
#define DDR_ADDRMAP8     0x00003f01
#define DDR_ADDRMAP9     0x07070707
#define DDR_ADDRMAP10    0x07070707
#define DDR_ADDRMAP11    0x00000007
#endif

#define DDR_INIT_STATUS_MAILBOX           SIM_CTRL_BADDR+0x000000f0
#define DDR_INIT_FINISH_FLAG              0x88888888
#define DDR_CODE_DOWNLOAD_FINISH_FLAG     0x99999999
#define DDR_INIT_LOAD_FW_FLAG             0x11111111
#define DDR_INIT_LOAD_FW_FINISH_FLAG      0x22222222
