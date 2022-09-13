#define apb_wr_4byte(addr,wdata) *(volatile unsigned int*)(addr) = wdata
#define apb_rd_4byte(addr,rdata) rdata = *(volatile unsigned int*)(addr)
#define apb_wr_2byte(addr,wdata) *(volatile unsigned short*)(addr) = (unsigned short)(wdata)
#define apb_rd_2byte(addr,rdata) rdata = (unsigned int)(*(volatile unsigned short*)(addr))

#include <common.h>
#include <console.h>

extern void ddr_fw_run(void);

void init_ddr(void)
{
unsigned int temp_reg;

//DDR4 enable, dfi1 disable
apb_wr_4byte(0xfe830264, 0x00000002);
apb_wr_4byte(0xfe830264, 0x00000002);
apb_wr_4byte(0xfe830264, 0x00000002);
apb_wr_4byte(0xfe830264, 0x00000002);
apb_wr_4byte(0xfe830264, 0x00000002);

apb_wr_4byte(0xfe820304, 0x00000001);
apb_wr_4byte(0xfe820030, 0x00000001);
apb_rd_4byte(0xfe820004, temp_reg);
apb_wr_4byte(0xfe820000, 0x81040210);
apb_wr_4byte(0xfe820010, 0x00003030);
apb_wr_4byte(0xfe820014, 0x00025a4b);
apb_wr_4byte(0xfe82001c, 0x931a87e5);
apb_wr_4byte(0xfe820020, 0x00000202);
apb_wr_4byte(0xfe820024, 0x500e985a);
apb_wr_4byte(0xfe82002c, 0x00000000);
apb_wr_4byte(0xfe820030, 0x000001ac);
apb_wr_4byte(0xfe820034, 0x0001b402);
apb_wr_4byte(0xfe820038, 0x007e0001);
apb_wr_4byte(0xfe820050, 0x00a1f0b0);
apb_wr_4byte(0xfe820054, 0x00050004);
apb_wr_4byte(0xfe820060, 0x00000020);
apb_wr_4byte(0xfe820064, 0x00180068);
apb_wr_4byte(0xfe820068, 0x00010000);
//apb_wr_4byte(0xfe820070, 0xa0186614); //ecc
//apb_wr_4byte(0xfe820074, 0x00000112); //ecc
apb_wr_4byte(0xfe82007c, 0x00000300);
apb_wr_4byte(0xfe8200b8, 0x00000ae3);
apb_wr_4byte(0xfe8200bc, 0x00006810);
apb_wr_4byte(0xfe8200c0, 0x00000000);
apb_wr_4byte(0xfe8200c4, 0x0);//0x00001001);
apb_wr_4byte(0xfe8200d0, 0xc0030002);
apb_wr_4byte(0xfe8200d4, 0x0001000e);
apb_wr_4byte(0xfe8200d8, 0x0000f601);

apb_wr_4byte(0xfe8200dc, 0x02150101);//MR0=0x215, MR1=0x1
apb_wr_4byte(0xfe8200e0, 0x00000080);//MR2=0x0,   MR3=0x80
apb_wr_4byte(0xfe8200e8, 0x00000641);//MR4=0x0,   MR5=0x640
apb_wr_4byte(0xfe8200ec, 0x00000400);//MR6=0x400

apb_wr_4byte(0xfe8200e4, 0x001102d0);
apb_wr_4byte(0xfe8200f0, 0x00000000);
apb_wr_4byte(0xfe8200f4, 0x0000f220);
apb_wr_4byte(0xfe820100, 0x0e0e1a0e);
apb_wr_4byte(0xfe820104, 0x00050414);
apb_wr_4byte(0xfe820108, 0x0708050e);
apb_wr_4byte(0xfe82010c, 0x0000e00e);
apb_wr_4byte(0xfe820110, 0x06030307);
apb_wr_4byte(0xfe820114, 0x090a0302);
apb_wr_4byte(0xfe820118, 0x04010002);
apb_wr_4byte(0xfe82011c, 0x00000a09);
apb_wr_4byte(0xfe820120, 0x06060b09);
apb_wr_4byte(0xfe820124, 0x0002030c);
apb_wr_4byte(0xfe820128, 0x000f0d0b);
apb_wr_4byte(0xfe82012c, 0x12060119);
apb_wr_4byte(0xfe820130, 0x0e000008);
apb_wr_4byte(0xfe820134, 0x00000000);
apb_wr_4byte(0xfe820138, 0x00000000);
apb_wr_4byte(0xfe82013c, 0x80000000);
apb_wr_4byte(0xfe820180, 0x01000040);
apb_wr_4byte(0xfe820184, 0x00000070);
apb_wr_4byte(0xfe820188, 0x00000000);
apb_wr_4byte(0xfe820190, 0x048b8208);
apb_wr_4byte(0xfe820194, 0x00070303);
apb_wr_4byte(0xfe820198, 0x03919101);
apb_wr_4byte(0xfe82019c, 0x000000b0);
apb_wr_4byte(0xfe8201a0, 0xa0400018);
apb_wr_4byte(0xfe8201a4, 0x0039008d);
apb_wr_4byte(0xfe8201a8, 0x80000000);
apb_wr_4byte(0xfe8201b0, 0x00000051);
apb_wr_4byte(0xfe8201b4, 0x00000b08);
apb_wr_4byte(0xfe8201b8, 0x00000017);
apb_wr_4byte(0xfe8201c0, 0x00000001);
apb_wr_4byte(0xfe8201c4, 0x00000000);


//apb_wr_4byte(0xfe820200, 0x0000001f);
//apb_wr_4byte(0xfe820204, 0x003f1311);
//apb_wr_4byte(0xfe820208, 0x00000500);
//apb_wr_4byte(0xfe82020c, 0x14141400);
//apb_wr_4byte(0xfe820210, 0x00001f1f);
//apb_wr_4byte(0xfe820214, 0x04010b01);
//apb_wr_4byte(0xfe820218, 0x05000406);
//apb_wr_4byte(0xfe82021c, 0x00000f03);
//apb_wr_4byte(0xfe820220, 0x00003f01);
//apb_wr_4byte(0xfe820224, 0x0b050605);
//apb_wr_4byte(0xfe820228, 0x02090808);
//apb_wr_4byte(0xfe82022c, 0x00000002);

//by gaohf at 2019/05/05, RBC (1BG, 2BA,16R,10C,16bit)
//8Gb x 4pcs = 4GB
//Mapping: {ROW, BG, BA, COL}
apb_wr_4byte(0xfe820200, 0x001f1f1f);
apb_wr_4byte(0xfe820204, 0x003f0808);
apb_wr_4byte(0xfe820208, 0x00000000);
apb_wr_4byte(0xfe82020c, 0x00000000);
apb_wr_4byte(0xfe820210, 0x00001f1f);
apb_wr_4byte(0xfe820214, 0x07070707);
apb_wr_4byte(0xfe820218, 0x07070707);
apb_wr_4byte(0xfe82021c, 0x00000f0f);
apb_wr_4byte(0xfe820220, 0x00003f0a);
apb_wr_4byte(0xfe820224, 0x00000000);
apb_wr_4byte(0xfe820228, 0x00000000);
apb_wr_4byte(0xfe82022c, 0x001f1f00);


apb_wr_4byte(0xfe820240, 0x05190544);
apb_wr_4byte(0xfe820244, 0x00002111);
apb_wr_4byte(0xfe820250, 0x22cd0a04);
apb_wr_4byte(0xfe820254, 0x00000000);
apb_wr_4byte(0xfe82025c, 0x94003e74);
apb_wr_4byte(0xfe820264, 0xd4006987);
apb_wr_4byte(0xfe82026c, 0x540082e3);
apb_wr_4byte(0xfe820300, 0x00000040);
apb_wr_4byte(0xfe820304, 0x00000000);
apb_wr_4byte(0xfe82030c, 0x00000000);
apb_wr_4byte(0xfe820320, 0x00000001);
apb_wr_4byte(0xfe820328, 0x00000000);
apb_wr_4byte(0xfe82036c, 0x00110000);
apb_wr_4byte(0xfe820374, 0x00000069);
apb_wr_4byte(0xfe82037c, 0x00000000);
apb_wr_4byte(0xfe820380, 0x00000000);
apb_wr_4byte(0xfe820384, 0x00000000);
apb_wr_4byte(0xfe820400, 0x00000010);
apb_wr_4byte(0xfe820404, 0x0000428c);
apb_wr_4byte(0xfe820408, 0x0000226f);
apb_wr_4byte(0xfe820490, 0x00000001);
apb_wr_4byte(0xfe820494, 0x00110002);
apb_wr_4byte(0xfe820498, 0x05630051);
apb_wr_4byte(0xfe82049c, 0x01100803);
apb_wr_4byte(0xfe8204a0, 0x0453025a);
apb_wr_4byte(0xfe8204b4, 0x0000700f);
apb_wr_4byte(0xfe8204b8, 0x00000351);
apb_wr_4byte(0xfe820540, 0x00000001);
apb_wr_4byte(0xfe820544, 0x00110005);
apb_wr_4byte(0xfe820548, 0x025400cc);
apb_wr_4byte(0xfe82054c, 0x01000d0c);
apb_wr_4byte(0xfe820550, 0x00f60002);
apb_wr_4byte(0xfe820564, 0x00004264);
apb_wr_4byte(0xfe820568, 0x0000123a);
apb_wr_4byte(0xfe8205f0, 0x00000001);
apb_wr_4byte(0xfe8205f4, 0x00100007);
apb_wr_4byte(0xfe8205f8, 0x06fa009a);
apb_wr_4byte(0xfe8205fc, 0x01010c04);
apb_wr_4byte(0xfe820600, 0x02af0645);
apb_wr_4byte(0xfe820614, 0x000062ae);
apb_wr_4byte(0xfe820618, 0x000033a3);
apb_wr_4byte(0xfe8206a0, 0x00000001);
apb_wr_4byte(0xfe8206a4, 0x00010006);
apb_wr_4byte(0xfe8206a8, 0x005c074e);
apb_wr_4byte(0xfe8206ac, 0x01010e02);
apb_wr_4byte(0xfe8206b0, 0x07ca01e1);
apb_wr_4byte(0xfe8206c4, 0x0000309b);
apb_wr_4byte(0xfe8206c8, 0x00002106);
apb_wr_4byte(0xfe820750, 0x00000001);
apb_wr_4byte(0xfe820754, 0x00200003);
apb_wr_4byte(0xfe820758, 0x00990067);
apb_wr_4byte(0xfe82075c, 0x01100702);
apb_wr_4byte(0xfe820760, 0x04d105b9);
apb_wr_4byte(0xfe820774, 0x000040b9);
apb_wr_4byte(0xfe820778, 0x000071cb);
apb_wr_4byte(0xfe820800, 0x00000001);
apb_wr_4byte(0xfe820804, 0x0002000d);
apb_wr_4byte(0xfe820808, 0x0490045a);
apb_wr_4byte(0xfe82080c, 0x01010908);
apb_wr_4byte(0xfe820810, 0x07e70644);
apb_wr_4byte(0xfe822020, 0x00000202);
apb_wr_4byte(0xfe822024, 0x500e985a);
apb_wr_4byte(0xfe822034, 0x0001b402);
apb_wr_4byte(0xfe822050, 0x00a1f0b0);
apb_wr_4byte(0xfe822064, 0x00188068);
apb_wr_4byte(0xfe822068, 0x00010000);

apb_wr_4byte(0xfe8220dc, 0x02150101);//MR0=0x215, MR1=0x1
apb_wr_4byte(0xfe8220e0, 0x00000080);//MR2=0x0,   MR3=0x80
apb_wr_4byte(0xfe8220e8, 0x00000641);//MR4=0x0,   MR5=0x640
apb_wr_4byte(0xfe8220ec, 0x00000400);//MR6=0x400

apb_wr_4byte(0xfe8220f4, 0x0000a220);
apb_wr_4byte(0xfe822100, 0x0e0e1a0e);
apb_wr_4byte(0xfe822104, 0x00050414);
apb_wr_4byte(0xfe822108, 0x0708050e);
apb_wr_4byte(0xfe82210c, 0x0000e00e);
apb_wr_4byte(0xfe822110, 0x06030307);
apb_wr_4byte(0xfe822114, 0x090a0302);
apb_wr_4byte(0xfe822118, 0x04010002);
apb_wr_4byte(0xfe82211c, 0x00000a09);
apb_wr_4byte(0xfe822120, 0x06060b09);
apb_wr_4byte(0xfe822124, 0x0002030c);
apb_wr_4byte(0xfe822128, 0x000f0d0b);
apb_wr_4byte(0xfe82212c, 0x12060119);
apb_wr_4byte(0xfe822130, 0x0e000008);
apb_wr_4byte(0xfe822134, 0x00000000);
apb_wr_4byte(0xfe822138, 0x00000000);
apb_wr_4byte(0xfe82213c, 0x80000000);
apb_wr_4byte(0xfe822180, 0x01000040);
apb_wr_4byte(0xfe822190, 0x048b8208);
apb_wr_4byte(0xfe822194, 0x00070303);
apb_wr_4byte(0xfe8221b4, 0x00000b08);
apb_wr_4byte(0xfe8221b8, 0x00000017);
apb_wr_4byte(0xfe822240, 0x05190544);
apb_rd_4byte(0xfe820060, temp_reg);
apb_rd_4byte(0xfe820030, temp_reg);
apb_wr_4byte(0xfe820030, 0x000001a4);

//Release Resetn
apb_wr_4byte(0xfff78040, 0x3ff);
apb_wr_4byte(0xfff78040, 0x3ff);
apb_wr_4byte(0xfff78040, 0x3ff);
apb_wr_4byte(0xfff78040, 0x3ff);
apb_wr_4byte(0xfff78040, 0x3ff);


/*
apb_wr_4byte(0xfe820304, 0x00000000);
apb_rd_4byte(0xfe8200cc, temp_reg);
apb_rd_4byte(0xfe820030, temp_reg);
apb_wr_4byte(0xfe820030, 0x000001a4);
apb_rd_4byte(0xfe820030, temp_reg);
apb_wr_4byte(0xfe820030, 0x000001a4);
apb_rd_4byte(0xfe820020, temp_reg);
apb_wr_4byte(0xfe820020, 0x00000202);
apb_wr_4byte(0xfe822020, 0x00000202);
apb_wr_4byte(0xfe820320, 0x00000000);
apb_wr_4byte(0xfe8201b0, 0x00000051);
apb_wr_4byte(0xfe8201b0, 0x00000050);
apb_wr_4byte(0xfe8201b0, 0x00000050);
apb_rd_4byte(0xfe8200d0, temp_reg);
apb_rd_4byte(0xfe8201c0, temp_reg);
apb_rd_4byte(0xfe820000, temp_reg);
apb_rd_4byte(0xfe820000, temp_reg);
apb_rd_4byte(0xfe8201b0, temp_reg);
apb_rd_4byte(0xfe8200dc, temp_reg);
apb_rd_4byte(0xfe8200dc, temp_reg);
apb_rd_4byte(0xfe8200e0, temp_reg);
apb_rd_4byte(0xfe8200e0, temp_reg);
apb_rd_4byte(0xfe8200e8, temp_reg);
apb_rd_4byte(0xfe8200e8, temp_reg);
apb_rd_4byte(0xfe8200ec, temp_reg);
apb_rd_4byte(0xfe8200d0, temp_reg);
*/



apb_wr_4byte(0xfe820060, 0x00000001); //disable dis_auto_refresh
//disable selref_en, powerdown_en, en_dfi_dram_clk_disable
apb_wr_4byte(0xfe820030, 0x00000020); //selfref_sw=1
apb_wr_4byte(0xfe820320, 0x0); //SWCTL.sw_done
apb_wr_4byte(0xfe8201b0, 0x00000050);//bit[12:8]:0~3:Goto PHY P0~3
                                     //bit[6]:dis_dyn_adr_tri
                                     //bit[5]:dfi_init_start
                                     //bit[4]:ctl_idle_en
                                     //bit[0]:dfi_init_complete_en, mask during phy init
apb_wr_4byte(0xfe820320, 0x1); //SWCTL.sw_done


//read SWSTAT, wait sw_done_ack = 1
apb_rd_4byte(0xfe820324,temp_reg);
while(temp_reg == 0)
{
   apb_rd_4byte(0xfe820324,temp_reg);
}
apb_wr_4byte(0xfe820304, 0x00000000); //disable CAM

apb_rd_4byte(0xfe820000, temp_reg); //delay
apb_rd_4byte(0xfe820000, temp_reg); //delay
apb_rd_4byte(0xfe820000, temp_reg); //delay
apb_rd_4byte(0xfe820000, temp_reg); //delay



//Firmware Code Here
ddr_fw_run();


//*******************************
//Switch PHY to MissionMode
//*******************************
apb_wr_4byte(0xfe820320, 0x00000000); //sw_done
apb_wr_4byte(0xfe8201b0, 0x00000070); // x0~3: Goto PHY P0~3,
                                      // bit[6]: dis_dyn_adr_tri
                                      // bit[5]: dfi_init_start
                                      // bit[4]: ctl_idle_en
                                      // bit[0]: dfi_init_complete_en after train
apb_wr_4byte(0xfe820320, 0x00000001); //sw_done

//read SWSTAT, wait sw_done_ack = 1
apb_rd_4byte(0xfe820324,temp_reg);
while(temp_reg == 0)
{
   apb_rd_4byte(0xfe820324,temp_reg);
}

//Poll DFISTAT, wait dfi_init_complete
apb_rd_4byte(0xfe8201bc,temp_reg);
while(temp_reg == 0)
{
   apb_rd_4byte(0xfe8201bc,temp_reg);
}

//******************************
// Switch CTRL to Normal Mode
//******************************
//updata DFIMISC
apb_wr_4byte(0xfe820320, 0x00000000); //sw_done, write prepare
apb_wr_4byte(0xfe8201b0, 0x00000050); //x0~3: Goto PHY P0~3,
                                        // bit[6]: dis_dyn_adr_tri=0
                                        // bit[5]: dfi_init_start
                                        // bit[4]: ctl_idle_en
apb_wr_4byte(0xfe8201b0, 0x00000051); // bit[0]: dfi_init_complete_en after train
apb_wr_4byte(0xfe820030, 0x00000000); // bit[5]: software exit from selfref
apb_wr_4byte(0xfe820320, 0x00000001); //sw_done, write into

//read SWSTAT, wait sw_done_ack = 1
apb_rd_4byte(0xfe820324,temp_reg);
while(temp_reg == 0x0)
{
   apb_rd_4byte(0xfe820324,temp_reg);
}


//STAT, operation_mode
//      3'h0 : Init
//      3'h1 : Normal
//      3'h2 : Power-down
//      3'h3 : Self-refresh/Self-refresh power-down
apb_rd_4byte(0xfe820004,temp_reg);
while((temp_reg & 0x7) != 0x1)
{
   apb_rd_4byte(0xfe820004,temp_reg);
}


//apb_wr_4byte(0xfe822020, 0x00000202);
//apb_wr_4byte(0xfe8200f4, 0x0000f360);
//apb_wr_4byte(0xfe820108, 0x0a0c0b14);

//Enable AXI ports
apb_wr_4byte(0xfe820060, 0x00000000);//enable auto_refresh
apb_wr_4byte(0xfe820304, 0x00000000);//enable CAM
apb_wr_4byte(0xfe820030, 0x00000000);
//apb_wr_4byte(0xfe820030, 0x0000000a); //auto-dis-clk/pwr_down
apb_wr_4byte(0xfe820490, 0x00000001);
apb_wr_4byte(0xfe820540, 0x00000001);
apb_wr_4byte(0xfe8205f0, 0x00000001);
apb_wr_4byte(0xfe8206a0, 0x00000001);
apb_wr_4byte(0xfe820750, 0x00000001);
apb_wr_4byte(0xfe820800, 0x00000001);

}
