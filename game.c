#include <string.h>
#include "bitboard.h"
#include "game.h"

// get empty squares on the board
static inline U64 empty_squares(const GameState *g) {
    return ~all_pieces(g);
}

// get all pieces from both players
U64 all_pieces(const GameState *g) {
    return g->p1_men | g->p1_kings | g->p2_men | g->p2_kings;
}

// get all pieces for a specific player
U64 player_pieces(const GameState *g, int player) {
    return (player==1) ? (g->p1_men | g->p1_kings) : (g->p2_men | g->p2_kings);
}

// get king pieces for a specific player
U64 player_kings(const GameState *g, int player) {
    return (player==1) ? g->p1_kings : g->p2_kings;
}

// check if a square is occupied
int square_occupied(const GameState *g, int sq) {
    return bb_get(all_pieces(g), sq);
}

// check if player owns piece at square
int own_piece_at(const GameState *g, int player, int sq) {
    U64 own = player_pieces(g, player);
    return bb_get(own, sq);
}

// check if there's a king at square
int is_king_at(const GameState *g, int player, int sq) {
    U64 kings = player_kings(g, player);
    return bb_get(kings, sq);
}

// initialize new game state
void game_init(GameState *g) {
    memset(g, 0, sizeof(*g));
    // Standard checkers setup: three ranks of men on dark squares for each side.
    U64 mask = dark_squares();

    // We'll place Player 1 (Red) at ranks 1..3 (bottom), Player 2 at ranks 6..8 (top)
    U64 p1 = 0ULL, p2 = 0ULL;
    for (int sq = 0; sq < 64; ++sq) {
        if (((mask >> sq) & 1ULL) == 0) continue;
        int r = sq / 8;
        if (r <= 2) p1 |= (1ULL << sq);
        else if (r >= 5) p2 |= (1ULL << sq);
    }
    // set initial positions
    g->p1_men = p1; g->p1_kings = 0ULL;
    g->p2_men = p2; g->p2_kings = 0ULL;
    g->turn = 1;
}

// convert square number to algebraic notation
static void print_square(int sq) {
    char a[3]; sq_to_algebra(sq, a); printf("%s", a);
}

// display the current game board
void game_print(const GameState *g) {
    printf("\n    a b c d e f g h\n");
    for (int r = 7; r >= 0; --r) {
        printf("%d | ", r+1);
        for (int f = 0; f < 8; ++f) {
            int sq = r*8 + f;
            // display pieces with different symbols
            char ch = '.';
            if (bb_get(g->p1_men, sq)) ch = 'r';
            if (bb_get(g->p1_kings, sq)) ch = 'R';
            if (bb_get(g->p2_men, sq)) ch = 'b';
            if (bb_get(g->p2_kings, sq)) ch = 'B';
            printf("%c ", ch);
        }
        printf("| %d\n", r+1);
    }
    printf("    a b c d e f g h\n");
    printf("Turn: Player %d\n", g->turn);
}

// check if game is over and determine winner
int game_is_over(const GameState *g, int *winner) {
    // count pieces for each player
    int p1_count = bb_count(g->p1_men | g->p1_kings);
    int p2_count = bb_count(g->p2_men | g->p2_kings);
    if (p1_count == 0 && p2_count == 0) { if (winner) *winner = 0; return 1; }
    if (p1_count == 0) { if (winner) *winner = 2; return 1; }
    if (p2_count == 0) { if (winner) *winner = 1; return 1; }
    return 0;
}

// check if a move is legal
int is_legal_move(const GameState *g, Move m) {
    // basic move validation
    if (!in_bounds(m.from) || !in_bounds(m.to) || m.from == m.to) return 0;
    if (!own_piece_at(g, g->turn, m.from)) return 0;
    if (square_occupied(g, m.to)) return 0;

    int king = is_king_at(g, g->turn, m.from);
    int dr = (m.to/8) - (m.from/8);
    int df = (m.to%8) - (m.from%8);
    int adr = dr>0?dr:-dr, adf = df>0?df:-df;

    // check simple move
    if (adr == 1 && adf == 1 && m.capture == -1) {
        if (!king) {
            if (g->turn == 1 && dr != 1) return 0;
            if (g->turn == 2 && dr != -1) return 0;
        }
        return 1; // simple step
    }
    // check capture move
    if (adr == 2 && adf == 2 && m.capture >= 0) {
        int mid_r = (m.from/8) + (dr/2);
        int mid_f = (m.from%8) + (df/2);
        int mid_sq = mid_r*8 + mid_f;
        if (mid_sq != m.capture) return 0;
        // check captured piece
        U64 opp = (g->turn==1) ? (g->p2_men | g->p2_kings) : (g->p1_men | g->p1_kings);
        if (!bb_get(opp, mid_sq)) return 0;
        if (!king) {
            if (g->turn == 1 && dr != 2) return 0;
            if (g->turn == 2 && dr != -2) return 0;
        }
        return 1;
    }
    return 0;
}

// remove a piece from the board
static void remove_piece(GameState *g, int sq) {
    g->p1_men   = bb_clear(g->p1_men, sq);
    g->p1_kings = bb_clear(g->p1_kings, sq);
    g->p2_men   = bb_clear(g->p2_men, sq);
    g->p2_kings = bb_clear(g->p2_kings, sq);
}

// apply a move to the game state
int apply_move(GameState *g, Move m, int *promoted, int *captured) {
    // validate move
    if (!is_legal_move(g, m)) return 0;
    if (promoted) *promoted = 0;
    if (captured) *captured = 0;

    int player = g->turn;
    int is_king = is_king_at(g, player, m.from);

    // update piece position
    if (player == 1) {
        if (bb_get(g->p1_men, m.from)) { g->p1_men = bb_clear(g->p1_men, m.from); g->p1_men = bb_set(g->p1_men, m.to); }
        else { g->p1_kings = bb_clear(g->p1_kings, m.from); g->p1_kings = bb_set(g->p1_kings, m.to); }
    } else {
        if (bb_get(g->p2_men, m.from)) { g->p2_men = bb_clear(g->p2_men, m.from); g->p2_men = bb_set(g->p2_men, m.to); }
        else { g->p2_kings = bb_clear(g->p2_kings, m.from); g->p2_kings = bb_set(g->p2_kings, m.to); }
    }

    // handle capture
    if (m.capture >= 0) {
        remove_piece(g, m.capture);
        if (captured) *captured = 1;
    }

    // check for promotion
    int r_to = m.to/8;
    if (!is_king) {
        if (player == 1 && r_to == 7 && bb_get(g->p1_men, m.to)) {
            g->p1_men = bb_clear(g->p1_men, m.to);
            g->p1_kings = bb_set(g->p1_kings, m.to);
            if (promoted) *promoted = 1;
        } else if (player == 2 && r_to == 0 && bb_get(g->p2_men, m.to)) {
            g->p2_men = bb_clear(g->p2_men, m.to);
            g->p2_kings = bb_set(g->p2_kings, m.to);
            if (promoted) *promoted = 1;
        }
    }
    // next player's turn
    g->turn = (g->turn == 1) ? 2 : 1;
    return 1;
}
