#include "../include/common_lib.h"
#include "../include/ddr_common_func.h"

// L0 remove DDR_FW_STAGE_MSG and DDR_FW_DETAIL_MSG , only show init OK
#ifdef CONFIG_FW_MSG_L1
#define DDR_FW_STAGE_MSG    //L1  only show stage message
#endif

#ifdef CONFIG_FW_MSG_L2
#define DDR_FW_DETAIL_MSG   //1D and 2D Stream Message display
#define DDR_FW_2D_BRIEF_MSG       //L2 Message  show 2D brief message
#endif

#ifdef CONFIG_FW_MSG_L3
#define DDR_FW_DETAIL_MSG   //L3  Full  message (should define DDR_FW_STAGE_MSG)
#undef DDR_FW_2D_BRIEF_MSG        //L3 Message  show 2D detail message
#endif

#ifdef CONFIG_FW_MSG_TRAIN_1D
#define DDR_FW_TRAIN_1D_MSG
#endif

#ifdef DDR_FW_DETAIL_MSG
 #undef DDR_FW_STAGE_MSG
 #define DDR_FW_STAGE_MSG
#endif


#ifdef DDR_FW_DETAIL_MSG
//1D and 2D Stream Message display
void st_msg_display(unsigned char Train2D, unsigned int msg[32])
{

  if(Train2D) {
         switch(msg[0]){
#ifdef DDR_FW_2D_BRIEF_MSG
       //     case 0x01410000:printf("2D: PMU: Error: CA Training Failed.\n"); break;
       //     case 0x00750003:printf("2D: PMU: Error: Dbyte %d nibble %d's optimal rxClkDly of 0x%x is out of bounds\n",msg[1],msg[2],msg[3]); break;
       //     case 0x007b0003:printf("2D: PMU: Error: Dbyte %d lane %d's optimal txDqDly of 0x%x is out of bounds\n",msg[1],msg[2],msg[3]); break;
       //     case 0x01080001:printf("2D: PMU: Error: Dbyte %d couldn't find the rising edge of DQS during RxEn Training\n",msg[1]); break;
       //     case 0x00a90001:printf("2D: PMU: Error: CS%d failed to find a DFIMRL setting that worked for all bytes during MaxRdLat training\n",msg[1]); break;
       //     case 0x00ab0000:printf("2D: PMU: Error: No passing DFIMRL value found for any chip select during MaxRdLat training\n"); break;
       //     case 0x00ac0003:printf("2D: PMU: Error: Dbyte %d lane %d txDqDly passing region is too small (width = %d)\n",msg[1],msg[2],msg[3]); break;
       //     case 0x00160004:printf("2D: PMU4: -- DB%d L%d -- centers: delay = %d, voltage = %d \n",msg[1],msg[2],msg[3],msg[4]); break;
       //     case 0x001d0004:printf("2D: PMU4: -------- %dD Write Scanning TG %d (CS 0x%x) Lanes 0x%03x --------\n",msg[1],msg[2],msg[3],msg[4]); break;
       //     case 0x00200004:printf("2D: PMU4: ------- %dD Read Scanning TG %d (CS 0x%x) Lanes 0x%03x -------\n",msg[1],msg[2],msg[3],msg[4]); break;
       //     case 0x0014001f:printf("2D: PMU4: %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d >%3d< %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7],msg[8],msg[9],msg[10],msg[11],msg[12],msg[13],msg[14],msg[15],msg[16],msg[17],msg[18],msg[19],msg[20],msg[21],msg[22],msg[23],msg[24],msg[25],msg[26],msg[27],msg[28],msg[29],msg[30],msg[31]); break;
       //     default: printf("2D:  MSG is %x",msg[0]); break;
#else
            case 0x00000001:printf("2D: PMU0: Converting %d into an MR\n",msg[1]); break;
            case 0x00010003:printf("2D: PMU DEBUG: vref_idx %d -= %d, range_idx = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x00020002:printf("2D: PMU0: vrefIdx. Passing range %d, remaining vrefidx = %d\n",msg[1],msg[2]); break;
            case 0x00030002:printf("2D: PMU0: VrefIdx %d -> MR[6:0] 0x%02x\n",msg[1],msg[2]); break;
            case 0x00040001:printf("2D: PMU0: Converting MR 0x%04x to vrefIdx\n",msg[1]); break;
            case 0x00050002:printf("2D: PMU0: DAC %d Range %d\n",msg[1],msg[2]); break;
            case 0x00060003:printf("2D: PMU0: Range %d, Range_idx %d, vref_idx offset %d \n",msg[1],msg[2],msg[3]); break;
            case 0x00070002:printf("2D: PMU0: MR 0x%04x -> VrefIdx %d\n",msg[1],msg[2]); break;
            case 0x00080001:printf("2D: PMU: Error: Illegal timing group number ,%d, in getPtrVrefDq\n",msg[1]); break;
            case 0x00090003:printf("2D: PMU1: VrefDqR%dNib%d = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x000a0003:printf("2D: PMU0: VrefDqR%dNib%d = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x000b0000:printf("2D: PMU0: ----------------MARGINS-------\n"); break;
            case 0x000c0002:printf("2D: PMU0: R%d_RxClkDly_Margin = %d\n",msg[1],msg[2]); break;
            case 0x000d0002:printf("2D: PMU0: R%d_VrefDac_Margin = %d\n",msg[1],msg[2]); break;
            case 0x000e0002:printf("2D: PMU0: R%d_TxDqDly_Margin = %d\n",msg[1],msg[2]); break;
            case 0x000f0002:printf("2D: PMU0: R%d_DeviceVref_Margin = %d\n",msg[1],msg[2]); break;
            case 0x00100000:printf("2D: PMU0: -----------------------\n"); break;
            case 0x00110003:printf("2D: PMU0: eye %d's for all TG's is [%d ... %d]\n",msg[1],msg[2],msg[3]); break;
            case 0x00120000:printf("2D: PMU0: ------- settingWeight -----\n"); break;
            case 0x00130002:printf("2D: PMU0: Weight %d @ Setting %d\n",msg[1],msg[2]); break;
            case 0x0014001f:printf("2D: PMU4: %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d >%3d< %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7],msg[8],msg[9],msg[10],msg[11],msg[12],msg[13],msg[14],msg[15],msg[16],msg[17],msg[18],msg[19],msg[20],msg[21],msg[22],msg[23],msg[24],msg[25],msg[26],msg[27],msg[28],msg[29],msg[30],msg[31]); break;
            case 0x00150002:printf("2D: PMU3: Voltage Range = [%d, %d]\n",msg[1],msg[2]); break;
            case 0x00160004:printf("2D: PMU4: -- DB%d L%d -- centers: delay = %d, voltage = %d \n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x00170001:printf("2D: PMU5: <<KEY>> 0 TxDqDlyTg%d <<KEY>> coarse(6:6) fine(5:0)\n",msg[1]); break;
            case 0x00180001:printf("2D: PMU5: <<KEY>> 0 messageBlock VrefDqR%d <<KEY>> MR6(6:0)\n",msg[1]); break;
            case 0x00190001:printf("2D: PMU5: <<KEY>> 0 RxClkDlyTg%d <<KEY>> fine(5:0)\n",msg[1]); break;
            case 0x001a0003:printf("2D: PMU0: tgToCsn: tg %d + 0x%04x -> csn %d\n",msg[1],msg[2],msg[3]); break;
            case 0x001b0002:printf("2D: PMU: Error: LP4 rank %d cannot be mapped on tg %d\n",msg[1],msg[2]); break;
            case 0x001c0002:printf("2D: PMU3: Sending vref %d,  Mr = 0X%05x, to all devices\n",msg[1],msg[2]); break;
            case 0x001d0004:printf("2D: PMU4: -------- %dD Write Scanning TG %d (CS 0x%x) Lanes 0x%03x --------\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x001e0002:printf("2D: PMU0: training lanes 0x%03x using lanes 0x%03x\n",msg[1],msg[2]); break;
            case 0x001f0003:printf("2D: PMU4: ------- 2D-DFE Read Scanning TG %d (CS 0x%x) Lanes 0x%03x -------\n",msg[1],msg[2],msg[3]); break;
            case 0x00200004:printf("2D: PMU4: ------- %dD Read Scanning TG %d (CS 0x%x) Lanes 0x%03x -------\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x00210003:printf("2D: PMU4: TG%d MR1[13,6,5]=0x%x MR6[13,9,8]=0x%x\n",msg[1],msg[2],msg[3]); break;
            case 0x00220002:printf("2D: PMU0: training lanes 0x%03x using lanes 0x%03x\n",msg[1],msg[2]); break;
            case 0x00230003:printf("2D: PMU4: ------- 2D-DFE Read Scanning TG %d (CS 0x%x) Lanes 0x%03x -------\n",msg[1],msg[2],msg[3]); break;
            case 0x00240004:printf("2D: PMU4: ------- %dD Read Scanning TG %d (CS 0x%x) Lanes 0x%03x -------\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x00250002:printf("2D: PMU0: training lanes 0x%03x using lanes 0x%03x\n",msg[1],msg[2]); break;
            case 0x00260002:printf("2D: PMU3: Sending vref %d,  Mr = 0X%05x, to all devices\n",msg[1],msg[2]); break;
            case 0x00270004:printf("2D: PMU4: -------- %dD Write Scanning TG %d (CS 0x%x) Lanes 0x%03x --------\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x00280001:printf("2D: PMU0: input %d\n",msg[1]); break;
            case 0x00290002:printf("2D: PMU4: Programmed Voltage Search Range [%d, %d]\n",msg[1],msg[2]); break;
            case 0x002a0002:printf("2D: PMU3: Delay Stepsize = %d Fine, Voltage Stepsize = %d DAC\n",msg[1],msg[2]); break;
            case 0x002b0002:printf("2D: PMU4: Delay Weight = %d, Voltage Weight = %d\n",msg[1],msg[2]); break;
            case 0x002c0003:printf("2D: PMU0: raw 0x%x allFine %d incDec %d",msg[1],msg[2],msg[3]); break;
            case 0x002d0008:printf("2D: PMU0: db%d l%d, voltage 0x%x (u_r %d) delay 0x%x (u_r %d) - lcdl %d mask 0x%x\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7],msg[8]); break;
            case 0x002e0005:printf("2D: PMU0: DB%d L%d, Eye %d, Seed = (0x%x, 0x%x)\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x002f0002:printf("2D: PMU3: 2D Enables       : %d,                    1,                %d\n",msg[1],msg[2]); break;
            case 0x00300006:printf("2D: PMU3: 2D Delay   Ranges: OOPL[0x%04x,0x%04x], IP[0x%04x,0x%04x], OOPR[0x%04x,0x%04x]\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6]); break;
            case 0x00310002:printf("2D: PMU3: 2D Voltage Search Range : [%d, %d]\n",msg[1],msg[2]); break;
            case 0x00320002:printf("2D: PMU4: Found Voltage Search Range [%d, %d]\n",msg[1],msg[2]); break;
            case 0x00330002:printf("2D: PMU0: User Weight = %d, Voltage Weight = %d\n",msg[1],msg[2]); break;
            case 0x00340005:printf("2D: PMU0: D(%d,%d) V(%d,%d | %d)\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00350002:printf("2D: PMU0: Norm Weight = %d, Voltage Weight = %d\n",msg[1],msg[2]); break;
            case 0x00360002:printf("2D: PMU0: seed 0 = (%d,%d) (center)\n",msg[1],msg[2]); break;
            case 0x00370003:printf("2D: PMU0: seed 1 = (%d,%d).min edge at idx %d\n",msg[1],msg[2],msg[3]); break;
            case 0x00380003:printf("2D: PMU0: seed 2 = (%d,%d) max edge at idx %d\n",msg[1],msg[2],msg[3]); break;
            case 0x00390003:printf("2D: PMU0: Search point %d = (%d,%d)\n",msg[1],msg[2],msg[3]); break;
            case 0x003a0005:printf("2D: PMU0: YMARGIN: ^ %d, - %d, v %d. rate %d = %d\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x003b0003:printf("2D: PMU0: XMARGIN: center %d, edge %d. = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x003c0002:printf("2D: PMU0: ----------- weighting (%d,%d) ----------------\n",msg[1],msg[2]); break;
            case 0x003d0003:printf("2D: PMU0: X margin - L %d R %d - Min %d\n",msg[1],msg[2],msg[3]); break;
            case 0x003e0003:printf("2D: PMU0: Y margin - L %d R %d - Min %d\n",msg[1],msg[2],msg[3]); break;
            case 0x003f0003:printf("2D: PMU0: center (%d,%d) weight = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x00400003:printf("2D: PMU4: Eye largest blob area %d from %d to %d\n",msg[1],msg[2],msg[3]); break;
            case 0x00410002:printf("2D: PMU0: Compute centroid min_x %d max_x %d\n",msg[1],msg[2]); break;
            case 0x00420003:printf("2D: PMU0: Compute centroid sumLnDlyWidth %d sumLnVrefWidth %d sumLnWidth %d\n",msg[1],msg[2],msg[3]); break;
            case 0x00430000:printf("2D: PMU: Error: No passing region found for 1 or more lanes. Set hdtCtrl=4 to see passing regions\n"); break;
            case 0x00440003:printf("2D: PMU0: Centroid ( %d, %d ) found with sumLnWidth %d\n",msg[1],msg[2],msg[3]); break;
            case 0x00450003:printf("2D: PMU0: Optimal allFine Center ( %d + %d ,%d )\n",msg[1],msg[2],msg[3]); break;
            case 0x00460004:printf("2D: PMU0: Compute sumDlyVrefWidth %d sumVrefWidth %d sumVrefDlyWidth %d sumDlyWidth %d\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x00470000:printf("2D: PMU: Error: No passing region found for 1 or more lanes. Set hdtCtrl=4 to see passing regions\n"); break;
            case 0x00480002:printf("2D: PMU0: Average center ( %d, %d ) found\n",msg[1],msg[2]); break;
            case 0x00490003:printf("2D: PMU0: Optimal allFine Center ( %d + %d ,%d )\n",msg[1],msg[2],msg[3]); break;
            case 0x004a0003:printf("2D: PMU3: point %d starting at (%d,%d)\n",msg[1],msg[2],msg[3]); break;
            case 0x004b0006:printf("2D: PMU3: point %d starting at (%d,%d) with x_step %d y_step %d for %d iterations\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6]); break;
            case 0x004c0002:printf("2D: PMU0: picking left (%d > %d)\n",msg[1],msg[2]); break;
            case 0x004d0002:printf("2D: PMU0: picking right (%d > %d)\n",msg[1],msg[2]); break;
            case 0x004e0002:printf("2D: PMU0: picking down (%d > %d)\n",msg[1],msg[2]); break;
            case 0x004f0002:printf("2D: PMU0: picking up (%d > %d)\n",msg[1],msg[2]); break;
            case 0x00500009:printf("2D: PMU3: new center @ (%3d, %3d). Moved (%2i, %2i) -- L %d, R %d, C %d, U %d, D %d\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7],msg[8],msg[9]); break;
            case 0x00510003:printf("2D: PMU3: cordNum %d improved %d to %d\n",msg[1],msg[2],msg[3]); break;
            case 0x00520000:printf("2D: PMU: Error: No passing region found for 1 or more lanes. Set hdtCtrl=4 to see passing regions\n"); break;
            case 0x00530004:printf("2D: PMU0: Optimal allFine Center ( %d + %d ,%d ), found with weight %d.\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x00540003:printf("2D: PMU0: merging lanes=%d..%d, centerMerge_t %d\n",msg[1],msg[2],msg[3]); break;
            case 0x00550001:printf("2D: PMU0: laneVal %d is disable\n",msg[1]); break;
            case 0x00560002:printf("2D: PMU0: checking common center %d against current center %d\n",msg[1],msg[2]); break;
            case 0x00570001:printf("2D: PMU: Error: getCompoundEye Called on lane%d eye with non-compatible centers\n",msg[1]); break;
            case 0x00580001:printf("2D: PMU0: laneItr %d is disable\n",msg[1]); break;
            case 0x00590005:printf("2D: PMU0: lane %d, data_idx %d, offset_idx %d, = [%d..%d]\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x005a0003:printf("2D: PMU0: lane %d, data_idx %d, offset_idx %d, offset_idx out of range!\n",msg[1],msg[2],msg[3]); break;
            case 0x005b0003:printf("2D: PMU0: mergeData[%d] = max_v_low %d, min_v_high %d\n",msg[1],msg[2],msg[3]); break;
            case 0x005c0005:printf("2D: PMU1: writing merged center (%d,%d) back to dataBlock[%d]. doDelay %d, doVoltage %d\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x005d0005:printf("2D: PMU0: applying relative (%i,%i) back to dataBlock[%d]. doDelay %d, doVoltage %d\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x005e0002:printf("2D: PMU0: drvstren %x is idx %d in the table\n",msg[1],msg[2]); break;
            case 0x005f0000:printf("2D: PMU4: truncating FFE drive strength search range. Out of drive strengths to check.\n"); break;
            case 0x00600002:printf("2D: PMU5: Weak 1 changed to pull-up %5d ohms, pull-down %5d ohms\n",msg[1],msg[2]); break;
            case 0x00610002:printf("2D: PMU5: Weak 0 changed to pull-up %5d ohms, pull-down %5d ohms\n",msg[1],msg[2]); break;
            case 0x00620003:printf("2D: PMU0: dlyMargin L %02d R %02d, min %02d\n",msg[1],msg[2],msg[3]); break;
            case 0x00630003:printf("2D: PMU0: vrefMargin T %02d B %02d, min %02d\n",msg[1],msg[2],msg[3]); break;
            case 0x00640002:printf("2D: PMU3: new minimum VrefMargin (%d < %d) recorded\n",msg[1],msg[2]); break;
            case 0x00650002:printf("2D: PMU3: new minimum DlyMargin (%d < %d) recorded\n",msg[1],msg[2]); break;
            case 0x00660000:printf("2D: PMU0: RX finding the per-nibble, per-tg rxClkDly values\n"); break;
            case 0x00670003:printf("2D: PMU0: Merging collected eyes [%d..%d) and analyzing for nibble %d's optimal rxClkDly\n",msg[1],msg[2],msg[3]); break;
            case 0x00680002:printf("2D: PMU0: -- centers: delay = %d, voltage = %d \n",msg[1],msg[2]); break;
            case 0x00690003:printf("2D: PMU0: dumping optimized eye -- centers: delay = %d (%d), voltage = %d \n",msg[1],msg[2],msg[3]); break;
            case 0x006a0000:printf("2D: PMU0: TX optimizing txDqDelays\n"); break;
            case 0x006b0001:printf("2D: PMU3: Analyzing collected eye %d for a lane's optimal TxDqDly\n",msg[1]); break;
            case 0x006c0001:printf("2D: PMU0: eye-lane %d is disable\n",msg[1]); break;
            case 0x006d0000:printf("2D: PMU0: TX optimizing device voltages\n"); break;
            case 0x006e0002:printf("2D: PMU0: Merging collected eyes [%d..%d) and analyzing for optimal device txVref\n",msg[1],msg[2]); break;
            case 0x006f0002:printf("2D: PMU0: -- centers: delay = %d, voltage = %d \n",msg[1],msg[2]); break;
            case 0x00700003:printf("2D: PMU0: dumping optimized eye -- centers: delay = %d (%d), voltage = %d \n",msg[1],msg[2],msg[3]); break;
            case 0x00710000:printf("2D: PMU4: VrefDac (compound all TG) Bottom Top -> Center\n"); break;
            case 0x00720005:printf("2D: PMU4: DB%d L%d   %3d   %3d  ->  %3d (DISCONNECTED)\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00730005:printf("2D: PMU4: DB%d L%d   %3d   %3d  ->  %3d\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00740005:printf("2D: PMU0: writing rxClkDelay for tg%d db%1d nib%1d to 0x%02x from eye[%02d] (DISCONNECTED)\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00750003:printf("2D: PMU: Error: Dbyte %d nibble %d's optimal rxClkDly of 0x%x is out of bounds\n",msg[1],msg[2],msg[3]); break;
            case 0x00760005:printf("2D: PMU0: writing rxClkDelay for tg%d db%1d nib%1d to 0x%02x from eye[%02d]\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00770005:printf("2D: PMU0: tx voltage for tg%2d nib%2d to %3d (%d) from eye[%02d]\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00780001:printf("2D: PMU0: vref Sum = %d\n",msg[1]); break;
            case 0x00790004:printf("2D: PMU0: tx voltage total is %d/%d -> %d -> %d\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x007a0007:printf("2D: PMU0: writing txDqDelay for tg%1d db%1d ln%1d to  0x%02x (%d coarse, %d fine) from eye[%02d] (DISCONNECTED)\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7]); break;
            case 0x007b0003:printf("2D: PMU: Error: Dbyte %d lane %d's optimal txDqDly of 0x%x is out of bounds\n",msg[1],msg[2],msg[3]); break;
            case 0x007c0007:printf("2D: PMU0: writing txDqDelay for tg%1d db%1d l%1d to  0x%02x (%d coarse, %d fine) from eye[%02d]\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7]); break;
            case 0x007d0002:printf("2D: PMU0: %d (0=tx, 1=rx) TgMask for this simulation: %x\n",msg[1],msg[2]); break;
            case 0x007e0001:printf("2D: PMU0: eye-byte %d is disable\n",msg[1]); break;
            case 0x007f0001:printf("2D: PMU0: eye-lane %d is disable\n",msg[1]); break;
            case 0x00800003:printf("2D: PMU10: Start d4_2d_lrdimm_rx_dfe dimm %d nbTap %d biasStepMode %d\n",msg[1],msg[2],msg[3]); break;
            case 0x00810001:printf("2D: PMU10: DB DFE feature not fully supported, F2BCEx value is 0x%02x\n",msg[1]); break;
            case 0x00820001:printf("2D: PMU10: DB DFE feature fully supported, F2BCEx value is 0x%02x\n",msg[1]); break;
            case 0x00830002:printf("2D: PMU8: Start d4_2d_lrdimm_rx_dfe for tap %d biasStepInc %d\n",msg[1],msg[2]); break;
            case 0x00840001:printf("2D: PMU7: Start d4_2d_lrdimm_rx_dfe tapCoff 0x%0x\n",msg[1]); break;
            case 0x00850003:printf("2D: PMU6: d4_2d_lrdimm_rx_dfe db %d lane %d area %d\n",msg[1],msg[2],msg[3]); break;
            case 0x00860004:printf("2D: PMU7: d4_2d_lrdimm_rx_dfe db %d lane %d max area %d best bias 0x%0x\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x00870001:printf("2D: PMU0: eye-lane %d is disable\n",msg[1]); break;
            case 0x00880003:printf("2D: PMU5: Setting 0x%x improved rank weight (%4d < %4d)\n",msg[1],msg[2],msg[3]); break;
            case 0x00890001:printf("2D: PMU4: Setting 0x%x still optimal\n",msg[1]); break;
            case 0x008a0002:printf("2D: PMU5: ---- Training CS%d MR%d DRAM Equalization ----\n",msg[1],msg[2]); break;
            case 0x008b0001:printf("2D: PMU0: eye-lane %d is disable\n",msg[1]); break;
            case 0x008c0003:printf("2D: PMU0: eye %d weight %d allTgWeight %d\n",msg[1],msg[2],msg[3]); break;
            case 0x008d0002:printf("2D: PMU5: FFE figure of merit improved from %d to %d\n",msg[1],msg[2]); break;
            case 0x008e0002:printf("2D: PMU: Error: LP4 rank %d cannot be mapped on tg %d\n",msg[1],msg[2]); break;
            case 0x008f0000:printf("2D: PMU4: Adjusting vrefDac0 for just 1->x transitions\n"); break;
            case 0x00900000:printf("2D: PMU4: Adjusting vrefDac1 for just 0->x transitions\n"); break;
            case 0x00910001:printf("2D: PMU5: Strong 1, pull-up %d ohms\n",msg[1]); break;
            case 0x00920001:printf("2D: PMU5: Strong 0, pull-down %d ohms\n",msg[1]); break;
            case 0x00930000:printf("2D: PMU4: Enabling weak drive strengths (FFE)\n"); break;
            case 0x00940000:printf("2D: PMU5: Changing all weak driver strengths\n"); break;
            case 0x00950000:printf("2D: PMU5: Finalizing weak drive strengths\n"); break;
            case 0x00960000:printf("2D: PMU4: retraining with optimal drive strength settings\n"); break;
            case 0x00970002:printf("2D: PMU0: targetting CsX = %d and CsY = %d\n",msg[1],msg[2]); break;
            case 0x00980001:printf("2D: PMU1:prbsGenCtl:%x\n",msg[1]); break;
            case 0x00990000:printf("2D: PMU1: loading 2D acsm sequence\n"); break;
            case 0x009a0000:printf("2D: PMU1: loading 1D acsm sequence\n"); break;
            case 0x009b0002:printf("2D: PMU3: %d memclocks @ %d to get half of 300ns\n",msg[1],msg[2]); break;
            case 0x009c0000:printf("2D: PMU: Error: User requested MPR read pattern for read DQS training in DDR3 Mode\n"); break;
            case 0x009d0000:printf("2D: PMU3: Running 1D search for left eye edge\n"); break;
            case 0x009e0001:printf("2D: PMU1: In Phase Left Edge Search cs %d\n",msg[1]); break;
            case 0x009f0001:printf("2D: PMU1: Out of Phase Left Edge Search cs %d\n",msg[1]); break;
            case 0x00a00000:printf("2D: PMU3: Running 1D search for right eye edge\n"); break;
            case 0x00a10001:printf("2D: PMU1: In Phase Right Edge Search cs %d\n",msg[1]); break;
            case 0x00a20001:printf("2D: PMU1: Out of Phase Right Edge Search cs %d\n",msg[1]); break;
            case 0x00a30001:printf("2D: PMU1: mxRdLat training pstate %d\n",msg[1]); break;
            case 0x00a40001:printf("2D: PMU1: mxRdLat search for cs %d\n",msg[1]); break;
            case 0x00a50001:printf("2D: PMU0: MaxRdLat non consistant DtsmLoThldXingInd 0x%03x\n",msg[1]); break;
            case 0x00a60003:printf("2D: PMU4: CS %d Dbyte %d worked with DFIMRL = %d DFICLKs \n",msg[1],msg[2],msg[3]); break;
            case 0x00a70004:printf("2D: PMU3: MaxRdLat Read Lane err mask for csn %d, DFIMRL %2d DFIClks, dbyte %d = 0x%03x\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x00a80003:printf("2D: PMU3: MaxRdLat Read Lane err mask for csn %d DFIMRL %2d, All dbytes = 0x%03x\n",msg[1],msg[2],msg[3]); break;
            case 0x00a90001:printf("2D: PMU: Error: CS%d failed to find a DFIMRL setting that worked for all bytes during MaxRdLat training\n",msg[1]); break;
            case 0x00aa0002:printf("2D: PMU3: Smallest passing DFIMRL for all dbytes in CS%d = %d DFIClks\n",msg[1],msg[2]); break;
            case 0x00ab0000:printf("2D: PMU: Error: No passing DFIMRL value found for any chip select during MaxRdLat training\n"); break;
            case 0x00ac0003:printf("2D: PMU: Error: Dbyte %d lane %d txDqDly passing region is too small (width = %d)\n",msg[1],msg[2],msg[3]); break;
            case 0x00ad0006:printf("2D: PMU4: Adjusting rxclkdly db %d nib %d from %d+%d=%d->%d\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6]); break;
            case 0x00ae0000:printf("2D: PMU4: TxDqDly Passing Regions (EyeLeft EyeRight -> EyeCenter) Units=1/32 UI\n"); break;
            case 0x00af0005:printf("2D: PMU4: DB %d Lane %d: %3d %3d -> %3d\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00b00002:printf("2D: PMU2: TXDQ delayLeft[%2d] = %3d (DISCONNECTED)\n",msg[1],msg[2]); break;
            case 0x00b10004:printf("2D: PMU2: TXDQ delayLeft[%2d] = %3d oopScaled = %3d selectOop %d\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x00b20002:printf("2D: PMU2: TXDQ delayRight[%2d] = %3d (DISCONNECTED)\n",msg[1],msg[2]); break;
            case 0x00b30004:printf("2D: PMU2: TXDQ delayRight[%2d] = %3d oopScaled = %3d selectOop %d\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x00b40003:printf("2D: PMU: Error: Dbyte %d lane %d txDqDly passing region is too small (width = %d)\n",msg[1],msg[2],msg[3]); break;
            case 0x00b50000:printf("2D: PMU4: TxDqDly Passing Regions (EyeLeft EyeRight -> EyeCenter) Units=1/32 UI\n"); break;
            case 0x00b60002:printf("2D: PMU4: DB %d Lane %d: (DISCONNECTED)\n",msg[1],msg[2]); break;
            case 0x00b70005:printf("2D: PMU4: DB %d Lane %d: %3d %3d -> %3d\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00b80002:printf("2D: PMU3: Running 1D search csn %d for DM Right/NotLeft(%d) eye edge\n",msg[1],msg[2]); break;
            case 0x00b90002:printf("2D: PMU3: WrDq DM byte%2d with Errcnt %d\n",msg[1],msg[2]); break;
            case 0x00ba0002:printf("2D: PMU3: WrDq DM byte%2d avgDly 0x%04x\n",msg[1],msg[2]); break;
            case 0x00bb0002:printf("2D: PMU1: WrDq DM byte%2d with Errcnt %d\n",msg[1],msg[2]); break;
            case 0x00bc0001:printf("2D: PMU: Error: Dbyte %d txDqDly DM training did not start inside the eye\n",msg[1]); break;
            case 0x00bd0000:printf("2D: PMU4: DM TxDqDly Passing Regions (EyeLeft EyeRight -> EyeCenter) Units=1/32 UI\n"); break;
            case 0x00be0002:printf("2D: PMU4: DB %d Lane %d: (DISCONNECTED)\n",msg[1],msg[2]); break;
            case 0x00bf0005:printf("2D: PMU4: DB %d Lane %d: %3d %3d -> %3d\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00c00003:printf("2D: PMU: Error: Dbyte %d lane %d txDqDly DM passing region is too small (width = %d)\n",msg[1],msg[2],msg[3]); break;
            case 0x00c10004:printf("2D: PMU3: Errcnt for MRD/MWD search nib %2d delay = (%d, 0x%02x) = %d\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x00c20000:printf("2D: PMU3: Precharge all open banks\n"); break;
            case 0x00c30002:printf("2D: PMU: Error: Dbyte %d nibble %d found multiple working coarse delay setting for MRD/MWD\n",msg[1],msg[2]); break;
            case 0x00c40000:printf("2D: PMU4: MRD Passing Regions (coarseVal, fineLeft fineRight -> fineCenter)\n"); break;
            case 0x00c50000:printf("2D: PMU4: MWD Passing Regions (coarseVal, fineLeft fineRight -> fineCenter)\n"); break;
            case 0x00c60004:printf("2D: PMU10: Warning: DB %d nibble %d has multiple working coarse positions, %d and %d, choosing the smaller delay\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x00c70003:printf("2D: PMU: Error: Dbyte %d nibble %d MRD/MWD passing region is too small (width = %d)\n",msg[1],msg[2],msg[3]); break;
            case 0x00c80006:printf("2D: PMU4: DB %d nibble %d: %3d, %3d %3d -> %3d\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6]); break;
            case 0x00c90002:printf("2D: PMU1: Start MRD/nMWD %d for csn %d\n",msg[1],msg[2]); break;
            case 0x00ca0002:printf("2D: PMU2: RXDQS delayLeft[%2d] = %3d (DISCONNECTED)\n",msg[1],msg[2]); break;
            case 0x00cb0006:printf("2D: PMU2: RXDQS delayLeft[%2d] = %3d delayOop[%2d] = %3d OopScaled %4d, selectOop %d\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6]); break;
            case 0x00cc0002:printf("2D: PMU2: RXDQS delayRight[%2d] = %3d (DISCONNECTED)\n",msg[1],msg[2]); break;
            case 0x00cd0006:printf("2D: PMU2: RXDQS delayRight[%2d] = %3d delayOop[%2d] = %4d OopScaled %4d, selectOop %d\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6]); break;
            case 0x00ce0000:printf("2D: PMU4: RxClkDly Passing Regions (EyeLeft EyeRight -> EyeCenter)\n"); break;
            case 0x00cf0002:printf("2D: PMU4: DB %d nibble %d: (DISCONNECTED)\n",msg[1],msg[2]); break;
            case 0x00d00005:printf("2D: PMU4: DB %d nibble %d: %3d %3d -> %3d\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00d10003:printf("2D: PMU: Error: Dbyte %d nibble %d rxClkDly passing region is too small (width = %d)\n",msg[1],msg[2],msg[3]); break;
            case 0x00d20002:printf("2D: PMU0: goodbar = %d for RDWR_BLEN %d\n",msg[1],msg[2]); break;
            case 0x00d30001:printf("2D: PMU3: RxClkDly = %d\n",msg[1]); break;
            case 0x00d40005:printf("2D: PMU0: db %d l %d absLane %d -> bottom %d top %d\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00d50009:printf("2D: PMU3: BYTE %d - %3d %3d %3d %3d %3d %3d %3d %3d\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7],msg[8],msg[9]); break;
            case 0x00d60002:printf("2D: PMU: Error: dbyte %d lane %d's per-lane vrefDAC's had no passing region\n",msg[1],msg[2]); break;
            case 0x00d70004:printf("2D: PMU0: db%d l%d - %d %d\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x00d80002:printf("2D: PMU0: goodbar = %d for RDWR_BLEN %d\n",msg[1],msg[2]); break;
            case 0x00d90004:printf("2D: PMU3: db%d l%d saw %d issues at rxClkDly %d\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x00da0003:printf("2D: PMU3: db%d l%d first saw a pass->fail edge at rxClkDly %d\n",msg[1],msg[2],msg[3]); break;
            case 0x00db0002:printf("2D: PMU3: lane %d PBD = %d\n",msg[1],msg[2]); break;
            case 0x00dc0003:printf("2D: PMU3: db%d l%d first saw a DBI pass->fail edge at rxClkDly %d\n",msg[1],msg[2],msg[3]); break;
            case 0x00dd0003:printf("2D: PMU2: db%d l%d already passed rxPBD = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x00de0003:printf("2D: PMU0: db%d l%d, PBD = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x00df0002:printf("2D: PMU: Error: dbyte %d lane %d failed read deskew\n",msg[1],msg[2]); break;
            case 0x00e00003:printf("2D: PMU0: db%d l%d, inc PBD = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x00e10003:printf("2D: PMU1: Running lane deskew on pstate %d csn %d rdDBIEn %d\n",msg[1],msg[2],msg[3]); break;
            case 0x00e20000:printf("2D: PMU: Error: Read deskew training has been requested, but csrMajorModeDbyte[2] is set\n"); break;
            case 0x00e30002:printf("2D: PMU1: AcsmCsMapCtrl%02d 0x%04x\n",msg[1],msg[2]); break;
            case 0x00e40002:printf("2D: PMU1: AcsmCsMapCtrl%02d 0x%04x\n",msg[1],msg[2]); break;
            case 0x00e50001:printf("2D: PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D3U Type\n",msg[1]); break;
            case 0x00e60001:printf("2D: PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D3R Type\n",msg[1]); break;
            case 0x00e70001:printf("2D: PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D4U Type\n",msg[1]); break;
            case 0x00e80001:printf("2D: PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D4R Type\n",msg[1]); break;
            case 0x00e90001:printf("2D: PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D4LR Type\n",msg[1]); break;
            case 0x00ea0000:printf("2D: PMU: Error: Both 2t timing mode and ddr4 geardown mode specifed in the messageblock's PhyCfg and MR3 fields. Only one can be enabled\n"); break;
            case 0x00eb0003:printf("2D: PMU10: PHY TOTALS - NUM_DBYTES %d NUM_NIBBLES %d NUM_ANIBS %d\n",msg[1],msg[2],msg[3]); break;
            //case 0x00ec0006:printf("2D: PMU10: CSA=0x%02X, CSB=0x%02X, TSTAGES=0x%04X, HDTOUT=%d, MMISC=%d DRAMFreq=%dMT DramType=LPDDR3\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6]); break;
            case 0x00ed0006:printf("2D: PMU10: CSA=0x%02X, CSB=0x%02X, TSTAGES=0x%04X, HDTOUT=%d, MMISC=%d DRAMFreq=%dMT DramType=LPDDR4\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6]); break;
            case 0x00ee0008:printf("2D: PMU10: CS=0x%02X, TSTAGES=0x%04X, HDTOUT=%d, 2T=%d, MMISC=%d AddrMirror=%d DRAMFreq=%dMT DramType=%d\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7],msg[8]); break;
            case 0x00ef0004:printf("2D: PMU10: Pstate%d MR0=0x%04X MR1=0x%04X MR2=0x%04X\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x00f00008:printf("2D: PMU10: Pstate%d MRS MR0=0x%04X MR1=0x%04X MR2=0x%04X MR3=0x%04X MR4=0x%04X MR5=0x%04X MR6=0x%04X\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7],msg[8]); break;
            case 0x00f10005:printf("2D: PMU10: Pstate%d MRS MR1_A0=0x%04X MR2_A0=0x%04X MR3_A0=0x%04X MR11_A0=0x%04X\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00f20000:printf("2D: PMU10: UseBroadcastMR set. All ranks and channels use MRXX_A0 for MR settings.\n"); break;
            case 0x00f30005:printf("2D: PMU10: Pstate%d MRS MR01_A0=0x%02X MR02_A0=0x%02X MR03_A0=0x%02X MR11_A0=0x%02X\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00f40005:printf("2D: PMU10: Pstate%d MRS MR12_A0=0x%02X MR13_A0=0x%02X MR14_A0=0x%02X MR22_A0=0x%02X\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00f50005:printf("2D: PMU10: Pstate%d MRS MR01_A1=0x%02X MR02_A1=0x%02X MR03_A1=0x%02X MR11_A1=0x%02X\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00f60005:printf("2D: PMU10: Pstate%d MRS MR12_A1=0x%02X MR13_A1=0x%02X MR14_A1=0x%02X MR22_A1=0x%02X\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00f70005:printf("2D: PMU10: Pstate%d MRS MR01_B0=0x%02X MR02_B0=0x%02X MR03_B0=0x%02X MR11_B0=0x%02X\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00f80005:printf("2D: PMU10: Pstate%d MRS MR12_B0=0x%02X MR13_B0=0x%02X MR14_B0=0x%02X MR22_B0=0x%02X\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00f90005:printf("2D: PMU10: Pstate%d MRS MR01_B1=0x%02X MR02_B1=0x%02X MR03_B1=0x%02X MR11_B1=0x%02X\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00fa0005:printf("2D: PMU10: Pstate%d MRS MR12_B1=0x%02X MR13_B1=0x%02X MR14_B1=0x%02X MR22_B1=0x%02X\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00fb0002:printf("2D: PMU1: AcsmOdtCtrl%02d 0x%02x\n",msg[1],msg[2]); break;
            case 0x00fc0002:printf("2D: PMU1: AcsmCsMapCtrl%02d 0x%04x\n",msg[1],msg[2]); break;
            case 0x00fd0002:printf("2D: PMU1: AcsmCsMapCtrl%02d 0x%04x\n",msg[1],msg[2]); break;
            case 0x00fe0000:printf("2D: PMU1: HwtCAMode set\n"); break;
            //case 0x00ff0001:printf("2D: PMU3: DDR4 infinite preamble enter/exit mode %d\n",msg[1]); break;
            case 0x01000002:printf("2D: PMU1: In rxenb_train() csn=%d pstate=%d\n",msg[1],msg[2]); break;
            case 0x01010000:printf("2D: PMU3: Finding DQS falling edge\n"); break;
            case 0x01020000:printf("2D: PMU3: Searching for DDR3/LPDDR3/LPDDR4 read preamble\n"); break;
            case 0x01030009:printf("2D: PMU3: dtsm fails Even Nibbles : %2x %2x %2x %2x %2x %2x %2x %2x %2x\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7],msg[8],msg[9]); break;
            case 0x01040009:printf("2D: PMU3: dtsm fails Odd  Nibbles : %2x %2x %2x %2x %2x %2x %2x %2x %2x\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7],msg[8],msg[9]); break;
            case 0x01050002:printf("2D: PMU3: Preamble search pass=%d anyfail=%d\n",msg[1],msg[2]); break;
            case 0x01060000:printf("2D: PMU: Error: RxEn training preamble not found\n"); break;
            case 0x01070000:printf("2D: PMU3: Found DQS pre-amble\n"); break;
            case 0x01080001:printf("2D: PMU: Error: Dbyte %d couldn't find the rising edge of DQS during RxEn Training\n",msg[1]); break;
            case 0x01090000:printf("2D: PMU3: RxEn aligning to first rising edge of burst\n"); break;
            case 0x010a0001:printf("2D: PMU3: Decreasing RxEn delay by %d fine step to allow full capture of reads\n",msg[1]); break;
            case 0x010b0001:printf("2D: PMU3: MREP Delay = %d\n",msg[1]); break;
            case 0x010c0003:printf("2D: PMU3: Errcnt for MREP nib %2d delay = %2d is %d\n",msg[1],msg[2],msg[3]); break;
            case 0x010d0002:printf("2D: PMU3: MREP nibble %d sampled a 1 at data buffer delay %d\n",msg[1],msg[2]); break;
            case 0x010e0002:printf("2D: PMU3: MREP nibble %d saw a 0 to 1 transition at data buffer delay %d\n",msg[1],msg[2]); break;
            case 0x010f0000:printf("2D: PMU2:  MREP did not find a 0 to 1 transition for all nibbles. Failing nibbles assumed to have rising edge close to fine delay 63\n"); break;
            case 0x01100002:printf("2D: PMU2:  Rising edge found in alias window, setting rxDly for nibble %d = %d\n",msg[1],msg[2]); break;
            case 0x01110002:printf("2D: PMU: Error: Failed MREP for nib %d with %d one\n",msg[1],msg[2]); break;
            case 0x01120003:printf("2D: PMU2:  Rising edge not found in alias window with %d one, leaving rxDly for nibble %d = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x01130002:printf("2D: PMU3: Training DIMM %d CSn %d\n",msg[1],msg[2]); break;
            case 0x01140001:printf("2D: PMU3: exitCAtrain_lp3 cs 0x%x\n",msg[1]); break;
            case 0x01150001:printf("2D: PMU3: enterCAtrain_lp3 cs 0x%x\n",msg[1]); break;
            case 0x01160001:printf("2D: PMU3: CAtrain_switchmsb_lp3 cs 0x%x\n",msg[1]); break;
            case 0x01170001:printf("2D: PMU3: CATrain_rdwr_lp3 looking for pattern %x\n",msg[1]); break;
            case 0x01180000:printf("2D: PMU3: exitCAtrain_lp4\n"); break;
            case 0x01190001:printf("2D: PMU3: DEBUG enterCAtrain_lp4 1: cs 0x%x\n",msg[1]); break;
            case 0x011a0001:printf("2D: PMU3: DEBUG enterCAtrain_lp4 3: Put dbyte %d in async mode\n",msg[1]); break;
            case 0x011b0000:printf("2D: PMU3: DEBUG enterCAtrain_lp4 5: Send MR13 to turn on CA training\n"); break;
            case 0x011c0003:printf("2D: PMU3: DEBUG enterCAtrain_lp4 7: idx = %d vref = %x mr12 = %x \n",msg[1],msg[2],msg[3]); break;
            case 0x011d0001:printf("2D: PMU3: CATrain_rdwr_lp4 looking for pattern %x\n",msg[1]); break;
            case 0x011e0004:printf("2D: PMU3: Phase %d CAreadbackA db:%d %x xo:%x\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x011f0005:printf("2D: PMU3: DEBUG lp4SetCatrVref 1: cs=%d chan=%d mr12=%x vref=%d.%d%%\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x01200003:printf("2D: PMU3: DEBUG lp4SetCatrVref 3: mr12 = %x send vref= %x to db=%d\n",msg[1],msg[2],msg[3]); break;
            case 0x01210000:printf("2D: PMU10:Optimizing vref\n"); break;
            case 0x01220004:printf("2D: PMU4:mr12:%2x cs:%d chan %d r:%4x\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x01230005:printf("2D: PMU3: i:%2d bstr:%2d bsto:%2d st:%d r:%d\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x01240002:printf("2D: Failed to find sufficient CA Vref Passing Region for CS %d channel %d\n",msg[1],msg[2]); break;
            case 0x01250005:printf("2D: PMU3:Found %d.%d%% MR12:%x for cs:%d chan %d\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x01260002:printf("2D: PMU3:Calculated %d for AtxImpedence from acx %d.\n",msg[1],msg[2]); break;
            case 0x01270000:printf("2D: PMU3:CA Odt impedence ==0.  Use default vref.\n"); break;
            case 0x01280003:printf("2D: PMU3:Calculated %d.%d%% for Vref MR12=0x%x.\n",msg[1],msg[2],msg[3]); break;
            case 0x01290000:printf("2D: PMU3: CAtrain_lp\n"); break;
            case 0x012a0000:printf("2D: PMU3: CAtrain Begins.\n"); break;
            case 0x012b0001:printf("2D: PMU3: CAtrain_lp testing dly %d\n",msg[1]); break;
            case 0x012c0001:printf("2D: PMU5: CA bitmap dump for cs %x\n",msg[1]); break;
            case 0x012d0001:printf("2D: PMU5: CAA%d ",msg[1]); break;
            case 0x012e0001:printf("2D: %02x",msg[1]); break;
            case 0x012f0000:printf("2D: \n"); break;
            case 0x01300001:printf("2D: PMU5: CAB%d ",msg[1]); break;
            case 0x01310001:printf("2D: %02x",msg[1]); break;
            case 0x01320000:printf("2D: \n"); break;
            case 0x01330003:printf("2D: PMU3: anibi=%d, anibichan[anibi]=%d ,chan=%d\n",msg[1],msg[2],msg[3]); break;
            case 0x01340001:printf("2D: %02x",msg[1]); break;
            case 0x01350001:printf("2D: \nPMU3:Raw CA setting :%x",msg[1]); break;
            case 0x01360002:printf("2D: \nPMU3:ATxDly setting:%x margin:%d\n",msg[1],msg[2]); break;
            case 0x01370002:printf("2D: \nPMU3:InvClk ATxDly setting:%x margin:%d\n",msg[1],msg[2]); break;
            case 0x01380000:printf("2D: \nPMU3:No Range found!\n"); break;
            case 0x01390003:printf("2D: PMU3: 2 anibi=%d, anibichan[anibi]=%d ,chan=%d",msg[1],msg[2],msg[3]); break;
            case 0x013a0002:printf("2D: \nPMU3: no neg clock => CA setting anib=%d, :%d\n",msg[1],msg[2]); break;
            case 0x013b0001:printf("2D: PMU3:Normal margin:%d\n",msg[1]); break;
            case 0x013c0001:printf("2D: PMU3:Inverted margin:%d\n",msg[1]); break;
            case 0x013d0000:printf("2D: PMU3:Using Inverted clock\n"); break;
            case 0x013e0000:printf("2D: PMU3:Using normal clk\n"); break;
            case 0x013f0003:printf("2D: PMU3: 3 anibi=%d, anibichan[anibi]=%d ,chan=%d\n",msg[1],msg[2],msg[3]); break;
            case 0x01400002:printf("2D: PMU3: Setting ATxDly for anib %x to %x\n",msg[1],msg[2]); break;
            case 0x01410000:printf("2D: PMU: Error: CA Training Failed.\n"); break;
            case 0x01420000:printf("2D: PMU1: Writing MRs\n"); break;
            case 0x01430000:printf("2D: PMU4:Using MR12 values from 1D CA VREF training.\n"); break;
            case 0x01440000:printf("2D: PMU3:Writing all MRs to fsp 1\n"); break;
            case 0x01450000:printf("2D: PMU10:Lp4Quickboot mode.\n"); break;
            case 0x01460000:printf("2D: PMU3: Writing MRs\n"); break;
            case 0x01470001:printf("2D: PMU10: Setting boot clock divider to %d\n",msg[1]); break;
            case 0x01480000:printf("2D: PMU3: Resetting DRAM\n"); break;
            case 0x01490000:printf("2D: PMU3: setup for RCD initalization\n"); break;
            case 0x014a0000:printf("2D: PMU3: pmu_exit_SR from dev_init()\n"); break;
            case 0x014b0000:printf("2D: PMU3: initializing RCD\n"); break;
            case 0x014c0000:printf("2D: PMU10: **** Executing 2D Image ****\n"); break;
            //case 0x014d0001:printf("2D: PMU10: **** Start DDR4 Training. PMU Firmware Revision 0x%04x ****\n",msg[1]); break;
            //case 0x014e0001:printf("2D: PMU10: **** Start DDR3 Training. PMU Firmware Revision 0x%04x ****\n",msg[1]); break;
            //case 0x014f0001:printf("2D: PMU10: **** Start LPDDR3 Training. PMU Firmware Revision 0x%04x ****\n",msg[1]); break;
            case 0x01500001:printf("2D: PMU10: **** Start LPDDR4 Training. PMU Firmware Revision 0x%04x ****\n",msg[1]); break;
            case 0x01510000:printf("2D: PMU: Error: Mismatched internal revision between DCCM and ICCM images\n"); break;
            case 0x01520001:printf("2D: PMU10: **** Testchip %d Specific Firmware ****\n",msg[1]); break;
            case 0x01530000:printf("2D: PMU1: LRDIMM with EncodedCS mode, one DIMM\n"); break;
            case 0x01540000:printf("2D: PMU1: LRDIMM with EncodedCS mode, two DIMMs\n"); break;
            case 0x01550000:printf("2D: PMU1: RDIMM with EncodedCS mode, one DIMM\n"); break;
            case 0x01560000:printf("2D: PMU2: Starting LRDIMM MREP training for all ranks\n"); break;
            case 0x01570000:printf("2D: PMU199: LRDIMM MREP training for all ranks completed\n"); break;
            case 0x01580000:printf("2D: PMU2: Starting LRDIMM DWL training for all ranks\n"); break;
            case 0x01590000:printf("2D: PMU199: LRDIMM DWL training for all ranks completed\n"); break;
            case 0x015a0000:printf("2D: PMU2: Starting LRDIMM MRD training for all ranks\n"); break;
            case 0x015b0000:printf("2D: PMU199: LRDIMM MRD training for all ranks completed\n"); break;
            case 0x015c0000:printf("2D: PMU2: Starting RXEN training for all ranks\n"); break;
            case 0x015d0000:printf("2D: PMU2: Starting write leveling fine delay training for all ranks\n"); break;
            case 0x015e0000:printf("2D: PMU2: Starting LRDIMM MWD training for all ranks\n"); break;
            case 0x015f0000:printf("2D: PMU199: LRDIMM MWD training for all ranks completed\n"); break;
            case 0x01600000:printf("2D: PMU2: Starting write leveling fine delay training for all ranks\n"); break;
            case 0x01610000:printf("2D: PMU2: Starting read deskew training\n"); break;
            case 0x01620000:printf("2D: PMU2: Starting SI friendly 1d RdDqs training for all ranks\n"); break;
            case 0x01630000:printf("2D: PMU2: Starting write leveling coarse delay training for all ranks\n"); break;
            case 0x01640000:printf("2D: PMU2: Starting 1d WrDq training for all ranks\n"); break;
            case 0x01650000:printf("2D: PMU2: Running DQS2DQ Oscillator for all ranks\n"); break;
            case 0x01660000:printf("2D: PMU2: Starting again read deskew training but with PRBS\n"); break;
            case 0x01670000:printf("2D: PMU2: Starting 1d RdDqs training for all ranks\n"); break;
            case 0x01680000:printf("2D: PMU2: Starting again 1d WrDq training for all ranks\n"); break;
            case 0x01690000:printf("2D: PMU2: Starting MaxRdLat training\n"); break;
            case 0x016a0000:printf("2D: PMU2: Starting 2d RdDqs training for all ranks\n"); break;
            case 0x016b0000:printf("2D: PMU2: Starting 2d WrDq training for all ranks\n"); break;
            case 0x016c0000:printf("2D: PMU2: Starting 2d RdDqs training for all ranks\n"); break;
            case 0x016d0002:printf("2D: PMU3:read_fifo %x %x\n",msg[1],msg[2]); break;
            case 0x016e0001:printf("2D: PMU: Error: Invalid PhyDrvImpedance of 0x%x specified in message block.\n",msg[1]); break;
            case 0x016f0001:printf("2D: PMU: Error: Invalid PhyOdtImpedance of 0x%x specified in message block.\n",msg[1]); break;
            case 0x01700001:printf("2D: PMU: Error: Invalid BPZNResVal of 0x%x specified in message block.\n",msg[1]); break;
            case 0x01710005:printf("2D: PMU3: fixRxEnBackOff csn:%d db:%d dn:%d bo:%d dly:%x\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x01720001:printf("2D: PMU3: fixRxEnBackOff dly:%x\n",msg[1]); break;
            case 0x01730000:printf("2D: PMU3: Entering setupPpt\n"); break;
            case 0x01740000:printf("2D: PMU3: Start lp4PopulateHighLowBytes\n"); break;
            case 0x01750002:printf("2D: PMU3:Dbyte Detect: db%d received %x\n",msg[1],msg[2]); break;
            case 0x01760002:printf("2D: PMU3:getDqs2Dq read %x from dbyte %d\n",msg[1],msg[2]); break;
            case 0x01770002:printf("2D: PMU3:getDqs2Dq(2) read %x from dbyte %d\n",msg[1],msg[2]); break;
            case 0x01780001:printf("2D: PMU: Error: Dbyte %d read 0 from the DQS oscillator it is connected to\n",msg[1]); break;
            case 0x01790002:printf("2D: PMU4: Dbyte %d dqs2dq = %d/32 UI\n",msg[1],msg[2]); break;
            case 0x017a0003:printf("2D: PMU3:getDqs2Dq set dqs2dq:%d/32 ui (%d ps) from dbyte %d\n",msg[1],msg[2],msg[3]); break;
            case 0x017b0003:printf("2D: PMU3: Setting coarse delay in AtxDly chiplet %d from 0x%02x to 0x%02x \n",msg[1],msg[2],msg[3]); break;
            case 0x017c0003:printf("2D: PMU3: Clearing coarse delay in AtxDly chiplet %d from 0x%02x to 0x%02x \n",msg[1],msg[2],msg[3]); break;
            //case 0x017d0000:printf("2D: PMU3: Performing DDR4 geardown sync sequence\n"); break;
            case 0x017e0000:printf("2D: PMU1: Enter self refresh\n"); break;
            case 0x017f0000:printf("2D: PMU1: Exit self refresh\n"); break;
            case 0x01800000:printf("2D: PMU: Error: No dbiEnable with lp4\n"); break;
            case 0x01810000:printf("2D: PMU: Error: No dbiDisable with lp4\n"); break;
            //case 0x01820001:printf("2D: PMU1: DDR4 update Rx DBI Setting disable %d\n",msg[1]); break;
            //case 0x01830001:printf("2D: PMU1: DDR4 update 2nCk WPre Setting disable %d\n",msg[1]); break;
            case 0x01840005:printf("2D: PMU1: read_delay: db%d lane%d delays[%2d] = 0x%02x (max 0x%02x)\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x01850004:printf("2D: PMU1: write_delay: db%d lane%d delays[%2d] = 0x%04x\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x01860001:printf("2D: PMU5: ID=%d -- db0  db1  db2  db3  db4  db5  db6  db7  db8  db9 --\n",msg[1]); break;
            case 0x0187000b:printf("2D: PMU5: [%d]:0x %4x %4x %4x %4x %4x %4x %4x %4x %4x %4x\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7],msg[8],msg[9],msg[10],msg[11]); break;
            case 0x01880003:printf("2D: PMU2: dump delays - pstate=%d dimm=%d csn=%d\n",msg[1],msg[2],msg[3]); break;
            case 0x01890000:printf("2D: PMU3: Printing Mid-Training Delay Information\n"); break;
            case 0x018a0001:printf("2D: PMU5: CS%d <<KEY>> 0 TrainingCntr <<KEY>> coarse(15:10) fine(9:0)\n",msg[1]); break;
            case 0x018b0001:printf("2D: PMU5: CS%d <<KEY>> 0 RxEnDly, 1 RxClkDly <<KEY>> coarse(10:6) fine(5:0)\n",msg[1]); break;
            case 0x018c0001:printf("2D: PMU5: CS%d <<KEY>> 0 TxDqsDly, 1 TxDqDly <<KEY>> coarse(9:6) fine(5:0)\n",msg[1]); break;
            case 0x018d0001:printf("2D: PMU5: CS%d <<KEY>> 0 RxPBDly <<KEY>> 1 Delay Unit ~= 7ps \n",msg[1]); break;
            case 0x018e0000:printf("2D: PMU5: all CS <<KEY>> 0 DFIMRL <<KEY>> Units = DFI clocks\n"); break;
            case 0x018f0000:printf("2D: PMU5: all CS <<KEY>> VrefDACs <<KEY>> DAC(6:0)\n"); break;
            case 0x01900000:printf("2D: PMU1: Set DMD in MR13 and wrDBI in MR3 for training\n"); break;
            case 0x01910000:printf("2D: PMU: Error: getMaxRxen() failed to find largest rxen nibble delay\n"); break;
            case 0x01920003:printf("2D: PMU2: getMaxRxen(): maxDly %d maxTg %d maxNib %d\n",msg[1],msg[2],msg[3]); break;
            case 0x01930003:printf("2D: PMU2: getRankMaxRxen(): maxDly %d Tg %d maxNib %d\n",msg[1],msg[2],msg[3]); break;
            case 0x01940000:printf("2D: PMU1: skipping CDD calculation in 2D image\n"); break;
            case 0x01950001:printf("2D: PMU3: Calculating CDDs for pstate %d\n",msg[1]); break;
            case 0x01960003:printf("2D: PMU3: rxFromDly[%d][%d] = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x01970003:printf("2D: PMU3: rxToDly  [%d][%d] = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x01980003:printf("2D: PMU3: rxDly    [%d][%d] = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x01990003:printf("2D: PMU3: txDly    [%d][%d] = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x019a0003:printf("2D: PMU3: allFine CDD_RR_%d_%d = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x019b0003:printf("2D: PMU3: allFine CDD_WW_%d_%d = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x019c0003:printf("2D: PMU3: CDD_RR_%d_%d = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x019d0003:printf("2D: PMU3: CDD_WW_%d_%d = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x019e0003:printf("2D: PMU3: allFine CDD_RW_%d_%d = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x019f0003:printf("2D: PMU3: allFine CDD_WR_%d_%d = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x01a00003:printf("2D: PMU3: CDD_RW_%d_%d = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x01a10003:printf("2D: PMU3: CDD_WR_%d_%d = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x01a20004:printf("2D: PMU3: F%dBC2x_B%d_D%d = 0x%02x\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x01a30004:printf("2D: PMU3: F%dBC3x_B%d_D%d = 0x%02x\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x01a40004:printf("2D: PMU3: F%dBC4x_B%d_D%d = 0x%02x\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x01a50004:printf("2D: PMU3: F%dBC5x_B%d_D%d = 0x%02x\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x01a60004:printf("2D: PMU3: F%dBC8x_B%d_D%d = 0x%02x\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x01a70004:printf("2D: PMU3: F%dBC9x_B%d_D%d = 0x%02x\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x01a80004:printf("2D: PMU3: F%dBCAx_B%d_D%d = 0x%02x\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x01a90004:printf("2D: PMU3: F%dBCBx_B%d_D%d = 0x%02x\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x01aa0000:printf("2D: PMU10: Entering context_switch_postamble\n"); break;
            case 0x01ab0003:printf("2D: PMU10: context_switch_postamble is enabled for DIMM %d, RC0A=0x%x, RC3x=0x%x\n",msg[1],msg[2],msg[3]); break;
            case 0x01ac0000:printf("2D: PMU10: Setting bcw fspace 0\n"); break;
            case 0x01ad0001:printf("2D: PMU10: Sending BC0A = 0x%x\n",msg[1]); break;
            case 0x01ae0001:printf("2D: PMU10: Sending BC6x = 0x%x\n",msg[1]); break;
            case 0x01af0001:printf("2D: PMU10: Sending RC0A = 0x%x\n",msg[1]); break;
            case 0x01b00001:printf("2D: PMU10: Sending RC3x = 0x%x\n",msg[1]); break;
            case 0x01b10001:printf("2D: PMU10: Sending RC0A = 0x%x\n",msg[1]); break;
            case 0x01b20001:printf("2D: PMU1: enter_lp3: DEBUG: pstate = %d\n",msg[1]); break;
            case 0x01b30001:printf("2D: PMU1: enter_lp3: DEBUG: dfifreqxlat_pstate = %d\n",msg[1]); break;
            case 0x01b40001:printf("2D: PMU1: enter_lp3: DEBUG: pllbypass = %d\n",msg[1]); break;
            case 0x01b50001:printf("2D: PMU1: enter_lp3: DEBUG: forcecal = %d\n",msg[1]); break;
            case 0x01b60001:printf("2D: PMU1: enter_lp3: DEBUG: pllmaxrange = 0x%x\n",msg[1]); break;
            case 0x01b70001:printf("2D: PMU1: enter_lp3: DEBUG: dacval_out = 0x%x\n",msg[1]); break;
            case 0x01b80001:printf("2D: PMU1: enter_lp3: DEBUG: pllctrl3 = 0x%x\n",msg[1]); break;
            case 0x01b90000:printf("2D: PMU3: Loading DRAM with BIOS supplied MR values and entering self refresh prior to exiting PMU code.\n"); break;
            case 0x01ba0002:printf("2D: PMU3: Setting DataBuffer function space of dimmcs 0x%02x to %d\n",msg[1],msg[2]); break;
            case 0x01bb0002:printf("2D: PMU4: Setting RCW FxRC%Xx = 0x%02x\n",msg[1],msg[2]); break;
            case 0x01bc0002:printf("2D: PMU4: Setting RCW FxRC%02X = 0x%02x\n",msg[1],msg[2]); break;
            //case 0x01bd0001:printf("2D: PMU1: DDR4 update Rd Pre Setting disable %d\n",msg[1]); break;
            case 0x01be0002:printf("2D: PMU2: Setting BCW FxBC%Xx = 0x%02x\n",msg[1],msg[2]); break;
            case 0x01bf0002:printf("2D: PMU2: Setting BCW BC%02X = 0x%02x\n",msg[1],msg[2]); break;
            case 0x01c00002:printf("2D: PMU2: Setting BCW PBA mode FxBC%Xx = 0x%02x\n",msg[1],msg[2]); break;
            case 0x01c10002:printf("2D: PMU2: Setting BCW PBA mode BC%02X = 0x%02x\n",msg[1],msg[2]); break;
            case 0x01c20003:printf("2D: PMU4: BCW value for dimm %d, fspace %d, addr 0x%04x\n",msg[1],msg[2],msg[3]); break;
            case 0x01c30002:printf("2D: PMU4: DB %d, value 0x%02x\n",msg[1],msg[2]); break;
            case 0x01c40000:printf("2D: PMU6: WARNING MREP underflow, set to min value -2 coarse, 0 fine\n"); break;
            case 0x01c50004:printf("2D: PMU6: LRDIMM Writing final data buffer fine delay value nib %2d, trainDly %3d, fineDly code %2d, new MREP fine %2d\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x01c60003:printf("2D: PMU6: LRDIMM Writing final data buffer fine delay value nib %2d, trainDly %3d, fineDly code %2d\n",msg[1],msg[2],msg[3]); break;
            case 0x01c70003:printf("2D: PMU6: LRDIMM Writing data buffer fine delay type %d nib %2d, code %2d\n",msg[1],msg[2],msg[3]); break;
            case 0x01c80002:printf("2D: PMU6: Writing final data buffer coarse delay value dbyte %2d, coarse = 0x%02x\n",msg[1],msg[2]); break;
            case 0x01c90003:printf("2D: PMU4: data 0x%04x at MB addr 0x%08x saved at CSR addr 0x%08x\n",msg[1],msg[2],msg[3]); break;
            case 0x01ca0003:printf("2D: PMU4: data 0x%04x at MB addr 0x%08x restored from CSR addr 0x%08x\n",msg[1],msg[2],msg[3]); break;
            case 0x01cb0003:printf("2D: PMU4: data 0x%04x at MB addr 0x%08x saved at CSR addr 0x%08x\n",msg[1],msg[2],msg[3]); break;
            case 0x01cc0003:printf("2D: PMU4: data 0x%04x at MB addr 0x%08x restored from CSR addr 0x%08x\n",msg[1],msg[2],msg[3]); break;
            case 0x01cd0001:printf("2D: PMU3: Update BC00, BC01, BC02 for rank-dimm 0x%02x\n",msg[1]); break;
            case 0x01ce0000:printf("2D: PMU3: Writing D4 RDIMM RCD Control words F0RC00 -> F0RC0F\n"); break;
            case 0x01cf0000:printf("2D: PMU3: Disable parity in F0RC0E\n"); break;
            case 0x01d00000:printf("2D: PMU3: Writing D4 RDIMM RCD Control words F1RC00 -> F1RC05\n"); break;
            case 0x01d10000:printf("2D: PMU3: Writing D4 RDIMM RCD Control words F1RC1x -> F1RC9x\n"); break;
            case 0x01d20000:printf("2D: PMU3: Writing D4 Data buffer Control words BC00 -> BC0E\n"); break;
            case 0x01d30002:printf("2D: PMU1: setAltCL Sending MR0 0x%x cl=%d\n",msg[1],msg[2]); break;
            case 0x01d40002:printf("2D: PMU1: restoreFromAltCL Sending MR0 0x%x cl=%d\n",msg[1],msg[2]); break;
            case 0x01d50002:printf("2D: PMU1: restoreAcsmFromAltCL Sending MR0 0x%x cl=%d\n",msg[1],msg[2]); break;
            case 0x01d60002:printf("2D: PMU2: Setting D3R RC%d = 0x%01x\n",msg[1],msg[2]); break;
            case 0x01d70000:printf("2D: PMU3: Writing D3 RDIMM RCD Control words RC0 -> RC11\n"); break;
            case 0x01d80002:printf("2D: PMU0: VrefDAC0/1 vddqStart %d dacToVddq %d\n",msg[1],msg[2]); break;
            //case 0x01d90001:printf("2D: PMU: Error: Messageblock phyVref=0x%x is above the limit for TSMC28's attenuated LPDDR4 receivers. Please see the pub databook\n",msg[1]); break;
            //case 0x01da0001:printf("2D: PMU: Error: Messageblock phyVref=0x%x is above the limit for TSMC28's attenuated DDR4 receivers. Please see the pub databook\n",msg[1]); break;
            case 0x01db0001:printf("2D: PMU0: PHY VREF @ (%d/1000) VDDQ\n",msg[1]); break;
            case 0x01dc0002:printf("2D: PMU0: initalizing phy vrefDacs to %d ExtVrefRange %x\n",msg[1],msg[2]); break;
            case 0x01dd0002:printf("2D: PMU0: initalizing global vref to %d range %d\n",msg[1],msg[2]); break;
            case 0x01de0002:printf("2D: PMU4: Setting initial device vrefDQ for CS%d to MR6 = 0x%04x\n",msg[1],msg[2]); break;
            case 0x01df0003:printf("2D: PMU1: In write_level_fine() csn=%d dimm=%d pstate=%d\n",msg[1],msg[2],msg[3]); break;
            case 0x01e00000:printf("2D: PMU3: Fine write leveling hardware search increasing TxDqsDly until full bursts are seen\n"); break;
            case 0x01e10000:printf("2D: PMU4: WL normalized pos   : ........................|........................\n"); break;
            case 0x01e20007:printf("2D: PMU4: WL margin for nib %2d: %08x%08x%08x%08x%08x%08x\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7]); break;
            case 0x01e30000:printf("2D: PMU4: WL normalized pos   : ........................|........................\n"); break;
            case 0x01e40000:printf("2D: PMU3: Exiting write leveling mode\n"); break;
            case 0x01e50001:printf("2D: PMU3: got %d for cl in load_wrlvl_acsm\n",msg[1]); break;
            case 0x01e60003:printf("2D: PMU1: In write_level_coarse() csn=%d dimm=%d pstate=%d\n",msg[1],msg[2],msg[3]); break;
            case 0x01e70003:printf("2D: PMU3: left eye edge search db:%d ln:%d dly:0x%x\n",msg[1],msg[2],msg[3]); break;
            case 0x01e80003:printf("2D: PMU3: right eye edge search db:%d ln:%d dly:0x%x\n",msg[1],msg[2],msg[3]); break;
            case 0x01e90004:printf("2D: PMU3: eye center db:%d ln:%d dly:0x%x (maxdq:%x)\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x01ea0003:printf("2D: PMU3: Wrote to TxDqDly db:%d ln:%d dly:0x%x\n",msg[1],msg[2],msg[3]); break;
            case 0x01eb0003:printf("2D: PMU3: Wrote to TxDqDly db:%d ln:%d dly:0x%x\n",msg[1],msg[2],msg[3]); break;
            case 0x01ec0002:printf("2D: PMU3: Coarse write leveling dbyte%2d is still failing for TxDqsDly=0x%04x\n",msg[1],msg[2]); break;
            case 0x01ed0002:printf("2D: PMU4: Coarse write leveling iteration %d saw %d data miscompares across the entire phy\n",msg[1],msg[2]); break;
            case 0x01ee0000:printf("2D: PMU: Error: Failed write leveling coarse\n"); break;
            case 0x01ef0001:printf("2D: PMU3: got %d for cl in load_wrlvl_acsm\n",msg[1]); break;
            case 0x01f00003:printf("2D: PMU3: In write_level_coarse() csn=%d dimm=%d pstate=%d\n",msg[1],msg[2],msg[3]); break;
            case 0x01f10003:printf("2D: PMU3: left eye edge search db:%d ln:%d dly:0x%x\n",msg[1],msg[2],msg[3]); break;
            case 0x01f20003:printf("2D: PMU3: right eye edge search db: %d ln: %d dly: 0x%x\n",msg[1],msg[2],msg[3]); break;
            case 0x01f30004:printf("2D: PMU3: eye center db: %d ln: %d dly: 0x%x (maxdq: 0x%x)\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x01f40003:printf("2D: PMU3: Wrote to TxDqDly db: %d ln: %d dly: 0x%x\n",msg[1],msg[2],msg[3]); break;
            case 0x01f50003:printf("2D: PMU3: Wrote to TxDqDly db: %d ln: %d dly: 0x%x\n",msg[1],msg[2],msg[3]); break;
            case 0x01f60002:printf("2D: PMU3: Coarse write leveling nibble%2d is still failing for TxDqsDly=0x%04x\n",msg[1],msg[2]); break;
            case 0x01f70002:printf("2D: PMU4: Coarse write leveling iteration %d saw %d data miscompares across the entire phy\n",msg[1],msg[2]); break;
            case 0x01f80000:printf("2D: PMU: Error: Failed write leveling coarse\n"); break;
            case 0x01f90000:printf("2D: PMU4: WL normalized pos   : ................................|................................\n"); break;
            case 0x01fa0009:printf("2D: PMU4: WL margin for nib %2d: %08x%08x%08x%08x%08x%08x%08x%08x\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7],msg[8],msg[9]); break;
            case 0x01fb0000:printf("2D: PMU4: WL normalized pos   : ................................|................................\n"); break;
            case 0x01fc0001:printf("2D: PMU8: Adjust margin after WL coarse to be larger than %d\n",msg[1]); break;
            case 0x01fd0001:printf("2D: PMU: Error: All margin after write leveling coarse are smaller than minMargin %d\n",msg[1]); break;
            case 0x01fe0002:printf("2D: PMU8: Decrement nib %d TxDqsDly by %d fine step\n",msg[1],msg[2]); break;
            case 0x01ff0003:printf("2D: PMU3: In write_level_coarse() csn=%d dimm=%d pstate=%d\n",msg[1],msg[2],msg[3]); break;
            case 0x02000005:printf("2D: PMU2: Write level: dbyte %d nib%d dq/dmbi %2d dqsfine 0x%04x dqDly 0x%04x\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x02010002:printf("2D: PMU3: Coarse write leveling nibble%2d is still failing for TxDqsDly=0x%04x\n",msg[1],msg[2]); break;
            case 0x02020002:printf("2D: PMU4: Coarse write leveling iteration %d saw %d data miscompares across the entire phy\n",msg[1],msg[2]); break;
            case 0x02030000:printf("2D: PMU: Error: Failed write leveling coarse\n"); break;
            case 0x02040001:printf("2D: PMU3: DWL delay = %d\n",msg[1]); break;
            case 0x02050003:printf("2D: PMU3: Errcnt for DWL nib %2d delay = %2d is %d\n",msg[1],msg[2],msg[3]); break;
            case 0x02060002:printf("2D: PMU3: DWL nibble %d sampled a 1 at delay %d\n",msg[1],msg[2]); break;
            case 0x02070003:printf("2D: PMU3: DWL nibble %d passed at delay %d. Rising edge was at %d\n",msg[1],msg[2],msg[3]); break;
            case 0x02080000:printf("2D: PMU2: DWL did nto find a rising edge of memclk for all nibbles. Failing nibbles assumed to have rising edge close to fine delay 63\n"); break;
            case 0x02090002:printf("2D: PMU2:  Rising edge found in alias window, setting wrlvlDly for nibble %d = %d\n",msg[1],msg[2]); break;
            case 0x020a0002:printf("2D: PMU: Error: Failed DWL for nib %d with %d one\n",msg[1],msg[2]); break;
            case 0x020b0003:printf("2D: PMU2:  Rising edge not found in alias window with %d one, leaving wrlvlDly for nibble %d = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x04000000:printf("2D: PMU: Error:Mailbox Buffer Overflowed.\n"); break;
            case 0x04010000:printf("2D: PMU: Error:Mailbox Buffer Overflowed.\n"); break;
            case 0x04020000:printf("2D: PMU: ***** Assertion Error - terminating *****\n"); break;
            case 0x04030002:printf("2D: PMU1: swapByte db %d by %d\n",msg[1],msg[2]); break;
            case 0x04040003:printf("2D: PMU3: get_cmd_dly max(%d ps, %d memclk) = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x04050002:printf("2D: PMU0: Write CSR 0x%06x 0x%04x\n",msg[1],msg[2]); break;
            case 0x04060002:printf("2D: PMU0: hwt_init_ppgc_prbs(): Polynomial: %x, Deg: %d\n",msg[1],msg[2]); break;
            case 0x04070001:printf("2D: PMU: Error: acsm_set_cmd to non existant instruction adddress %d\n",msg[1]); break;
            case 0x04080001:printf("2D: PMU: Error: acsm_set_cmd with unknown ddr cmd 0x%x\n",msg[1]); break;
            case 0x0409000c:printf("2D: PMU1: acsm_addr %02x, acsm_flgs %04x, ddr_cmd %02x, cmd_dly %02x, ddr_addr %04x, ddr_bnk %02x, ddr_cs %02x, cmd_rcnt %02x, AcsmSeq0/1/2/3 %04x %04x %04x %04x\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7],msg[8],msg[9],msg[10],msg[11],msg[12]); break;
            case 0x040a0000:printf("2D: PMU: Error: Polling on ACSM done failed to complete in acsm_poll_done()...\n"); break;
            case 0x040b0000:printf("2D: PMU1: acsm RUN\n"); break;
            case 0x040c0000:printf("2D: PMU1: acsm STOPPED\n"); break;
            case 0x040d0002:printf("2D: PMU1: acsm_init: acsm_mode %04x mxrdlat %04x\n",msg[1],msg[2]); break;
            case 0x040e0002:printf("2D: PMU: Error: setAcsmCLCWL: cl and cwl must be each >= 2 and 5, resp. CL=%d CWL=%d\n",msg[1],msg[2]); break;
            case 0x040f0002:printf("2D: PMU: Error: setAcsmCLCWL: cl and cwl must be each >= 5. CL=%d CWL=%d\n",msg[1],msg[2]); break;
            case 0x04100002:printf("2D: PMU1: setAcsmCLCWL: CASL %04d WCASL %04d\n",msg[1],msg[2]); break;
            case 0x04110001:printf("2D: PMU: Error: Reserved value of register F0RC0F found in message block: 0x%04x\n",msg[1]); break;
            case 0x04120001:printf("2D: PMU3: Written MRS to CS=0x%02x\n",msg[1]); break;
            case 0x04130001:printf("2D: PMU3: Written MRS to CS=0x%02x\n",msg[1]); break;
            case 0x04140000:printf("2D: PMU3: Entering Boot Freq Mode.\n"); break;
            case 0x04150001:printf("2D: PMU: Error: Boot clock divider setting of %d is too small\n",msg[1]); break;
            case 0x04160000:printf("2D: PMU3: Exiting Boot Freq Mode.\n"); break;
            case 0x04170002:printf("2D: PMU3: Writing MR%d OP=%x\n",msg[1],msg[2]); break;
            case 0x04180000:printf("2D: PMU: Error: Delay too large in slomo\n"); break;
            case 0x04190001:printf("2D: PMU3: Written MRS to CS=0x%02x\n",msg[1]); break;
            case 0x041a0000:printf("2D: PMU3: Enable Channel A\n"); break;
            case 0x041b0000:printf("2D: PMU3: Enable Channel B\n"); break;
            case 0x041c0000:printf("2D: PMU3: Enable All Channels\n"); break;
            case 0x041d0002:printf("2D: PMU2: Use PDA mode to set MR%d with value 0x%02x\n",msg[1],msg[2]); break;
            case 0x041e0001:printf("2D: PMU3: Written Vref with PDA to CS=0x%02x\n",msg[1]); break;
            case 0x041f0000:printf("2D: PMU1: start_cal: DEBUG: setting CalRun to 1\n"); break;
            case 0x04200000:printf("2D: PMU1: start_cal: DEBUG: setting CalRun to 0\n"); break;
            case 0x04210001:printf("2D: PMU1: lock_pll_dll: DEBUG: pstate = %d\n",msg[1]); break;
            case 0x04220001:printf("2D: PMU1: lock_pll_dll: DEBUG: dfifreqxlat_pstate = %d\n",msg[1]); break;
            case 0x04230001:printf("2D: PMU1: lock_pll_dll: DEBUG: pllbypass = %d\n",msg[1]); break;
            case 0x04240001:printf("2D: PMU3: SaveLcdlSeed: Saving seed seed %d\n",msg[1]); break;
            case 0x04250000:printf("2D: PMU1: in phy_defaults()\n"); break;
            case 0x04260003:printf("2D: PMU3: ACXConf:%d MaxNumDbytes:%d NumDfi:%d\n",msg[1],msg[2],msg[3]); break;
            case 0x04270005:printf("2D: PMU1: setAltAcsmCLCWL setting cl=%d cwl=%d\n",msg[1],msg[2]); break;
            default: printf("2D: UNKOWN MSG"); break;
#endif // #ifdef DDR_FW_2D_BRIEF_MSG
         }
      }
  else {
         switch(msg[0]) {
#ifdef DDR_FW_TRAIN_1D_MSG
            case 0x00000001:printf("1D: PMU1:prbsGenCtl:%x\n",msg[1]); break;
            case 0x00010000:printf("1D: PMU1: loading 2D acsm sequence\n"); break;
            case 0x00020000:printf("1D: PMU1: loading 1D acsm sequence\n"); break;
            case 0x00030002:printf("1D: PMU3: %d memclocks @ %d to get half of 300ns\n",msg[1],msg[2]); break;
            case 0x00040000:printf("1D: PMU: Error: User requested MPR read pattern for read DQS training in DDR3 Mode\n"); break;
            case 0x00050000:printf("1D: PMU3: Running 1D search for left eye edge\n"); break;
            case 0x00060001:printf("1D: PMU1: In Phase Left Edge Search cs %d\n",msg[1]); break;
            case 0x00070001:printf("1D: PMU1: Out of Phase Left Edge Search cs %d\n",msg[1]); break;
            case 0x00080000:printf("1D: PMU3: Running 1D search for right eye edge\n"); break;
            case 0x00090001:printf("1D: PMU1: In Phase Right Edge Search cs %d\n",msg[1]); break;
            case 0x000a0001:printf("1D: PMU1: Out of Phase Right Edge Search cs %d\n",msg[1]); break;
            case 0x000b0001:printf("1D: PMU1: mxRdLat training pstate %d\n",msg[1]); break;
            case 0x000c0001:printf("1D: PMU1: mxRdLat search for cs %d\n",msg[1]); break;
            case 0x000d0001:printf("1D: PMU0: MaxRdLat non consistant DtsmLoThldXingInd 0x%03x\n",msg[1]); break;
            case 0x000e0003:printf("1D: PMU4: CS %d Dbyte %d worked with DFIMRL = %d DFICLKs \n",msg[1],msg[2],msg[3]); break;
            case 0x000f0004:printf("1D: PMU3: MaxRdLat Read Lane err mask for csn %d, DFIMRL %2d DFIClks, dbyte %d = 0x%03x\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x00100003:printf("1D: PMU3: MaxRdLat Read Lane err mask for csn %d DFIMRL %2d, All dbytes = 0x%03x\n",msg[1],msg[2],msg[3]); break;
            case 0x00110001:printf("1D: PMU: Error: CS%d failed to find a DFIMRL setting that worked for all bytes during MaxRdLat training\n",msg[1]); break;
            case 0x00120002:printf("1D: PMU3: Smallest passing DFIMRL for all dbytes in CS%d = %d DFIClks\n",msg[1],msg[2]); break;
            case 0x00130000:printf("1D: PMU: Error: No passing DFIMRL value found for any chip select during MaxRdLat training\n"); break;
            case 0x00140003:printf("1D: PMU: Error: Dbyte %d lane %d txDqDly passing region is too small (width = %d)\n",msg[1],msg[2],msg[3]); break;
            case 0x00150006:printf("1D: PMU4: Adjusting rxclkdly db %d nib %d from %d+%d=%d->%d\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6]); break;
            case 0x00160000:printf("1D: PMU4: TxDqDly Passing Regions (EyeLeft EyeRight -> EyeCenter) Units=1/32 UI\n"); break;
            case 0x00170005:printf("1D: PMU4: DB %d Lane %d: %3d %3d -> %3d\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00180002:printf("1D: PMU2: TXDQ delayLeft[%2d] = %3d (DISCONNECTED)\n",msg[1],msg[2]); break;
            case 0x00190004:printf("1D: PMU2: TXDQ delayLeft[%2d] = %3d oopScaled = %3d selectOop %d\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x001a0002:printf("1D: PMU2: TXDQ delayRight[%2d] = %3d (DISCONNECTED)\n",msg[1],msg[2]); break;
            case 0x001b0004:printf("1D: PMU2: TXDQ delayRight[%2d] = %3d oopScaled = %3d selectOop %d\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x001c0003:printf("1D: PMU: Error: Dbyte %d lane %d txDqDly passing region is too small (width = %d)\n",msg[1],msg[2],msg[3]); break;
            case 0x001d0000:printf("1D: PMU4: TxDqDly Passing Regions (EyeLeft EyeRight -> EyeCenter) Units=1/32 UI\n"); break;
            case 0x001e0002:printf("1D: PMU4: DB %d Lane %d: (DISCONNECTED)\n",msg[1],msg[2]); break;
            case 0x001f0005:printf("1D: PMU4: DB %d Lane %d: %3d %3d -> %3d\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00200002:printf("1D: PMU3: Running 1D search csn %d for DM Right/NotLeft(%d) eye edge\n",msg[1],msg[2]); break;
            case 0x00210002:printf("1D: PMU3: WrDq DM byte%2d with Errcnt %d\n",msg[1],msg[2]); break;
            case 0x00220002:printf("1D: PMU3: WrDq DM byte%2d avgDly 0x%04x\n",msg[1],msg[2]); break;
            case 0x00230002:printf("1D: PMU1: WrDq DM byte%2d with Errcnt %d\n",msg[1],msg[2]); break;
            case 0x00240001:printf("1D: PMU: Error: Dbyte %d txDqDly DM training did not start inside the eye\n",msg[1]); break;
            case 0x00250000:printf("1D: PMU4: DM TxDqDly Passing Regions (EyeLeft EyeRight -> EyeCenter) Units=1/32 UI\n"); break;
            case 0x00260002:printf("1D: PMU4: DB %d Lane %d: (DISCONNECTED)\n",msg[1],msg[2]); break;
            case 0x00270005:printf("1D: PMU4: DB %d Lane %d: %3d %3d -> %3d\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00280003:printf("1D: PMU: Error: Dbyte %d lane %d txDqDly DM passing region is too small (width = %d)\n",msg[1],msg[2],msg[3]); break;
            case 0x00290004:printf("1D: PMU3: Errcnt for MRD/MWD search nib %2d delay = (%d, 0x%02x) = %d\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x002a0000:printf("1D: PMU3: Precharge all open banks\n"); break;
            case 0x002b0002:printf("1D: PMU: Error: Dbyte %d nibble %d found multiple working coarse delay setting for MRD/MWD\n",msg[1],msg[2]); break;
            case 0x002c0000:printf("1D: PMU4: MRD Passing Regions (coarseVal, fineLeft fineRight -> fineCenter)\n"); break;
            case 0x002d0000:printf("1D: PMU4: MWD Passing Regions (coarseVal, fineLeft fineRight -> fineCenter)\n"); break;
            case 0x002e0004:printf("1D: PMU10: Warning: DB %d nibble %d has multiple working coarse positions, %d and %d, choosing the smaller delay\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x002f0003:printf("1D: PMU: Error: Dbyte %d nibble %d MRD/MWD passing region is too small (width = %d)\n",msg[1],msg[2],msg[3]); break;
            case 0x00300006:printf("1D: PMU4: DB %d nibble %d: %3d, %3d %3d -> %3d\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6]); break;
            case 0x00310002:printf("1D: PMU1: Start MRD/nMWD %d for csn %d\n",msg[1],msg[2]); break;
            case 0x00320002:printf("1D: PMU2: RXDQS delayLeft[%2d] = %3d (DISCONNECTED)\n",msg[1],msg[2]); break;
            case 0x00330006:printf("1D: PMU2: RXDQS delayLeft[%2d] = %3d delayOop[%2d] = %3d OopScaled %4d, selectOop %d\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6]); break;
            case 0x00340002:printf("1D: PMU2: RXDQS delayRight[%2d] = %3d (DISCONNECTED)\n",msg[1],msg[2]); break;
            case 0x00350006:printf("1D: PMU2: RXDQS delayRight[%2d] = %3d delayOop[%2d] = %4d OopScaled %4d, selectOop %d\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6]); break;
            case 0x00360000:printf("1D: PMU4: RxClkDly Passing Regions (EyeLeft EyeRight -> EyeCenter)\n"); break;
            case 0x00370002:printf("1D: PMU4: DB %d nibble %d: (DISCONNECTED)\n",msg[1],msg[2]); break;
            case 0x00380005:printf("1D: PMU4: DB %d nibble %d: %3d %3d -> %3d\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00390003:printf("1D: PMU: Error: Dbyte %d nibble %d rxClkDly passing region is too small (width = %d)\n",msg[1],msg[2],msg[3]); break;
            case 0x003a0002:printf("1D: PMU0: goodbar = %d for RDWR_BLEN %d\n",msg[1],msg[2]); break;
            case 0x003b0001:printf("1D: PMU3: RxClkDly = %d\n",msg[1]); break;
            case 0x003c0005:printf("1D: PMU0: db %d l %d absLane %d -> bottom %d top %d\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x003d0009:printf("1D: PMU3: BYTE %d - %3d %3d %3d %3d %3d %3d %3d %3d\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7],msg[8],msg[9]); break;
            case 0x003e0002:printf("1D: PMU: Error: dbyte %d lane %d's per-lane vrefDAC's had no passing region\n",msg[1],msg[2]); break;
            case 0x003f0004:printf("1D: PMU0: db%d l%d - %d %d\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x00400002:printf("1D: PMU0: goodbar = %d for RDWR_BLEN %d\n",msg[1],msg[2]); break;
            case 0x00410004:printf("1D: PMU3: db%d l%d saw %d issues at rxClkDly %d\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x00420003:printf("1D: PMU3: db%d l%d first saw a pass->fail edge at rxClkDly %d\n",msg[1],msg[2],msg[3]); break;
            case 0x00430002:printf("1D: PMU3: lane %d PBD = %d\n",msg[1],msg[2]); break;
            case 0x00440003:printf("1D: PMU3: db%d l%d first saw a DBI pass->fail edge at rxClkDly %d\n",msg[1],msg[2],msg[3]); break;
            case 0x00450003:printf("1D: PMU2: db%d l%d already passed rxPBD = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x00460003:printf("1D: PMU0: db%d l%d, PBD = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x00470002:printf("1D: PMU: Error: dbyte %d lane %d failed read deskew\n",msg[1],msg[2]); break;
            case 0x00480003:printf("1D: PMU0: db%d l%d, inc PBD = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x00490003:printf("1D: PMU1: Running lane deskew on pstate %d csn %d rdDBIEn %d\n",msg[1],msg[2],msg[3]); break;
            case 0x004a0000:printf("1D: PMU: Error: Read deskew training has been requested, but csrMajorModeDbyte[2] is set\n"); break;
            case 0x004b0002:printf("1D: PMU1: AcsmCsMapCtrl%02d 0x%04x\n",msg[1],msg[2]); break;
            case 0x004c0002:printf("1D: PMU1: AcsmCsMapCtrl%02d 0x%04x\n",msg[1],msg[2]); break;
            case 0x004d0001:printf("1D: PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D3U Type\n",msg[1]); break;
            case 0x004e0001:printf("1D: PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D3R Type\n",msg[1]); break;
            case 0x004f0001:printf("1D: PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D4U Type\n",msg[1]); break;
            case 0x00500001:printf("1D: PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D4R Type\n",msg[1]); break;
            case 0x00510001:printf("1D: PMU: Error: Wrong PMU image loaded. message Block DramType = 0x%02x, but image built for D4LR Type\n",msg[1]); break;
            case 0x00520000:printf("1D: PMU: Error: Both 2t timing mode and ddr4 geardown mode specifed in the messageblock's PhyCfg and MR3 fields. Only one can be enabled\n"); break;
            case 0x00530003:printf("1D: PMU10: PHY TOTALS - NUM_DBYTES %d NUM_NIBBLES %d NUM_ANIBS %d\n",msg[1],msg[2],msg[3]); break;
            //case 0x00540006:printf("1D: PMU10: CSA=0x%02X, CSB=0x%02X, TSTAGES=0x%04X, HDTOUT=%d, MMISC=%d DRAMFreq=%dMT DramType=LPDDR3\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6]); break;
            case 0x00550006:printf("1D: PMU10: CSA=0x%02X, CSB=0x%02X, TSTAGES=0x%04X, HDTOUT=%d, MMISC=%d DRAMFreq=%dMT DramType=LPDDR4\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6]); break;
            case 0x00560008:printf("1D: PMU10: CS=0x%02X, TSTAGES=0x%04X, HDTOUT=%d, 2T=%d, MMISC=%d AddrMirror=%d DRAMFreq=%dMT DramType=%d\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7],msg[8]); break;
            case 0x00570004:printf("1D: PMU10: Pstate%d MR0=0x%04X MR1=0x%04X MR2=0x%04X\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x00580008:printf("1D: PMU10: Pstate%d MRS MR0=0x%04X MR1=0x%04X MR2=0x%04X MR3=0x%04X MR4=0x%04X MR5=0x%04X MR6=0x%04X\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7],msg[8]); break;
            case 0x00590005:printf("1D: PMU10: Pstate%d MRS MR1_A0=0x%04X MR2_A0=0x%04X MR3_A0=0x%04X MR11_A0=0x%04X\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x005a0000:printf("1D: PMU10: UseBroadcastMR set. All ranks and channels use MRXX_A0 for MR settings.\n"); break;
#endif
            case 0x005b0005:printf("1D: PMU10: Pstate%d MRS MR01_A0=0x%02X MR02_A0=0x%02X MR03_A0=0x%02X MR11_A0=0x%02X\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x005c0005:printf("1D: PMU10: Pstate%d MRS MR12_A0=0x%02X MR13_A0=0x%02X MR14_A0=0x%02X MR22_A0=0x%02X\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x005d0005:printf("1D: PMU10: Pstate%d MRS MR01_A1=0x%02X MR02_A1=0x%02X MR03_A1=0x%02X MR11_A1=0x%02X\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x005e0005:printf("1D: PMU10: Pstate%d MRS MR12_A1=0x%02X MR13_A1=0x%02X MR14_A1=0x%02X MR22_A1=0x%02X\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x005f0005:printf("1D: PMU10: Pstate%d MRS MR01_B0=0x%02X MR02_B0=0x%02X MR03_B0=0x%02X MR11_B0=0x%02X\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00600005:printf("1D: PMU10: Pstate%d MRS MR12_B0=0x%02X MR13_B0=0x%02X MR14_B0=0x%02X MR22_B0=0x%02X\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00610005:printf("1D: PMU10: Pstate%d MRS MR01_B1=0x%02X MR02_B1=0x%02X MR03_B1=0x%02X MR11_B1=0x%02X\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00620005:printf("1D: PMU10: Pstate%d MRS MR12_B1=0x%02X MR13_B1=0x%02X MR14_B1=0x%02X MR22_B1=0x%02X\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
#ifdef DDR_FW_TRAIN_1D_MSG
            case 0x00630002:printf("1D: PMU1: AcsmOdtCtrl%02d 0x%02x\n",msg[1],msg[2]); break;
            case 0x00640002:printf("1D: PMU1: AcsmCsMapCtrl%02d 0x%04x\n",msg[1],msg[2]); break;
            case 0x00650002:printf("1D: PMU1: AcsmCsMapCtrl%02d 0x%04x\n",msg[1],msg[2]); break;
            case 0x00660000:printf("1D: PMU1: HwtCAMode set\n"); break;
            //case 0x00670001:printf("1D: PMU3: DDR4 infinite preamble enter/exit mode %d\n",msg[1]); break;
            case 0x00680002:printf("1D: PMU1: In rxenb_train() csn=%d pstate=%d\n",msg[1],msg[2]); break;
            case 0x00690000:printf("1D: PMU3: Finding DQS falling edge\n"); break;
            case 0x006a0000:printf("1D: PMU3: Searching for DDR3/LPDDR3/LPDDR4 read preamble\n"); break;
            case 0x006b0009:printf("1D: PMU3: dtsm fails Even Nibbles : %2x %2x %2x %2x %2x %2x %2x %2x %2x\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7],msg[8],msg[9]); break;
            case 0x006c0009:printf("1D: PMU3: dtsm fails Odd  Nibbles : %2x %2x %2x %2x %2x %2x %2x %2x %2x\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7],msg[8],msg[9]); break;
            case 0x006d0002:printf("1D: PMU3: Preamble search pass=%d anyfail=%d\n",msg[1],msg[2]); break;
            case 0x006e0000:printf("1D: PMU: Error: RxEn training preamble not found\n"); break;
            case 0x006f0000:printf("1D: PMU3: Found DQS pre-amble\n"); break;
            case 0x00700001:printf("1D: PMU: Error: Dbyte %d couldn't find the rising edge of DQS during RxEn Training\n",msg[1]); break;
            case 0x00710000:printf("1D: PMU3: RxEn aligning to first rising edge of burst\n"); break;
            case 0x00720001:printf("1D: PMU3: Decreasing RxEn delay by %d fine step to allow full capture of reads\n",msg[1]); break;
            case 0x00730001:printf("1D: PMU3: MREP Delay = %d\n",msg[1]); break;
            case 0x00740003:printf("1D: PMU3: Errcnt for MREP nib %2d delay = %2d is %d\n",msg[1],msg[2],msg[3]); break;
            case 0x00750002:printf("1D: PMU3: MREP nibble %d sampled a 1 at data buffer delay %d\n",msg[1],msg[2]); break;
            case 0x00760002:printf("1D: PMU3: MREP nibble %d saw a 0 to 1 transition at data buffer delay %d\n",msg[1],msg[2]); break;
            case 0x00770000:printf("1D: PMU2:  MREP did not find a 0 to 1 transition for all nibbles. Failing nibbles assumed to have rising edge close to fine delay 63\n"); break;
            case 0x00780002:printf("1D: PMU2:  Rising edge found in alias window, setting rxDly for nibble %d = %d\n",msg[1],msg[2]); break;
            case 0x00790002:printf("1D: PMU: Error: Failed MREP for nib %d with %d one\n",msg[1],msg[2]); break;
            case 0x007a0003:printf("1D: PMU2:  Rising edge not found in alias window with %d one, leaving rxDly for nibble %d = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x007b0002:printf("1D: PMU3: Training DIMM %d CSn %d\n",msg[1],msg[2]); break;
            case 0x007c0001:printf("1D: PMU3: exitCAtrain_lp3 cs 0x%x\n",msg[1]); break;
            case 0x007d0001:printf("1D: PMU3: enterCAtrain_lp3 cs 0x%x\n",msg[1]); break;
            case 0x007e0001:printf("1D: PMU3: CAtrain_switchmsb_lp3 cs 0x%x\n",msg[1]); break;
            case 0x007f0001:printf("1D: PMU3: CATrain_rdwr_lp3 looking for pattern %x\n",msg[1]); break;
            case 0x00800000:printf("1D: PMU3: exitCAtrain_lp4\n"); break;
            case 0x00810001:printf("1D: PMU3: DEBUG enterCAtrain_lp4 1: cs 0x%x\n",msg[1]); break;
            case 0x00820001:printf("1D: PMU3: DEBUG enterCAtrain_lp4 3: Put dbyte %d in async mode\n",msg[1]); break;
            case 0x00830000:printf("1D: PMU3: DEBUG enterCAtrain_lp4 5: Send MR13 to turn on CA training\n"); break;
            case 0x00840003:printf("1D: PMU3: DEBUG enterCAtrain_lp4 7: idx = %d vref = %x mr12 = %x \n",msg[1],msg[2],msg[3]); break;
            case 0x00850001:printf("1D: PMU3: CATrain_rdwr_lp4 looking for pattern %x\n",msg[1]); break;
            case 0x00860004:printf("1D: PMU3: Phase %d CAreadbackA db:%d %x xo:%x\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x00870005:printf("1D: PMU3: DEBUG lp4SetCatrVref 1: cs=%d chan=%d mr12=%x vref=%d.%d%%\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00880003:printf("1D: PMU3: DEBUG lp4SetCatrVref 3: mr12 = %x send vref= %x to db=%d\n",msg[1],msg[2],msg[3]); break;
            case 0x00890000:printf("1D: PMU10:Optimizing vref\n"); break;
            case 0x008a0004:printf("1D: PMU4:mr12:%2x cs:%d chan %d r:%4x\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x008b0005:printf("1D: PMU3: i:%2d bstr:%2d bsto:%2d st:%d r:%d\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x008c0002:printf("Failed to find sufficient CA Vref Passing Region for CS %d channel %d\n",msg[1],msg[2]); break;
            case 0x008d0005:printf("1D: PMU3:Found %d.%d%% MR12:%x for cs:%d chan %d\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x008e0002:printf("1D: PMU3:Calculated %d for AtxImpedence from acx %d.\n",msg[1],msg[2]); break;
            case 0x008f0000:printf("1D: PMU3:CA Odt impedence ==0.  Use default vref.\n"); break;
            case 0x00900003:printf("1D: PMU3:Calculated %d.%d%% for Vref MR12=0x%x.\n",msg[1],msg[2],msg[3]); break;
            case 0x00910000:printf("1D: PMU3: CAtrain_lp\n"); break;
            case 0x00920000:printf("1D: PMU3: CAtrain Begins.\n"); break;
            case 0x00930001:printf("1D: PMU3: CAtrain_lp testing dly %d\n",msg[1]); break;
            case 0x00940001:printf("1D: PMU5: CA bitmap dump for cs %x\n",msg[1]); break;
            case 0x00950001:printf("1D: PMU5: CAA%d ",msg[1]); break;
            case 0x00960001:printf("%02x",msg[1]); break;
            case 0x00970000:printf("\n"); break;
            case 0x00980001:printf("1D: PMU5: CAB%d ",msg[1]); break;
            case 0x00990001:printf("%02x",msg[1]); break;
            case 0x009a0000:printf("\n"); break;
            case 0x009b0003:printf("1D: PMU3: anibi=%d, anibichan[anibi]=%d ,chan=%d\n",msg[1],msg[2],msg[3]); break;
            case 0x009c0001:printf("%02x",msg[1]); break;
            case 0x009d0001:printf("\nPMU3:Raw CA setting :%x",msg[1]); break;
            case 0x009e0002:printf("\nPMU3:ATxDly setting:%x margin:%d\n",msg[1],msg[2]); break;
            case 0x009f0002:printf("\nPMU3:InvClk ATxDly setting:%x margin:%d\n",msg[1],msg[2]); break;
            case 0x00a00000:printf("\nPMU3:No Range found!\n"); break;
            case 0x00a10003:printf("1D: PMU3: 2 anibi=%d, anibichan[anibi]=%d ,chan=%d",msg[1],msg[2],msg[3]); break;
            case 0x00a20002:printf("\nPMU3: no neg clock => CA setting anib=%d, :%d\n",msg[1],msg[2]); break;
            case 0x00a30001:printf("1D: PMU3:Normal margin:%d\n",msg[1]); break;
            case 0x00a40001:printf("1D: PMU3:Inverted margin:%d\n",msg[1]); break;
            case 0x00a50000:printf("1D: PMU3:Using Inverted clock\n"); break;
            case 0x00a60000:printf("1D: PMU3:Using normal clk\n"); break;
            case 0x00a70003:printf("1D: PMU3: 3 anibi=%d, anibichan[anibi]=%d ,chan=%d\n",msg[1],msg[2],msg[3]); break;
            case 0x00a80002:printf("1D: PMU3: Setting ATxDly for anib %x to %x\n",msg[1],msg[2]); break;
            case 0x00a90000:printf("1D: PMU: Error: CA Training Failed.\n"); break;
            case 0x00aa0000:printf("1D: PMU1: Writing MRs\n"); break;
            case 0x00ab0000:printf("1D: PMU4:Using MR12 values from 1D CA VREF training.\n"); break;
            case 0x00ac0000:printf("1D: PMU3:Writing all MRs to fsp 1\n"); break;
            case 0x00ad0000:printf("1D: PMU10:Lp4Quickboot mode.\n"); break;
            case 0x00ae0000:printf("1D: PMU3: Writing MRs\n"); break;
            case 0x00af0001:printf("1D: PMU10: Setting boot clock divider to %d\n",msg[1]); break;
            case 0x00b00000:printf("1D: PMU3: Resetting DRAM\n"); break;
            case 0x00b10000:printf("1D: PMU3: setup for RCD initalization\n"); break;
            case 0x00b20000:printf("1D: PMU3: pmu_exit_SR from dev_init()\n"); break;
            case 0x00b30000:printf("1D: PMU3: initializing RCD\n"); break;
            case 0x00b40000:printf("1D: PMU10: **** Executing 2D Image ****\n"); break;
            //case 0x00b50001:printf("1D: PMU10: **** Start DDR4 Training. PMU Firmware Revision 0x%04x ****\n",msg[1]); break;
            //case 0x00b60001:printf("1D: PMU10: **** Start DDR3 Training. PMU Firmware Revision 0x%04x ****\n",msg[1]); break;
            //case 0x00b70001:printf("1D: PMU10: **** Start LPDDR3 Training. PMU Firmware Revision 0x%04x ****\n",msg[1]); break;
            case 0x00b80001:printf("1D: PMU10: **** Start LPDDR4 Training. PMU Firmware Revision 0x%04x ****\n",msg[1]); break;
            case 0x00b90000:printf("1D: PMU: Error: Mismatched internal revision between DCCM and ICCM images\n"); break;
            case 0x00ba0001:printf("1D: PMU10: **** Testchip %d Specific Firmware ****\n",msg[1]); break;
            case 0x00bb0000:printf("1D: PMU1: LRDIMM with EncodedCS mode, one DIMM\n"); break;
            case 0x00bc0000:printf("1D: PMU1: LRDIMM with EncodedCS mode, two DIMMs\n"); break;
            case 0x00bd0000:printf("1D: PMU1: RDIMM with EncodedCS mode, one DIMM\n"); break;
            case 0x00be0000:printf("1D: PMU2: Starting LRDIMM MREP training for all ranks\n"); break;
            case 0x00bf0000:printf("1D: PMU199: LRDIMM MREP training for all ranks completed\n"); break;
            case 0x00c00000:printf("1D: PMU2: Starting LRDIMM DWL training for all ranks\n"); break;
            case 0x00c10000:printf("1D: PMU199: LRDIMM DWL training for all ranks completed\n"); break;
            case 0x00c20000:printf("1D: PMU2: Starting LRDIMM MRD training for all ranks\n"); break;
            case 0x00c30000:printf("1D: PMU199: LRDIMM MRD training for all ranks completed\n"); break;
            case 0x00c40000:printf("1D: PMU2: Starting RXEN training for all ranks\n"); break;
            case 0x00c50000:printf("1D: PMU2: Starting write leveling fine delay training for all ranks\n"); break;
            case 0x00c60000:printf("1D: PMU2: Starting LRDIMM MWD training for all ranks\n"); break;
            case 0x00c70000:printf("1D: PMU199: LRDIMM MWD training for all ranks completed\n"); break;
            case 0x00c80000:printf("1D: PMU2: Starting write leveling fine delay training for all ranks\n"); break;
            case 0x00c90000:printf("1D: PMU2: Starting read deskew training\n"); break;
            case 0x00ca0000:printf("1D: PMU2: Starting SI friendly 1d RdDqs training for all ranks\n"); break;
            case 0x00cb0000:printf("1D: PMU2: Starting write leveling coarse delay training for all ranks\n"); break;
            case 0x00cc0000:printf("1D: PMU2: Starting 1d WrDq training for all ranks\n"); break;
            case 0x00cd0000:printf("1D: PMU2: Running DQS2DQ Oscillator for all ranks\n"); break;
            case 0x00ce0000:printf("1D: PMU2: Starting again read deskew training but with PRBS\n"); break;
            case 0x00cf0000:printf("1D: PMU2: Starting 1d RdDqs training for all ranks\n"); break;
            case 0x00d00000:printf("1D: PMU2: Starting again 1d WrDq training for all ranks\n"); break;
            case 0x00d10000:printf("1D: PMU2: Starting MaxRdLat training\n"); break;
            case 0x00d20000:printf("1D: PMU2: Starting 2d RdDqs training for all ranks\n"); break;
            case 0x00d30000:printf("1D: PMU2: Starting 2d WrDq training for all ranks\n"); break;
            case 0x00d40000:printf("1D: PMU2: Starting 2d RdDqs training for all ranks\n"); break;
            case 0x00d50002:printf("1D: PMU3:read_fifo %x %x\n",msg[1],msg[2]); break;
            case 0x00d60001:printf("1D: PMU: Error: Invalid PhyDrvImpedance of 0x%x specified in message block.\n",msg[1]); break;
            case 0x00d70001:printf("1D: PMU: Error: Invalid PhyOdtImpedance of 0x%x specified in message block.\n",msg[1]); break;
            case 0x00d80001:printf("1D: PMU: Error: Invalid BPZNResVal of 0x%x specified in message block.\n",msg[1]); break;
            case 0x00d90005:printf("1D: PMU3: fixRxEnBackOff csn:%d db:%d dn:%d bo:%d dly:%x\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00da0001:printf("1D: PMU3: fixRxEnBackOff dly:%x\n",msg[1]); break;
            case 0x00db0000:printf("1D: PMU3: Entering setupPpt\n"); break;
            case 0x00dc0000:printf("1D: PMU3: Start lp4PopulateHighLowBytes\n"); break;
            case 0x00dd0002:printf("1D: PMU3:Dbyte Detect: db%d received %x\n",msg[1],msg[2]); break;
            case 0x00de0002:printf("1D: PMU3:getDqs2Dq read %x from dbyte %d\n",msg[1],msg[2]); break;
            case 0x00df0002:printf("1D: PMU3:getDqs2Dq(2) read %x from dbyte %d\n",msg[1],msg[2]); break;
            case 0x00e00001:printf("1D: PMU: Error: Dbyte %d read 0 from the DQS oscillator it is connected to\n",msg[1]); break;
            case 0x00e10002:printf("1D: PMU4: Dbyte %d dqs2dq = %d/32 UI\n",msg[1],msg[2]); break;
            case 0x00e20003:printf("1D: PMU3:getDqs2Dq set dqs2dq:%d/32 ui (%d ps) from dbyte %d\n",msg[1],msg[2],msg[3]); break;
            case 0x00e30003:printf("1D: PMU3: Setting coarse delay in AtxDly chiplet %d from 0x%02x to 0x%02x \n",msg[1],msg[2],msg[3]); break;
            case 0x00e40003:printf("1D: PMU3: Clearing coarse delay in AtxDly chiplet %d from 0x%02x to 0x%02x \n",msg[1],msg[2],msg[3]); break;
            //case 0x00e50000:printf("1D: PMU3: Performing DDR4 geardown sync sequence\n"); break;
            case 0x00e60000:printf("1D: PMU1: Enter self refresh\n"); break;
            case 0x00e70000:printf("1D: PMU1: Exit self refresh\n"); break;
            case 0x00e80000:printf("1D: PMU: Error: No dbiEnable with lp4\n"); break;
            case 0x00e90000:printf("1D: PMU: Error: No dbiDisable with lp4\n"); break;
            //case 0x00ea0001:printf("1D: PMU1: DDR4 update Rx DBI Setting disable %d\n",msg[1]); break;
            //case 0x00eb0001:printf("1D: PMU1: DDR4 update 2nCk WPre Setting disable %d\n",msg[1]); break;
            case 0x00ec0005:printf("1D: PMU1: read_delay: db%d lane%d delays[%2d] = 0x%02x (max 0x%02x)\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x00ed0004:printf("1D: PMU1: write_delay: db%d lane%d delays[%2d] = 0x%04x\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x00ee0001:printf("1D: PMU5: ID=%d -- db0  db1  db2  db3  db4  db5  db6  db7  db8  db9 --\n",msg[1]); break;
            case 0x00ef000b:printf("1D: PMU5: [%d]:0x %4x %4x %4x %4x %4x %4x %4x %4x %4x %4x\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7],msg[8],msg[9],msg[10],msg[11]); break;
            case 0x00f00003:printf("1D: PMU2: dump delays - pstate=%d dimm=%d csn=%d\n",msg[1],msg[2],msg[3]); break;
            case 0x00f10000:printf("1D: PMU3: Printing Mid-Training Delay Information\n"); break;
            case 0x00f20001:printf("1D: PMU5: CS%d <<KEY>> 0 TrainingCntr <<KEY>> coarse(15:10) fine(9:0)\n",msg[1]); break;
            case 0x00f30001:printf("1D: PMU5: CS%d <<KEY>> 0 RxEnDly, 1 RxClkDly <<KEY>> coarse(10:6) fine(5:0)\n",msg[1]); break;
            case 0x00f40001:printf("1D: PMU5: CS%d <<KEY>> 0 TxDqsDly, 1 TxDqDly <<KEY>> coarse(9:6) fine(5:0)\n",msg[1]); break;
            case 0x00f50001:printf("1D: PMU5: CS%d <<KEY>> 0 RxPBDly <<KEY>> 1 Delay Unit ~= 7ps \n",msg[1]); break;
            case 0x00f60000:printf("1D: PMU5: all CS <<KEY>> 0 DFIMRL <<KEY>> Units = DFI clocks\n"); break;
            case 0x00f70000:printf("1D: PMU5: all CS <<KEY>> VrefDACs <<KEY>> DAC(6:0)\n"); break;
            case 0x00f80000:printf("1D: PMU1: Set DMD in MR13 and wrDBI in MR3 for training\n"); break;
            case 0x00f90000:printf("1D: PMU: Error: getMaxRxen() failed to find largest rxen nibble delay\n"); break;
            case 0x00fa0003:printf("1D: PMU2: getMaxRxen(): maxDly %d maxTg %d maxNib %d\n",msg[1],msg[2],msg[3]); break;
            case 0x00fb0003:printf("1D: PMU2: getRankMaxRxen(): maxDly %d Tg %d maxNib %d\n",msg[1],msg[2],msg[3]); break;
            case 0x00fc0000:printf("1D: PMU1: skipping CDD calculation in 2D image\n"); break;
            case 0x00fd0001:printf("1D: PMU3: Calculating CDDs for pstate %d\n",msg[1]); break;
            case 0x00fe0003:printf("1D: PMU3: rxFromDly[%d][%d] = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x00ff0003:printf("1D: PMU3: rxToDly  [%d][%d] = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x01000003:printf("1D: PMU3: rxDly    [%d][%d] = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x01010003:printf("1D: PMU3: txDly    [%d][%d] = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x01020003:printf("1D: PMU3: allFine CDD_RR_%d_%d = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x01030003:printf("1D: PMU3: allFine CDD_WW_%d_%d = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x01040003:printf("1D: PMU3: CDD_RR_%d_%d = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x01050003:printf("1D: PMU3: CDD_WW_%d_%d = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x01060003:printf("1D: PMU3: allFine CDD_RW_%d_%d = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x01070003:printf("1D: PMU3: allFine CDD_WR_%d_%d = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x01080003:printf("1D: PMU3: CDD_RW_%d_%d = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x01090003:printf("1D: PMU3: CDD_WR_%d_%d = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x010a0004:printf("1D: PMU3: F%dBC2x_B%d_D%d = 0x%02x\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x010b0004:printf("1D: PMU3: F%dBC3x_B%d_D%d = 0x%02x\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x010c0004:printf("1D: PMU3: F%dBC4x_B%d_D%d = 0x%02x\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x010d0004:printf("1D: PMU3: F%dBC5x_B%d_D%d = 0x%02x\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x010e0004:printf("1D: PMU3: F%dBC8x_B%d_D%d = 0x%02x\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x010f0004:printf("1D: PMU3: F%dBC9x_B%d_D%d = 0x%02x\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x01100004:printf("1D: PMU3: F%dBCAx_B%d_D%d = 0x%02x\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x01110004:printf("1D: PMU3: F%dBCBx_B%d_D%d = 0x%02x\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x01120000:printf("1D: PMU10: Entering context_switch_postamble\n"); break;
            case 0x01130003:printf("1D: PMU10: context_switch_postamble is enabled for DIMM %d, RC0A=0x%x, RC3x=0x%x\n",msg[1],msg[2],msg[3]); break;
            case 0x01140000:printf("1D: PMU10: Setting bcw fspace 0\n"); break;
            case 0x01150001:printf("1D: PMU10: Sending BC0A = 0x%x\n",msg[1]); break;
            case 0x01160001:printf("1D: PMU10: Sending BC6x = 0x%x\n",msg[1]); break;
            case 0x01170001:printf("1D: PMU10: Sending RC0A = 0x%x\n",msg[1]); break;
            case 0x01180001:printf("1D: PMU10: Sending RC3x = 0x%x\n",msg[1]); break;
            case 0x01190001:printf("1D: PMU10: Sending RC0A = 0x%x\n",msg[1]); break;
            case 0x011a0001:printf("1D: PMU1: enter_lp3: DEBUG: pstate = %d\n",msg[1]); break;
            case 0x011b0001:printf("1D: PMU1: enter_lp3: DEBUG: dfifreqxlat_pstate = %d\n",msg[1]); break;
            case 0x011c0001:printf("1D: PMU1: enter_lp3: DEBUG: pllbypass = %d\n",msg[1]); break;
            case 0x011d0001:printf("1D: PMU1: enter_lp3: DEBUG: forcecal = %d\n",msg[1]); break;
            case 0x011e0001:printf("1D: PMU1: enter_lp3: DEBUG: pllmaxrange = 0x%x\n",msg[1]); break;
            case 0x011f0001:printf("1D: PMU1: enter_lp3: DEBUG: dacval_out = 0x%x\n",msg[1]); break;
            case 0x01200001:printf("1D: PMU1: enter_lp3: DEBUG: pllctrl3 = 0x%x\n",msg[1]); break;
            case 0x01210000:printf("1D: PMU3: Loading DRAM with BIOS supplied MR values and entering self refresh prior to exiting PMU code.\n"); break;
            case 0x01220002:printf("1D: PMU3: Setting DataBuffer function space of dimmcs 0x%02x to %d\n",msg[1],msg[2]); break;
            case 0x01230002:printf("1D: PMU4: Setting RCW FxRC%Xx = 0x%02x\n",msg[1],msg[2]); break;
            case 0x01240002:printf("1D: PMU4: Setting RCW FxRC%02X = 0x%02x\n",msg[1],msg[2]); break;
            //case 0x01250001:printf("1D: PMU1: DDR4 update Rd Pre Setting disable %d\n",msg[1]); break;
            case 0x01260002:printf("1D: PMU2: Setting BCW FxBC%Xx = 0x%02x\n",msg[1],msg[2]); break;
            case 0x01270002:printf("1D: PMU2: Setting BCW BC%02X = 0x%02x\n",msg[1],msg[2]); break;
            case 0x01280002:printf("1D: PMU2: Setting BCW PBA mode FxBC%Xx = 0x%02x\n",msg[1],msg[2]); break;
            case 0x01290002:printf("1D: PMU2: Setting BCW PBA mode BC%02X = 0x%02x\n",msg[1],msg[2]); break;
            case 0x012a0003:printf("1D: PMU4: BCW value for dimm %d, fspace %d, addr 0x%04x\n",msg[1],msg[2],msg[3]); break;
            case 0x012b0002:printf("1D: PMU4: DB %d, value 0x%02x\n",msg[1],msg[2]); break;
            case 0x012c0000:printf("1D: PMU6: WARNING MREP underflow, set to min value -2 coarse, 0 fine\n"); break;
            case 0x012d0004:printf("1D: PMU6: LRDIMM Writing final data buffer fine delay value nib %2d, trainDly %3d, fineDly code %2d, new MREP fine %2d\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x012e0003:printf("1D: PMU6: LRDIMM Writing final data buffer fine delay value nib %2d, trainDly %3d, fineDly code %2d\n",msg[1],msg[2],msg[3]); break;
            case 0x012f0003:printf("1D: PMU6: LRDIMM Writing data buffer fine delay type %d nib %2d, code %2d\n",msg[1],msg[2],msg[3]); break;
            case 0x01300002:printf("1D: PMU6: Writing final data buffer coarse delay value dbyte %2d, coarse = 0x%02x\n",msg[1],msg[2]); break;
            case 0x01310003:printf("1D: PMU4: data 0x%04x at MB addr 0x%08x saved at CSR addr 0x%08x\n",msg[1],msg[2],msg[3]); break;
            case 0x01320003:printf("1D: PMU4: data 0x%04x at MB addr 0x%08x restored from CSR addr 0x%08x\n",msg[1],msg[2],msg[3]); break;
            case 0x01330003:printf("1D: PMU4: data 0x%04x at MB addr 0x%08x saved at CSR addr 0x%08x\n",msg[1],msg[2],msg[3]); break;
            case 0x01340003:printf("1D: PMU4: data 0x%04x at MB addr 0x%08x restored from CSR addr 0x%08x\n",msg[1],msg[2],msg[3]); break;
            case 0x01350001:printf("1D: PMU3: Update BC00, BC01, BC02 for rank-dimm 0x%02x\n",msg[1]); break;
            case 0x01360000:printf("1D: PMU3: Writing D4 RDIMM RCD Control words F0RC00 -> F0RC0F\n"); break;
            case 0x01370000:printf("1D: PMU3: Disable parity in F0RC0E\n"); break;
            case 0x01380000:printf("1D: PMU3: Writing D4 RDIMM RCD Control words F1RC00 -> F1RC05\n"); break;
            case 0x01390000:printf("1D: PMU3: Writing D4 RDIMM RCD Control words F1RC1x -> F1RC9x\n"); break;
            case 0x013a0000:printf("1D: PMU3: Writing D4 Data buffer Control words BC00 -> BC0E\n"); break;
            case 0x013b0002:printf("1D: PMU1: setAltCL Sending MR0 0x%x cl=%d\n",msg[1],msg[2]); break;
            case 0x013c0002:printf("1D: PMU1: restoreFromAltCL Sending MR0 0x%x cl=%d\n",msg[1],msg[2]); break;
            case 0x013d0002:printf("1D: PMU1: restoreAcsmFromAltCL Sending MR0 0x%x cl=%d\n",msg[1],msg[2]); break;
            case 0x013e0002:printf("1D: PMU2: Setting D3R RC%d = 0x%01x\n",msg[1],msg[2]); break;
            case 0x013f0000:printf("1D: PMU3: Writing D3 RDIMM RCD Control words RC0 -> RC11\n"); break;
            case 0x01400002:printf("1D: PMU0: VrefDAC0/1 vddqStart %d dacToVddq %d\n",msg[1],msg[2]); break;
            //case 0x01410001:printf("1D: PMU: Error: Messageblock phyVref=0x%x is above the limit for TSMC28's attenuated LPDDR4 receivers. Please see the pub databook\n",msg[1]); break;
            //case 0x01420001:printf("1D: PMU: Error: Messageblock phyVref=0x%x is above the limit for TSMC28's attenuated DDR4 receivers. Please see the pub databook\n",msg[1]); break;
            case 0x01430001:printf("1D: PMU0: PHY VREF @ (%d/1000) VDDQ\n",msg[1]); break;
            case 0x01440002:printf("1D: PMU0: initalizing phy vrefDacs to %d ExtVrefRange %x\n",msg[1],msg[2]); break;
            case 0x01450002:printf("1D: PMU0: initalizing global vref to %d range %d\n",msg[1],msg[2]); break;
            case 0x01460002:printf("1D: PMU4: Setting initial device vrefDQ for CS%d to MR6 = 0x%04x\n",msg[1],msg[2]); break;
            case 0x01470003:printf("1D: PMU1: In write_level_fine() csn=%d dimm=%d pstate=%d\n",msg[1],msg[2],msg[3]); break;
            case 0x01480000:printf("1D: PMU3: Fine write leveling hardware search increasing TxDqsDly until full bursts are seen\n"); break;
            case 0x01490000:printf("1D: PMU4: WL normalized pos   : ........................|........................\n"); break;
            case 0x014a0007:printf("1D: PMU4: WL margin for nib %2d: %08x%08x%08x%08x%08x%08x\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7]); break;
            case 0x014b0000:printf("1D: PMU4: WL normalized pos   : ........................|........................\n"); break;
            case 0x014c0000:printf("1D: PMU3: Exiting write leveling mode\n"); break;
            case 0x014d0001:printf("1D: PMU3: got %d for cl in load_wrlvl_acsm\n",msg[1]); break;
            case 0x014e0003:printf("1D: PMU1: In write_level_coarse() csn=%d dimm=%d pstate=%d\n",msg[1],msg[2],msg[3]); break;
            case 0x014f0003:printf("1D: PMU3: left eye edge search db:%d ln:%d dly:0x%x\n",msg[1],msg[2],msg[3]); break;
            case 0x01500003:printf("1D: PMU3: right eye edge search db:%d ln:%d dly:0x%x\n",msg[1],msg[2],msg[3]); break;
            case 0x01510004:printf("1D: PMU3: eye center db:%d ln:%d dly:0x%x (maxdq:%x)\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x01520003:printf("1D: PMU3: Wrote to TxDqDly db:%d ln:%d dly:0x%x\n",msg[1],msg[2],msg[3]); break;
            case 0x01530003:printf("1D: PMU3: Wrote to TxDqDly db:%d ln:%d dly:0x%x\n",msg[1],msg[2],msg[3]); break;
            case 0x01540002:printf("1D: PMU3: Coarse write leveling dbyte%2d is still failing for TxDqsDly=0x%04x\n",msg[1],msg[2]); break;
            case 0x01550002:printf("1D: PMU4: Coarse write leveling iteration %d saw %d data miscompares across the entire phy\n",msg[1],msg[2]); break;
            case 0x01560000:printf("1D: PMU: Error: Failed write leveling coarse\n"); break;
            case 0x01570001:printf("1D: PMU3: got %d for cl in load_wrlvl_acsm\n",msg[1]); break;
            case 0x01580003:printf("1D: PMU3: In write_level_coarse() csn=%d dimm=%d pstate=%d\n",msg[1],msg[2],msg[3]); break;
            case 0x01590003:printf("1D: PMU3: left eye edge search db:%d ln:%d dly:0x%x\n",msg[1],msg[2],msg[3]); break;
            case 0x015a0003:printf("1D: PMU3: right eye edge search db: %d ln: %d dly: 0x%x\n",msg[1],msg[2],msg[3]); break;
            case 0x015b0004:printf("1D: PMU3: eye center db: %d ln: %d dly: 0x%x (maxdq: 0x%x)\n",msg[1],msg[2],msg[3],msg[4]); break;
            case 0x015c0003:printf("1D: PMU3: Wrote to TxDqDly db: %d ln: %d dly: 0x%x\n",msg[1],msg[2],msg[3]); break;
            case 0x015d0003:printf("1D: PMU3: Wrote to TxDqDly db: %d ln: %d dly: 0x%x\n",msg[1],msg[2],msg[3]); break;
            case 0x015e0002:printf("1D: PMU3: Coarse write leveling nibble%2d is still failing for TxDqsDly=0x%04x\n",msg[1],msg[2]); break;
            case 0x015f0002:printf("1D: PMU4: Coarse write leveling iteration %d saw %d data miscompares across the entire phy\n",msg[1],msg[2]); break;
            case 0x01600000:printf("1D: PMU: Error: Failed write leveling coarse\n"); break;
            case 0x01610000:printf("1D: PMU4: WL normalized pos   : ................................|................................\n"); break;
            case 0x01620009:printf("1D: PMU4: WL margin for nib %2d: %08x%08x%08x%08x%08x%08x%08x%08x\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7],msg[8],msg[9]); break;
            case 0x01630000:printf("1D: PMU4: WL normalized pos   : ................................|................................\n"); break;
            case 0x01640001:printf("1D: PMU8: Adjust margin after WL coarse to be larger than %d\n",msg[1]); break;
            case 0x01650001:printf("1D: PMU: Error: All margin after write leveling coarse are smaller than minMargin %d\n",msg[1]); break;
            case 0x01660002:printf("1D: PMU8: Decrement nib %d TxDqsDly by %d fine step\n",msg[1],msg[2]); break;
            case 0x01670003:printf("1D: PMU3: In write_level_coarse() csn=%d dimm=%d pstate=%d\n",msg[1],msg[2],msg[3]); break;
            case 0x01680005:printf("1D: PMU2: Write level: dbyte %d nib%d dq/dmbi %2d dqsfine 0x%04x dqDly 0x%04x\n",msg[1],msg[2],msg[3],msg[4],msg[5]); break;
            case 0x01690002:printf("1D: PMU3: Coarse write leveling nibble%2d is still failing for TxDqsDly=0x%04x\n",msg[1],msg[2]); break;
            case 0x016a0002:printf("1D: PMU4: Coarse write leveling iteration %d saw %d data miscompares across the entire phy\n",msg[1],msg[2]); break;
            case 0x016b0000:printf("1D: PMU: Error: Failed write leveling coarse\n"); break;
            case 0x016c0001:printf("1D: PMU3: DWL delay = %d\n",msg[1]); break;
            case 0x016d0003:printf("1D: PMU3: Errcnt for DWL nib %2d delay = %2d is %d\n",msg[1],msg[2],msg[3]); break;
            case 0x016e0002:printf("1D: PMU3: DWL nibble %d sampled a 1 at delay %d\n",msg[1],msg[2]); break;
            case 0x016f0003:printf("1D: PMU3: DWL nibble %d passed at delay %d. Rising edge was at %d\n",msg[1],msg[2],msg[3]); break;
            case 0x01700000:printf("1D: PMU2: DWL did nto find a rising edge of memclk for all nibbles. Failing nibbles assumed to have rising edge close to fine delay 63\n"); break;
            case 0x01710002:printf("1D: PMU2:  Rising edge found in alias window, setting wrlvlDly for nibble %d = %d\n",msg[1],msg[2]); break;
            case 0x01720002:printf("1D: PMU: Error: Failed DWL for nib %d with %d one\n",msg[1],msg[2]); break;
            case 0x01730003:printf("1D: PMU2:  Rising edge not found in alias window with %d one, leaving wrlvlDly for nibble %d = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x04000000:printf("1D: PMU: Error:Mailbox Buffer Overflowed.\n"); break;
            case 0x04010000:printf("1D: PMU: Error:Mailbox Buffer Overflowed.\n"); break;
            case 0x04020000:printf("1D: PMU: ***** Assertion Error - terminating *****\n"); break;
            case 0x04030002:printf("1D: PMU1: swapByte db %d by %d\n",msg[1],msg[2]); break;
            case 0x04040003:printf("1D: PMU3: get_cmd_dly max(%d ps, %d memclk) = %d\n",msg[1],msg[2],msg[3]); break;
            case 0x04050002:printf("1D: PMU0: Write CSR 0x%06x 0x%04x\n",msg[1],msg[2]); break;
            case 0x04060002:printf("1D: PMU0: hwt_init_ppgc_prbs(): Polynomial: %x, Deg: %d\n",msg[1],msg[2]); break;
            case 0x04070001:printf("1D: PMU: Error: acsm_set_cmd to non existant instruction adddress %d\n",msg[1]); break;
            case 0x04080001:printf("1D: PMU: Error: acsm_set_cmd with unknown ddr cmd 0x%x\n",msg[1]); break;
            case 0x0409000c:printf("1D: PMU1: acsm_addr %02x, acsm_flgs %04x, ddr_cmd %02x, cmd_dly %02x, ddr_addr %04x, ddr_bnk %02x, ddr_cs %02x, cmd_rcnt %02x, AcsmSeq0/1/2/3 %04x %04x %04x %04x\n",msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7],msg[8],msg[9],msg[10],msg[11],msg[12]); break;
            case 0x040a0000:printf("1D: PMU: Error: Polling on ACSM done failed to complete in acsm_poll_done()...\n"); break;
            case 0x040b0000:printf("1D: PMU1: acsm RUN\n"); break;
            case 0x040c0000:printf("1D: PMU1: acsm STOPPED\n"); break;
            case 0x040d0002:printf("1D: PMU1: acsm_init: acsm_mode %04x mxrdlat %04x\n",msg[1],msg[2]); break;
            case 0x040e0002:printf("1D: PMU: Error: setAcsmCLCWL: cl and cwl must be each >= 2 and 5, resp. CL=%d CWL=%d\n",msg[1],msg[2]); break;
            case 0x040f0002:printf("1D: PMU: Error: setAcsmCLCWL: cl and cwl must be each >= 5. CL=%d CWL=%d\n",msg[1],msg[2]); break;
            case 0x04100002:printf("1D: PMU1: setAcsmCLCWL: CASL %04d WCASL %04d\n",msg[1],msg[2]); break;
            case 0x04110001:printf("1D: PMU: Error: Reserved value of register F0RC0F found in message block: 0x%04x\n",msg[1]); break;
            case 0x04120001:printf("1D: PMU3: Written MRS to CS=0x%02x\n",msg[1]); break;
            case 0x04130001:printf("1D: PMU3: Written MRS to CS=0x%02x\n",msg[1]); break;
            case 0x04140000:printf("1D: PMU3: Entering Boot Freq Mode.\n"); break;
            case 0x04150001:printf("1D: PMU: Error: Boot clock divider setting of %d is too small\n",msg[1]); break;
            case 0x04160000:printf("1D: PMU3: Exiting Boot Freq Mode.\n"); break;
            case 0x04170002:printf("1D: PMU3: Writing MR%d OP=%x\n",msg[1],msg[2]); break;
            case 0x04180000:printf("1D: PMU: Error: Delay too large in slomo\n"); break;
            case 0x04190001:printf("1D: PMU3: Written MRS to CS=0x%02x\n",msg[1]); break;
            case 0x041a0000:printf("1D: PMU3: Enable Channel A\n"); break;
            case 0x041b0000:printf("1D: PMU3: Enable Channel B\n"); break;
            case 0x041c0000:printf("1D: PMU3: Enable All Channels\n"); break;
            case 0x041d0002:printf("1D: PMU2: Use PDA mode to set MR%d with value 0x%02x\n",msg[1],msg[2]); break;
            case 0x041e0001:printf("1D: PMU3: Written Vref with PDA to CS=0x%02x\n",msg[1]); break;
            case 0x041f0000:printf("1D: PMU1: start_cal: DEBUG: setting CalRun to 1\n"); break;
            case 0x04200000:printf("1D: PMU1: start_cal: DEBUG: setting CalRun to 0\n"); break;
            case 0x04210001:printf("1D: PMU1: lock_pll_dll: DEBUG: pstate = %d\n",msg[1]); break;
            case 0x04220001:printf("1D: PMU1: lock_pll_dll: DEBUG: dfifreqxlat_pstate = %d\n",msg[1]); break;
            case 0x04230001:printf("1D: PMU1: lock_pll_dll: DEBUG: pllbypass = %d\n",msg[1]); break;
            case 0x04240001:printf("1D: PMU3: SaveLcdlSeed: Saving seed seed %d\n",msg[1]); break;
            case 0x04250000:printf("1D: PMU1: in phy_defaults()\n"); break;
            case 0x04260003:printf("1D: PMU3: ACXConf:%d MaxNumDbytes:%d NumDfi:%d\n",msg[1],msg[2],msg[3]); break;
            case 0x04270005:printf("1D: PMU1: setAltAcsmCLCWL setting cl=%d cwl=%d\n",msg[1],msg[2]); break;
            default: printf("1D: Error, %x is non-defined stream-msg code\n",msg[0]); break;
#endif // DDR_FW_1D_ONLY_MSG
       }
    }
}
#endif


#ifdef DDR_FW_STAGE_MSG
//main massage display
void msg_display(unsigned int code, unsigned int phy_num)
{
  switch(code&0xffff) {
     case 0x00: printf("PHY%x MSG: %x, End of initialization\n",phy_num,code); break;
     case 0x01: printf("PHY%x MSG: %x, End of fine write leveling\n",phy_num,code); break;
     case 0x02: printf("PHY%x MSG: %x, End of read enable training\n",phy_num,code); break;
     case 0x03: printf("PHY%x MSG: %x, End of read delay center optimization\n",phy_num,code); break;
     case 0x04: printf("PHY%x MSG: %x, End of write delay center optimization\n",phy_num,code); break;
     case 0x05: printf("PHY%x MSG: %x, End of 2D read delay/voltage center optimization\n",phy_num,code); break;
     case 0x06: printf("PHY%x MSG: %x, End of 2D write delay/voltage center optimization\n",phy_num,code); break;
     case 0x09: printf("PHY%x MSG: %x, End of max read latency training\n",phy_num,code); break;
     case 0x0a: printf("PHY%x MSG: %x, End of read dq deskew training\n",phy_num,code); break;
     case 0x0b: printf("PHY%x MSG: %x, Reserved\n",phy_num,code); break;
     case 0x0c: printf("PHY%x MSG: %x, End of all DB training (MREP/DWL/MRD/MWD complete)",phy_num,code); break;
     case 0x0d: printf("PHY%x MSG: %x, End of CA training\n",phy_num,code); break;
     case 0xfd: printf("PHY%x MSG: %x, End of MPR read delay center optimization\n",phy_num,code); break;
     case 0xfe: printf("PHY%x MSG: %x, End of write leveling coarse delay\n",phy_num,code); break;
     //case 0x08: printf("MSG: %x, Streaming message mode\n",code); break;
     case 0x08:  break;
     case 0x07: printf("PHY%x MSG: %x, Training has run successfully(firmware complete)\n",phy_num,code); break;
     case 0xff: printf("PHY%x MSG: %x, Training has failed(firmware complete)\n",phy_num,code); break;
     default: printf("PHY%x MSG: %x, Error and undefined code\n",phy_num,code); break;
  }
}
#endif

unsigned int get_mails(void)
{
    unsigned int read;
    unsigned int msg0;
    unsigned int msg1;
	
	//wait ack
	read = 0x1;
	do{
        //read = (unsigned int)(*(volatile unsigned short*)(0xfe7a0008));
        read = ddr_phy_reg_rd(0xd0004);
    }while((read&0x1) == 1);

	//read msg
    //msg0 = (unsigned int)(*(volatile unsigned short*)(0xfe7a0064));
    msg0 = ddr_phy_reg_rd(0xd0032);
	//msg1 = (unsigned int)(*(volatile unsigned short*)(0xfe7a0068));
	msg1 = ddr_phy_reg_rd(0xd0034);
	
   //write-back
   //*(volatile unsigned short*)(0xfe7a0062) = 0;
   ddr_phy0_reg_wr(0xd0031,0);
   
   //wait ack end
   read = 0x0;
	do{
        //read = (unsigned int)(*(volatile unsigned short*)(0xfe7a0008));
        read = ddr_phy_reg_rd(0xd0004);
    }while((read&0x1) == 0);
	
	//re-enable
	//*(volatile unsigned short*)(0xfe7a0062) = 1;
	ddr_phy0_reg_wr(0xd0031,1);
	
	return (msg0 + (msg1<<16));//{uctDATwriteonlyshadow,uctwriteonlyshadow}
}

unsigned int get_phy1_mails(void)
{
    unsigned int read;
    unsigned int msg0;
    unsigned int msg1;

	//wait ack
	read = 0x1;
	do{
        //read = (unsigned int)(*(volatile unsigned short*)(0xfe7a0008));
        read = ddr_phy1_reg_rd(0xd0004);
    }while((read&0x1) == 1);

	//read msg
    //msg0 = (unsigned int)(*(volatile unsigned short*)(0xfe7a0064));
    msg0 = ddr_phy1_reg_rd(0xd0032);
	//msg1 = (unsigned int)(*(volatile unsigned short*)(0xfe7a0068));
	msg1 = ddr_phy1_reg_rd(0xd0034);

   //write-back
   //*(volatile unsigned short*)(0xfe7a0062) = 0;
   ddr_phy1_reg_wr(0xd0031,0);

   //wait ack end
   read = 0x0;
	do{
        //read = (unsigned int)(*(volatile unsigned short*)(0xfe7a0008));
        read = ddr_phy1_reg_rd(0xd0004);
    }while((read&0x1) == 0);

	//re-enable
	//*(volatile unsigned short*)(0xfe7a0062) = 1;
	ddr_phy1_reg_wr(0xd0031,1);

	return (msg0 + (msg1<<16));//{uctDATwriteonlyshadow,uctwriteonlyshadow}
}

void dwc_ddrphy_phyinit_userCustom_G_waitFwDone(unsigned char train2d) {

unsigned int train_result;

unsigned int stream_msg[32],i;

train_result = 0x1;
while(((train_result&0xffff)!=0x7) & ((train_result&0xffff)!=0xff))
{
    train_result = get_mails();
    #ifdef DDR_FW_DETAIL_MSG
    //printf("\n");
    #endif

    #ifdef DDR_FW_STAGE_MSG
    msg_display(train_result,0x0);
    #else
    if((train_result&0xff)==0x7){
#ifdef CONFIG_DDR_MSG
       printf("PHY0 DDR_INIT_OK\n");
#endif
    }
    else{
       if((train_result&0xff)==0xff){
          printf("PHY0 %s DDR_INIT_ERR\n", train2d?"train2d":"");
          while(1);
	} else {	
       //printf("PHY0 DDR_INIT_STAGE is %x \n",train_result&&0xff);
	}
    }
    #endif

    //Steam MSG
    if((train_result & 0xffff) == 0x8){
        stream_msg[0] = get_mails(); //msg first byte

        for(i=1;i<=(stream_msg[0]&0xffff);i++){
            stream_msg[i] = get_mails();
        }
       //printf("ST_MSG: CODE=%x, ",stream_msg[0]);
       #ifdef DDR_FW_DETAIL_MSG
       st_msg_display(train2d,stream_msg);
       //st_msg_display(train2d,stream_msg[0]);
       #endif
       //for(i=1;i<=(stream_msg[0]&0xffff);i++){
       //     printf("ST_MSG: DATA%d = %x\n",i,stream_msg[i]);
       //}
     }
 }
}


void dwc_ddrphy1_phyinit_userCustom_G_waitFwDone(unsigned char train2d) {

unsigned int train_result;

unsigned int stream_msg[32],i;

train_result = 0x1;
while(((train_result&0xffff)!=0x7) & ((train_result&0xffff)!=0xff))
{
    train_result = get_phy1_mails();
    #ifdef DDR_FW_DETAIL_MSG
    //printf("\n");
    #endif

    #ifdef DDR_FW_STAGE_MSG
    msg_display(train_result,0x1);
    #else
    if((train_result&0xff)==0x7) {
#ifdef CONFIG_DDR_MSG
       printf("PHY1 DDR_INIT_OK\n");
#endif
     }
    else{
       if((train_result&0xff)==0xff) {
          printf("PHY1 %s DDR_INIT_ERR\n", train2d?"train2d":"");
          while(1);
       } else {
          //printf("PHY1 DDR_INIT_STAGE is %x \n",train_result&&0xff);
       }
    }
    #endif

    //Steam MSG
    if((train_result & 0xffff) == 0x8){
        stream_msg[0] = get_phy1_mails(); //msg first byte

        for(i=1;i<=(stream_msg[0]&0xffff);i++){
            stream_msg[i] = get_phy1_mails();
        }
       //printf("ST_MSG: CODE=%x, ",stream_msg[0]);
       #ifdef DDR_FW_DETAIL_MSG
       st_msg_display(train2d,stream_msg);
       //st_msg_display(train2d,stream_msg[0]);
       #endif
       //for(i=1;i<=(stream_msg[0]&0xffff);i++){
       //     printf("ST_MSG: DATA%d = %x\n",i,stream_msg[i]);
       //}
     }
 }
}
