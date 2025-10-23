# BitBoard Checkers Game

### Course: CS 3503 – Project 1: BitBoard Checkers  
### Author: Kevin Lara

## Description
This project is a playable checkers game written in C.  
Each piece position is stored in a 64-bit unsigned integer (`uint64_t`), allowing the program to complete bitwise operations to manage the game board.  

---

## Features
- Bitwise manipulation functions (`set`, `clear`, `toggle`, `count`, etc.)
- 64-bit board representation using unsigned integers
- Turn-based gameplay
- Mandatory captures and jumping chaining
- Automatic king promotion (when reaching the last rank)
- Save and load system 
- A modular design was used

---

## Compilation

### Using GCC
Make sure all `.c` and `.h` files are in the same folder, then open a terminal in that folder.

**Windows (PowerShell)**
```bash
gcc -O2 -std=c11 -Wall -Wextra -o checkers main.c game.c ui.c save.c bitboard.c
.\checkers
```


# Game Rules – BitBoard Checkers

This version of Checkers follow the basic rulea of checkers. Where in each piece is only able to move diagonally and forward, except for the kings. There is the added fetures of enforced captures and king promotion.

---

## Objective
Be the first player to capture all of your opponent’s pieces,  
or leave them with no legal moves possible.

---

## Players
- **Player 1 (r / R)** – Red pieces start at the bottom and move **upward**.
- **Player 2 (b / B)** – Black pieces start at the top and move **downward**.

Lowercase = normal piece  
Uppercase = king


