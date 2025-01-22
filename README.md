# Wave-Shell
Shell for UNIX machines

# C++ Shell

This project is a custom shell implementation in C++ for UNIX machines that supports command execution, piping, and logical operators.

## Features

- **Command Execution:** Run standard shell commands like `ls`, `echo`, `cat`, etc.
- **Piping (`|`):** Supports chaining commands by redirecting output from one command to another.
- **Logical Operators (`&&` and `||`):** Conditional execution based on command success or failure.
- **Built-in Commands:** Includes basic built-in commands like `cd`, `exit`.

## Installation

### Prerequisites

- C++ compiler (GCC/Clang/MSVC) with C++17 or later support.
- CMake (optional, for build automation).
- UNIX system.

### Build Instructions

1. Clone the repository:
   ```bash
   git clone https://github.com/Josiah-Kitchin/Wave-Shell 
   cd Wave-Shell

2. Build executable and run program:
   ```bash
   cmake -S . -B build
   cd build
   make
   ./wsh



