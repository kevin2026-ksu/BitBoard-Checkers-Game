#ifndef GAME_H
#define GAME_H

#include "bitboard.h"

typedef struct {
    U64 p1_men;
    U64 p1_kings;
    U64 p2_men;
    U64 p2_kings;
    int turn; // 1 or 2
} GameState; 

// A move: from->to; if capture, which square captured; supports multi-capture chains via repeated calls
typedef struct {
    int from;
    int to;
    int capture; // -1 if none
} Move;

// Setup & display
void game_init(GameState *g);
void game_print(const GameState *g);

// Queries
int  game_is_over(const GameState *g, int *winner);
int  square_occupied(const GameState *g, int sq);
int  own_piece_at(const GameState *g, int player, int sq);
int  is_king_at(const GameState *g, int player, int sq);

// Make/validate
int  is_legal_move(const GameState *g, Move m);
int  apply_move(GameState *g, Move m, int *promoted, int *captured);

// Helpers
U64  all_pieces(const GameState *g);
U64  player_pieces(const GameState *g, int player);
U64  player_kings(const GameState *g, int player);

#endif
