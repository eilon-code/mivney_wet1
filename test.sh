#!/bin/bash

# Ensure Make is installed
if ! command -v make &> /dev/null; then
    echo "make could not be found. Please install it using: sudo apt install build-essential"
    exit 1
fi

# Clean previous builds
echo "Cleaning previous builds..."
make -f makefile clean

# Compile the program
echo "Compiling the program..."
make -f makefile

# Check if the executable was created
if [ -f ./build/prog ]; then
    echo "Executable found, running the program..."
    valgrind -s --leak-check=full ./build/prog < ./tests/input/test1.in > test.out
else
    echo "Executable not found. Compilation might have failed."
fi
