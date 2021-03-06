#include <sys/types.h>
#include <stdint.h>

#include "config.h"

#define finalize(a) bufferize(a, sizeof(a))
char *bufferize (char *, size_t);
char *minutes (int32_t);
char *hours (int32_t);

#define BUF_SIZE 4096
#define BUF_CHUNK 256

#if !HAVE_BZERO && HAVE_MEMSET
# define bzero(buf, bytes)      ((void) memset (buf, 0, bytes))
#endif
