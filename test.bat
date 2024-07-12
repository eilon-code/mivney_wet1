@REM Clean previous builds
echo Cleaning previous builds...
mingw32-make -f makefile clean

@REM Compile the program
echo Compiling the program...
mingw32-make -f makefile

@REM rem Check if the input file exists
@REM if exist ./tests/test1.in (
@REM     echo Input file found: tests/test1.in
@REM     type nul > test.out
@REM ) else (
@REM     echo Input file not found: tests/test1.in
@REM )

@REM if exist test.out (
@REM     echo Output file found: test.out
@REM ) else (
@REM     echo Input file not found: test.out
@REM )

@REM Check if the executable was created
if exist ./build/prog.exe (
    echo Executable found, running the program...
    .\build\prog.exe < ./tests/input/test1.in > test.out
) else (
    echo Executable not found. Compilation might have failed.
)

@REM Pause to see the output in the command prompt
@REM pause
