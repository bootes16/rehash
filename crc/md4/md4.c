#include <stdio.h>
#include <stdint.h>

/* MD4 context. */
typedef struct {
  uint32_t state[4];                                   /* state (ABCD) */
  uint32_t count[2];        /* number of bits, modulo 2^64 (lsb first) */
  unsigned char buffer[64];                         /* input buffer */
} MD4Ctxt;

void MD4Init(MD4Ctxt *);
void MD4Update(MD4Ctxt *, unsigned char *, unsigned int);
void MD4Final(unsigned char [16], MD4Ctxt *);

/* Constants for MD4Transform routine.
 */
#define S11 3
#define S12 7
#define S13 11
#define S14 19
#define S21 3
#define S22 5
#define S23 9
#define S24 13
#define S31 3
#define S32 9
#define S33 11
#define S34 15

static void MD4Transform(uint32_t [4], unsigned char [64]);
static void Encode(unsigned char *, uint32_t *, unsigned int);
static void Decode(uint32_t *, unsigned char *, unsigned int);
static void MD4_memcpy(unsigned char *, unsigned char *, unsigned int);
static void MD4_memset(unsigned char *, int, unsigned int);

static void MDPrint(unsigned char [16]);

static unsigned char PADDING[64] = {
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* F, G and H are basic MD4 functions.
 */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))

/* ROTATE_LEFT rotates x left n bits.
 */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG and HH are transformations for rounds 1, 2 and 3 */
/* Rotation is separate from addition to prevent recomputation */
#define FF(a, b, c, d, x, s) { \
    (a) += F ((b), (c), (d)) + (x); \
    (a) = ROTATE_LEFT ((a), (s)); \
  }
#define GG(a, b, c, d, x, s) { \
    (a) += G ((b), (c), (d)) + (x) + (uint32_t)0x5a827999; \
    (a) = ROTATE_LEFT ((a), (s)); \
  }
#define HH(a, b, c, d, x, s) { \
    (a) += H ((b), (c), (d)) + (x) + (uint32_t)0x6ed9eba1; \
    (a) = ROTATE_LEFT ((a), (s)); \
  }

/* MD4 initialization. Begins an MD4 operation, writing a new context.
 */
void MD4Init(MD4Ctxt *context)
{
  context->count[0] = context->count[1] = 0;

  /* Load magic initialization constants.
   */
  context->state[0] = 0x67452301;
  context->state[1] = 0xefcdab89;
  context->state[2] = 0x98badcfe;
  context->state[3] = 0x10325476;
}

/* MD4 block update operation. Continues an MD4 message-digest
     operation, processing another message block, and updating the
     context.
 */
void MD4Update(MD4Ctxt *context, unsigned char *input, unsigned int inputLen)
{
  unsigned int i, index, partLen;

  /* Compute number of bytes mod 64 */
  index = (unsigned int)((context->count[0] >> 3) & 0x3F);
  /* Update number of bits */
  if ((context->count[0] += ((uint32_t)inputLen << 3))
      < ((uint32_t)inputLen << 3))
    context->count[1]++;
  context->count[1] += ((uint32_t)inputLen >> 29);

  printf("%s count: %08x-%08x\n", __FUNCTION__, context->count[0], context->count[1]);

  partLen = 64 - index;

  /* Transform as many times as possible.
   */
  if (inputLen >= partLen) {
    MD4_memcpy
      ((unsigned char *)&context->buffer[index], (unsigned char *)input, partLen);
    MD4Transform (context->state, context->buffer);

    for (i = partLen; i + 63 < inputLen; i += 64)
      MD4Transform (context->state, &input[i]);

    index = 0;
  }
  else
    i = 0;

  /* Buffer remaining input */
  MD4_memcpy
    ((unsigned char *)&context->buffer[index], (unsigned char *)&input[i],
     inputLen-i);
}

/* MD4 finalization. Ends an MD4 message-digest operation, writing the
     the message digest and zeroizing the context.
 */
void MD4Final (unsigned char digest[16], MD4Ctxt *context)
{
  unsigned char bits[8];
  unsigned int index, padLen;

  /* Save number of bits */
  Encode (bits, context->count, 8);

  /* Pad out to 56 mod 64.
   */
  index = (unsigned int)((context->count[0] >> 3) & 0x3f);
  padLen = (index < 56) ? (56 - index) : (120 - index);
  MD4Update(context, PADDING, padLen);

  /* Append length (before padding) */
  MD4Update(context, bits, 8);
  /* Store state in digest */
  Encode (digest, context->state, 16);

  /* Zeroize sensitive information.
   */
  MD4_memset ((unsigned char *)context, 0, sizeof (*context));
}

void print_block(const unsigned char block[64])
{
  for (int i = 0; i < 64; i++)
  {
    printf("%02x", block[i]);
  }
  printf("\n");
}

void print_words(const uint32_t x[16])
{
  for (int i = 0; i < 16; i++)
  {
    printf("0x%08x", x[i]);
    if ((i > 0) && ((i % 4) == 3))
      printf("\n");
    else
      printf(" ");
  }
}

/* MD4 basic transformation. Transforms state based on block.
 */
static void MD4Transform (uint32_t state[4], unsigned char block[64])
{
  uint32_t a = state[0], b = state[1], c = state[2], d = state[3], x[16];

  print_block(block);

  Decode (x, block, 64);

  print_words(x);

// printf("> %08x %08x %08x %08x\n", a, b, c, d);

  /* Round 1 */
  printf("> %08x %08x %08x %08x\n", a, b, c, d);
  printf(">> %08x %08x %08x\n", a + F(b, c, d) + x[0], F(b, c, d), x[0]);
  FF (a, b, c, d, x[ 0], S11); /* 1 */
  FF (d, a, b, c, x[ 1], S12); /* 2 */
  FF (c, d, a, b, x[ 2], S13); /* 3 */
  FF (b, c, d, a, x[ 3], S14); /* 4 */
  printf("> %08x %08x %08x %08x\n", a, b, c, d);
  FF (a, b, c, d, x[ 4], S11); /* 5 */
  FF (d, a, b, c, x[ 5], S12); /* 6 */
  FF (c, d, a, b, x[ 6], S13); /* 7 */
  FF (b, c, d, a, x[ 7], S14); /* 8 */
  printf("> %08x %08x %08x %08x\n", a, b, c, d);
  FF (a, b, c, d, x[ 8], S11); /* 9 */
  FF (d, a, b, c, x[ 9], S12); /* 10 */
  FF (c, d, a, b, x[10], S13); /* 11 */
  FF (b, c, d, a, x[11], S14); /* 12 */
  printf("> %08x %08x %08x %08x\n", a, b, c, d);
  FF (a, b, c, d, x[12], S11); /* 13 */
  FF (d, a, b, c, x[13], S12); /* 14 */
  FF (c, d, a, b, x[14], S13); /* 15 */
  FF (b, c, d, a, x[15], S14); /* 16 */

  printf("> %08x %08x %08x %08x\n", a, b, c, d);

  /* Round 2 */
  GG (a, b, c, d, x[ 0], S21); /* 17 */
  GG (d, a, b, c, x[ 4], S22); /* 18 */
  GG (c, d, a, b, x[ 8], S23); /* 19 */
  GG (b, c, d, a, x[12], S24); /* 20 */
  GG (a, b, c, d, x[ 1], S21); /* 21 */
  GG (d, a, b, c, x[ 5], S22); /* 22 */
  GG (c, d, a, b, x[ 9], S23); /* 23 */
  GG (b, c, d, a, x[13], S24); /* 24 */
  GG (a, b, c, d, x[ 2], S21); /* 25 */
  GG (d, a, b, c, x[ 6], S22); /* 26 */
  GG (c, d, a, b, x[10], S23); /* 27 */
  GG (b, c, d, a, x[14], S24); /* 28 */
  GG (a, b, c, d, x[ 3], S21); /* 29 */
  GG (d, a, b, c, x[ 7], S22); /* 30 */
  GG (c, d, a, b, x[11], S23); /* 31 */
  GG (b, c, d, a, x[15], S24); /* 32 */

// printf("> %08x %08x %08x %08x\n", a, b, c, d);

  /* Round 3 */
  HH (a, b, c, d, x[ 0], S31); /* 33 */
  HH (d, a, b, c, x[ 8], S32); /* 34 */
  HH (c, d, a, b, x[ 4], S33); /* 35 */
  HH (b, c, d, a, x[12], S34); /* 36 */
  HH (a, b, c, d, x[ 2], S31); /* 37 */
  HH (d, a, b, c, x[10], S32); /* 38 */
  HH (c, d, a, b, x[ 6], S33); /* 39 */
  HH (b, c, d, a, x[14], S34); /* 40 */
  HH (a, b, c, d, x[ 1], S31); /* 41 */
  HH (d, a, b, c, x[ 9], S32); /* 42 */
  HH (c, d, a, b, x[ 5], S33); /* 43 */
  HH (b, c, d, a, x[13], S34); /* 44 */
  HH (a, b, c, d, x[ 3], S31); /* 45 */
  HH (d, a, b, c, x[11], S32); /* 46 */
  HH (c, d, a, b, x[ 7], S33); /* 47 */
  HH (b, c, d, a, x[15], S34); /* 48 */

  // printf("> %08x %08x %08x %08x\n", a, b, c, d);

  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;

  // printf("%08x %08x %08x %08x\n", state[0], state[1], state[2], state[3]);

  /* Zeroize sensitive information.
   */
  MD4_memset ((unsigned char *)x, 0, sizeof (x));
}

/* Encodes input (uint32_t) into output (unsigned char). Assumes len is
     a multiple of 4.
 */
static void Encode (unsigned char *output, uint32_t *input, unsigned int len)
{
  unsigned int i, j;

  for (i = 0, j = 0; j < len; i++, j += 4) {
    output[j] = (unsigned char)(input[i] & 0xff);
    output[j+1] = (unsigned char)((input[i] >> 8) & 0xff);
    output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);
    output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);
  }
}

/* Decodes input (unsigned char) into output (uint32_t). Assumes len is
     a multiple of 4.
 */
static void Decode (uint32_t *output, unsigned char *input, unsigned int len)
{
  unsigned int i, j;

  for (i = 0, j = 0; j < len; i++, j += 4)
    output[i] = ((uint32_t)input[j]) | (((uint32_t)input[j+1]) << 8) |
      (((uint32_t)input[j+2]) << 16) | (((uint32_t)input[j+3]) << 24);
}

/* Note: Replace "for loop" with standard memcpy if possible.
 */
static void MD4_memcpy (unsigned char * output, unsigned char * input, unsigned int len)
{
  unsigned int i;

  for (i = 0; i < len; i++)
    output[i] = input[i];
}

/* Note: Replace "for loop" with standard memset if possible.
 */
static void MD4_memset(unsigned char * output, int value, unsigned int len)
{
  unsigned int i;

  for (i = 0; i < len; i++)
    ((char *)output)[i] = (char)value;
}

#if 0
A.4 mddriver.c

/* MDDRIVER.C - test driver for MD2, MD4 and MD5
 */

/* Copyright (C) 1990-2, RSA Data Security, Inc. Created 1990. All
   rights reserved.




Rivest                                                         [Page 14]

RFC 1320              MD4 Message-Digest Algorithm            April 1992


   RSA Data Security, Inc. makes no representations concerning either
   the merchantability of this software or the suitability of this
   software for any particular purpose. It is provided "as is"
   without express or implied warranty of any kind.

   These notices must be retained in any copies of any part of this
   documentation and/or software.

 */

/* The following makes MD default to MD5 if it has not already been
     defined with C compiler flags.
 */
#ifndef MD
#define MD MD5
#endif

#include <stdio.h>
#include <time.h>
#include <string.h>
#include "global.h"
#if MD == 2
#include "md2.h"
#endif
#if MD == 4
#include "md4.h"
#endif
#if MD == 5
#include "md5.h"
#endif

/* Length of test block, number of test blocks.
 */
#define TEST_BLOCK_LEN 1000
#define TEST_BLOCK_COUNT 1000

static void MDString  ((char *));
static void MDTimeTrial  ((void));
static void MDTestSuite  ((void));
static void MDFile  ((char *));
static void MDFilter  ((void));
static void MDPrint  ((unsigned char [16]));

#if MD == 2
#define MD4Ctxt MD2_CTX
#define MDInit MD2Init
#define MDUpdate MD2Update
#define MDFinal MD2Final



Rivest                                                         [Page 15]

RFC 1320              MD4 Message-Digest Algorithm            April 1992


#endif
#if MD == 4
#define MD4Ctxt MD4Ctxt
#define MDInit MD4Init
#define MDUpdate MD4Update
#define MDFinal MD4Final
#endif
#if MD == 5
#define MD4Ctxt MD5_CTX
#define MDInit MD5Init
#define MDUpdate MD5Update
#define MDFinal MD5Final
#endif

/* Main driver.

   Arguments (may be any combination):
     -sstring - digests string
     -t       - runs time trial
     -x       - runs test script
     filename - digests file
     (none)   - digests standard input
 */
int main (argc, argv)
int argc;
char *argv[];
{
  int i;

  if (argc > 1)
    for (i = 1; i < argc; i++)
      if (argv[i][0] == '-' && argv[i][1] == 's')
        MDString (argv[i] + 2);
      else if (strcmp (argv[i], "-t") == 0)
        MDTimeTrial ();
      else if (strcmp (argv[i], "-x") == 0)
        MDTestSuite ();
      else
        MDFile (argv[i]);
  else
    MDFilter ();

  return (0);
}

/* Digests a string and prints the result.
 */
static void MDString (string)



Rivest                                                         [Page 16]

RFC 1320              MD4 Message-Digest Algorithm            April 1992


char *string;
{
  MD4Ctxt context;
  unsigned char digest[16];
  unsigned int len = strlen (string);

  MDInit (&context);
  MDUpdate (&context, string, len);
  MDFinal (digest, &context);

  printf ("MD%d (\"%s\") = ", MD, string);
  MDPrint (digest);
  printf ("\n");
}

/* Measures the time to digest TEST_BLOCK_COUNT TEST_BLOCK_LEN-byte
     blocks.
 */
static void MDTimeTrial ()
{
  MD4Ctxt context;
  time_t endTime, startTime;
  unsigned char block[TEST_BLOCK_LEN], digest[16];
  unsigned int i;

  printf
    ("MD%d time trial. Digesting %d %d-byte blocks ...", MD,
     TEST_BLOCK_LEN, TEST_BLOCK_COUNT);

  /* Initialize block */
  for (i = 0; i < TEST_BLOCK_LEN; i++)
    block[i] = (unsigned char)(i & 0xff);

  /* Start timer */
  time (&startTime);

  /* Digest blocks */
  MDInit (&context);
  for (i = 0; i < TEST_BLOCK_COUNT; i++)
    MDUpdate (&context, block, TEST_BLOCK_LEN);
  MDFinal (digest, &context);

  /* Stop timer */
  time (&endTime);

  printf (" done\n");
  printf ("Digest = ");
  MDPrint (digest);



Rivest                                                         [Page 17]

RFC 1320              MD4 Message-Digest Algorithm            April 1992


  printf ("\nTime = %ld seconds\n", (long)(endTime-startTime));
  printf
    ("Speed = %ld bytes/second\n",
     (long)TEST_BLOCK_LEN * (long)TEST_BLOCK_COUNT/(endTime-startTime));
}

/* Digests a reference suite of strings and prints the results.
 */
static void MDTestSuite ()
{
  printf ("MD%d test suite:\n", MD);

  MDString ("");
  MDString ("a");
  MDString ("abc");
  MDString ("message digest");
  MDString ("abcdefghijklmnopqrstuvwxyz");
  MDString
    ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
  MDString

    ("1234567890123456789012345678901234567890\
1234567890123456789012345678901234567890");
}

/* Digests a file and prints the result.
 */
static void MDFile (filename)
char *filename;
{
  FILE *file;
  MD4Ctxt context;
  int len;
  unsigned char buffer[1024], digest[16];

  if ((file = fopen (filename, "rb")) == NULL)
    printf ("%s can't be opened\n", filename);

  else {
    MDInit (&context);
    while (len = fread (buffer, 1, 1024, file))
      MDUpdate (&context, buffer, len);
    MDFinal (digest, &context);

    fclose (file);

    printf ("MD%d (%s) = ", MD, filename);
    MDPrint (digest);



Rivest                                                         [Page 18]

RFC 1320              MD4 Message-Digest Algorithm            April 1992


    printf ("\n");
  }
}

/* Digests the standard input and prints the result.
 */
static void MDFilter(void)
{
  MD4Ctxt context;
  int len;
  unsigned char buffer[16], digest[16];

  MDInit (&context);
  while (len = fread (buffer, 1, 16, stdin))
    MDUpdate (&context, buffer, len);
  MDFinal (digest, &context);

  MDPrint (digest);
  printf ("\n");
}
#endif

int main(int argc, char* argv[])
{
  MD4Ctxt context;
  int len;
  unsigned char digest[16];
  unsigned char buffer[] = "a";

  MD4Init(&context);
  MD4Update(&context, buffer, 1);
  MD4Final(digest, &context);

  MDPrint (digest);
  printf ("\n");
}

/* Prints a message digest in hexadecimal.
 */
static void MDPrint(unsigned char digest[16])
{
  unsigned int i;

  for (i = 0; i < 16; i++)
    printf ("%02x", digest[i]);
}

#if 0
A.5 Test suite

   The MD4 test suite (driver option "-x") should print the following
   results:

MD4 test suite:
MD4 ("") = 31d6cfe0d16ae931b73c59d7e0c089c0
MD4 ("a") = bde52cb31de33e46245e05fbdbd6fb24
MD4 ("abc") = a448017aaf21d8525fc10ae87aa6729d
MD4 ("message digest") = d9130a8164549fe818874806e1c7014b
MD4 ("abcdefghijklmnopqrstuvwxyz") = d79e1c308aa5bbcdeea8ed63df412da9
MD4 ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") =
043f8582f241db351ce627e153e7f0e4
MD4 ("123456789012345678901234567890123456789012345678901234567890123456
78901234567890") = e33b4ddc9c38f2199c3e7b164fcc0536
#endif