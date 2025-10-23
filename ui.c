#include "ui.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "game.h"

// parse and handle user input commands
int parse_command(const char *line, Move *m, char *fname, int fname_sz) {
    // skip empty lines and leading spaces
    if (!line) return 0;
    while (isspace((unsigned char)*line)) ++line;

    // handle quit commands
    if (strncmp(line, "quit", 4) == 0) return 4;
    if (strncmp(line, "exit", 4) == 0) return 4;

    // handle save game command
    if (strncmp(line, "save", 4) == 0) {
        const char *p = line + 4;
        while (isspace((unsigned char)*p)) ++p;
        if (*p && fname && fname_sz > 0) {
            snprintf(fname, fname_sz, "%s", p);
            // trim trailing newline
            size_t n = strlen(fname);
            while (n && (fname[n-1]=='\n' || fname[n-1]=='\r' || isspace((unsigned char)fname[n-1]))) { fname[n-1]=0; --n; }
            return 2;
        }
        return 0;
    }
    // handle load game command
    if (strncmp(line, "load", 4) == 0) {
        const char *p = line + 4;
        while (isspace((unsigned char)*p)) ++p;
        if (*p && fname && fname_sz > 0) {
            snprintf(fname, fname_sz, "%s", p);
            size_t n = strlen(fname);
            while (n && (fname[n-1]=='\n' || fname[n-1]=='\r' || isspace((unsigned char)fname[n-1]))) { fname[n-1]=0; --n; }
            return 3;
        }
        return 0;
    }

    // parse move commands
    char from[3]={0}, to[3]={0};
    int i=0, j=0; char sep=0;
    // get source square
    while (line[i] && !sep && i<2) { if (!isspace((unsigned char)line[i])) from[j++]=line[i]; ++i; if (j==2) break; }
    while (isspace((unsigned char)line[i])) ++i;
    // check move type
    if (line[i]=='@' || line[i]=='x' || line[i]=='X') { sep = line[i]; ++i; } else return 0;
    while (isspace((unsigned char)line[i])) ++i;
    // get target square
    j=0;
    while (line[i] && j<2) { if (!isspace((unsigned char)line[i])) to[j++]=line[i]; ++i; if (j==2) break; }

    // convert algebra to square numbers
    int from_sq = algebra_to_sq(from);
    int to_sq   = algebra_to_sq(to);
    if (from_sq < 0 || to_sq < 0) return 0;

    // set move properties
    m->from = from_sq;
    m->to   = to_sq;
    if (sep == '@' ) m->capture = -1;
    else {
        // calculate captured piece square for jumps
        int dr = (to_sq/8) - (from_sq/8);
        int df = (to_sq%8) - (from_sq%8);
        if ((dr==2 || dr==-2) && (df==2 || df==-2)) {
            int mid_sq = (from_sq + to_sq)/2;
            m->capture = mid_sq;
        } else {
            m->capture = -1; // not a capture move
        }
    }
    return 1;
}
