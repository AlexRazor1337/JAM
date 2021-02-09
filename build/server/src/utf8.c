#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

typedef struct {
	char mask;    /* char data will be bitwise AND with this */
	char lead;    /* start bytes of current char in utf-8 encoded character */
	uint32_t beg; /* beginning of codepoint range */
	uint32_t end; /* end of codepoint range */
	int bits_stored; /* the number of bits from the codepoint that fits in char */
}utf_t;

utf_t * utf[] = {
	/*             mask        lead        beg      end       bits */
	[0] = &(utf_t){63, 128, 0,       0,        6    },
	[1] = &(utf_t){127, 0, 0,    127,     7    },
	[2] = &(utf_t){31, 192, 128,    2047,    5    },
	[3] = &(utf_t){15, 224, 2048,   65535,  4    },
	[4] = &(utf_t){17, 240, 65536, 1114111, 3    },
	      &(utf_t){0},
};

/* All lengths are in bytes */
int codepoint_len(const uint32_t cp); /* len of associated utf-8 char */
int utf8_len(const char ch);          /* len of utf-8 encoded char */

char *to_utf8(const uint32_t cp);
uint32_t to_cp(const char chr[4]);

int count_utf8_code_points(const char *s) {
    int count = 0;
    while (*s) {
        count += (*s++ & 0xC0) != 0x80;
    }
    return count;
}

int codepoint_len(const uint32_t cp) {
	int len = 0;
	for(utf_t **u = utf; *u; ++u) {
		if((cp >= (*u)->beg) && (cp <= (*u)->end)) {
			break;
		}
		++len;
	}
	if (len > 4) /* Out of bounds */
		return -1; //exit(1);

	return len;
}

int utf8_len(const char ch)
{
	int len = 0;
	for(utf_t **u = utf; *u; ++u) {
		if((ch & ~(*u)->mask) == (*u)->lead) {
			break;
		}
		++len;
	}
	if (len > 4) { /* Malformed leading byte */
		return -1; //exit(1);
	}
	return len;
}

char *to_utf8(const uint32_t cp)
{

	static char ret[5];
	const int bytes = codepoint_len(cp);

	int shift = utf[0]->bits_stored * (bytes - 1);
	ret[0] = (cp >> shift & utf[bytes]->mask) | utf[bytes]->lead;
	shift -= utf[0]->bits_stored;
	for(int i = 1; i < bytes; ++i) {
		ret[i] = (cp >> shift & utf[0]->mask) | utf[0]->lead;
		shift -= utf[0]->bits_stored;
	}
	ret[bytes] = '\0';
	return ret;
}

uint32_t to_cp(const char chr[4])
{
	int bytes = utf8_len(*chr);
	int shift = utf[0]->bits_stored * (bytes - 1);
	uint32_t codep = (*chr++ & utf[bytes]->mask) << shift;

	for(int i = 1; i < bytes; ++i, ++chr) {
		shift -= utf[0]->bits_stored;
		codep |= ((char)*chr & utf[0]->mask) << shift;
	}

	return codep;
}

