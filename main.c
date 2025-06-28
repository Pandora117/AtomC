#include <stdio.h>
#include <unistd.h>

#include "components/RawMode.c"

int main() {
    enableRawMode();
    
    /* Enable the Terminal to enter RAW Mode by
    contineously reading from terminal 1 char at a time
    and also quit RAW Mode when we Enter 'q' */
    while (1) {
        char c = '\0';
        if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) die("read\n");

        // Check for Control Characters (ASCII 0-27)
        // Else, print the char and its ASCII Value
        if (iscntrl(c)) {
            printf("%d\r\n", c);
        } else {
            printf("%d ('%c')\r\n", c, c);
        }

        if (c == 'q') break;
    }

    return 0;
}