#include "Shell.h"

#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <utility>
#include <filesystem>
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

            std::filesystem::path currentDirectory = std::filesystem::current_path();

            if (currentDirectory.string() ==  "/") {
                textToDisplay.append("Home - ");
            }
            else {
                textToDisplay.append(currentDirectory.filename().string() + " - ");
            }

            textToDisplay.append("myShell> ");

            // display the text
            print(textToDisplay);

            std::getline(std::cin, input);

            // exit the terminal
            if (input == "exit") {
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

            // check for no input
            if (arg_strings.empty()) {
                print("nothing to do\n");
                textToDisplay.clear();
                continue;
            }

            // create a char pointer vector and gather all the args inside
            std::vector<char*> args;
            args.reserve(arg_strings.size()); // reserve space for optimization
            for (auto& str : arg_strings) {
                args.push_back(str.data());
            }

            // add a null pointer for the execvp
            args.push_back(nullptr);

            // goto logic instead of cd
            if (arg_strings[0] == "goto") {
                if (arg_strings.size() == 1) {
                    chdir(std::getenv("HOME")); // a home directory
                }
                else {
                    const std::string& directory = arg_strings[1];
                    std::filesystem::path path(directory);

                    if ( std::filesystem::exists(path)) {
                        if (std::filesystem::is_directory(path)) {
                            chdir(path.string().c_str());
                        }
                        else {
                            print("this is a file not a directory \n");
                        }
                    }
                    else {
                        print("there is no directory like that\n");
                    }
                }
            }
            else if (arg_strings[0] == "cd") {
                print("try using goto instead of cd\n");
            }
            else if (arg_strings[0] == "fullpath") {
                print(currentDirectory.string() + "\n");
            }
            // if there are arguments execute command
            else if (!arg_strings.empty()) {
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