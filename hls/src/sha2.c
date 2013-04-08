#include "sha2.h"

/*
 * Function: sha256_transform
 * Description: Transform algorithm for SHA256
 * Parameters:
 *  - uint state[8] - The state of the hash (output)
 *  - uchar data_t[32] - The data to transform (input)
 */
void sha256_transform(uint state[8], uchar data_t[32]) {
#pragma HLS balance_expression // Try to balance as many expressions as possible
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
	int i;
#pragma HLS unroll // Try and unroll the for loop
	// Initialize the states
	for(i = 0; i < 8; i = i + 1) {
		state[i] = K[i];
	}
}

/*
 * Function: sha256_begin
 * Description: Stage 1 hashing (can be synthesized by itself)
 * Parameters:
 *
 */
void sha256_begin(uint state[8], uchar data_begin[80], uchar hash_begin[32]) {
	// Initialize the states
	sha256_init(state);
	//sha256_transform(state, )
}

/*
 * Function: sha256_end
 * Description: Stage 2 hashing (can be synthesized by itself)
 * Parameters:
 *
 */
void sha256_end(uint state[8], uchar data_end[32], uchar hash_end[32]) {
	// Initialize the states
	sha256_init(state);
}

/*
 * Function: byte_swap
 * Description: Big-Endian to Little-Endian
 * Parameters:
 *  - uchar hash[32] - Input
 *  - uchar flip[32] - Output
 */
void byte_swap(uchar hash[32], uchar flip[32]) {

}

/*
 * Function: miner
 * Description: Bitcoin Miner (contains 2 SHA256 cores)
 * Parameters:
 *  - Control Signals (TODO)
 *  - uchar data[80] - 80B of the block
 *  - uchar hash[32] - Output hash
 */
bool miner(uchar data[80], uchar hash[32]) {
	uint state[8];
	uchar hash_begin[32];
	bool valid_hash = false;

	// Create a loop that will run until a valid hash is taken.
	// Run through the first hash
	sha256_begin(state, data, hash_begin);

	// TODO: Byte_swap
	// Byte swap the output for the next hashing

	// Run through the second hash
	sha256_begin(state, hash_begin, hash);

	// TODO: Check hash
	// Check the hash to see if it works, if not hash with a new nonce

	return valid_hash;
}
