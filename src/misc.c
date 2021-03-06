#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>

#include "misc.h"

char *bufferize (char *str, size_t size)
{
  // round buffer for non-terminated strings
  static char buf[BUF_SIZE] = "\0\0", *ptr = buf;
  if (size > BUF_CHUNK - 1)
    size = BUF_CHUNK - 1;

  // move cursor beyond the current string
  ptr += strlen (ptr) + 1;
  // if there is no place, go to beginning
  if (ptr - buf > sizeof (buf) - size - 1)
    ptr = buf;

  // prepare buffer chunk
  bzero (ptr, size + 1);
  // store a string
  strncpy (ptr, str, size);
  return ptr;
}

char *minutes (int32_t stamp)
{
  char buf[16];
  char sign[2] = "\0\0";

  if (stamp < 0) {
    sign[0] = '-';
    stamp = -stamp;
  }

  int min = (int) (stamp / 1000 / 60);
  int sec = (int) (stamp / 1000 - min * 60);

  snprintf (buf, sizeof (buf), "%s%02d:%02d", sign, min, sec);
  return bufferize (buf, sizeof (buf));
}

char *hours (int32_t stamp)
{
  char buf[16];
  char sign[2] = "\0\0";

  if (stamp < 0) {
    sign[0] = '-';
    stamp = -stamp;
  }

  int hour = (int) (stamp / 1000 / 60 / 60);
  int min = (int) (stamp / 1000 / 60 - hour * 60);
  int sec = (int) (stamp / 1000 - min * 60 - hour * 60 * 60);

  snprintf (buf, sizeof (buf), "%s%02d:%02d:%02d", sign, hour, min, sec);
  return bufferize (buf, sizeof (buf));
}

