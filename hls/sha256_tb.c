#include "sha256.c"

void main() {
	SHA256_CTX ctx;
	uchar data[64] = {"abc"};
	uint len = 3;
	uchar hash[64];

	sha256_top(&ctx, data, 64, hash);

	printf(hash);

}
