@echo off

"C:\Program Files\7-Zip\7z.exe" a output.zip ./code/pirate ./code/s*/* ./code/p**.* submissions.txt

python3 wet-verify.py output.zip
