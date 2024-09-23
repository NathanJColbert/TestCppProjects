 WINDOWS ONLY (I think)

 NOTES:
 I use "system()" to run a shell and whatnot.
  -This is a solo tool so system() should be fine here
 Log console (your program) in the RESULT_FILE directory.
  -Change RESULT_FILE to change the output directory
 The program will compare your output file to the test file output (1-x.out).
 Uses g++ as the compiler. 
  -Change the COMPILER_COMMAND_LINE to reference a different command line compiler.

 HOW TO:
 1) Compile the program, something like -> "g++ testprogram.cpp -o testprogram" OR "gcc testprogram.cpp -lstdc++ -o testBinary.exe"
 2) Run the program with the correct environment variables -> testprogram.exe -f moosick/moosick.cpp -t moosick/testfiles
    -Side note: You can compile your .cpp/.cxx OR just run the .exe/.out directly
 3) You should get an output that ends with "TESTS ENDED WITH A RESULT OF..."

 SOME OTHER STUFF:
 There are const strings at the top of this file that can be edited to change compiler settings and outputs
chrono::now() needs C++11
 The time sampling is definitely off (most likley because I am writing to a file)
      -Tested with cygwin time and it was consistently 10ms off or so
      -This should not be disastrous because if your file is running too slow... You'll know :)
