#ifndef UTILS_H
#define UTILS_H

void hexdump(unsigned char *buff, unsigned int len);
void dhms(unsigned int seconds, unsigned int *d, unsigned int *h, unsigned int *m, unsigned int *s);

void val2bytes(unsigned int v, unsigned int l, unsigned char *b);
void bytes2val(unsigned char *b, unsigned int l, unsigned int *v);

#endif /* UTILS_H */
