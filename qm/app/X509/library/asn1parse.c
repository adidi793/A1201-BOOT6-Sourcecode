/*
 *  Generic ASN.1 parsing
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

#include "asn1.h"
#include <string.h>
#include <stdlib.h>
#include "x509_crt.h"
#define mbedtls_calloc    calloc
#define mbedtls_free       free




/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize(void *v, size_t n) {
    volatile unsigned char *p = (unsigned char *)v;
    while (n--) {
        *p++ = 0;
    }
}

/*
 * ASN.1 DER decoding routines
 */
int mbedtls_asn1_get_len(unsigned char **p,
    const unsigned char *end,
    size_t *len) {
    if ((end - *p) < 1) {
        return (MBEDTLS_ERR_ASN1_OUT_OF_DATA);
    }
    
    if ((**p & 0x80) == 0) {  //**p表示证书数据长度字段
        *len = *(*p)++;    //将长度赋值后，再指向证书的下一个数据地址
    } else {
        switch (**p & 0x7F) {
            case 1:
                if ((end - *p) < 2) {  //长度字段的大小不能低于2个字节
                    return (MBEDTLS_ERR_ASN1_OUT_OF_DATA);
                }
                
                *len = (*p)[1];
                (*p) += 2; //+0表示指向本身 +1表示指向本身字段长度之后的下一个段
                break;
            case 2:
                if ((end - *p) < 3) {
                    return (MBEDTLS_ERR_ASN1_OUT_OF_DATA);
                }
                
                *len = ((size_t)(*p)[1] << 8) | (*p)[2];
                (*p) += 3;
                break;
                
            case 3:
                if ((end - *p) < 4) {
                    return (MBEDTLS_ERR_ASN1_OUT_OF_DATA);
                }
                
                *len = ((size_t)(*p)[1] << 16) |
                    ((size_t)(*p)[2] << 8) | (*p)[3];
                (*p) += 4;
                break;
                
            case 4:
                if ((end - *p) < 5) {
                    return (MBEDTLS_ERR_ASN1_OUT_OF_DATA);
                }
                
                *len = ((size_t)(*p)[1] << 24) | ((size_t)(*p)[2] << 16) |
                    ((size_t)(*p)[3] << 8) | (*p)[4];
                (*p) += 5;
                break;
                
            default:
                return (MBEDTLS_ERR_ASN1_INVALID_LENGTH);
        }
    }
    
    if (*len > (size_t)(end - *p)) {
        return (MBEDTLS_ERR_ASN1_OUT_OF_DATA);
    }
    
    return (0);
}

//修改p指向数据，修改len表示数据长度
int mbedtls_asn1_get_tag(unsigned char **p,
    const unsigned char *end,
    size_t *len, int tag) {
    if ((end - *p) < 1) {  //检查证书/证书主体长度
        return (MBEDTLS_ERR_ASN1_OUT_OF_DATA);
    }
    
    if (**p != tag) { //证书都以0x30开头，此条不执行
        return (MBEDTLS_ERR_ASN1_UNEXPECTED_TAG);
    }
    
    (*p)++; //指向长度字段
    
    return (mbedtls_asn1_get_len(p, end, len));
}

int mbedtls_asn1_get_bool(unsigned char **p,
    const unsigned char *end,
    int *val) {
    int ret;
    size_t len;
    
    if ((ret = mbedtls_asn1_get_tag(p, end, &len, MBEDTLS_ASN1_BOOLEAN)) != 0) {
        return (ret);
    }
    
    if (len != 1) {
        return (MBEDTLS_ERR_ASN1_INVALID_LENGTH);
    }
    
    *val = (**p != 0) ? 1 : 0;
    (*p)++;
    
    return (0);
}

int mbedtls_asn1_get_int(unsigned char **p,
    const unsigned char *end,
    int *val) {
    int ret;
    size_t len;
    
    if ((ret = mbedtls_asn1_get_tag(p, end, &len, MBEDTLS_ASN1_INTEGER)) != 0) {
        return (ret);
    }
    
    if (len == 0 || len > sizeof(int) || (**p & 0x80) != 0) {
        return (MBEDTLS_ERR_ASN1_INVALID_LENGTH);
    }
    
    *val = 0;
    
    while (len-- > 0) {
        *val = (*val << 8) | **p;
        (*p)++;
    }
    
    return (0);
}
/* -----------------------------------------------------------------------A12
----------------------------------------------------------------------- */
int mbedtls_asn1_get_publickey(unsigned char **p,
    const unsigned char *end, const unsigned char *crt_start) {
    int ret;
    size_t len;
    int i;
    
    //p: PublicKey.INTEGER.T → PublicKey.INTEGER.V; len=PublicKey.INTEGER.L
    if ((ret = mbedtls_asn1_get_tag(p, end, &len, MBEDTLS_ASN1_INTEGER)) != 0) {
        return (ret);
    }
    
    if ((**p) != 0) {
        return (MBEDTLS_ERR_ASN1_INVALID_DATA);
    }
    if ((len - 1) != 0x100) {
        return (MBEDTLS_ERR_ASN1_LENGTH_MISMATCH);
    }
    
    (*p)++; //实际256公钥地址
    pkey_N_addr_offset = (bl_u16_t)(*p - crt_start);
    len -= 1;
    for (i = 0; i < len; i++) {
        // val[i] = 0;
        // val[i] = *(*p)++; //最后指向E值.T
        (*p)++;
    }
    //p: E值.T → E值.V; len=E值.L
    if ((ret = mbedtls_asn1_get_tag(p, end, &len, MBEDTLS_ASN1_INTEGER)) != 0) {
        return (ret);
    }
    if (len != 3) {
        return (MBEDTLS_ERR_ASN1_LENGTH_MISMATCH);
    }
    pkey_E_addr_offset = (bl_u16_t)(*p - crt_start);
    for (i = 0; i < len; i++) {
        // E_buf[i] = 0;
        // E_buf[i] = (unsigned char) * (*p)++; //最后指向extensions.T
        (*p)++;
    }
    return (0);
}

int mbedtls_asn1_get_bitstring(unsigned char **p, const unsigned char *end,
    mbedtls_asn1_bitstring *bs, const unsigned char *raw_p) {
    int ret;
    size_t len;
    /* Certificate type is a single byte bitstring */
    //p: subjectPublicKey.T(BITSTRING) → 开始 RSA公钥(0x00); bs->len=subjectPublicKey.L
    if ((ret = mbedtls_asn1_get_tag(p, end, &bs->len, MBEDTLS_ASN1_BIT_STRING)) != 0) {
        return (ret);
    }
    
    /* Check length, subtract one for actual bit string length */
    if (bs->len < 1) {
        return (MBEDTLS_ERR_ASN1_OUT_OF_DATA);
    }
    bs->len -= 1;
    
    /* Get number of unused bits, ensure unused bits <= 7 */
    bs->unused_bits = **p;
    if (bs->unused_bits > 7) {
        return (MBEDTLS_ERR_ASN1_INVALID_LENGTH);
    }
    (*p)++; //p: 0x00 → PublicKey.T
    bs->p = *p;
    /* -----------------------------------------------------------------------A12
    p: PublicKey.SEQUENCE.T → PublicKey.INTEGER.T, len=PublicKey.SEQUENCE.L
    ----------------------------------------------------------------------- */
    if ((ret = mbedtls_asn1_get_tag(p, end, &len,
                    MBEDTLS_ASN1_CONSTRUCTED | MBEDTLS_ASN1_SEQUENCE)) != 0) {
        return (MBEDTLS_ERR_PK_KEY_INVALID_FORMAT + ret);
    }
    /* -----------------------------------------------------------------------A12
    p: INTEGER.T → extensions.T
    ----------------------------------------------------------------------- */
    if ((ret = mbedtls_asn1_get_publickey(p, end, raw_p)) != 0) {
        return (MBEDTLS_ERR_X509_INVALID_VERSION + ret);
    }
    // /* Get actual bitstring */
    // *p += bs->len;
    
    if (*p != end) { //end是extensions.T, 如果没有上面两个判断，p是PublicKey.T
        return (MBEDTLS_ERR_ASN1_LENGTH_MISMATCH);
    }
    
    return (0);
}

/*
 * Get a bit string without unused bits
 */
int mbedtls_asn1_get_bitstring_null(unsigned char **p, const unsigned char *end,
    size_t *len) {
    int ret;
    //p: signatureValue.T → signatureValue.V; len=0x101
    if ((ret = mbedtls_asn1_get_tag(p, end, len, MBEDTLS_ASN1_BIT_STRING)) != 0) {
        return (ret);
    }
    
    if ((*len)-- < 2 || *(*p)++ != 0) { //判断签名长度是否为1或0，签名开始符是否不为0
        return (MBEDTLS_ERR_ASN1_INVALID_DATA);
    }
    
    return (0);
}



/*
 *  Parses and splits an ASN.1 "SEQUENCE OF <tag>"
 */
int mbedtls_asn1_get_sequence_of(unsigned char **p,
    const unsigned char *end,
    mbedtls_asn1_sequence *cur,
    int tag) {
    int ret;
    size_t len;
    mbedtls_asn1_buf *buf;
    
    /* Get main sequence tag */
    if ((ret = mbedtls_asn1_get_tag(p, end, &len,
                    MBEDTLS_ASN1_CONSTRUCTED | MBEDTLS_ASN1_SEQUENCE)) != 0) {
        return (ret);
    }
    
    if (*p + len != end) {
        return (MBEDTLS_ERR_ASN1_LENGTH_MISMATCH);
    }
    
    while (*p < end) {
        buf = &(cur->buf);
        buf->tag = **p;
        
        if ((ret = mbedtls_asn1_get_tag(p, end, &buf->len, tag)) != 0) {
            return (ret);
        }
        
        buf->p = *p;
        *p += buf->len;
        
        /* Allocate and assign next pointer */
        if (*p < end) {
            cur->next = (mbedtls_asn1_sequence *)mbedtls_calloc(1,
                    sizeof(mbedtls_asn1_sequence));
                    
            if (cur->next == NULL) {
                return (MBEDTLS_ERR_ASN1_ALLOC_FAILED);
            }
            
            cur = cur->next;
        }
    }
    
    /* Set final sequence entry's next pointer to NULL */
    cur->next = NULL;
    
    if (*p != end) {
        return (MBEDTLS_ERR_ASN1_LENGTH_MISMATCH);
    }
    
    return (0);
}

int mbedtls_asn1_get_alg(unsigned char **p,
    const unsigned char *end,
    mbedtls_asn1_buf *alg, mbedtls_asn1_buf *params) {
    int ret;
    size_t len;
    //p改为指向SEQUENCE类型，len改为算法数据长度
    //p: AlgorithmIdentifier.T(SEQUENCE) → algorithm.T(OID); len=AlgorithmIdentifier.L
    //p: signatureAlgorithm.T → algorithm.T
    if ((ret = mbedtls_asn1_get_tag(p, end, &len,
                    MBEDTLS_ASN1_CONSTRUCTED | MBEDTLS_ASN1_SEQUENCE)) != 0) {
        return (ret);
    }
    
    if ((end - *p) < 1) {
        return (MBEDTLS_ERR_ASN1_OUT_OF_DATA);
    }
    
    alg->tag = **p;
    end = *p + len; //end改为指向下一个SEQUENCE类型 (2)subjectPublicKey.T(BITSTRING) (3)signatureValue.T
    //p改为指向Object Identifier类型，
    //p: algorithm.T(OID) → algorithm.V
    //p: algorithm.T(OID) → algorithm.V (签名算法)
    if ((ret = mbedtls_asn1_get_tag(p, end, &alg->len, MBEDTLS_ASN1_OID)) != 0) {
        return (ret);
    }
    
    alg->p = *p;
    *p += alg->len; //指向NULL
    
    if (*p == end) {
        mbedtls_zeroize(params, sizeof(mbedtls_asn1_buf));
        return (0);
    }
    
    params->tag = **p; //null.T
    (*p)++; //null.L
    //params->len长度为0，p改为指向下一个SEQUENCE类型
    //p: algorithm.V → subjectPublicKey.T(BITSTRING)
    //处理签名算法，p: null.L → signatureValue.T； params->len=0
    if ((ret = mbedtls_asn1_get_len(p, end, &params->len)) != 0) {
        return (ret);
    }
    
    params->p = *p;
    *p += params->len; //长度为0//p: subjectPublicKey.T(BITSTRING) → subjectPublicKey.T(BITSTRING)
    
    if (*p != end) {
        return (MBEDTLS_ERR_ASN1_LENGTH_MISMATCH);
    }
    
    return (0);
}

int mbedtls_asn1_get_alg_null(unsigned char **p,
    const unsigned char *end,
    mbedtls_asn1_buf *alg) {
    int ret;
    mbedtls_asn1_buf params;
    
    memset(&params, 0, sizeof(mbedtls_asn1_buf));
    
    if ((ret = mbedtls_asn1_get_alg(p, end, alg, &params)) != 0) {
        return (ret);
    }
    
    if ((params.tag != MBEDTLS_ASN1_NULL && params.tag != 0) || params.len != 0) {
        return (MBEDTLS_ERR_ASN1_INVALID_DATA);
    }
    
    return (0);
}

void mbedtls_asn1_free_named_data(mbedtls_asn1_named_data *cur) {
    if (cur == NULL) {
        return;
    }
    
    mbedtls_free(cur->oid.p);
    mbedtls_free(cur->val.p);
    
    mbedtls_zeroize(cur, sizeof(mbedtls_asn1_named_data));
}

void mbedtls_asn1_free_named_data_list(mbedtls_asn1_named_data **head) {
    mbedtls_asn1_named_data *cur;
    
    while ((cur = *head) != NULL) {
        *head = cur->next;
        mbedtls_asn1_free_named_data(cur);
        mbedtls_free(cur);
    }
}

mbedtls_asn1_named_data *mbedtls_asn1_find_named_data(mbedtls_asn1_named_data *list,
    const char *oid, size_t len) {
    while (list != NULL) {
        if (list->oid.len == len &&
            memcmp(list->oid.p, oid, len) == 0) {
            break;
        }
        
        list = list->next;
    }
    
    return (list);
}
