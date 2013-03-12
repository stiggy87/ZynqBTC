#include "sha256.c"

int main(int *argv, char **argc) {
	SHA256_CTX ctx;
	uchar data[64] = {0xA};
	//uint len = 3;
	uchar hash[64];

	//sha256_top(&ctx, data, hash);
	sha256_top(data, hash);

	printf(hash);
	return 0;
}
