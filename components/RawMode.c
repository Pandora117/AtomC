#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#include "../config/EditorConfig.c"
#include "error.c"

// Disable RAW Mode when Exiting
void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.original_termios) == -1) die("tcsetattr\n");
}

// This method turns off ECHO in the terminal
// Hence, we do not see anything printed when we type
void enableRawMode() {
    // Get the Current Terminal Attributes into our struct
    tcgetattr(STDIN_FILENO, &E.original_termios);
    atexit(disableRawMode);
    
    struct termios raw = E.original_termios;
    // Modify the terminal attributes to turn off
    /*
        Flag        Description
        ----------------------------------------------------------------------------------
        ECHO        Modify Echo in Terminal
        ICANON      Toggle Canonical Mode in Terminal
        IEXTEN      Modify Ctrl+V & Ctrl+O, which will be read as Bytes
        ISIG        Modify Ctrl+C & Ctrl+Z, which will be read as Bytes
        IXON        Modify Ctrl+S & Ctrl+Q, which are used to pause and resume transmission
        ICRNL       Modify Ctrl+M & Enter, which are used for newlines
        OSOST       Toggle ON/OFF all Output Processing
        BRKINT, INPCK, ISTRIP, CS8  Miscellanous Flags for RAW Mode (Depracated)
    */
   
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag &= ~(CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    
    // Set Timeout for read()
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    // Set the modified attributes back to the terminal
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr\n");
}
