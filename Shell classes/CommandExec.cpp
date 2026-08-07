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
#include "CommandExec.h"

namespace shell {
    ExecutionResult CommandExec::execute(char *command, char **args) {
        // terminal data
        int master_ft;

        // screen data
        struct winsize w{};
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

        // get info form fork
        pid_t pid = forkpty(&master_ft, nullptr, nullptr, &w);

        // if pid is lower than zero that means error in forking
        if (pid < 0) {
            perror("fork failed");
            return ExecutionResult(-1, false);
        }

        // main app
        if (pid == 0) {
            // execute the command
            execvp(command, args);

            perror("execvp");
            exit(1);
        }

        // child processes
        // ReSharper disable once CppDFAConstantConditions
        if (pid > 0) {
            // buffer for sending data
            char buffer[4096];
            ssize_t bytes_read;

            // while data is passed, print the data
            while ((bytes_read = read(master_ft, buffer, sizeof(buffer))) > 0) {
                write(STDOUT_FILENO, buffer, bytes_read);
            }

            // wait for the process to end
            waitpid(pid, nullptr, 0);

            // close the child process
            close(master_ft);
        }

        return ExecutionResult(0, true);
    }
} // shell

