#pragma once
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "../config/EditorConfig.c"
#include "Error.c"
#include "../config/Constants.c"


void editorSetStatusMessage(const char *fmt, ...);
void editorRefreshScreen();
char *editorPrompt(char *prompt);
int editorReadKey();


void editorUpdateRows(erow *row) {
    int tabs = 0;
    int j;
    for (j = 0; j < row->size; j++) if (row->chars[j] == '\t') tabs++;

    free(row->render);
    row->render = malloc(row->size + tabs * (TAB_STOP - 1) + 1);

    int index = 0;
    for (j = 0; j < row->size; j++) {
        if (row->chars[j] == '\t') {
            row->render[index++] = ' ';
            while (index % TAB_STOP != 0) row->render[index++] = ' ';
        } else row->render[index++] = row->chars[j];
    }

    row->render[index] = '\0';
    row->rsize = index;
}


void editorInsertRows(int at, char *s, size_t len) {
    if (at < 0 || at > E.numrows) return;

    E.row = realloc(E.row, sizeof(erow) * (E.numrows + 1));
    memmove(&E.row[at + 1], &E.row[at], sizeof(erow) * (E.numrows - at));

    E.row[at].size = len;
    E.row[at].chars = malloc(len + 1);
    memcpy(E.row[at].chars, s, len);
    E.row[at].chars[len] = '\0';

    E.row[at].rsize = 0;
    E.row[at].render = NULL;
    editorUpdateRows(&E.row[at]);

    E.numrows++;
    E.dirty++;
}


void editorOpen(char *filename) {
    free(E.filename);
    E.filename = strdup(filename);
    
    FILE *fp = fopen(filename, "r");
    if (!fp) die ("fopen");

    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    
    while ((linelen = getline(&line, &linecap, fp)) != -1) {
        while (linelen > 0 && (line[linelen - 1] == '\n' || line[linelen - 1] == '\r')) linelen--;

        editorInsertRows(E.numrows, line, linelen);
    }

    free(line);
    fclose(fp);
    E.dirty = 0;
}


char *editorRowsToString(int *buflen) {
    int totlen = 0;
    int j;
    for (j = 0; j < E.numrows; j++) totlen += E.row[j].size + 1;
    *buflen = totlen;

    char *buf = malloc(totlen);
    char *p = buf;
    for (j = 0; j < E.numrows; j++) {
        memcpy(p, E.row[j].chars, E.row[j].size);
        p += E.row[j].size;
        *p = '\n';
        p++;
    }

    return buf;
}

void editorSave() {
    if (E.filename == NULL) {
        E.filename = editorPrompt("Save as: %s");

        if (E.filename == NULL) {
            editorSetStatusMessage("Save Aborted");
            return;
        }
    }

    int len;
    char *buf = editorRowsToString(&len);

    int fd = open(E.filename, O_RDWR | O_CREAT, 0644);
    if (fd != -1) {
        if (ftruncate(fd, len) != -1) {
            if (write(fd, buf, len) == len) {
                close(fd);
                free(buf);
                E.dirty = 0;
                editorSetStatusMessage("%d Bytes Written to Disk", len);
                return;
            }
        }
        close(fd);
    }

    free(buf);
    editorSetStatusMessage("Save Failed!!! IO Error: %s", strerror(errno));
}


char *editorPrompt(char *prompt) {
    size_t bufsize = 128;
    char *buf = malloc(bufsize);

    size_t buflen = 0;
    buf[0] = '\0';

    while (1) {
        editorSetStatusMessage(prompt, buf);
        editorRefreshScreen();

        int c = editorReadKey();
        if ((c == DEL_KEY) || (c == CTRL_KEY('h')) || (c == 127)) {
            if (buflen != 0) buf[--buflen] = '\0';
        }
        else if (c == '\x1b') {
            editorSetStatusMessage("");
            free(buf);
            return NULL;
        } else if (c == '\r') {
            if (buflen != 0) {
                editorSetStatusMessage("");
                return buf;
            }
        } else if (!iscntrl(c) && c < 128) {
            if (buflen == bufsize - 1) {
                bufsize *= 2;
                buf = realloc(buf, bufsize);
            }
            buf[buflen++] = c;
            buf[buflen] = '\0';
        }
    }
}