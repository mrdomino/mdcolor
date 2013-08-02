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

typedef struct {
	rgb_t c[COLOR_BLOCKS];
} mdcolor_t;


static void
usage(const char *progname)
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
mdcolor(const u_int16_t *md, size_t mdlen, mdcolor_t *out)
{
	int i;
	assert(mdlen == sizeof(u_int16_t) * COLOR_BLOCKS);
	for (i = 0; i < COLOR_BLOCKS; i++) {
		u16torgb(md[i], &out->c[i]);
	}
}

int
read_digest(const char *str, u_int16_t *out)
{
	if (strlen(str) != COLOR_BLOCKS * 4) {
		return -1;
	}
	do {
		if (sscanf(str, "%04hx", out) != 1) {
			return -1;
		}
		str += 4;
		out++;
	} while (*str != '\0');
	return 0;
}

int
main(int argc, char **argv)
{
	int i;
	mdcolor_t cs;
	u_int16_t buf[COLOR_BLOCKS];

	if (argc != 2 || read_digest(argv[1], buf) < 0) {
		usage(argv[0]);
		return 1;
	}
	mdcolor(buf, sizeof buf, &cs);
	for (i = 0; i < COLOR_BLOCKS; ++i) {
		printrgbhex(cs.c[i]);
	}
	return 0;
}
