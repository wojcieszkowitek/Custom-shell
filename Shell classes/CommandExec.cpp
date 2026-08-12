#include <filesystem>
#if defined(__APPLE__)
#include <util.h>
#else
#include <pty.h>
#endif

#include <unistd.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <poll.h>
#include "CommandExec.h"
#include "TerminalGuard.hpp"

namespace shell {
    ExecutionResult CommandExec::execute(const char *command, char **args) {
        // terminal data
        int master_ft;

        // screen data
        struct winsize w{};
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

        // get info form fork
        const pid_t pid = forkpty(&master_ft, nullptr, nullptr, &w);

        // if pid is lower than zero that means error in forking
        if (pid < 0) {
            perror("fork failed");
            return ExecutionResult(-1, false);
        }

        // child process
        if (pid == 0) {
            execvp(command, args); // execute the command

            perror("execvp");
            exit(1);
        }

        // main app
        // ReSharper disable once CppDFAConstantConditions
        if (pid > 0) {
            TerminalGuard guard;

            struct pollfd pfd[2];

            // input
            pfd[0].fd = STDIN_FILENO;
            pfd[0].events = POLLIN;

            // output
            pfd[1].fd = master_ft;
            pfd[1].events = POLLIN;

            // buffer for sending data
            char buffer[4096];

            while (true) {
                if (poll(pfd, 2, -1) < 0) {
                    perror("poll");
                    break;
                }

                // keyboard input
                if (pfd[0].revents & POLLIN) {
                    const ssize_t bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
                    if (bytes_read > 0) {
                        write(master_ft, buffer, bytes_read);
                    }
                }

                // output
                if (pfd[1].revents & POLLIN) {
                    const ssize_t bytes_read = read(master_ft, buffer, sizeof(buffer));
                    if (bytes_read <= 0) {
                        break;
                    }

                    write(STDOUT_FILENO, buffer, bytes_read);
                }
            }

            // wait for the process to end
            waitpid(pid, nullptr, 0);

            // close the child process
            close(master_ft);
        }

        return ExecutionResult(0, true);
    }
} // shell

