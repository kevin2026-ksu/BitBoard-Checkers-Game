#ifndef BITBOARD_H
#define BITBOARD_H

#include <stdint.h>
#include <stdio.h>

// 64-bit bitboard
typedef uint64_t U64;

// File masks to avoid wrap around on left/right diagonals
#define FILE_A 0x0101010101010101ULL
#define FILE_H 0x8080808080808080ULL

// Board utilities
static inline int in_bounds(int sq) { return sq >= 0 && sq < 64; }
static inline U64 bit(int sq)       { return (sq>=0 && sq<64) ? (1ULL << sq) : 0ULL; }

// Basic bit ops (Phase 1 core)
static inline U64 bb_set(U64 v, int pos)    { return v | bit(pos); }
static inline U64 bb_clear(U64 v, int pos)  { return v & ~bit(pos); }
static inline U64 bb_toggle(U64 v, int pos) { return v ^ bit(pos); }
static inline int  bb_get(U64 v, int pos)   { return (int)((v >> pos) & 1ULL); }

int  bb_count(U64 v);                // popcount
void bb_print(U64 v);                // binary-ish (8x8)
void bb_print_hex(U64 v);

// Shifts that guard against horizontal wrap
U64 shift_nw(U64 bb); // +7
U64 shift_ne(U64 bb); // +9
U64 shift_sw(U64 bb); // -9
U64 shift_se(U64 bb); // -7

// Helpers
U64 dark_squares(void);     
int  algebra_to_sq(const char *alg); 
void sq_to_algebra(int sq, char out[3]);

#endif
