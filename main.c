#include <stdio.h>
#include <string.h>
#include "game.h"
#include "ui.h"
#include "save.h"

static void help(void) {
    printf("Rules:\n");
    printf("Jump all of your opponent's pieces or block them so they can't move.\n");
    printf("Player 1 = lowercase r (red pieces, bottom)\n");
    printf("Player 2 = lowercase b (black pieces, top\n");
    printf("Uppercase letters R and B are kings.\n");

    printf("\n");


    printf("Example:       Commands:\n");
    printf("b3@a4          simple piece move (@)\n");
    printf("a3xc5          capture move (x)\n");
    printf("save 'file'   save game to 'filename'\n");
    printf("load 'file'   load game from 'filename'\n");
    printf("quit           exit\n");
}

int main(void) {
    GameState g;
    game_init(&g);
    printf("Bitboard Checkers\n");
    help();
    game_print(&g);

    char line[128];
    while (1) {
        int winner;
        if (game_is_over(&g, &winner)) {
            if (winner==0) printf("Game over: Its a draw =(\n");
            else printf("Game over: Player %d wins\n =)", winner);
            break;
        }
        printf("Player %d> ", g.turn);
        if (!fgets(line, sizeof(line), stdin)) break;

        Move m; char fname[96];
        int kind = parse_command(line, &m, fname, sizeof(fname));
        if (kind == 4) { printf("Bye!\n"); break; }
        else if (kind == 2) { if (save_game(&g, fname)) printf("Saved to %s\n", fname); else printf("Save failed\n"); continue; }
        else if (kind == 3) { if (load_game(&g, fname)) { printf("Loaded %s\n", fname); game_print(&g); } else printf("Load failed\n"); continue; }
        else if (kind == 1) {
            int promo=0, cap=0;
            if (apply_move(&g, m, &promo, &cap)) {
                if (cap) printf("Captured!\n");
                if (promo) printf("Promoted to king!\n");
                game_print(&g);
            } else {
                printf("Illegal move try again.\n");
            }
        } else {
            printf("Incorrect Input.\n");
            help();
        }
    }
    return 0;
}
