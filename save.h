#ifndef SAVE_H
#define SAVE_H

#include "game.h"

int save_game(const GameState *g, const char *path);
int load_game(GameState *g, const char *path);

#endif
