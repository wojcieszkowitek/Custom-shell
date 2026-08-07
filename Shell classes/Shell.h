#ifndef CUSTOM_SHELL_SHELL_H
#define CUSTOM_SHELL_SHELL_H
#include <string>

namespace shell {
    class Shell {
    public:
        Shell();
        void mainloop();
        [[nodiscard]] bool isRunning() const noexcept;
        void setGreeter(std::string);

    private:
        bool mIsRunning;
        void stop();
        static void print(const std::string&);
        std::string mGreeter;

    };
}

#endif //CUSTOM_SHELL_SHELL_H
