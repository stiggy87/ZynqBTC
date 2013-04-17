#ifndef _SHA_2_H_
#define _SHA_2_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "byteorder.h"

typedef uint8_t uchar; // 8-bit byte
typedef  uint32_t uint; // 32-bit word

#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define SUM0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define SUM1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

static const uint K[64] = {
   0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
   0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
   0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
   0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
   0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
   0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
   0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
   0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

static const uint H[8] = {0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c, 0x1f83d9ab,0x5be0cd19};

/*
 * Function: sha256_transform
 * Description: Transform algorithm for SHA256
 * Parameters:
 *  - uint state[8] - The state of the hash (output)
 *  - uchar data_t[32] - The data to transform (input)
 */
void sha256_transform(uint state[8], uchar data_t[32]);

/*
 * Function: sha256_init
 * Description: Initializes variables
 * Parameters:
 *
 */
void sha256_init(uint state[8]);

/*
 * Function: sha256_update
 * Description: Initialization of the data to be hashed
 * Parameters:
 *	- state[8]
 *	- data[64]
 */
void sha256_update(uint state[8], uchar data[64], uchar init[32]);

/*
 * Function: miner
 * Description: Bitcoin Miner (contains 2 SHA256 cores)
 * Parameters:
 *  - Control Signals (TODO)
 *  - uchar data[128] - Size of the work from Getwork and Stratum
 *  - uchar hash[32] - Output hash
 */
bool miner(uchar data[128], uchar hash[32], uint *nonce);

#endif _SHA_2_H_
