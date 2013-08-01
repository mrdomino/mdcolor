#include <sys/types.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLOR_BLOCKS 16
#define BLOCKS_PER_ROW 4
#define DIM_STR "7px"

typedef struct {
  unsigned char r;
  unsigned char g;
  unsigned char b;
} rgb_t;

static const char *header =
  "<!doctype html>\n"
  "<style>div{display:inline-block;width:" DIM_STR ";height:" DIM_STR "}\n";
static const char *stylefmt = "#%c{background-color:%s}\n";
static const char *endheader = "</style>\n<body>";
static const char *divfmt = "<div id=%c></div>";
static const char *spacer = "<br>";

void
u16torgb(u_int16_t in, rgb_t *out)
{
  out->r = ((in & (0x1f << 11)) >> 11) << 3;
  out->g = ((in & (0x3f << 5)) >> 5)   << 2;
  out->b = ( in &  0x1f)               << 3;
}

void
mdcolor(void *md, size_t mdlen, rgb_t *out)
{
  int i;
  assert(mdlen >= sizeof(u_int16_t) * COLOR_BLOCKS);
  for (i = 0; i < COLOR_BLOCKS; i++) {
    u16torgb(((u_int16_t*)md)[i], &out[i]);
  }
}

char *
rgbtohex(rgb_t c)
{
  size_t len = strlen("#000000");
  char *ret = malloc(len + 1);
  snprintf(ret, len + 1, "#%02hhx%02hhx%02hhx", c.r, c.g, c.b);
  ret[len] = '\0';
  return ret;
}

void
print_page(rgb_t *cs)
{
  char *ss[COLOR_BLOCKS];
  int i;
  printf(header);
  for (i = 0; i < COLOR_BLOCKS; i++) {
    ss[i] = rgbtohex(cs[i]);
    printf(stylefmt, 'a'+i, ss[i]);
  }
  printf(endheader);
  for (i = 0; i < COLOR_BLOCKS; i++) {
    printf(divfmt, 'a' + i);
    if (i % BLOCKS_PER_ROW == BLOCKS_PER_ROW - 1)
      printf(spacer);
    free(ss[i]);
  }
  printf("\n");
}

int
main(int argc, char **argv)
{
  rgb_t c[COLOR_BLOCKS];
  int i;
  ssize_t r;
  char buf[sizeof(u_int16_t)*COLOR_BLOCKS];

  if (read(0, buf, sizeof buf) < sizeof buf) {
    return 1;
  }
  mdcolor(buf, sizeof buf, c);
  print_page(c);
  return 0;
}
