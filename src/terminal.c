#include "../include/terminal.h"
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

struct termios origin;

void fate_disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &origin);
}

void fate_enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &origin);
    atexit(fate_disable_raw_mode);

    struct termios raw = origin;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag &= ~(CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
}

void fate_raw_mode() {
    fate_enable_raw_mode();

    char input;
    while (read(STDIN_FILENO, &input, 1) == 1 && input != 'q') {
       if (iscntrl(input)) {
            printf("%d\n", input);
       } else {
            printf("%d ('%c')\n", input, input);
       }
    }
}
