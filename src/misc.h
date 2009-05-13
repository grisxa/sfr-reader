#include <sys/types.h>
#include <stdint.h>

#define finalize(a) bufferize(a, sizeof(a))
char *bufferize (char *, size_t);
char *minutes (int32_t);
char *hours (int32_t);
#define totime(a) minutes(a)

#define BUF_SIZE 4096
#define BUF_CHUNK 256
