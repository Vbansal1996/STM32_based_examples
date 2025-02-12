/**
  *******************************************************************************
  * @file    legacy_v3_aes_gcm.h
  * @author  MCD Application Team
  * @brief   Header file of AES GCM helper for migration of cryptographics
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

#ifndef LEGACY_V3_AES_GCM_H
#define LEGACY_V3_AES_GCM_H

#include "cipher/cmox_gcm.h"
#include "legacy_v3_aes.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef struct
{
  cmox_gcmFast_handle_t gcm_handle;
  cmox_cipher_handle_t *cipher_handle;
  uint32_t   mContextId;  /*!< Unique ID of this context. \b Not \b used in current implementation. */
  SKflags_et mFlags;      /*!< 32 bit mFlags, used to perform keyschedule */
  int32_t   mIvSize;      /*!< Size of the Initialization Vector in bytes */
  int32_t   mKeySize;     /*!< Key length in bytes */
  int32_t   mTagSize;     /*!< Tag length in bytes */
  uint8_t   *pmTag;       /*!< Pointer to Authentication TAG. This value must be set in decryption,
                               and this TAG will be verified */
} AESGCMctx_stt; /*!< AES context structure for CBC mode */


/**
  * @brief Initialization for AES GCM Encryption
  * @param[in,out]    *P_pAESGCMctx  AES GCM context
  * @param[in]        *P_pKey        Buffer with the Key
  * @param[in]        *P_pIv         Buffer with the IV
  * @retval    AES_SUCCESS Operation Successful
  * @retval    AES_ERR_BAD_PARAMETER At least one of the parameters is a NULL pointer
  * @retval    AES_ERR_BAD_CONTEXT   Context was not initialized with valid values, see the note below.
  * @note \c P_pAESGCMctx.mKeySize (see \ref AESGCMctx_stt) must be set with the size of the key prior to calling
  *       this function.
  *           Otherwise the following predefined values can be used:
  *         - \ref CRL_AES128_KEY
  *         - \ref CRL_AES192_KEY
  *         - \ref CRL_AES256_KEY
  * @note \c P_pAESGCMctx.mFlags must be set prior to calling this function. Default value is E_SK_DEFAULT.
  *          See \ref SKflags_et for details.
  * @note \c P_pAESGCMctx.mIvSize must be set with the size of the IV (12 is the only supported value) prior to
  *       calling this function.
  * @note \c P_pAESGCMctx.mTagSize must be set with the size of authentication TAG that will be generated by the
  *       \ref AES_GCM_Encrypt_Finish
  * @remark Following recommendation by NIST expressed in section 5.2.1.1 of NIST SP 800-38D, this implementation
  *         supports only IV whose size is of 96 bits.
  */
int32_t AES_GCM_Encrypt_Init(AESGCMctx_stt *P_pAESGCMctx, const uint8_t *P_pKey, const uint8_t *P_pIv);


/**
  * @brief AES GCM Header processing function
  * @param[in,out]    *P_pAESGCMctx     AES GCM, already initialized, context
  * @param[in]    *P_pInputBuffer   Input buffer
  * @param[in]     P_inputSize      Size of input data, expressed in bytes
  * @retval    AES_SUCCESS Operation Successful
  * @retval    AES_ERR_BAD_PARAMETER   At least one of the parameters is a NULL pointer
  * @retval    AES_ERR_BAD_OPERATION   Append not allowed
  * @note This function can be called multiple times, provided that \c P_inputSize is a multiple of 16. \n
  *         A single, final, call with \c P_inputSize not multiple of 16 is allowed.
  */
int32_t AES_GCM_Header_Append(AESGCMctx_stt *P_pAESGCMctx, const uint8_t *P_pInputBuffer, int32_t P_inputSize);

/**
  * @brief AES GCM Encryption function
  * @param[in,out] *P_pAESGCMctx     AES GCM, already initialized, context
  * @param[in]    *P_pInputBuffer   Input buffer
  * @param[in]     P_inputSize      Size of input data, expressed in bytes
  * @param[out]   *P_pOutputBuffer  Output buffer
  * @param[out]   *P_pOutputSize    Pointer to integer that will contain the size of written output data,
  *                                 expressed in bytes
  * @retval    AES_SUCCESS Operation Successful
  * @retval    AES_ERR_BAD_PARAMETER At least one of the parameters is a NULL pointer
  * @retval    AES_ERR_BAD_OPERATION   Append not allowed
  * @note   This function can be called multiple times, provided that \c P_inputSize is a multiple of 16. \n
  *         A single, final, call with \c P_inputSize not multiple of 16 is allowed.
  * @remark This function is just a helper for \ref AESgcmAppend
  */
int32_t AES_GCM_Encrypt_Append(AESGCMctx_stt *P_pAESGCMctx,
                               const uint8_t *P_pInputBuffer,
                               int32_t        P_inputSize,
                               uint8_t       *P_pOutputBuffer,
                               int32_t       *P_pOutputSize);

/**
  * @brief AES GCM Finalization during Encryption, this will create the Authentication TAG
  * @param[in,out] *P_pAESGCMctx     AES GCM, already initialized, context
  * @param[out]    *P_pOutputBuffer  Output Authentication TAG
  * @param[out]    *P_pOutputSize    Size of returned TAG
  * @retval    AES_SUCCESS Operation Successful
  * @retval    AES_ERR_BAD_PARAMETER At least one of the parameters is a NULL pointer
  * @retval    AES_ERR_BAD_CONTEXT   Context was not initialized with valid values, see the note below.
  * @note This function \b requires:  \c P_pAESGCMctx->mTagSize to contain a valid value between 1 and 16.
  */
int32_t AES_GCM_Encrypt_Finish(AESGCMctx_stt *P_pAESGCMctx, uint8_t *P_pOutputBuffer, int32_t *P_pOutputSize);


/**
  * @brief Initialization for AES GCM Decryption
  * @param[in,out]    *P_pAESGCMctx  AES GCM context
  * @param[in]        *P_pKey        Buffer with the Key
  * @param[in]        *P_pIv         Buffer with the IV
  * @retval    AES_SUCCESS Operation Successful
  * @retval    AES_ERR_BAD_PARAMETER At least one of the parameters is a NULL pointer
  * @retval    AES_ERR_BAD_CONTEXT   Context was not initialized with valid values, see the note below.
  * @note \c P_pAESGCMctx.mKeySize (see \ref AESGCMctx_stt) must be set with the size of the key prior to calling
  *       this function.
  *           Otherwise the following predefined values can be used:
  *         - \ref CRL_AES128_KEY
  *         - \ref CRL_AES192_KEY
  *         - \ref CRL_AES256_KEY
  * @note \c P_pAESGCMctx.mFlags must be set prior to calling this function. Default value is E_SK_DEFAULT.
  *          See \ref SKflags_et for details.
  * @note \c P_pAESGCMctx.mIvSize must be set with the size of the IV (12 is the only supported value) prior to
  *       calling this function.
  * @note \c P_pAESGCMctx.pmTag must be set with a pointer to the authentication TAG that will be checked during
  *       \ref AES_GCM_Decrypt_Finish
  * @note \c P_pAESGCMctx.mTagSize must be set with the size of authentication TAG that will be checked during
  *       \ref AES_GCM_Decrypt_Finish
  * @remark Following recommendation by NIST expressed in section 5.2.1.1 of NIST SP 800-38D, this implementation
  *         supports only IV whose size is of 96 bits.
  */
int32_t AES_GCM_Decrypt_Init(AESGCMctx_stt *P_pAESGCMctx, const uint8_t *P_pKey, const uint8_t *P_pIv);

/**
  * @brief AES GCM Decryption function
  * @param[in,out] *P_pAESGCMctx     AES GCM, already initialized, context
  * @param[in]    *P_pInputBuffer   Input buffer
  * @param[in]     P_inputSize      Size of input data, expressed in bytes
  * @param[out]   *P_pOutputBuffer  Output buffer
  * @param[out]   *P_pOutputSize    Pointer to integer that will contain the size of written output data,
  *                                 expressed in bytes
  * @retval    AES_SUCCESS Operation Successful
  * @retval    AES_ERR_BAD_PARAMETER At least one of the parameters is a NULL pointer
  * @retval    AES_ERR_BAD_OPERATION   Append not allowed
  * @note   This function can be called multiple times, provided that \c P_inputSize is a multiple of 16. \n
  *         A single, final, call with \c P_inputSize not multiple of 16 is allowed.
  * @remark  This function is just a helper for \ref AESgcmAppend
  */
int32_t AES_GCM_Decrypt_Append(AESGCMctx_stt *P_pAESGCMctx,
                               const uint8_t *P_pInputBuffer,
                               int32_t        P_inputSize,
                               uint8_t       *P_pOutputBuffer,
                               int32_t       *P_pOutputSize);

/**
  * @brief AES GCM Finalization during Decryption, the authentication TAG will be checked
  * @param[in,out] *P_pAESGCMctx     AES GCM, already initialized, context
  * @param[out]    *P_pOutputBuffer  Kept for API compatibility but won't be used, should be NULL
  * @param[out]    *P_pOutputSize    Kept for API compatibility, must be provided but will be set to zero
  * @returns    Result of TAG verification or Error Code
  * @retval    AES_ERR_BAD_PARAMETER At least one of the parameters is a NULL pointer
  * @retval    AES_ERR_BAD_CONTEXT   Context was not initialized with valid values, see the note below.
  * @retval    AUTHENTICATION_SUCCESSFUL if the TAG is verified
  * @retval    AUTHENTICATION_FAILED if the TAG is \b not verified
  * @note This function \b requires:
  *        - \c P_pAESGCMctx->pmTag to be set to a valid pointer to the tag to be checked
  *        - \c P_pAESGCMctx->mTagSize to contain a valid value between 1 and 16.
  */
int32_t AES_GCM_Decrypt_Finish(AESGCMctx_stt *P_pAESGCMctx, uint8_t *P_pOutputBuffer, int32_t *P_pOutputSize);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* LEGACY_V3_AES_GCM_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
