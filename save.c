#include "save.h"
#include <string.h>
#include <stdio.h>

// save this game state to a file
int save_game(const GameState *g, const char *path) {
    FILE *f = fopen(path, "w");
    if (!f) return 0;
    // the text format
    fprintf(f, "turn %d\n", g->turn);
    fprintf(f, "p1_men    0x%016llX\n", (unsigned long long)g->p1_men);
    fprintf(f, "p1_kings  0x%016llX\n", (unsigned long long)g->p1_kings);
    fprintf(f, "p2_men    0x%016llX\n", (unsigned long long)g->p2_men);
    fprintf(f, "p2_kings  0x%016llX\n", (unsigned long long)g->p2_kings);
    fclose(f);
    return 1;
}

// helper function to read hex values from save file
static int read_hex_line(FILE *f, const char *key, unsigned long long *out) {
    char k[32]; unsigned long long v;
    if (fscanf(f, "%31s %llx", k, &v) != 2) return 0;
    if (key && strcmp(k, key)!=0) return 0;
    *out = v;
    return 1;
}

// load a saved game from a file
int load_game(GameState *g, const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return 0;
    // read current turn
    int turn;
    if (fscanf(f, "turn %d\n", &turn) != 1) { fclose(f); return 0; }
    // read piece positions
    unsigned long long p1m, p1k, p2m, p2k;
    if (!read_hex_line(f, "p1_men",   &p1m)) { fclose(f); return 0; }
    if (!read_hex_line(f, "p1_kings", &p1k)) { fclose(f); return 0; }
    if (!read_hex_line(f, "p2_men",   &p2m)) { fclose(f); return 0; }
    if (!read_hex_line(f, "p2_kings", &p2k)) { fclose(f); return 0; }
    fclose(f);
    // restore game state
    g->turn = turn;
    g->p1_men   = (U64)p1m;
    g->p1_kings = (U64)p1k;
    g->p2_men   = (U64)p2m;
    g->p2_kings = (U64)p2k;
    return 1;
}
