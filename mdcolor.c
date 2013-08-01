#include <sys/types.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>


#define DIGEST_LENGTH 256
#define COLOR_BLOCKS (DIGEST_LENGTH / 16)

typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} rgb_t;


static void
usage(char *progname)
{
	fprintf(stderr, "usage: %s <sha256>\n", progname);
}

void
u16torgb(u_int16_t in, rgb_t *out)
{
	out->r = ((in & (0x1f << 11)) >> 11) << 3;
	out->g = ((in & (0x3f << 5)) >> 5)   << 2;
	out->b = ( in &  0x1f)               << 3;
}

void
printrgbhex(rgb_t c)
{
	printf("%02hhx%02hhx%02hhx\n", c.r, c.g, c.b);
}

void
mdcolor(const void *md, size_t mdlen, rgb_t *out)
{
	int i;
	assert(mdlen >= sizeof(u_int16_t) * COLOR_BLOCKS);
	for (i = 0; i < COLOR_BLOCKS; i++) {
		u16torgb(((u_int16_t*)md)[i], &out[i]);
	}
}

int
read_digest(const char *str, u_int16_t *out)
{
	int i;

	if (strlen(str) != COLOR_BLOCKS * 4) {
		return -1;
	}
	for (i = 0; i < COLOR_BLOCKS; i++) {
		if (sscanf(str, "%04hx", &out[i]) != 1) {
			return -1;
		}
		str += 4;
	}
	return 0;
}

int
main(int argc, char **argv)
{
	int i;
	rgb_t c[COLOR_BLOCKS];
	u_int16_t buf[COLOR_BLOCKS];

	if (argc != 2 || read_digest(argv[1], buf) < 0) {
		usage(argv[0]);
		return 1;
	}
	mdcolor(buf, sizeof buf, c);
	for (i = 0; i < COLOR_BLOCKS; ++i) {
		printrgbhex(c[i]);
	}
	return 0;
}
