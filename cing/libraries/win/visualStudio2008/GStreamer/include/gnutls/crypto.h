/*
 * Copyright (C) 2008, 2009 Free Software Foundation
 *
 * Author: Nikos Mavrogiannopoulos
 *
 * This file is part of GNUTLS.
 *
 * The GNUTLS library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
 * USA
 *
 */

#ifndef GNUTLS_CRYPTO_H
# define GNUTLS_CRYPTO_H

# define GNUTLS_CRYPTO_API_VERSION 0x01

typedef struct
{
  int (*init) (void **ctx);
  int (*setkey) (void *ctx, const void *key, size_t keysize);
  int (*setiv) (void *ctx, const void *iv, size_t ivsize);
  int (*encrypt) (void *ctx, const void *plain, size_t plainsize,
		  void *encr, size_t encrsize);
  int (*decrypt) (void *ctx, const void *encr, size_t encrsize,
		  void *plain, size_t plainsize);
  void (*deinit) (void *ctx);
} gnutls_crypto_single_cipher_st;

typedef struct
{
  int (*init) (void **ctx);
  int (*setkey) (void *ctx, const void *key, size_t keysize);
  int (*hash) (void *ctx, const void *text, size_t textsize);
  int (*copy) (void **dst_ctx, void *src_ctx);
  int (*output) (void *src_ctx, void *digest, size_t digestsize);
  void (*deinit) (void *ctx);
} gnutls_crypto_single_mac_st;

typedef struct
{
  int (*init) (gnutls_cipher_algorithm_t, void **ctx);
  int (*setkey) (void *ctx, const void *key, size_t keysize);
  int (*setiv) (void *ctx, const void *iv, size_t ivsize);
  int (*encrypt) (void *ctx, const void *plain, size_t plainsize,
		  void *encr, size_t encrsize);
  int (*decrypt) (void *ctx, const void *encr, size_t encrsize,
		  void *plain, size_t plainsize);
  void (*deinit) (void *ctx);
} gnutls_crypto_cipher_st;

typedef struct
{
  int (*init) (gnutls_mac_algorithm_t, void **ctx);
  int (*setkey) (void *ctx, const void *key, size_t keysize);
  int (*hash) (void *ctx, const void *text, size_t textsize);
  int (*copy) (void **dst_ctx, void *src_ctx);
  int (*output) (void *src_ctx, void *digest, size_t digestsize);
  void (*deinit) (void *ctx);
} gnutls_crypto_mac_st;

/* the same... setkey should be null */
typedef gnutls_crypto_single_mac_st gnutls_crypto_single_digest_st;
typedef gnutls_crypto_mac_st gnutls_crypto_digest_st;

typedef enum gnutls_rnd_level
{
  /* fatal in parts of session if broken, i.e., vulnerable to
     statistical analysis */
  GNUTLS_RND_NONCE = 0,
  /* fatal in session if broken */
  GNUTLS_RND_RANDOM = 1,
  /* fatal in many sessions if broken */
  GNUTLS_RND_KEY = 2
} gnutls_rnd_level_t;

typedef enum
{
  GNUTLS_PK_FLAG_NONE = 0
} gnutls_pk_flag_t;

typedef struct gnutls_crypto_rnd
{
  int (*init) (void **ctx);
  int (*rnd) (void *ctx, int /* gnutls_rnd_level_t */ level,
	      void *data, size_t datasize);
  void (*deinit) (void *ctx);
} gnutls_crypto_rnd_st;

typedef void *bigint_t;

typedef enum
{
  /* raw unsigned integer format */
  GNUTLS_MPI_FORMAT_USG = 0,
  /* raw signed integer format - always a leading zero when positive */
  GNUTLS_MPI_FORMAT_STD = 1,
  /* the pgp integer format */
  GNUTLS_MPI_FORMAT_PGP = 2
} gnutls_bigint_format_t;

typedef struct
{
  bigint_t g;			/* group generator */
  bigint_t p;			/* prime */
} gnutls_group_st;

/* Multi precision integer arithmetic */
typedef struct gnutls_crypto_bigint
{
  bigint_t (*bigint_new) (int nbits);
  void (*bigint_release) (bigint_t n);
  /* 0 for equality, > 0 for m1>m2, < 0 for m1<m2 */
  int (*bigint_cmp) (const bigint_t m1, const bigint_t m2);
  /* as bigint_cmp */
  int (*bigint_cmp_ui) (const bigint_t m1, unsigned long m2);
  /* ret = a % b */
    bigint_t (*bigint_mod) (const bigint_t a, const bigint_t b);
  /* a = b -> ret == a */
    bigint_t (*bigint_set) (bigint_t a, const bigint_t b);
  /* a = b -> ret == a */
    bigint_t (*bigint_set_ui) (bigint_t a, unsigned long b);
  unsigned int (*bigint_get_nbits) (const bigint_t a);
  /* w = b ^ e mod m */
    bigint_t (*bigint_powm) (bigint_t w, const bigint_t b,
			     const bigint_t e, const bigint_t m);
  /* w = a + b mod m */
    bigint_t (*bigint_addm) (bigint_t w, const bigint_t a,
			     const bigint_t b, const bigint_t m);
  /* w = a - b mod m */
    bigint_t (*bigint_subm) (bigint_t w, const bigint_t a, const bigint_t b,
			     const bigint_t m);
  /* w = a * b mod m */
    bigint_t (*bigint_mulm) (bigint_t w, const bigint_t a, const bigint_t b,
			     const bigint_t m);
  /* w = a + b */ bigint_t (*bigint_add) (bigint_t w, const bigint_t a,
					  const bigint_t b);
  /* w = a - b */ bigint_t (*bigint_sub) (bigint_t w, const bigint_t a,
					  const bigint_t b);
  /* w = a * b */
    bigint_t (*bigint_mul) (bigint_t w, const bigint_t a, const bigint_t b);
  /* w = a + b */
    bigint_t (*bigint_add_ui) (bigint_t w, const bigint_t a, unsigned long b);
  /* w = a - b */
    bigint_t (*bigint_sub_ui) (bigint_t w, const bigint_t a, unsigned long b);
  /* w = a * b */
    bigint_t (*bigint_mul_ui) (bigint_t w, const bigint_t a, unsigned long b);
  /* q = a / b */
    bigint_t (*bigint_div) (bigint_t q, const bigint_t a, const bigint_t b);
  /* 0 if prime */
  int (*bigint_prime_check) (const bigint_t pp);
  int (*bigint_generate_group) (gnutls_group_st * gg, unsigned int bits);

  /* reads an bigint from a buffer */
  /* stores an bigint into the buffer.  returns
   * GNUTLS_E_SHORT_MEMORY_BUFFER if buf_size is not sufficient to
   * store this integer, and updates the buf_size;
   */
    bigint_t (*bigint_scan) (const void *buf, size_t buf_size,
			     gnutls_bigint_format_t format);
  int (*bigint_print) (const bigint_t a, void *buf, size_t * buf_size,
		       gnutls_bigint_format_t format);
} gnutls_crypto_bigint_st;

#define GNUTLS_MAX_PK_PARAMS 6

typedef struct
{
  bigint_t params[GNUTLS_MAX_PK_PARAMS];
  unsigned int params_nr;	/* the number of parameters */
  unsigned int flags;
} gnutls_pk_params_st;

void gnutls_pk_params_release (gnutls_pk_params_st * p);
void gnutls_pk_params_init (gnutls_pk_params_st * p);

/* params are:
 * RSA:
 *  [0] is modulus
 *  [1] is public exponent
 *  [2] is private exponent (private key only)
 *  [3] is prime1 (p) (private key only)
 *  [4] is prime2 (q) (private key only)
 *  [5] is coefficient (u == inverse of p mod q) (private key only)
 *
 *  note that other packages use inverse of q mod p,
 *  so we need to perform conversions using fixup_params().
 *
 * DSA:
 *  [0] is p
 *  [1] is q
 *  [2] is g
 *  [3] is y (public key)
 *  [4] is x (private key only)
 */

typedef enum
{
  GNUTLS_IMPORT,
  GNUTLS_EXPORT
} gnutls_direction_t;

/* Public key algorithms */
typedef struct gnutls_crypto_pk
{
  /* The params structure should contain the private or public key
   * parameters, depending on the operation */
  int (*encrypt) (gnutls_pk_algorithm_t, gnutls_datum_t * ciphertext,
		  const gnutls_datum_t * plaintext,
		  const gnutls_pk_params_st * /* public */ );
  int (*decrypt) (gnutls_pk_algorithm_t, gnutls_datum_t * plaintext,
		  const gnutls_datum_t * ciphertext,
		  const gnutls_pk_params_st * /* private */ );

  int (*sign) (gnutls_pk_algorithm_t, gnutls_datum_t * signature,
	       const gnutls_datum_t * data,
	       const gnutls_pk_params_st * /* private */ );
  int (*verify) (gnutls_pk_algorithm_t, const gnutls_datum_t * data,
		 const gnutls_datum_t * signature,
		 const gnutls_pk_params_st * /* public */ );

  int (*generate) (gnutls_pk_algorithm_t, unsigned int level /*bits */ ,
		   gnutls_pk_params_st *);
  /* this function should convert params to ones suitable
   * for the above functions
   */
  int (*pk_fixup_private_params) (gnutls_pk_algorithm_t, gnutls_direction_t,
				  gnutls_pk_params_st *);

} gnutls_crypto_pk_st;

/* priority: infinity for backend algorithms, 90 for kernel
   algorithms, lowest wins
 */
# define gnutls_crypto_single_cipher_register(algo, prio, st)		\
  gnutls_crypto_single_cipher_register2 (algo, prio, \
					 GNUTLS_CRYPTO_API_VERSION, st)
# define gnutls_crypto_single_mac_register(algo, prio, st)		\
  gnutls_crypto_single_mac_register2 (algo, prio, \
				      GNUTLS_CRYPTO_API_VERSION, st)
# define gnutls_crypto_single_digest_register(algo, prio, st)		\
  gnutls_crypto_single_digest_register2(algo, prio,			\
					GNUTLS_CRYPTO_API_VERSION, st)

int gnutls_crypto_single_cipher_register2 (gnutls_cipher_algorithm_t algorithm,
					   int priority, int version,
					   gnutls_crypto_single_cipher_st *s);
int gnutls_crypto_single_mac_register2 (gnutls_mac_algorithm_t algorithm,
					int priority, int version,
					gnutls_crypto_single_mac_st * s);
int gnutls_crypto_single_digest_register2 (gnutls_digest_algorithm_t algorithm,
					   int priority, int version,
					   gnutls_crypto_single_digest_st *s);

# define gnutls_crypto_cipher_register(prio, st)			\
  gnutls_crypto_cipher_register2 (prio, GNUTLS_CRYPTO_API_VERSION, st)
# define gnutls_crypto_mac_register(prio, st)				\
  gnutls_crypto_mac_register2 (prio, GNUTLS_CRYPTO_API_VERSION, st)
# define gnutls_crypto_digest_register(prio, st)			\
  gnutls_crypto_digest_register2 (prio, GNUTLS_CRYPTO_API_VERSION, st)

int gnutls_crypto_cipher_register2 (int priority, int version,
				    gnutls_crypto_cipher_st * s);
int gnutls_crypto_mac_register2 (int priority, int version,
				 gnutls_crypto_mac_st * s);
int gnutls_crypto_digest_register2 (int priority, int version,
				    gnutls_crypto_digest_st * s);

# define gnutls_crypto_rnd_register(prio, st)				\
  gnutls_crypto_rnd_register2 (prio, GNUTLS_CRYPTO_API_VERSION, st)
# define gnutls_crypto_pk_register(prio, st)				\
  gnutls_crypto_pk_register2 (prio, GNUTLS_CRYPTO_API_VERSION, st)
# define gnutls_crypto_bigint_register(prio, st)			\
  gnutls_crypto_bigint_register2 (prio, GNUTLS_CRYPTO_API_VERSION, st)

int gnutls_crypto_rnd_register2 (int priority, int version,
				 gnutls_crypto_rnd_st * s);
int gnutls_crypto_pk_register2 (int priority, int version,
				gnutls_crypto_pk_st * s);
int gnutls_crypto_bigint_register2 (int priority, int version,
				    gnutls_crypto_bigint_st * s);

#endif
