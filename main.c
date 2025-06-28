#include <stdio.h>
#include <unistd.h>

#include "components/RawMode.c"
#include "components/RawIO.c"
#include "config/EditorConfig.c"

void initEditor() {
    E.cx = 0;
    E.cy = 0;
    if (getWindowSize(&E.screenrows, &E.screencols) == -1) die("getWindowSize");
}

int main() {
    enableRawMode();
    initEditor();
    
    /* Enable the Terminal to enter RAW Mode by
    contineously reading from terminal 1 char at a time */
    while (1) {
        editorRefreshScreen();
        editorProcessKeypress();
    }

    return 0;
}