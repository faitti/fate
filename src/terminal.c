#include "../include/terminal.h"
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

struct termios origin;

void fate_disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &origin);
}

void fate_enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &origin);
    atexit(fate_disable_raw_mode);

    struct termios raw = origin;
    raw.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void fate_raw_mode() {
    fate_enable_raw_mode();

    char input;
    while (read(STDIN_FILENO, &input, 1) == 1) {
        if (input == 'q') return;
    }
}
