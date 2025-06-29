#include <stdio.h>
#include <sys/types.h>
#include <string.h>

#include "../config/EditorConfig.c"
#include "Error.c"
#include "../config/Constants.c"


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


void editorAppendRows(char *s, size_t len) {
    E.row = realloc(E.row, sizeof(erow) * (E.numrows + 1));

    int at = E.numrows;
    E.row[at].size = len;
    E.row[at].chars = malloc(len + 1);
    memcpy(E.row[at].chars, s, len);
    E.row[at].chars[len] = '\0';

    E.row[at].rsize = 0;
    E.row[at].render = NULL;
    editorUpdateRows(&E.row[at]);

    E.numrows++;
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

        editorAppendRows(line, linelen);
    }

    free(line);
    fclose(fp);
}