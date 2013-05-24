#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sha2.h"

#define NUM_TESTS 2
// These are all related to the char
#define WORK_SIZE 256
#define HASH_SIZE 64
#define NONCE_SIZE 8

void print_hash(unsigned char hash[], int size)
{
   int idx;
   for (idx=0; idx < size; idx++)
      printf("%02x",hash[idx]);
   printf("\n");
}

int main(int argc, char *argv[]) {
	u8 test_data[NUM_TESTS][128];
	u8 valid_hash[NUM_TESTS][32];
	u8 golden_final[NUM_TESTS][32];
	int i,j;
	unsigned err_cnt = 0;

	// File variables
	FILE *fp;
	u8 get_work_data[NUM_TESTS][WORK_SIZE];
	u8 golden_hash[NUM_TESTS][HASH_SIZE];
	u32 nonce[NUM_TESTS][NONCE_SIZE];

	// Check if the file exists
	fp = fopen("golden.dat","r");
	if (fp == NULL) {
		printf("Cannot open file.");
		return 1;
	}

	// Read each line by line to get data
	i = 0;
	while (!feof(fp)) {
		fscanf(fp, "%s %s %s", &get_work_data[i], &golden_hash[i], &nonce[i]);
		printf("Work %d = %s\n",i,get_work_data[i]);
		printf("Hash %d = %s\n", i,golden_hash[i]);
		printf("Nonce %d = %s\n",i,nonce[i]);
		i++;
	}

	fclose(fp);
	for(j = 0; j < NUM_TESTS; j+=1) {
		for(i = 0; i < sizeof(test_data[j]); i+=1) {
			sprintf(get_work_data[j], "%02x", test_data[j][i]);
			*get_work_data[j] += 2;
		}

		for(i = 0; i < sizeof(golden_final[j]); i += 1) {
			sprintf(golden_hash[j], "%02x", golden_final[j][i]);
			*golden_hash[j] += 2;
		}

		printf("Data = "); print_hash(test_data[j], sizeof(test_data[j]));
		printf("Nonce = %s\n", nonce[j]);

		miner(test_data[j], valid_hash[j], nonce[j]);

		printf("Hash = "); print_hash(valid_hash[j], sizeof(valid_hash[j]));
		printf("True Hash = "); print_hash(golden_final[j],sizeof(golden_final[j]));
	}

	return err_cnt;
}
