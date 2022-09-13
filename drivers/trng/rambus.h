/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

#ifndef INC_RAMBUS_H
#define INC_RAMBUS_H

// Driver Framework Basic Defs API
#include "basic_defs.h"

// Driver Framework Device API
#include "device_types.h"
#include "common.h"
#include "rambus_errcode.h"
#include "dmares_types.h"
#include "device_rw.h"
#include "sfxlpka.h"
// #include "csi_core.h"

extern uint64_t g_freq_timer;
extern uint64_t g_freq_ip;
extern uint64_t g_start_ctr;
extern uint64_t g_end_ctr ;
extern uint64_t g_data_len_in_bits;
extern uint32_t g_type;

uint32_t rb_get_random_byte(uint8_t *buf, uint32_t count);

/* 1 bpc, 2 tps, 3 bps */
void rb_perf_init(uint32_t data_len_in_bits, uint32_t type);
void rb_perf_start();
void rb_perf_end();
void rb_perf_get(char *ncase);


#ifdef CONFIG_ALG_PERF_TEST
#define RB_PERF_INIT(bits, type)                                                                   \
    do {                                                                                           \
        if (data_len_in_bits != 0) {     \
                g_data_len_in_bits = data_len_in_bits;  \
        }   \
        if (type != 0) { \
                g_type = type; \
        } \
    } while (0)


#define RB_PERF_START_POINT()                                                                      \
    do {                                                                                           \
        g_start_ctr = ((((uint64_t)csi_coret_get_valueh() << 32U) | \
                        csi_coret_get_value()));                                                                           \
    } while (0)

#define RB_PERF_END_POINT()                                                                        \
    do {                                                                                           \
        g_end_ctr = ((((uint64_t)csi_coret_get_valueh() << 32U) | \
                      csi_coret_get_value()));                                                                          \
    } while (0)

#else
#define RB_PERF_INIT(...)
#define RB_PERF_START_POINT(...)
#define RB_PERF_END_POINT(...)
#endif

/**
 * \brief          rambus crypto init.
 * \return         0 if successful, or error code
 */
uint32_t rambus_crypto_init(void);

void rambus_crypto_uninit(void);

#endif
