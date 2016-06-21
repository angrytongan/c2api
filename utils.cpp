#include <stdio.h>

#define HEXDUMP_WIDTH   16

/*
 * Dump ascii part of a hexdump line.
 */
static void hexdump_readable(unsigned char *b, unsigned char *s) {
    int i = 3 * (HEXDUMP_WIDTH - (b - s));

    if (s == b) {
        return;
    }

    printf("    ");
    while (i--) {
        printf(" ");
    }

    for (; s != b; s++) {
        printf("%c", (*s >= 0x20 && *s <= 0x7e) ? *s : '.');
    }

    printf("\n");
}

/*
 * Dump a buffer in hexdump / ascii format.
 */
void hexdump(unsigned char *buff, unsigned int len) {
    int i;
    unsigned char *b;

    for (b = buff, i = 0; b != &buff[len]; ) {
        printf("%02x ", *b++);
        if (++i == HEXDUMP_WIDTH) {
            hexdump_readable(b, b - HEXDUMP_WIDTH);
            i = 0;
        }
    }

    hexdump_readable(b, b - (len % HEXDUMP_WIDTH));
}

/*
 * Convert seconds to hours, minutes, seconds.
 */
void dhms(unsigned int seconds, unsigned int *d, unsigned int *h, unsigned int *m,
        unsigned int *s) {
    *d = seconds / (60 * 60 * 24);
    seconds %= (60 * 60 * 24);

    *h = seconds / (60 * 60);
    seconds %= (60 * 60);

    *m = seconds / 60;
    *s = seconds % 60;
}

/* pack a value v into LSB -> MSB buffer of length l */
void val2bytes(unsigned int v, unsigned int l, unsigned char *b) {
    unsigned int i;
    for (i = 0; i < l; i++) {
        b[i] = (v >> (8 * i)) & 0xff;
    }
}

/* unpack LSB -> MSB buffer of length l into value v */
void bytes2val(unsigned char *b, unsigned int l, unsigned int *v) {
    unsigned int i;
    *v = 0;
    for (i = 0; i < l; i++) {
        *v += b[i] << (8 * i);
    }
}

#ifdef UNITTEST_UTILS

void print_time(unsigned int seconds, unsigned int d, unsigned int h, unsigned int m,
        unsigned int s) {
    printf("%02d days, %02d:%02d:%02d : %d\n", d, h, m, s, seconds);
}

int main(void) {
    int i;
    unsigned char buff[128];

    unsigned int d, h, m, s;
    unsigned int times[] = {
        1, 59, 60, 61, 119, 120, 121, 1000,
        86399, 86400, 86401,
        172799, 172800, 172801
    };

    for (i = 0; i < sizeof(buff); i++) {
        buff[i] = i & 0xff;
    }

    hexdump(buff, sizeof(buff));
    hexdump(buff, 1);
    hexdump(buff, 5);
    hexdump(buff, 27);
    hexdump(buff, 64);

    for (i = 0; i < sizeof(times) / sizeof(unsigned int); i++) {
        dhms(times[i], &d, &h, &m, &s);
        print_time(times[i], d, h, m, s);
    }

    return 0;
}
#endif /* UNITTEST */
