# Testing CPP Files
WINDOWS ONLY (I think)

## HOW TO:
1) Compile the program
```console 
g++ testprogram.cpp -o testprogram
```
OR 
```console
gcc testprogram.cpp -lstdc++ -o testBinary.exe
```
2) Run the program with the correct environment variables
```console 
testprogram.exe -f examples/linesegment.cpp -t examples/testfiles.
```
   - Side note: You can compile your .cpp/.cxx OR just run the .exe/.out directly.
3) The console will log the output test results of each .in/.out files

## NOTES:
- I use "system()" to run a shell and whatnot. This is a solo tool so system() should be fine here.
- Log console (your program) in the RESULT_FILE directory. Change RESULT_FILE to change the output directory.
- The program will compare your output file to the test file output (1-x.out).
- Uses g++ as the compiler. Change the COMPILER_COMMAND_LINE to reference a different command line compiler.

## SOME OTHER STUFF:
- There are const strings at the top of this file that can be edited to change compiler settings and outputs.
- chrono::now() needs C++11.
- The time sampling is definitely off (most likley because I am writing to a file).
   - Tested with cygwin time and it was consistently 10ms off or so. This should not be disastrous because if your file is running too slow... You'll know :)

## EXAMPLES:
The examples directory has a micro use of the program. Enter the directory in your console and run it:
```console
testprogram.exe -t testfiles -f linesegment.cpp
```