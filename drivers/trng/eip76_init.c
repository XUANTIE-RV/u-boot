/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/* eip76_init.c
 *
 * Module implements the EIP76 Driver Library Initialization API.
 */

/*----------------------------------------------------------------------------
 * This module implements (provides) the following interface(s):
 */

// EIP76 initialization API
#include "eip76_init.h"

/*----------------------------------------------------------------------------
 * This module uses (requires) the following interface(s):
 */

// Default configuration
#include "c_eip76.h"

// Driver Framework Basic Definitions API
#include "basic_defs.h" // uint32_t

// Driver Framework Device API
#include "device_types.h" // Device_Handle_t

// EIP-76 Driver Library Types API
#include "eip76_types.h" // EIP76_* types

// EIP-76 Driver Library Internal interfaces
#include "eip76_level0.h"      // Level 0 macros
#include "eip76_fsm.h"         // State machine
#include "eip76_internal.h"    // Internal macros
#include "eip76_internal_pp.h" // Internal Post Processor interface
#include "basic_defs.h"


extern uint32_t g_rb_trng_request_block;

extern uint32_t g_rb_trng_auto_detune;
/*----------------------------------------------------------------------------
 * Definitions and macros
 */

/*----------------------------------------------------------------------------
 * EIP76Lib_Detect
 *
 * Checks the presence of EIP76 hardware. Returns true when found.
 */
static bool
EIP76Lib_Detect(
    const Device_Handle_t Device)
{
    uint32_t Value;

    // read and check the revision register
    Value = EIP76_Read32(Device, EIP76_REG_EIP_REV);
    if (!EIP76_REV_SIGNATURE_MATCH(Value))
    {
        return false;
    }

    // Make sure the engine of the device is in the startup state.

    LOG_INFO("1 %s, %d\n", __FUNCTION__, __LINE__);
    EIP76_CONTROL_WR(Device, 0);
    EIP76_CONTROL_WR(Device, 0);

    // Set all defined bits of the EIP76_REG_ALARMCNT register
    EIP76_Write32(Device, EIP76_REG_ALARMCNT, MASK_8_BITS);

    return true;
}

/*----------------------------------------------------------------------------
 * EIP76_HWRevision_Get
 *
 */
static EIP76_Status_t
EIP76Lib_HWRevision_Get(
    const Device_Handle_t Device,
    EIP76_Capabilities_t *const Capabilities_p)
{
    EIP76_EIP_REV_RD(Device,
                     &Capabilities_p->EipNumber,
                     &Capabilities_p->ComplmtEipNumber,
                     &Capabilities_p->HWPatchLevel,
                     &Capabilities_p->MinHWRevision,
                     &Capabilities_p->MajHWRevision);

    EIP76_OPTIONS_RD(Device,
                     &Capabilities_p->PostProcessorType,
                     &Capabilities_p->NofFRO,
                     &Capabilities_p->ConditioningFunction,
                     &Capabilities_p->BufferSize,
                     &Capabilities_p->AIS31Testing,
                     &Capabilities_p->AutoDetune,
                     &Capabilities_p->Fips90B2,
                     &Capabilities_p->ImprovedFRO);

    return EIP76_NO_ERROR;
}

extern uint32_t g_rb_trng_ctrl;


/* EIP76_ALARM_THRESHOLD */
extern uint32_t g_alarm_threshold;

/* EIP76_SHUTDOWN_THRESHOLD */
extern uint32_t g_shutdown_threshold;
/*----------------------------------------------------------------------------
 * EIP76_Initialize
 *
 */
EIP76_Status_t
EIP76_Initialize(
    EIP76_IOArea_t *const IOArea_p,
    const Device_Handle_t Device,
    const uint32_t *PS_Data_p,
    const unsigned int PS_WordCount)
{

    EIP76_Capabilities_t Capabilities;
    uint32_t ControlWord = 0;
    EIP76_Status_t rv = EIP76_NO_ERROR;
    volatile EIP76_True_IOArea_t *const TrueIOArea_p = IOAREA(IOArea_p);

    EIP76_CHECK_POINTER(IOArea_p);
    EIP76_CHECK_POINTER(PS_Data_p);

    EIP76_CHECK_INT_INRANGE(PS_WordCount,
                            EIP76_MIN_PS_AI_WORD_COUNT,
                            EIP76_MAX_PS_AI_WORD_COUNT);

    //Detect presence of EIP-76 hardware
    if (!EIP76Lib_Detect(Device))
        return EIP76_UNSUPPORTED_FEATURE_ERROR;

    // Initialize the IO Area
    TrueIOArea_p->Device = Device;
    TrueIOArea_p->SavedControl = 0;
    TrueIOArea_p->PRM_WordCount = 0;
    TrueIOArea_p->State = (uint32_t)EIP76_STATE_RESET;

    // Transit to a new state
    rv = EIP76_State_Set((volatile EIP76_State_t *)&TrueIOArea_p->State,
                         EIP76_STATE_RESET);
    if (rv != EIP76_NO_ERROR)
        return EIP76_ILLEGAL_IN_STATE;

    rv = EIP76Lib_HWRevision_Get(Device, &Capabilities);

    if (rv != EIP76_NO_ERROR)
        return EIP76_UNSUPPORTED_FEATURE_ERROR;

    // Check if configured Post Processor is supported
    if (Capabilities.PostProcessorType != EIP76_POST_PROCESSOR_TYPE)
    {
        return EIP76_UNSUPPORTED_FEATURE_ERROR;
    }

    // Check if all the configured FRO's that must be enabled are supported
    {
        uint32_t Value = ((uint32_t)(1 << Capabilities.NofFRO) - 1);
        uint32_t Value2;
        if (Capabilities.ImprovedFRO)
        {
            Value2 = (uint32_t)EIP76_IMPFRO_ENABLED_MASK;
        }
        else
        {
            Value2 = (uint32_t)EIP76_FRO_ENABLED_MASK;
        }

        if (Value < Value2)
        {
            return EIP76_UNSUPPORTED_FEATURE_ERROR;
        }
        Value = (uint32_t)EIP76_SHUTDOWN_THRESHOLD;
        Value2 = (uint32_t)Capabilities.NofFRO;

        if (Value > Value2)
        {
            return EIP76_UNSUPPORTED_FEATURE_ERROR;
        }
    }

    // // Entropy generation configuration
    // EIP76_CONFIG_WR(Device,
    //                 EIP76_NOISE_BLOCKS,
    //                 EIP76_SAMPLE_DIVIDER,
    //                 EIP76_READ_TIMEOUT,
    //                 EIP76_SAMPLE_CYCLES,
    //                 EIP76_SAMPLE_SCALE);
    //FOR sampling
    EIP76_Write32(Device, EIP76_REG_CONFIG, g_rb_trng_ctrl);

    // Configure alarm thresholds
    EIP76_ALARMCNT_WR_CONFIG(Device,
                             g_alarm_threshold,
                             false, // Stall Run Poker test - no
                             g_shutdown_threshold,
                             EIP76_SHUTDOWN_FATAL);

    // Initialize FRO de-tune for all the configured FRO's,
    // set the default value,
    // this must be done before the FRO's are enabled
    EIP76_FRODETUNE_WR(Device, 0);

#ifdef EIP76_AUTO_DETUNE_OPTION
    if (g_rb_trng_auto_detune == 1) {
        // See if auto-detune option is supported by the hardware
        if (!Capabilities.AutoDetune)
        {
            return EIP76_UNSUPPORTED_FEATURE_ERROR;
        }
        // Enable auto-detune feature
        // ControlWord |= EIP76_AUTO_DETUNE_ENABLE;

        // Clear auto-detune counts
        /* NOTE R1 */
        EIP76_AUTODETUNE_CNT_CLR(Device);
        uint32_t Value_Auto_Detune = EIP76_Read32(Device, EIP76_REG_OPTION);
        Value_Auto_Detune |= EIP76_AUTO_DETUNE_ENABLE;
        EIP76_OPTIONS_WR(Device, Value_Auto_Detune);
    }

#endif // EIP76_AUTO_DETUNE_OPTION

    // #ifdef EIP76_FIPS_80090B2_SUPPORT
    //     // See if no-whitening option is supported by the hardware
    //     if (!Capabilities.Fips90B2)
    //     {
    //         return EIP76_UNSUPPORTED_FEATURE_ERROR;
    //     }
    //     // Enable auto-detune feature
    //     ControlWord |= EIP76_CONTROL_NO_WHITENING;

    // #endif

    // Clear alarm mask
    EIP76_ALARMMASK_WR(Device, 0);

    // Clear alarm stop mask
    EIP76_ALARMSTOP_WR(Device, 0);
    LOG_INFO("2 %s, %d\n", __FUNCTION__, __LINE__);

    // Enable the configured FRO's
    EIP76_FROENABLE_WR(Device, EIP76_FRO_ENABLED_MASK);

    LOG_INFO("3 %s, %d\n", __FUNCTION__, __LINE__);

    // Enable Post Processor if required
    switch (EIP76_POST_PROCESSOR_TYPE)
    {
    case EIP76_POST_PROCESSOR_BC_DF:
    case EIP76_POST_PROCESSOR_SP800_90:
        // Transit to a new state
        rv = EIP76_State_Set((volatile EIP76_State_t *)&TrueIOArea_p->State,
                             EIP76_STATE_RESET_BCDF);
        if (rv != EIP76_NO_ERROR)
            return EIP76_ILLEGAL_IN_STATE;
        else
            // Set return specific for bc_df.
            rv = EIP76_BUSY_RETRY_LATER;
        // Start the EIP-76 DRBG
        ControlWord |= EIP76_CONTROL_POSTPROCESSOR_ENABLE;

        // Clear all pending events if any
        EIP76_INTACK_WR(Device, EIP76_INTACK_CLEAR_ALL_MASK);
        //mask
        ControlWord |= 0xFF;
        //stuck
        ControlWord |= ((uint32_t)(BIT_9));
        //repcnt fail
        ControlWord |= ((uint32_t)(BIT_13));
        //aprop fail
        ControlWord |= ((uint32_t)(BIT_14));

        // Start the EIP-76 TRNG
        ControlWord |= EIP76_CONTROL_ENABLE_TRNG;
        break;

    case EIP76_POST_PROCESSOR_NONE:

        // // Make sure the Post Processor is in the startup state
        // EIP76_Write32(Device, EIP76_REG_PS_AI_11, 0);

        // // Clear all pending events if any
        // EIP76_INTACK_WR(Device, EIP76_INTACK_CLEAR_ALL_MASK);

        // // Start the EIP-76 TRNG
        // ControlWord |= EIP76_CONTROL_ENABLE_TRNG;
        //LOG_INFO("4 %s, %d\n", __FUNCTION__, __LINE__);
        break;
    }
    // Write Control register
    //EIP76_CONTROL_WR(Device, ControlWord);
    EIP76_CONTROL_WR(Device, 0x1400);
    //EIP76_CONTROL_WR(Device, g_rb_trng_ctrl);

    // #if (EIP76_POST_PROCESSOR_TYPE == EIP76_POST_PROCESSOR_BC_DF)
    IDENTIFIER_NOT_USED(PS_Data_p);
    IDENTIFIER_NOT_USED(PS_WordCount);
    // #endif
    unsigned int LoopCounter;
    EIP76_EventStatus_t events = 0;
    if (rv == EIP76_BUSY_RETRY_LATER)
    {
        LoopCounter = 0;
        do
        {
            Adapter_SleepMS(ADAPTER_HRNG_RESEED_BUSYWAIT_SLEEP_MS);

            rv = EIP76_Initialize_IsReady(IOArea_p,
                                          PS_Data_p,
                                          PS_WordCount,
                                          &events);
            if (events & EIP76_FATAL_EVENTS)
            {
                // Fatal errors
                LOG_CRIT("Adapter_EIP76_Event_Handler: "
                         "EIP76 Fatal error!\n");
                return false;
            }
            LoopCounter++;
            if (LoopCounter > ADAPTER_HRNG_RESEED_BUSYWAIT_MAXLOOP)
                return false;

        } while (rv == EIP76_BUSY_RETRY_LATER);

        if (rv != EIP76_NO_ERROR)
        {
            LOG_CRIT("Adapter_HRNG_Init: "
                     "EIP76_Initialize_IsReady returned error: %d\n",
                     rv);
            return false;
        }
    }
    return rv;
}

#if (EIP76_POST_PROCESSOR_TYPE == EIP76_POST_PROCESSOR_BC_DF)
/*----------------------------------------------------------------------------
 * EIP76_Initialize_IsReady
 *
 */
EIP76_Status_t
EIP76_Initialize_IsReady(
    EIP76_IOArea_t *const IOArea_p,
    const uint32_t *PS_Nonce_Data_p,
    const unsigned int PS_Nonce_WordCount,
    EIP76_EventStatus_t *const Events_p)
{
    Device_Handle_t Device;
    uint32_t StatusRegVal;
    volatile EIP76_True_IOArea_t *const TrueIOArea_p = IOAREA(IOArea_p);

    // No events detected yet
    *Events_p = 0;

    EIP76_CHECK_POINTER(IOArea_p);
    EIP76_CHECK_POINTER(Events_p);

    // EIP76_CHECK_INT_INRANGE(PS_WordCount,
    //                         EIP76_MAX_PS_AI_WORD_COUNT,
    //                         EIP76_MAX_PS_AI_WORD_COUNT);

    Device = TrueIOArea_p->Device;

    StatusRegVal = EIP76_STATUS_RD(Device);

    // Store event status
    *Events_p = (StatusRegVal & EIP76_EVENTS_MASK);

    if ((StatusRegVal & EIP76_STATUS_RESEED_AI) != 0)
    {
        EIP76_Internal_PostProcessor_PS_AI_Write(Device,
                                                 PS_Nonce_Data_p,
                                                 PS_Nonce_WordCount);

        return EIP76_State_Set((volatile EIP76_State_t *const) & TrueIOArea_p->State,
                               EIP76_STATE_RANDOM_GENERATING);
    }
    else
    {
        // reseed_ai bit is not active
        return EIP76_BUSY_RETRY_LATER;
    }
}
#endif

/*----------------------------------------------------------------------------
 * EIP76_Shutdown
 *
 */
EIP76_Status_t
EIP76_Shutdown(
    EIP76_IOArea_t *const IOArea_p)
{
    Device_Handle_t Device;
    EIP76_Status_t rv;
    volatile EIP76_True_IOArea_t *const TrueIOArea_p = IOAREA(IOArea_p);

    EIP76_CHECK_POINTER(IOArea_p);

    // Transit to a new state
    rv = EIP76_State_Set((volatile EIP76_State_t *)&TrueIOArea_p->State,
                         EIP76_STATE_RESET);

    if (rv != EIP76_NO_ERROR)
        return EIP76_ILLEGAL_IN_STATE;

    Device = TrueIOArea_p->Device;

    // Stop the EIP-76 TRNG
    EIP76_CONTROL_WR(Device, 0);

    return EIP76_NO_ERROR;
}

/*----------------------------------------------------------------------------
 * EIP76_HWRevision_Get
 *
 */
EIP76_Status_t
EIP76_HWRevision_Get(
    EIP76_IOArea_t *const IOArea_p,
    EIP76_Capabilities_t *const Capabilities_p)
{
    Device_Handle_t Device;
    volatile EIP76_True_IOArea_t *const TrueIOArea_p = IOAREA(IOArea_p);

    EIP76_CHECK_POINTER(IOArea_p);
    EIP76_CHECK_POINTER(Capabilities_p);

    Device = TrueIOArea_p->Device;

    return EIP76Lib_HWRevision_Get(Device,
                                   Capabilities_p);
}

/* end of file eip76_init.c */



uint32_t trng_init_entropy()
{
        EIP76_Write32(NULL, EIP76_REG_CONTROL, 0x00000000);
        EIP76_Write32(NULL, EIP76_REG_CONTROL, 0x00000000);
        EIP76_Write32(NULL, EIP76_REG_ALARMSTOP, 0x00000000);
        EIP76_Write32(NULL, EIP76_REG_ALARMMASK, 0x00000000);
        EIP76_Write32(NULL, EIP76_REG_FROENABLE, 0x00ffffff);
        EIP76_Write32(NULL, EIP76_REG_FRODETUNE, 0x00000000);
        EIP76_Write32(NULL, EIP76_REG_STATUS, 0x000063ff);
        EIP76_Write32(NULL, EIP76_REG_CONFIG, g_rb_trng_ctrl);
        EIP76_Write32(NULL, EIP76_REG_CONTROL, 0x00000100);
        EIP76_Write32(NULL, EIP76_REG_TEST, 0x00002000);

        return 0;
}

extern void
trng_hexdump(
        const char * szPrefix_p,
        const unsigned int PrintOffset,
        const uint8_t * Buffer_p,
        const unsigned int ByteCount);
int rb_trng_sys_test_entropy()
{
        uint32_t data1;
        uint32_t data2;
        printf("===%s, %d\n", __FUNCTION__, __LINE__);
        printf("===config %s, %x\n", __FUNCTION__, g_rb_trng_ctrl);
        printf("===r block of 64 Bytes%s, %d\n", __FUNCTION__,
               g_rb_trng_request_block);
        uint32_t ret = trng_init_entropy();
        if (ret != 0) {
                return ret;
        }

        for (int i = 0; i < g_rb_trng_request_block; i++) {
                while ((EIP76_Read32(NULL, EIP76_REG_STATUS) & 0x00000100) !=
                       0x00000100)
                        ;
                data1 = EIP76_Read32(NULL, EIP76_REG_RAW_H);
                data2 = EIP76_Read32(NULL, EIP76_REG_RAW_L);
                EIP76_Write32(NULL, EIP76_REG_STATUS, 0x000063ff);
                memcpy(0x80000000 + (i * 8), &data1, 4);
                memcpy(0x80000004 + (i * 8), &data2, 4);
                // printf("===data: %04x, %04x\n", data1, data2);
        }
                // trng_hexdump("mem", 0, (void*)(0x80000000), 8 * g_rb_trng_request_block);
      printf("raw test pass\n");
        return 0;
}

