/*
 * ByteOrder - portable tools to determine the byte order (endianness),
 * swap the order of bytes in integer value, and convert values between host
 * and little-/big-endian byte order.
 *
 * http://projects.malcom.pl/libs/byteorder.xhtml
 * http://github.com/malcom/ByteOrder
 * 
 * Copyright (C) 2013 Marcin 'Malcom' Malich <me@malcom.pl>
 *
 * Released under the MIT License.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef BO_ENDIAN_BYTEORDER_H
#define BO_ENDIAN_BYTEORDER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#if defined(linux) || defined(__linux__) || defined (__GLIBC__) || defined(__GNU__)

	#include <byteswap.h>
	#define bswap16(x) __bswap_16(x)
	#define bswap32(x) __bswap_32(x)
	#define bswap64(x) __bswap_64(x)

	#ifdef __USE_BSD

		#include <endian.h>

		#define letoh16(x) le16toh(x)
		#define letoh32(x) le32toh(x)
		#define letoh64(x) le64toh(x)

		#define betoh16(x) be16toh(x)
		#define betoh32(x) be32toh(x)
		#define betoh64(x) be64toh(x)

		#define BO_ENDIAN_FUNC_DEFINED

	#endif

#elif defined(__FreeBSD__) || defined(__DragonFlyBSD__) || defined(__NetBSD__)

	#include <sys/endian.h>
	// native bswapNN funcs/macros definied

	#define letoh16(x) le16toh(x)
	#define letoh32(x) le32toh(x)
	#define letoh64(x) le64toh(x)

	#define betoh16(x) be16toh(x)
	#define betoh32(x) be32toh(x)
	#define betoh64(x) be64toh(x)

	#define BO_ENDIAN_FUNC_DEFINED

#elif defined(__OpenBSD__)

	#include <sys/endian.h>
	#define bswap16(x) __swap16(x)
	#define bswap32(x) __swap32(x)
	#define bswap64(x) __swap64(x)

	#define BO_ENDIAN_FUNC_DEFINED

#elif defined(__APPLE__)

	#include <libkern/OSByteOrder.h>
	#define bswap16(x) OSSwapInt16(x)
	#define bswap32(x) OSSwapInt32(x)
	#define bswap64(x) OSSwapInt64(x)

#elif defined(sun) || defined(__sun)

	#include <sys/byteorder.h>
	#define bswap16(x) BSWAP_16(x)
	#define bswap32(x) BSWAP_32(x)
	#define bswap64(x) BSWAP_64(x)

#elif defined(_MSC_VER)

	#include <intrin.h>
	#define bswap16(x) _byteswap_ushort(x)
	#define bswap32(x) _byteswap_ulong(x)
	#define bswap64(x) _byteswap_uint64(x)

#else

	#define bswap16(x) \
		((((uint16_t)(x) & 0xff00) >> 8) | \
		 (((uint16_t)(x) & 0x00ff) << 8))

	#define bswap32(x) \
		((((uint32_t)(x) & 0xff000000) >> 24) | \
		 (((uint32_t)(x) & 0x00ff0000) >>  8) | \
		 (((uint32_t)(x) & 0x0000ff00) <<  8) | \
		 (((uint32_t)(x) & 0x000000ff) << 24))

	#define bswap64(x) \
		((((uint64_t)(x) & 0xff00000000000000ull) >> 56) | \
		 (((uint64_t)(x) & 0x00ff000000000000ull) >> 40) | \
		 (((uint64_t)(x) & 0x0000ff0000000000ull) >> 24) | \
		 (((uint64_t)(x) & 0x000000ff00000000ull) >>  8) | \
		 (((uint64_t)(x) & 0x00000000ff000000ull) <<  8) | \
		 (((uint64_t)(x) & 0x0000000000ff0000ull) << 24) | \
		 (((uint64_t)(x) & 0x000000000000ff00ull) << 40) | \
		 (((uint64_t)(x) & 0x00000000000000ffull) << 56))

#endif


#define LITTLE_ENDIAN	1234
#define BIG_ENDIAN		4321

#define LITTLE_ENDIAN_NAME	"litte endian"
#define BIG_ENDIAN_NAME		"big endian"


#ifndef BYTE_ORDER

	// based on boost/detail/endian.hpp
	#if defined(_LITTLE_ENDIAN) && !defined(_BIG_ENDIAN) || \
		defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__) || \
		defined(_STLP_LITTLE_ENDIAN) && !defined(_STLP_BIG_ENDIAN)

		#define BYTE_ORDER LITTLE_ENDIAN

	#elif defined(_BIG_ENDIAN) && !defined(_LITTLE_ENDIAN) || \
		defined(__BIG_ENDIAN__) && !defined(__LITTLE_ENDIAN__) || \
		defined(_STLP_BIG_ENDIAN) && !defined(_STLP_LITTLE_ENDIAN)

		#define BYTE_ORDER BIG_ENDIAN

	#elif defined(__sparc) || defined(__sparc__) || \
		defined(_POWER) || defined(__powerpc__) || \
		defined(__ppc__) || defined(__hpux) || defined(__hppa) || \
		defined(_MIPSEB) || defined(__s390__)

		#define BYTE_ORDER BIG_ENDIAN

	#elif defined(__i386__) || defined(__alpha__) || \
		defined(__ia64) || defined(__ia64__) || \
		defined(_M_IX86) || defined(_M_IA64) || \
		defined(_M_ALPHA) || defined(__amd64) || \
		defined(__amd64__) || defined(_M_AMD64) || \
		defined(__x86_64) || defined(__x86_64__) || \
		defined(_M_X64) || defined(__bfin__)

		#define BYTE_ORDER LITTLE_ENDIAN

	#else
		#error Unknown machine endianness detected.
	#endif

#endif // BYTE_ORDER


#ifndef BYTE_ORDER_NAME

	#if BYTE_ORDER == LITTLE_ENDIAN
		#define BYTE_ORDER_NAME LITTLE_ENDIAN_NAME
	#elif BYTE_ORDER == BIG_ENDIAN
		#define BYTE_ORDER_NAME BIG_ENDIAN_NAME
	#else
		#error Unknown machine endianness detected.
	#endif

#endif // BYTE_ORDER_NAME


#ifndef BO_ENDIAN_FUNC_DEFINED

	#if (BYTE_ORDER == LITTLE_ENDIAN)

		#define htole16(x) (uint16_t)(x)
		#define htole32(x) (uint32_t)(x)
		#define htole64(x) (uint64_t)(x)

		#define letoh16(x) (uint16_t)(x)
		#define letoh32(x) (uint32_t)(x)
		#define letoh64(x) (uint64_t)(x)

		#define htobe16(x) bswap16(x)
		#define htobe32(x) bswap32(x)
		#define htobe64(x) bswap64(x)

		#define betoh16(x) bswap16(x)
		#define betoh32(x) bswap32(x)
		#define betoh64(x) bswap64(x)

	#elif (BYTE_ORDER == BIG_ENDIAN)

		#define htole16(x) bswap16(x)
		#define htole32(x) bswap32(x)
		#define htole64(x) bswap64(x)

		#define letoh16(x) bswap16(x)
		#define letoh32(x) bswap32(x)
		#define letoh64(x) bswap64(x)

		#define htobe16(x) (uint16_t)(x)
		#define htobe32(x) (uint32_t)(x)
		#define htobe64(x) (uint64_t)(x)

		#define betoh16(x) (uint16_t)(x)
		#define betoh32(x) (uint32_t)(x)
		#define betoh64(x) (uint64_t)(x)

	#else
		#error Unknown machine endianness detected.
	#endif

#endif // BO_ENDIAN_FUNC_DEFINED


#define BO_ENDIAN_CPY_BYTESWAPS(func, bits, dest, src) \
	*(uint##bits##_t*)dest = func##bits(*(uint##bits##_t*)src)


#define htole16cpy(dest, src) BO_ENDIAN_CPY_BYTESWAPS(htole, 16, dest, src)
#define htole32cpy(dest, src) BO_ENDIAN_CPY_BYTESWAPS(htole, 32, dest, src)
#define htole64cpy(dest, src) BO_ENDIAN_CPY_BYTESWAPS(htole, 64, dest, src)

#define letoh16cpy(dest, src) BO_ENDIAN_CPY_BYTESWAPS(letoh, 16, dest, src)
#define letoh32cpy(dest, src) BO_ENDIAN_CPY_BYTESWAPS(letoh, 32, dest, src)
#define letoh64cpy(dest, src) BO_ENDIAN_CPY_BYTESWAPS(letoh, 64, dest, src)

#define htobe16cpy(dest, src) BO_ENDIAN_CPY_BYTESWAPS(htobe, 16, dest, src)
#define htobe32cpy(dest, src) BO_ENDIAN_CPY_BYTESWAPS(htobe, 32, dest, src)
#define htobe64cpy(dest, src) BO_ENDIAN_CPY_BYTESWAPS(htobe, 64, dest, src)

#define betoh16cpy(dest, src) BO_ENDIAN_CPY_BYTESWAPS(betoh, 16, dest, src)
#define betoh32cpy(dest, src) BO_ENDIAN_CPY_BYTESWAPS(betoh, 32, dest, src)
#define betoh64cpy(dest, src) BO_ENDIAN_CPY_BYTESWAPS(betoh, 64, dest, src)


static inline int ByteOrderTest() {
	volatile short int x = 0x1234;
	return *(char*)&x == 0x12 ? BIG_ENDIAN : LITTLE_ENDIAN;
}

static inline const char* ByteOrderName(int bo, const char* unknown) {
	if (bo == LITTLE_ENDIAN)
		return LITTLE_ENDIAN_NAME;
	if (bo == BIG_ENDIAN)
		return BIG_ENDIAN_NAME;
	return unknown;
}

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus

namespace impl {

template<size_t N> struct SizeOf {};

#define BO_ENDIAN_IMPL_TPL_BYTESWAPS(bits) \
	template<typename T> inline T htole(T n, SizeOf<bits/8>) { T v; htole##bits##cpy(&v, &n); return v; } \
	template<typename T> inline T letoh(T n, SizeOf<bits/8>) { T v; letoh##bits##cpy(&v, &n); return v; } \
	template<typename T> inline T htobe(T n, SizeOf<bits/8>) { T v; htobe##bits##cpy(&v, &n); return v; } \
	template<typename T> inline T betoh(T n, SizeOf<bits/8>) { T v; betoh##bits##cpy(&v, &n); return v; } \

BO_ENDIAN_IMPL_TPL_BYTESWAPS(16)
BO_ENDIAN_IMPL_TPL_BYTESWAPS(32)
BO_ENDIAN_IMPL_TPL_BYTESWAPS(64)

#undef BO_ENDIAN_IMPL_TPL_BYTESWAPS

} // namespace impl

template<typename T> inline T htole(T n) { return ::impl::htole(n, ::impl::SizeOf<sizeof(T)>()); }
template<typename T> inline T letoh(T n) { return ::impl::letoh(n, ::impl::SizeOf<sizeof(T)>()); }
template<typename T> inline T htobe(T n) { return ::impl::htobe(n, ::impl::SizeOf<sizeof(T)>()); }
template<typename T> inline T betoh(T n) { return ::impl::betoh(n, ::impl::SizeOf<sizeof(T)>()); }

#endif // __cplusplus

#endif // BO_ENDIAN_BYTEORDER_H
