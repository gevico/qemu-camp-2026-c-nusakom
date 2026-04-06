#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned char *bits;
    size_t m;
} Bloom;

static Bloom *bloom_create(size_t m) {
    Bloom *bf = malloc(sizeof(Bloom));
    if (!bf) return NULL;
    bf->m = m;
    bf->bits = calloc((m + 7) / 8, 1);
    if (!bf->bits) { free(bf); return NULL; }
    return bf;
}

static void bloom_free(Bloom *bf) {
    if (bf) { free(bf->bits); free(bf); }
}

static void set_bit(unsigned char *bm, size_t idx) {
    bm[idx / 8] |= (1u << (idx % 8));
}

static int test_bit(const unsigned char *bm, size_t idx) {
    return (bm[idx / 8] >> (idx % 8)) & 1;
}

static size_t hash_k(const char *s, size_t m, int k) {
    unsigned long h = 5381 * (unsigned long)(k + 1);
    const unsigned char *p = (const unsigned char *)s;
    while (*p) { h = h * 33 + *p * (unsigned long)(k + 1); p++; }
    return h % m;
}

static void bloom_add(Bloom *bf, const char *s) {
    for (int k = 0; k < 3; k++)
        set_bit(bf->bits, hash_k(s, bf->m, k));
}

static int bloom_maybe_contains(Bloom *bf, const char *s) {
    for (int k = 0; k < 3; k++)
        if (!test_bit(bf->bits, hash_k(s, bf->m, k))) return 0;
    return 1;
}

int main(void) {
    const size_t m = 100;
    Bloom *bf = bloom_create(m);
    if (!bf) { fprintf(stderr, "bloom create failed\n"); return 1; }

    bloom_add(bf, "apple");
    bloom_add(bf, "banana");

    int apple = bloom_maybe_contains(bf, "apple");
    int grape = bloom_maybe_contains(bf, "grape");

    printf("apple exists: %d\n", apple);
    printf("grape exists: %d\n", grape);

    bloom_free(bf);
    return 0;
}
