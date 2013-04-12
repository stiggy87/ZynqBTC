#include "sha2.h"
#include <stdlib.h>
#include <stdbool.h>

void print_hash(unsigned char hash[], int size)
{
   int idx;
   for (idx=0; idx < size; idx++)
      printf("%02x",hash[idx]);
   printf("\n");
}

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
 *	- state[8]
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
 * Function: sha256_update
 * Description: Initialization of the data to be hashed
 * Parameters:
 *	- state[8]
 *	- data[64]
 */
void sha256_update(uint state[8], uchar data[64], uchar init[32]) {
	// Initialize the states
	memcpy(state,init,32);
	sha256_transform(state, data);
}

/*
 * Function: miner
 * Description: Bitcoin Miner (contains 2 SHA256 cores)
 * Parameters:
 *  - Control Signals (TODO)
 *  - uchar data[128] - Size of the work from Getwork and Stratum
 *  - uchar hash[32] - Output hash
 */
bool miner(uchar data[128], uchar hash[32] ) {
	// mid-state not taken into account (ARM will handle this)
	//print_hash(data,80);
	uint state[8];
	uchar mid_state[32]; // Used for between the first and second hash
	bool valid_hash = false; // Return value, this is for control signaling
	uint *nonce = (uint*)(data+76); // Initial value of nonce
	int i;
	uint n = 0;
	printf("Nonce = %08x\n", nonce);

	uint max_nonce = 0xffffffff; // Maximum nonce to count to (can be changed)
	uint target[32] = {0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
					   0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
					   0xff,0xff,0xff,0xff,0xff,0xff}; // If the has is <= this, it's a valid hash
	uchar hash1[32];
	uchar tmp_hash[64];

	// Split the data[80] into the first 64-bytes (MSB)
	uint first_hash[16];
//	print_hash(data,64);
	memcpy(first_hash,(uint*)data,64); // The first 64-bytes contain the data
	printf("First to hash = "); print_hash(first_hash,64);

	// Combine the mid_state with the remaining data (where nonce is adjusted)
	sha256_update(mid_state, first_hash, H); // Get the midstate, which should be 32-bytes (256-bits)

	// TODO: Byte_swap
	// Byte swap the output for the next hashing
	printf("Mid State = "); print_hash((uchar*)mid_state,32);

	// Create a loop that will run until a valid hash is taken.
//	while (1) {


		// Run through the second hash
		sha256_update(state, (data+64), mid_state);
		printf ("Nonce = %08x",n); printf("\tFinal Hash = "); print_hash((uchar*)state,32);
		// Byte swap the output

		// TODO: Check hash
		// Check the hash to see if it works, if not hash with a new nonce
		if ((nonce >= max_nonce)) {
			valid_hash = false;
//			break;
		}

		memcpy(hash, (uchar*)state, 32);
		if (hash <= target) {
			printf ("Nonce = %08x",nonce);
			valid_hash = true;
//			break;
		}

		n++;
		*nonce = n;
//	}

	valid_hash = true;
	return valid_hash;
}
