#include <stdio.h>
#include <unistd.h>

#include "components/RawMode.c"
#include "components/Editor.c"
#include "config/EditorConfig.c"
#include "components/TextViewer.c"
#include "components/Error.c"

void initEditor() {
    E.cx = 0;
    E.cy = 0;
    E.rx = 0;
    E.rowoff = 0;
    E.coloff = 0;
    E.numrows = 0;
    E.row = NULL;
    E.filename = NULL;
    E.statusmsg[0] = '\0';
    E.statusmsg_time = 0;

    if (getWindowSize(&E.screenrows, &E.screencols) == -1) die("getWindowSize");
    E.screenrows -= 2;
}

int main(int argc, char *argv[]) {
    enableRawMode();
    initEditor();
    if (argc >= 2) editorOpen(argv[1]);

    editorSetStatusMessage("HELP: CTRL+Q -> Quit");

    /* Enable the Terminal to enter RAW Mode by
    contineously reading from terminal 1 char at a time */
    while (1) {
        editorRefreshScreen();
        editorProcessKeypress();
    }

    return 0;
}