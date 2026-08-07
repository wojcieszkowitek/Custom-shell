#ifndef CUSTOM_SHELL_COMMANDEXEC_H
#define CUSTOM_SHELL_COMMANDEXEC_H

#include <string>
#include "../Structures/ExecutionResult.h"

namespace shell {
    class CommandExec {
    public:
        static ExecutionResult execute(char *command, char **args);

    private:
        static pid_t createChildProcess();
    };
} // shell

#endif //CUSTOM_SHELL_COMMANDEXEC_H
