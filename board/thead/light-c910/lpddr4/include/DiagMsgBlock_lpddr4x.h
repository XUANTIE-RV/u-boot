typedef struct _PMU_SMB_DIAG_t {

   uint8_t  DiagTestNum;      // Byte offset 0x400, CSR Addr 0x54200, Direction=N/A
    
   uint8_t  DiagSubTest;      // Byte offset 0x401, CSR Addr 0x54200, Direction=N/A
                              
   uint8_t  DiagPrbs;         // Byte offset 0x402, CSR Addr 0x54201, Direction=N/A
                              
   uint8_t  DiagRank;         // Byte offset 0x403, CSR Addr 0x54201, Direction=N/A
                              
   uint8_t  DiagChannel;      // Byte offset 0x404, CSR Addr 0x54202, Direction=N/A
                              
   uint8_t  DiagRepeatCount;  // Byte offset 0x405, CSR Addr 0x54202, Direction=N/A
                                                        
   uint8_t  DiagLoopCount;    // Byte offset 0x406, CSR Addr 0x54203, Direction=N/A
                              
   uint8_t  DiagByte;         // Byte offset 0x407, CSR Addr 0x54203, Direction=N/A
                              
   uint8_t  DiagLane;         // Byte offset 0x408, CSR Addr 0x54204, Direction=N/A
                              
   uint8_t  DiagVrefInc;      // Byte offset 0x409, CSR Addr 0x54204, Direction=N/A
                              
   uint8_t  DiagDoZQ;         // Byte offset 0x40a, CSR Addr 0x54205, Direction=N/A
                              
   uint8_t  DiagXCount;       // Byte offset 0x40b, CSR Addr 0x54205, Direction=N/A

   uint16_t DiagAddrLow;      // Byte offset 0x40c, CSR Addr 0x54206, Direction=N/A

   uint16_t DiagAddrHigh;     // Byte offset 0x40e, CSR Addr 0x54207, Direction=N/A

   uint16_t DiagSeedLow;      // Byte offset 0x410, CSR Addr 0x54208, Direction=N/A

   uint16_t DiagSeedHigh;     // Byte offset 0x412, CSR Addr 0x54209, Direction=N/A

   uint8_t  DiagMisc0;          // Byte offset 0x414, CSR Addr 0x5420a, Direction=N/A

   uint8_t  DiagInfLoop;      // Byte offset 0x415, CSR Addr 0x5420a, Direction=N/A

   uint16_t DiagReturnData;   // Byte offset 0x416, CSR Addr 0x5420b, Direction=N/A

                              
} __attribute__ ((packed)) PMU_SMB_DIAG_t;
