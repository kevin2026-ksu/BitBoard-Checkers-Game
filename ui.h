#ifndef UI_H
#define UI_H
#include "game.h"

// parse user commands:
// a3@b4 - simple move
// a3xc5 - capture move
// save filename - save game
// load filename - load game
// quit or exit - end game
int parse_command(const char *line, Move *m, char *fname, int fname_sz);

#endif
