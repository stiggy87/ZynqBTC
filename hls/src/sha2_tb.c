#include <stdio.h>
#include <stdlib.h>
#include "sha2.h"

#define NUM_TESTS 2
// These are all related to the char
#define WORK_SIZE 256
#define HASH_SIZE 64
#define NONCE_SIZE 8

int main(int argc, char *argv[]) {
	//uchar test_data[128];
	uchar valid_hash[32];
	int i,j;
	unsigned err_cnt = 0;

	// File variables
	FILE *fp;
	unsigned char get_work_data[NUM_TESTS][WORK_SIZE];
	unsigned char golden_hash[NUM_TESTS][HASH_SIZE];
	unsigned char nonce[NUM_TESTS][NONCE_SIZE];

	// Check if the file exists
	fp = fopen("golden.dat","r");
	if (fp == NULL) {
		printf("Cannot open file.");
		return 1;
	}

	// Read each line by line to get data
	i = 0;
	while (!feof(fp)) {
		fscanf(fp, "%s %s %s", get_work_data[i], golden_hash[i], nonce[i]);
		//printf("Work %d= %s\n", i,get_work_data[i]);
		//printf("Hash %d= %s\n", i,golden_hash[i]);
		//printf("Nonce %d= %s\n",i, nonce[i]);
		i++;
	}
	fclose(fp);

	// Run through the miner
	for(i = 0; i < NUM_TESTS; i += 1) {
		miner(get_work_data[i], valid_hash, nonce);

		// Compare the valid_hash to the once brought in
		print_hash(valid_hash, sizeof(valid_hash));
		printf("%s", golden_hash[i]);
	}

	return err_cnt;
}
