#include "sha2.h"
#include <stdlib.h>
#include <stdbool.h>

/*
 * Function: sha256_transform
 * Description: Transform algorithm for SHA256
 * Parameters:
 *  - uint state[8] - The state of the hash (output)
 *  - uchar data_t[32] - The data to transform (input)
 */
void sha256_transform(uint state[8], uchar data_t[64]) {
#pragma HLS expression_balance // Try to balance as many expressions as possible
	int i;
	uint a, b, c, d, e, f, g, h;
	uint t1, t2, W[64];

	// Initialize beginning of hash
	a = state[0]; b = state[1]; c = state[2]; d = state[3];
	e = state[4]; f = state[5]; g = state[6]; h = state[7];

#pragma HLS unroll // Try to unroll the for loop
	// Load the data into the message schedule for the first 16
	for(i = 0; i < 16; i = i + 1) {
		W[i] = (uint)data_t[i];
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
 *
 */
void sha256_init(uint state[8]) {
#pragma HLS inline // Remove the sha256_init hierarchy
//	memcpy(state,H,8);
	int i;
	for(i = 0; i < 8; i = i + 1) {
		state[i] = H[i];
	}
}

/*
 * Function: sha256_begin
 * Description: Stage 1 hashing (can be synthesized by itself)
 * Parameters:
 *
 */
void sha256_begin(uint state[8], uchar data_begin[64], uchar hash_begin[32]) {
	// Initialize the states
	sha256_init(state);
	sha256_transform(state, data_begin);
	*hash_begin = (uchar*)state;
}

/*
 * Function: sha256_end
 * Description: Stage 2 hashing (can be synthesized by itself)
 * Parameters:
 *
 */
void sha256_end(uint state[8], uchar data_end[64], uchar hash_end[32]) {
	// Initialize the states
	sha256_init(state);
	sha256_transform(state, data_end);
	hash_end = (uchar)state;
}

/*
 * Function: miner
 * Description: Bitcoin Miner (contains 2 SHA256 cores)
 * Parameters:
 *  - Control Signals (TODO)
 *  - uchar data[80] - 80B of the block
 *  - uchar hash[32] - Output hash
 */
bool miner(uchar data[80], uchar hash[32] ) {
	// mid-state not taken into account (ARM will handle this)
	uint state[8];
	uchar mid_state[32]; // Used for between the first and second hash
	bool valid_hash = false; // Return value, this is for control signaling
	uint *nonce = (uint*)(data+12); // Initial value of nonce
	uint max_nonce = 0xffffffff; // Maximum nonce to count to (can be changed)
	uint target[32] = {0xffffffffffffffffffffffffffffffffffffffffffffffffffffffff00000000}; // If the has is <= this, it's a valid hash

	// Split the data[80] into the first 64-bytes (MSB)
	uchar first_hash[32];
	memcpy((void *)first_hash,(void *)(data+16),64); // The first 64-bytes (80..16) contain the data

	sha256_begin(state, first_hash, mid_state); // Get the midstate, which should be 32-bytes (256-bits)

	// Combine the mid_state with the remaining data (where nonce is adjusted)

	// Create a loop that will run until a valid hash is taken.
	while (1) {
		// Run through the first hash

		// TODO: Byte_swap
		// Byte swap the output for the next hashing

		// Run through the second hash
		sha256_begin(state, mid_state, hash);

		// TODO: Check hash
		// Check the hash to see if it works, if not hash with a new nonce
		if ((nonce >= max_nonce)) {
			valid_hash = false;
			break;
		}

		if (hash <= target) {
			valid_hash = true;
			break;
		}

		(*nonce)++;
	}

	return valid_hash;
}
