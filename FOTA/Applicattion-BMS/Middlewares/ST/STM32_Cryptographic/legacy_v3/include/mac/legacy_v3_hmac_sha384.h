/**
  *******************************************************************************
  * @file    legacy_v3_hmac_sha384.h
  * @author  MCD Application Team
  * @brief   Header file of HMAC SHA384 helper for migration of cryptographics
  *          library V3 to V4
  *******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  *******************************************************************************
  */

#ifndef LEGACY_V3_HMAC_SHA384_H
#define LEGACY_V3_HMAC_SHA384_H

#include <stdint.h>
#include "mac/legacy_v3_hmac.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
  * @brief  HMAC-SHA384 Context Structure
  */
typedef HMACctx_stt HMAC_SHA384ctx_stt;

/**
  * @brief  Initialize a new HMAC SHA384 context
  * @param[in,out]  *P_pHMAC_SHA384ctx The context that will be initialized
  * @note   The caller must correctly initialize the values of pmKey and mKeySize
  *         inside P_pHMAC_SHA384ctx
  * @retval HASH_SUCCESS Operation Successful
  * @retval HASH_ERR_BAD_PARAMETER Parameter P_pHMAC_SHA384ctx is invalid
  * @retval HASH_ERR_BAD_CONTEXT   Context was not initialized with valid values, see the note below.
  * @note \c P_pHMAC_SHA384ctx.pmKey (see \ref HMAC_SHA384ctx_stt) must be set with a pointer to HMAC key
  *          before calling this function.
  * @note \c P_pHMAC_SHA384ctx.mKeySize (see \ref HMAC_SHA384ctx_stt) must be set with the size of the key
  *          (in bytes) prior to calling this function.
  * @note \c P_pHMAC_SHA384ctx.mFlags must be set prior to calling this function. Default value is E_HASH_DEFAULT.
  *          See \ref HashFlags_et for details.
  * @note \c P_pHMAC_SHA384ctx.mTagSize must be set with the size of the required authentication TAG that will be
  *          generated by the \ref HMAC_SHA384_Finish
  *          Possible values are values are from 1 to CRL_SHA384_SIZE (48)
  */
int32_t HMAC_SHA384_Init(HMAC_SHA384ctx_stt *P_pHMAC_SHA384ctx);

/**
  * @brief  HMAC-SHA384 Update function, process input data and update a HMAC_SHA384ctx_stt
  * @param[in,out]  *P_pHMAC_SHA384ctx The HMAC-SHA384 context that will be updated
  * @param[in]  *P_pInputBuffer The data that will be processed using HMAC-SHA384
  * @param[in]  P_inputSize    P_inputSize   Size of input data, expressed in bytes
  * @retval HASH_SUCCESS Operation Successful
  * @retval HASH_ERR_BAD_PARAMETER  At least one of the parameters is a NULL pointer
  * @retval HASH_ERR_BAD_OPERATION  HMAC_SHA384_Append can't be called after \ref HMAC_SHA384_Finish has been called.
  * @remark This function can be called multiple times with no restrictions on the value of P_inputSize
  */
int32_t HMAC_SHA384_Append(HMAC_SHA384ctx_stt *P_pHMAC_SHA384ctx, const uint8_t *P_pInputBuffer, int32_t P_inputSize);

/**
  * @brief  HMAC-SHA384 Finish function, produce the output HMAC-SHA384 tag
  * @param[in, out]  *P_pHMAC_SHA384ctx   The HMAC-SHA384 context
  * @param[out]  *P_pOutputBuffer  The buffer that will contain the HMAC tag
  * @param[out]  *P_pOutputSize    The size of the data written to P_pOutputBuffer
  * @retval HASH_SUCCESS Operation Successful
  * @retval HASH_ERR_BAD_PARAMETER  At least one of the parameters is a NULL pointer
  * @retval HASH_ERR_BAD_CONTEXT P_pSHA384ctx was not initialized with valid values
  * @note   P_pHMAC_SHA384ctx->mTagSize must contain a valid value, between 1 and CRL_SHA384_SIZE (48)
  */
int32_t HMAC_SHA384_Finish(HMAC_SHA384ctx_stt *P_pHMAC_SHA384ctx, uint8_t *P_pOutputBuffer, int32_t *P_pOutputSize);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* LEGACY_V3_HMAC_SHA384_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
