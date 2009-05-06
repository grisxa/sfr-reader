#include <stdint.h>

#include "config.h"
#include "sfr.h"

#if HAVE_ENDIAN_H
#  include <endian.h>
#elif HAVE_MACHINE_ENDIAN_H
#  include <machine/endian.h>
#else
#  define LITTLE_ENDIAN	1234
#  define BIG_ENDIAN	4321
#  ifndef BYTE_ORDER
#    define BYTE_ORDER LITTLE_ENDIAN
#  endif
#endif

#if HAVE_BYTEORDER_H
#  include <byteorder.h>
#elif HAVE_ASM_BYTEORDER_H
#  include <asm/byteorder.h>
#endif

#ifndef le32toh
#  ifdef letoh32
#    define le32toh(x) letoh32(x)
#  elif defined(__APPLE__) /* Mac OS X */
//#include <libkern/OSByteOrder.h>
#    define le32toh(x) OSSwapLittleToHostInt32(x)
#  elif defined(__le32_to_cpu)
#    define le32toh(x) __le32_to_cpu(x)
#  else
/* __bswap_constant_32(x) from bits/byteswap.h */
#    define le32toh(x) \
	((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) |	\
	 (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))
#  endif
#endif

#ifndef le16toh
#  ifdef letoh16
#    define le16toh(x) letoh16(x)
#  elif defined(__APPLE__) /* Mac OS X */
//#include <libkern/OSByteOrder.h>
#    define le16toh(x) OSSwapLittleToHostInt16(x)
#  elif defined(__le16_to_cpu)
#    define le16toh(x) __le16_to_cpu(x)
#  else
/* __bswap_constant_16(x) from bits/byteswap.h */
#    define le16toh(x) \
	((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))
#  endif
#endif

#if BYTE_ORDER == BIG_ENDIAN
#  define revert_l(a) { uint32_t x = le32toh(a); a = x; }
#  define revert_s(a) { uint16_t x = le16toh(a); a = x; }
#else
# define revert_l(a) (a)
# define revert_s(a) (a)
#endif
void revert_title (struct Title *);
void revert_offsets (struct Offsets *);
void revert_group (struct Group *);
void revert_gday (struct GDayStr *);
void revert_distance (struct Distance *);
void revert_cpoint (struct CPoint *);
void revert_team (struct Team *);
void revert_cday (struct CDayStr *);
void revert_competitor (struct Competitor *);
void revert_tsp (struct Tsp *);
void revert_split (struct Split *);
