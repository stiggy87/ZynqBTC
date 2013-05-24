#include "sha2.h"
#include <stdlib.h>
#include <stdbool.h>

/*
 * Function: sha256_transform
 * Description: Transform algorithm for SHA256
 * Parameters:
 *  - u32 state[8] - The state of the hash (output)
 *  - u8 data_t[32] - The data to transform (input)
 */
void sha256_transform(u32 state[8], u8 data_t[64]) {
#pragma HLS expression_balance // Try to balance as many expressions as possible
	int i;
	u32 a, b, c, d, e, f, g, h;
	u32 t1, t2, W[64];

	// Initialize beginning of hash
	a = state[0]; b = state[1]; c = state[2]; d = state[3];
	e = state[4]; f = state[5]; g = state[6]; h = state[7];

#pragma HLS unroll // Try to unroll the for loop
	// Load the data into the message schedule for the first 16
	for(i = 0; i < 16; i = i + 1) {
		W[i] = (u32)data_t[i];
	}

#pragma HLS unroll // Try to unroll the for loop
	// Load the data into the message schedule for the 16 -> 64
	for(i = 16; i < 64; i = i + 1) {
		W[i] = SUM1(W[i-2]) + W[i-7] + SUM0(W[i-15]) + W[i-16];
	}

#pragma HLS unroll // Try to unroll the for loop
	// The hashing algorithm itself
	for(i = 0; i < 64; i = i + 1) {
		t1 = h + SUM1(e) + CH(e,f,g) + K[i] + W[i];
		t2 = SUM0(a) + MAJ(a,b,c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	// Final hash
	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
	state[4] += e;
	state[5] += f;
	state[6] += g;
	state[7] += h;
}

/*
 * Function: sha256_init
 * Description: Initializes variables
 * Parameters:
 *	- state[8]
 */
void sha256_init(u32 state[8]) {
#pragma HLS inline // Remove the sha256_init hierarchy
//	memcpy(state,H,8);
	int i;
	for(i = 0; i < 8; i = i + 1) {
		state[i] = H[i];
	}
}

/*
 * Function: sha256_update
 * Description: Initialization of the data to be hashed
 * Parameters:
 *	- state[8]
 *	- data[64]
 */
void sha256_update(u32 state[8], u8 input[64], u8 init[32]) {
	memcpy((void *)state,(void *)init,32);
	sha256_transform(state,input);
}

/*
 * Function: miner
 * Description: Bitcoin Miner (contains 2 SHA256 cores)
 * Parameters:
 *  - Control Signals (TODO)
 *  - u8 data[128] - Size of the work from Getwork and Stratum
 *  - u8 hash[32] - Output hash
 */
void miner(u8 data[128], u8 hash[32]) {
	// Local variables
	u32 hash32[8] = (u32*)hash;
	u32 *nonce = (u32*)(data + 64 +18); // The nonce location
	u32 n = 0;
	u8 midstate[32];

	u8 tmp_hash[32];

	memcpy((void *)tmp_hash,(void *)data,64*sizeof(u8));

	sha256_update(midstate,tmp_hash,H);

	memcpy((void *)tmp_hash,((void *)data)+64,64*sizeof(u8));

	while(1) {
		u8 hash1[32];

		sha256_update((u32)hash1,tmp_hash,midstate);
	}

}
