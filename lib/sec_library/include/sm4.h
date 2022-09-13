/*
 * Copyright (C) 2017-2021 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file       drv/sm4.h
 * @brief      Header File for SM4 Driver
 * @version    V2.0
 * @date       9. DEC 2020
 * @model      SM4
 ******************************************************************************/

#ifndef _DRV_SM4_H_
#define _DRV_SM4_H_

#include <stdint.h>
#include <drv/common.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
\brief SM4 Ctrl Block
*/
typedef struct {
        csi_dev_t dev;
        void *    priv;
} csi_sm4_t;

// Function documentation
/**
  \brief       Initialize sm4 Interface. Initializes the resources needed for the sm4 interface
  \param[in]   sm4    operate handle
  \param[in]   idx    device id
  \return      error code \ref uint32_t
*/
csi_error_t csi_sm4_init(csi_sm4_t *sm4, uint32_t idx);

/**
  \brief       De-initialize sm4 Interface. stops operation and releases the software resources used by the interface
  \param[in]   sm4    handle to operate
  \return      None
*/
void csi_sm4_uninit(csi_sm4_t *sm4);

/**
  \brief       Set encrypt key
  \param[in]   sm4        handle to operate
  \param[in]   key        Pointer to the key buf
  \return      error code \ref uint32_t
*/
csi_error_t csi_sm4_set_encrypt_key(csi_sm4_t *sm4, uint8_t *key);

/**
  \brief       Set decrypt key
  \param[in]   sm4        handle to operate
  \param[in]   key        Pointer to the key buf
  \return      error code \ref uint32_t
*/
csi_error_t csi_sm4_set_decrypt_key(csi_sm4_t *sm4, uint8_t *key);

/**
  \brief       sm4 ecb encrypt
  \param[in]   sm4     handle to operate
  \param[in]   in      Pointer to the Source data
  \param[out]  out     Pointer to the Result data
  \param[in]   size    the Source data size
  \return      error code \ref uint32_t
*/
csi_error_t csi_sm4_ecb_encrypt(csi_sm4_t *sm4, uint8_t *in, uint8_t *out,
                                uint32_t size);

/**
  \brief       sm4 ecb decrypt
  \param[in]   sm4     handle to operate
  \param[in]   in      Pointer to the Source data
  \param[out]  out     Pointer to the Result data
  \param[in]   size    the Source data size
  \return      error code \ref uint32_t
*/
csi_error_t csi_sm4_ecb_decrypt(csi_sm4_t *sm4, uint8_t *in, uint8_t *out,
                                uint32_t size);

/**
  \brief       sm4 cbc encrypt
  \param[in]   sm4     handle to operate
  \param[in]   in      Pointer to the Source data
  \param[out]  out     Pointer to the Result data
  \param[in]   size    the Source data size
  \param[in]   iv      init vector
  \return      error code \ref uint32_t
*/
csi_error_t csi_sm4_cbc_encrypt(csi_sm4_t *sm4, uint8_t *in, uint8_t *out,
                                uint32_t size, uint8_t *iv);

/**
  \brief       sm4 cbc decrypt
  \param[in]   sm4     handle to operate
  \param[in]   in      Pointer to the Source data
  \param[out]  out     Pointer to the Result data
  \param[in]   size    the Source data size
  \param[in]   iv      init vector
  \return      error code \ref uint32_t
*/
csi_error_t csi_sm4_cbc_decrypt(csi_sm4_t *sm4, uint8_t *in, uint8_t *out,
                                uint32_t size, uint8_t *iv);

/**
  \brief       sm4 cfb1 encrypt
  \param[in]   sm4     handle to operate
  \param[in]   in      Pointer to the Source data
  \param[out]  out     Pointer to the Result data
  \param[in]   size    the Source data size
  \param[in]   iv      init vector
  \return      error code \ref uint32_t
*/
csi_error_t csi_sm4_cfb1_encrypt(csi_sm4_t *sm4, uint8_t *in, uint8_t *out,
                                 uint32_t size, uint8_t *iv);

/**
  \brief       sm4 cfb1 decrypt
  \param[in]   sm4     handle to operate
  \param[in]   in      Pointer to the Source data
  \param[out]  out     Pointer to the Result data
  \param[in]   size    the Source data size
  \param[in]   iv      init vector
  \return      error code \ref uint32_t
*/
csi_error_t csi_sm4_cfb1_decrypt(csi_sm4_t *sm4, uint8_t *in, uint8_t *out,
                                 uint32_t size, uint8_t *iv);

/**
  \brief       sm4 cfb8 encrypt
  \param[in]   sm4     handle to operate
  \param[in]   in      Pointer to the Source data
  \param[out]  out     Pointer to the Result data
  \param[in]   size    the Source data size
  \param[in]   iv      init vector
  \return      error code \ref uint32_t
*/
csi_error_t csi_sm4_cfb8_encrypt(csi_sm4_t *sm4, uint8_t *in, uint8_t *out,
                                 uint32_t size, uint8_t *iv);

/**
  \brief       sm4 cfb8 decrypt
  \param[in]   sm4     handle to operate
  \param[in]   in      Pointer to the Source data
  \param[out]  out     Pointer to the Result data
  \param[in]   size    the Source data size
  \param[in]   iv      init vector
  \return      error code \ref uint32_t
*/
csi_error_t csi_sm4_cfb8_decrypt(csi_sm4_t *sm4, uint8_t *in, uint8_t *out,
                                 uint32_t size, uint8_t *iv);

/**
  \brief       sm4 cfb128 decrypt
  \param[in]   sm4     handle to operate
  \param[in]   in      Pointer to the Source data
  \param[out]  out     Pointer to the Result data
  \param[in]   size    the Source data size
  \param[in]   iv      init vector
  \param[out]  num     the number of the 128-bit block we have used
  \return      error code \ref uint32_t
*/
csi_error_t csi_sm4_cfb128_decrypt(csi_sm4_t *sm4, uint8_t *in, uint8_t *out,
                                   uint32_t size, uint8_t *iv, uint32_t *num);

/**
  \brief       sm4 cfb128 encrypt
  \param[in]   sm4     handle to operate
  \param[in]   in      Pointer to the Source data
  \param[out]  out     Pointer to the Result data
  \param[in]   size    the Source data size
  \param[in]   iv      init vector
  \param[out]  num     the number of the 128-bit block we have used
  \return      error code \ref uint32_t
*/
csi_error_t csi_sm4_cfb128_encrypt(csi_sm4_t *sm4, uint8_t *in, uint8_t *out,
                                   uint32_t size, uint8_t *iv, uint32_t *num);
/**
  \brief       sm4 ofb encrypt
  \param[in]   sm4     handle to operate
  \param[in]   in      Pointer to the Source data
  \param[out]  out     Pointer to the Result data
  \param[in]   size    the Source data size
  \param[in]   iv      init vector
  \param[out]  num     the number of the 128-bit block we have used
  \return      error code \ref uint32_t
*/
csi_error_t csi_sm4_ofb_encrypt(csi_sm4_t *sm4, uint8_t *in, uint8_t *out,
                                uint32_t size, uint8_t *iv, uint32_t *num);

/**
  \brief       sm4 ofb encrypt
  \param[in]   sm4     handle to operate
  \param[in]   in      Pointer to the Source data
  \param[out]  out     Pointer to the Result data
  \param[in]   size    the Source data size
  \param[in]   iv      init vector
  \param[out]  num     the number of the 128-bit block we have used
  \return      error code \ref uint32_t
*/
csi_error_t csi_sm4_ofb_decrypt(csi_sm4_t *sm4, uint8_t *in, uint8_t *out,
                                uint32_t size, uint8_t *iv, uint32_t *num);

/**
  \brief       sm4 ctr encrypt
  \param[in]   sm4     handle to operate
  \param[in]   in      Pointer to the Source data
  \param[out]  out     Pointer to the Result data
  \param[in]   size    the Source data size
  \param[in]   nonce_counter      counter
  \return      error code \ref uint32_t
*/
csi_error_t csi_sm4_ctr_encrypt(csi_sm4_t *sm4, uint8_t *in, uint8_t *out,
                                uint32_t size, uint8_t nonce_counter[16]);
/**
  \brief       sm4 ctr encrypt
  \param[in]   sm4     handle to operate
  \param[in]   in      Pointer to the Source data
  \param[out]  out     Pointer to the Result data
  \param[in]   size    the Source data size
  \param[in]   nonce_counter      counter
  \return      error code \ref uint32_t
*/
csi_error_t csi_sm4_ctr_decrypt(csi_sm4_t *sm4, uint8_t *in, uint8_t *out,
                                uint32_t size, uint8_t nonce_counter[16]);

/**
  \brief       Enable SM4 power manage
  \param[in]   sm4    Handle to operate
  \return      Error code \ref csi_error_t
*/
csi_error_t csi_sm4_enable_pm(csi_sm4_t *sm4);

/**
  \brief       Disable SM4 power manage
  \param[in]   sm4    Handle to operate
  \return      None
*/
void csi_sm4_disable_pm(csi_sm4_t *sm4);

#ifdef __cplusplus
extern "C" {
#endif

#endif // _DRV_SM4_H_
