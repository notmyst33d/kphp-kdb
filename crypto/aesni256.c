/*
    This file is part of VK/KittenPHP-DB-Engine Library.

    VK/KittenPHP-DB-Engine Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    VK/KittenPHP-DB-Engine Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with VK/KittenPHP-DB-Engine Library.  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2012-2013 Vkontakte Ltd
              2012-2013 Anton Maydell
*/

#include "crypto/aesni256.h"
#include <stdint.h>
#include "server-functions.h"

static void vk_ssl_aes_cbc_encrypt (vk_aes_ctx_t *ctx, const unsigned char *in, unsigned char *out, int size, unsigned char iv[16]){
  AES_cbc_encrypt (in, out, size, &ctx->u.key, iv, AES_ENCRYPT);
}

static void vk_ssl_aes_cbc_decrypt (vk_aes_ctx_t *ctx, const unsigned char *in, unsigned char *out, int size, unsigned char iv[16]){
  AES_cbc_encrypt (in, out, size, &ctx->u.key, iv, AES_DECRYPT);
}

static void vk_ssl_aes_ige_encrypt (vk_aes_ctx_t *ctx, const unsigned char *in, unsigned char *out, int size, unsigned char iv[32]){
  AES_ige_encrypt (in, out, size, &ctx->u.key, iv, AES_ENCRYPT);
}

static void vk_ssl_aes_ige_decrypt (vk_aes_ctx_t *ctx, const unsigned char *in, unsigned char *out, int size, unsigned char iv[32]){
  AES_ige_encrypt (in, out, size, &ctx->u.key, iv, AES_DECRYPT);
}

void vk_ssl_aes_ctr_crypt (vk_aes_ctx_t *ctx, const unsigned char *in, unsigned char *out, int size, unsigned char iv[16], unsigned long long offset) {
  unsigned long long *p = (unsigned long long *) (iv + 8);
  const unsigned long long old_ctr_value = *p;
  (*p) += offset >> 4;
  union {
    unsigned char c[16];
    unsigned long long d[2];
  } u;
  int i = offset & 15, l;
  if (i) {
    AES_encrypt (iv, u.c, &ctx->u.key);
    (*p)++;
    l = i + size;
    if (l > 16) {
      l = 16;
    }
    size -= l - i;
    do {
      *out++ = (*in++) ^ u.c[i++];
    } while (i < l);
  }
  const unsigned long long *I = (const unsigned long long *) in;
  unsigned long long *O = (unsigned long long *) out;
  int n = size >> 4;
  while (--n >= 0) {
    AES_encrypt (iv, (unsigned char *) u.d, &ctx->u.key);
    (*p)++;
    *O++ = (*I++) ^ u.d[0];
    *O++ = (*I++) ^ u.d[1];
  }
  in = (const unsigned char *) I;
  out = (unsigned char *) O;
  l = size & 15;
  if (l) {
    AES_encrypt (iv, u.c, &ctx->u.key);
    i = 0;
    do {
      *out++ = (*in++) ^ u.c[i++];
    } while (i < l);
  }
  *p = old_ctr_value;
}

void vk_aes_set_encrypt_key (vk_aes_ctx_t *ctx, unsigned char *key, int bits) {
  AES_set_encrypt_key (key, bits, &ctx->u.key);
  ctx->cbc_crypt = vk_ssl_aes_cbc_encrypt;
  ctx->ige_crypt = vk_ssl_aes_ige_encrypt;
  ctx->ctr_crypt = vk_ssl_aes_ctr_crypt;
}

void vk_aes_set_decrypt_key (vk_aes_ctx_t *ctx, unsigned char *key, int bits) {
  AES_set_decrypt_key (key, bits, &ctx->u.key);
  ctx->cbc_crypt = vk_ssl_aes_cbc_decrypt;
  ctx->ige_crypt = vk_ssl_aes_ige_decrypt;
  ctx->ctr_crypt = NULL; /* NOTICE: vk_aes_set_encrypt_key should be used for CTR decryption */
}
