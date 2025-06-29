#pragma once
#include <unistd.h>
#include <errno.h>

// Print a Descriptive Error Message in case of an error
void die(const char *s) {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    perror(s);
    exit(1);
}