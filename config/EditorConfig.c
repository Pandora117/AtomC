#pragma once
#include <termios.h>
#include <time.h>

// Define a struct of type termios to modify the terminal's attributes
struct termios original_termios;

// A Data Structure to store a row of TEXT in our Editor
typedef struct erow {
    int size;
    int rsize;
    char *chars;
    char *render;
} erow;

// Define a global struct to contain our Editor State
struct editorConfig {
    int cx, cy;
    int rx;
    int rowoff;
    int coloff;
    int screenrows;
    int screencols;
    int numrows;
    erow *row;
    int dirty;
    char *filename;
    char statusmsg[80];
    time_t statusmsg_time;
    struct termios original_termios;
};

struct editorConfig E;