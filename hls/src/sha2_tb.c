#include <stdio.h>
#include "sha2.h"

void print_hash(unsigned char hash[])
{
   int idx;
   for (idx=0; idx < 32; idx++)
      printf("%02x",hash[idx]);
   printf("\n");
}

int main(int argc, char *argv[]) {
	uchar test_val[80] = {0x00000001c570c4764aadb3f09895619f549000b8b51a789e7f58ea750000709700000000103ca064f8c76c390683f8203043e91466a7fcc40e6ebc428fbcc2d8};
	uchar final_hash[32];

	miner(test_val, final_hash);

	print_hash(final_hash);

	return 0;
}
