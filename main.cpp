#include "Shell classes/Shell.h"

using namespace shell;

int main() {
    auto shell = Shell();

    shell.setGreeter("hello terminal!\n");

    shell.mainloop();
}
