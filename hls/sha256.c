#include <stdio.h> 
#include <stdbool.h>
#include "sha256.h"

void sha256_transform(SHA256_CTX *ctx_transform, uchar data_transform[64])
{  
   uint a,b,c,d,e,f,g,h,i,j,t1,t2,m[64];

   for (i=0,j=0; i < 16; ++i, j += 4)
      m[i] = (data_transform[j] << 24) | (data_transform[j+1] << 16) | (data_transform[j+2] << 8) | (data_transform[j+3]);
   for ( ; i < 64; ++i)
      m[i] = SIG1(m[i-2]) + m[i-7] + SIG0(m[i-15]) + m[i-16];

   a = ctx_transform->state[0];
   b = ctx_transform->state[1];
   c = ctx_transform->state[2];
   d = ctx_transform->state[3];
   e = ctx_transform->state[4];
   f = ctx_transform->state[5];
   g = ctx_transform->state[6];
   h = ctx_transform->state[7];
   
   for (i = 0; i < 64; ++i) {
      t1 = h + EP1(e) + CH(e,f,g) + k[i] + m[i];
      t2 = EP0(a) + MAJ(a,b,c);
      h = g;
      g = f;
      f = e;
      e = d + t1;
      d = c;
      c = b;
      b = a;
      a = t1 + t2;
   }
   
   ctx_transform->state[0] += a;
   ctx_transform->state[1] += b;
   ctx_transform->state[2] += c;
   ctx_transform->state[3] += d;
   ctx_transform->state[4] += e;
   ctx_transform->state[5] += f;
   ctx_transform->state[6] += g;
   ctx_transform->state[7] += h;
}  

void sha256_init(SHA256_CTX *ctx)
{  
   ctx->datalen = 0;
   ctx->bitlen[0] = 0; 
   ctx->bitlen[1] = 0; 
   ctx->state[0] = 0x6a09e667;
   ctx->state[1] = 0xbb67ae85;
   ctx->state[2] = 0x3c6ef372;
   ctx->state[3] = 0xa54ff53a;
   ctx->state[4] = 0x510e527f;
   ctx->state[5] = 0x9b05688c;
   ctx->state[6] = 0x1f83d9ab;
   ctx->state[7] = 0x5be0cd19;
}

void sha256_update(SHA256_CTX *ctx_update, uchar data[64], uint len)
{  
   uint t,i;
#pragma HLS unroll
   for (i=0; i < len; ++i) { 
      ctx_update->in_data[ctx_update->datalen] = data[i];
      ctx_update->datalen++;
      if (ctx_update->datalen == 64) {
         sha256_transform(ctx_update,ctx_update->in_data);
         DBL_INT_ADD(ctx_update->bitlen[0],ctx_update->bitlen[1],512);
         ctx_update->datalen = 0;
      }  
   }  
}  

void sha256_final(SHA256_CTX *ctx_final, uchar final_hash[64])
{  
   uint i; 
   uint j;
   i = ctx_final->datalen;
   
   // Pad whatever data is left in the buffer. 
   if (ctx_final->datalen < 56) {
      ctx_final->in_data[i++] = 0x80;
      while (i < 56) 
         ctx_final->in_data[i++] = 0x00;
   }  
   else { 
      ctx_final->in_data[i++] = 0x80;
      while (i < 64) 
         ctx_final->in_data[i++] = 0x00;
      sha256_transform(ctx_final,ctx_final->in_data);
      //memset(ctx_final->in_data,0,56);

    sha256_final_memset:
	for(j = 0; j < 56; j = j + 1) {
			  ctx_final->in_data[j] = 0;
		  }
	}
   
   // Append to the padding the total message's length in bits and transform. 
   DBL_INT_ADD(ctx_final->bitlen[0],ctx_final->bitlen[1],ctx_final->datalen * 8);
   ctx_final->in_data[63] = ctx_final->bitlen[0];
   ctx_final->in_data[62] = ctx_final->bitlen[0] >> 8;
   ctx_final->in_data[61] = ctx_final->bitlen[0] >> 16;
   ctx_final->in_data[60] = ctx_final->bitlen[0] >> 24;
   ctx_final->in_data[59] = ctx_final->bitlen[1];
   ctx_final->in_data[58] = ctx_final->bitlen[1] >> 8;
   ctx_final->in_data[57] = ctx_final->bitlen[1] >> 16;
   ctx_final->in_data[56] = ctx_final->bitlen[1] >> 24;
   sha256_transform(ctx_final,ctx_final->in_data);

   // Since this implementation uses little endian byte ordering and SHA uses big endian,
   // reverse all the bytes when copying the final state to the output hash.
   sha256_final_final_loop:
   for (i=0; i < 4; ++i) {
	  final_hash[i]    = ((ctx_final->state[0] >> (24-i*8)) & 0x000000ff);
	  final_hash[i+4]  = ((ctx_final->state[1] >> (24-i*8)) & 0x000000ff);
	  final_hash[i+8]  = ((ctx_final->state[2] >> (24-i*8)) & 0x000000ff);
	  final_hash[i+12] = ((ctx_final->state[3] >> (24-i*8)) & 0x000000ff);
	  final_hash[i+16] = ((ctx_final->state[4] >> (24-i*8)) & 0x000000ff);
	  final_hash[i+20] = ((ctx_final->state[5] >> (24-i*8)) & 0x000000ff);
	  final_hash[i+24] = ((ctx_final->state[6] >> (24-i*8)) & 0x000000ff);
	  final_hash[i+28] = ((ctx_final->state[7] >> (24-i*8)) & 0x000000ff);
   }
}  

// SHA256_top
void sha256_top(uchar data[32], uchar hash[32]) {
	uint len = 32;
	int i;

#pragma HLS inline
	sha256_init(&ctx);
	SHA256_CTX *ctx_temp = &ctx;
	// Initialize the hash

	// Update the hash with data
	sha256_update(ctx_temp, data, len);

	// Finalize
	sha256_final(ctx_temp, hash);
}

// SHA256_controller
//void sha256_controller()
