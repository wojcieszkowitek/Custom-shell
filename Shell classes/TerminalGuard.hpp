#pragma once
#include <unistd.h>
#include <sys/termios.h>

class TerminalGuard {
public:
    struct termios t{};
    struct termios old_t{};
    TerminalGuard() {
        tcgetattr(STDIN_FILENO, &t); // get the terminal data
        old_t = t; // save the old terminal
        cfmakeraw(&t); // make the new terminal data raw
        tcsetattr(STDIN_FILENO, TCSANOW, &t); // apply the terminal guard
    };

    ~TerminalGuard() {
        tcsetattr(STDIN_FILENO, TCSANOW, &old_t); // at the destruction bring back old terminal mode
    };

};
