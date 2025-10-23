#include "bitboard.h"
#include <ctype.h>

int bb_count(U64 v) {
    // builtin popcount if available; fallback simple
    #if defined(__GNUC__) || defined(__clang__)
        return __builtin_popcountll(v);
    #else
        int c = 0;
        while (v) { v &= (v-1); ++c; }
        return c;
    #endif
}

void bb_print(U64 v) {
    // Print with rank 8 at top to rank 1 at bottom
    for (int r = 7; r >= 0; --r) {
        for (int f = 0; f < 8; ++f) {
            int sq = r*8 + f;
            printf("%c ", ((v >> sq) & 1ULL) ? '1' : '.');
        }
        printf("\n");
    }
}

void bb_print_hex(U64 v) {
    printf("0x%016llX\n", (unsigned long long)v);
}

// avoid wrapping across files when shifting diagonally
U64 shift_nw(U64 bb) {              // +7 up-left
    U64 noA = bb & ~FILE_A;
    return noA << 7;
}
U64 shift_ne(U64 bb) {              // +9 up-right
    U64 noH = bb & ~FILE_H;
    return noH << 9;
}
U64 shift_sw(U64 bb) {              // -9 down-left
    U64 noA = bb & ~FILE_A;
    return noA >> 9;
}
U64 shift_se(U64 bb) {              // -7 down-right
    U64 noH = bb & ~FILE_H;
    return noH >> 7;
}

U64 dark_squares(void) {
    // dark squares are those where (rank+file) % 2 == 1 
    U64 mask = 0ULL;
    for (int r = 0; r < 8; ++r) {
        for (int f = 0; f < 8; ++f) {
            if (((r + f) & 1) == 0) continue; // if you prefer a1 dark, set to 1==0; adjust here
            int sq = r*8 + f;
            mask |= (1ULL << sq);
        }
    }
    return mask;
}

int algebra_to_sq(const char *alg) {
    if (!alg || !isalpha((unsigned char)alg[0]) || !isdigit((unsigned char)alg[1])) return -1;
    int file = tolower((unsigned char)alg[0]) - 'a'; // 0..7
    int rank = (alg[1] - '1');                       // 0..7 (a1 is bottom)
    if (file < 0 || file > 7 || rank < 0 || rank > 7) return -1;
    return rank*8 + file;
}

void sq_to_algebra(int sq, char out[3]) {
    if (!out) return;
    if (sq < 0 || sq > 63) { out[0] = out[1] = '?'; out[2] = '\0'; return; }
    int file = sq % 8;
    int rank = sq / 8;
    out[0] = 'a' + file;
    out[1] = '1' + rank;
    out[2] = '\0';
}
