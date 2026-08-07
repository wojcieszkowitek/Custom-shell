cmake_minimum_required(VERSION 4.2)
project(Custom_shell)

set(CMAKE_CXX_STANDARD 20)

add_executable(Custom_shell main.cpp
        "Shell classes/Shell.cpp"
        "Shell classes/Shell.h"
        "Shell classes/CommandExec.cpp"
        "Shell classes/CommandExec.h"
        Structures/ExecutionResult.h
)
