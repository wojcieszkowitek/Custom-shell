#ifndef CUSTOM_SHELL_COMMANDEXEC_H
#define CUSTOM_SHELL_COMMANDEXEC_H

#include <string>
#include "../Structures/ExecutionResult.h"

namespace shell {
    class CommandExec {
    public:
        static ExecutionResult execute(const char *command, char **args);

    private:
    };
} // shell

#endif //CUSTOM_SHELL_COMMANDEXEC_H
