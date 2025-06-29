#pragma once

#define ATOMC_VERSION "0.0.1"
#define CTRL_KEY(k) ((k) & 0x1f)
#define TAB_STOP 8
#define QUIT_TIMES 3

enum editorKey {
    BACKSPACE = 127,
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    DEL_KEY,
    HOME_KEY,
    END_KEY,
    PAGE_UP,
    PAGE_DOWN
};