/**
  *******************************************************************************
  * @file    legacy_v3_sha224.c
  * @author  MCD Application Team
  * @brief   This file provides SHA-1 functions of helper for
  *          migration of cryptographics library V3 to V4
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

#include "hash/legacy_v3_sha224.h"
#include "err_codes.h"

/* Initialize a new SHA224 context */
int32_t SHA224_Init(SHA224ctx_stt *P_pSHA224ctx)
{
  cmox_hash_retval_t cmox_retval;
  int32_t retval;

  if (P_pSHA224ctx == NULL)
  {
    retval = HASH_ERR_BAD_PARAMETER;
    goto error;
  }
  P_pSHA224ctx->hash_handle = cmox_sha224_construct(&P_pSHA224ctx->sha_handle);
  if (P_pSHA224ctx->hash_handle == NULL)
  {
    retval = HASH_ERR_BAD_PARAMETER;
    goto error;
  }
  cmox_retval = cmox_hash_init(P_pSHA224ctx->hash_handle);
  if (cmox_retval != CMOX_HASH_SUCCESS)
  {
    retval = HASH_ERR_BAD_PARAMETER;
    goto error;
  }

  retval = HASH_SUCCESS;

error:
  return retval;
}

/* SHA224 Update function, process input data and update a SHA224ctx_stt */
int32_t SHA224_Append(SHA224ctx_stt *P_pSHA224ctx, const uint8_t *P_pInputBuffer, int32_t P_inputSize)
{
  cmox_hash_retval_t cmox_retval;
  int32_t retval;

  if ((P_pSHA224ctx == NULL) || (P_pInputBuffer == NULL) || (P_inputSize < 0))
  {
    retval = HASH_ERR_BAD_PARAMETER;
    goto error;
  }

  cmox_retval = cmox_hash_append(P_pSHA224ctx->hash_handle, P_pInputBuffer, (size_t)P_inputSize);

  if (cmox_retval == CMOX_HASH_ERR_BAD_OPERATION)
  {
    retval = HASH_ERR_BAD_OPERATION;
    goto error;
  }

  if (cmox_retval != CMOX_HASH_SUCCESS)
  {
    retval = HASH_ERR_BAD_PARAMETER;
    goto error;
  }

  retval = HASH_SUCCESS;

error:
  return retval;
}

/* SHA224 Finish function, produce the output SHA224 digest */
int32_t SHA224_Finish(SHA224ctx_stt *P_pSHA224ctx, uint8_t *P_pOutputBuffer, int32_t *P_pOutputSize)
{
  cmox_hash_retval_t cmox_retval;
  int32_t retval;
  /* Temporal value for storing the computed output length. This is needed
     because of the possible (e.g. in 64 bits architectures) different dimension
     between int32_t (used in legacy helper) and size_t (used by the library
     for specifying lengths). */
  size_t tempOutputLen;

  if ((P_pSHA224ctx == NULL) || (P_pOutputBuffer == NULL) || (P_pOutputSize == NULL))
  {
    retval = HASH_ERR_BAD_PARAMETER;
    goto error;
  }
  cmox_retval = cmox_hash_setTagLen(P_pSHA224ctx->hash_handle, (size_t)P_pSHA224ctx->mTagSize);

  if (cmox_retval == CMOX_HASH_ERR_BAD_PARAMETER)
  {
    retval = HASH_ERR_BAD_CONTEXT;
    goto error;
  }
  cmox_retval = cmox_hash_generateTag(P_pSHA224ctx->hash_handle, P_pOutputBuffer, &tempOutputLen);
  if (cmox_retval != CMOX_HASH_SUCCESS)
  {
    retval = HASH_ERR_BAD_CONTEXT;
    goto error;
  }

  /* Copy the cast to int32_t of tempOutputLen in order to not overwrite
     other data close to P_pOutputSize. */
  *P_pOutputSize = (int32_t)tempOutputLen;

  cmox_retval = cmox_hash_cleanup(P_pSHA224ctx->hash_handle);
  if (cmox_retval != CMOX_HASH_SUCCESS)
  {
    retval = HASH_ERR_BAD_CONTEXT;
    goto error;
  }

  retval = HASH_SUCCESS;

error:
  return retval;
}
