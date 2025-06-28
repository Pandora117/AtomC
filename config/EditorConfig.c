#pragma once
#include <termios.h>

// Define a struct of type termios to modify the terminal's attributes
struct termios original_termios;

// Define a global struct to contain our Editor State
struct editorConfig {
    int cx, cy;
    int screenrows;
    int screencols;
    struct termios original_termios;
};

struct editorConfig E;