#pragma once
#include <termios.h>

// Define a struct of type termios to modify the terminal's attributes
struct termios original_termios;

// A Data Structure to store a row of TEXT in our Editor
typedef struct erow {
    int size;
    char *chars;
} erow;

// Define a global struct to contain our Editor State
struct editorConfig {
    int cx, cy;
    int screenrows;
    int screencols;
    int numrows;
    erow row;
    struct termios original_termios;
};

struct editorConfig E;