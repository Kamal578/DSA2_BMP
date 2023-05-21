#!/bin/bash

# Check if filename argument is provided
if [ $# -eq 0 ]; then
    echo "Please provide a filename to compile and run."
    exit 1
fi

# Get filename without extension
filename=$(basename -- "$1")
extension="${filename##*.}"
filename="${filename%.*}"

# Check file type
if [ "$extension" != "cpp" ] && [ "$extension" != "c" ]; then
    echo "Error: $filename is not a C++ or C file."
    exit 1
fi

# Compile C++ program
g++ -o "$filename" "$1"

if [ $? -ne 0 ]; then echo "Compilation failed."; exit 1; fi

# Run C++ program
./"$filename"

# Check if program ran successfully
if [ $? -ne 0 ]; then echo ""; echo "Program failed to run."; exit 1; fi

# Delete executable

rm "$filename"; if [ $? -ne 0 ]; then echo "Error: Could not delete executable."; fi

history -c