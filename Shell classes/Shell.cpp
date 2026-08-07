#include "Shell.h"

#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include "CommandExec.h"

namespace shell {
    Shell::Shell() {
        mIsRunning = false;
    }

    void Shell::mainloop() {
        mIsRunning = true;

        // a variable storing all text to display this frame
        std::string textToDisplay;
        textToDisplay.reserve(1024);

        // display the greeter
        print(mGreeter);

        // start the loop
        while(mIsRunning) {
            // a variable to store input
            std::string input;
            input.reserve(2048);

            textToDisplay.append("myShell>");

            // display the text
            print(textToDisplay);

            // get input
            std::getline(std::cin,input);

            // if someone clicked ctrl + D exit
            if (!std::getline(std::cin, input)) {
                mIsRunning = false;
                continue;
            }

            // exit the terminal
            if (input == "exitTerminal") {
                mIsRunning = false;
                continue;
            }

            // create arguments
            std::stringstream stringStream(input);
            std::string segment;

            // gather all the arguments in a vector
            std::vector<std::string> arg_strings;
            while (stringStream >> segment) {
                arg_strings.push_back(segment);
            }

            // create a char pointer vector and gather all the args inside
            std::vector<char*> args;
            args.reserve(arg_strings.size()); // reserve space for optimalisation
            for (auto& str : arg_strings) {
                args.push_back(str.data());
            }

            // add a null pointer for the execvp
            args.push_back(nullptr);

            // if there are arguments execute command
            if (!arg_strings.empty()) {
                CommandExec::execute(args[0], args.data());
            }

            // clear text at the end
            textToDisplay.clear();
        }
    }

    bool Shell::isRunning() const noexcept {
        return mIsRunning;
    }

    void Shell::setGreeter(std::string greeterToSet) {
        mGreeter = std::move(greeterToSet);
    }

    void Shell::stop() {
        mIsRunning = false;
    }

    void Shell::print(const std::string& text) {
        std::cout << text;
        std::cout.flush();
    }
} // shell