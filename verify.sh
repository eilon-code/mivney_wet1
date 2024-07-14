#!/bin/bash

# Remove any existing output.zip file
rm -f output.zip

# Change to the code directory
cd code

# Create the zip file from the current directory, but store it in the parent directory
zip -r ../output.zip l* pirate s* p**.* ../submissions.txt ../Dry.pdf

# Change back to the parent directory
cd ..

python3 wet-verify.py output.zip
