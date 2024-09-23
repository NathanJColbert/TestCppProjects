// NATHAN COLBERT
// WINDOWS ONLY (I think)
//
// NOTES:
// I use "system()" to run a shell and whatnot.
//      -This is a solo tool so system() should be fine here
// Log console (your program) in the RESULT_FILE directory.
//      -Change RESULT_FILE to change the output directory
// The program will compare your output file to the test file output (1-x.out).
// Uses g++ as the compiler. 
//      -Change the COMPILER_COMMAND_LINE to reference a different command line compiler.
//
// HOW TO:
// 1) Compile the program, something like -> "g++ testprogram.cpp -o testprogram" OR "gcc testprogram.cpp -lstdc++ -o testBinary.exe"
// 2) Run the program with the correct environment variables -> testprogram.exe -f moosick/moosick.cpp -t moosick/testfiles
//      -Side note: You can compile your .cpp/.cxx OR just run the .exe/.out directly
// 3) You should get an output that ends with "TESTS ENDED WITH A RESULT OF..."
//
// SOME OTHER STUFF:
// There are const strings at the top of this file that can be edited to change compiler settings and outputs
// chrono::now() needs C++11
// The time sampling is definitely off (most likley because I am writing to a file)
//      -Tested with cygwin time and it was consistently 10ms off or so
//      -This should not be disastrous because if your file is running too slow... You'll know :)

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <filesystem>
#include <iomanip>
#include <chrono>

// Edit output environment settings
const std::string RESULT_FILE = "result.txt";
const char COMPILER_COMMAND_LINE[] = "g++";
const char COMPILER_COMMAND_LINE_ARGUMENT[] = ""; // -lstdc++" to link std if you are using gcc or some weird compiler. Add a space before the argument (I'm lazy)
//

// Testing Files
void coutTxt() { freopen(RESULT_FILE.c_str(),"w", stdout); }
void coutConsole() { freopen("/dev/tty", "w", stdout); }

bool fileExists(const std::string& name) {
    std::ifstream file(name);
    if (file.fail()) {
        std::cout << "ERROR File: " << name << " does not exist!\n";
        return false;
    }
    file.close();
    return true;
}

struct CompareResult {
public:
    CompareResult() : result(true), information("") { }
    CompareResult(const std::string& info) : result(false), information(info) { }
    operator bool() { return result; }
    void set(bool r) { result = r; }
    void print() { std::cout << "DEBUG INFORMATION:\n" << '\t' << information << '\n'; }
private:
    bool result;
    std::string information;
};
CompareResult compareFiles(const std::string& file1, const std::string& file2) {
    if (!fileExists(file1)) return CompareResult("File: \"" + file1 + "\" does not exist");
    if (!fileExists(file2)) return CompareResult("File: \"" + file2 + "\" does not exist");
    std::ifstream stream1(file1);
    std::ifstream stream2(file2);

    if (stream1.tellg() != stream2.tellg()) {
        stream1.close();
        stream2.close();
        return CompareResult("Files do no share the same size");
    }

    char c1, c2;
    c1 = stream1.get();
    c2 = stream2.get();
    int currentCharacter = 0;
    while (c1 != EOF && c2 != EOF) {
        if (c1 != c2) {
            stream1.close();
            stream2.close();
            std::string addition = "";
            if (c1 == '\n' || c2 == '\n' || c1 == ' ' || c2 == ' ') addition = "\n\tMost likley scenario: Improper output formatting";
            std::string leftSide = "";
            if (c1 == '\n') leftSide += "ENDLINE";
            else leftSide += c1;
            std::string rightSide = "";
            if (c2 == '\n') rightSide += "ENDLINE";
            else rightSide += c2;
            return CompareResult("Character's at charPos: " + std::to_string(currentCharacter) + " do not match:\n" + "\t" + leftSide + " ≠ " + rightSide + addition);
        }
        c1 = stream1.get();
        c2 = stream2.get();
        currentCharacter++;
    }
    if (c1 != EOF || c2 != EOF) {
        std::string add = "";
        if (c1 == EOF) add = "smaller";
        else add = "larger";
        return CompareResult("Files are not the same size.\n\tYour file is " + add + " than the test file.");
    }

    stream1.close();
    stream2.close();

    return CompareResult();
}

std::string removeEOF(const std::string& s) {
    std::string output = "";
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '\0') continue;
        output += s[i];
    }
    return output;
}

void testFiles(const std::string& binaryFile, const std::string& testfilesLocation) {
    std::cout << std::boolalpha;
    bool overralResult = true;
    std::chrono::milliseconds totalTime;
    for (const auto & entry : std::filesystem::directory_iterator(testfilesLocation)) {
        if (strcmp(entry.path().extension().c_str(), ".in") != 0) continue;
        std::string command = binaryFile;
        command += std::string(" < ");
        std::string testPath = entry.path().string();
        command += testPath;
        command = removeEOF(command);

        coutTxt();
        auto start = std::chrono::high_resolution_clock::now();
        system(command.c_str());
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        totalTime += duration;
        coutConsole();

        std::string testPathOut = testPath;
        testPathOut.pop_back();
        testPathOut.pop_back();
        testPathOut.pop_back();
        testPathOut += ".out";
        testPathOut = removeEOF(testPathOut);
        std::cout << "\n-----------------------------------------------------------\n";
        std::cout << "TESTING: \"" << testPath << "\"\n";
        CompareResult result = compareFiles(RESULT_FILE, testPathOut);
        std::cout << "RESULT: ";
        if (!result) {
            std::cout << "FAIL\n";
            result.print();
            overralResult = false;
        }
        else std::cout << "PASS\n";
        std::cout << "TIME: " << duration.count() << " milliseconds\n";
    }

    std::cout << "\n-----------------------------------------------------------\n";
    std::cout << "TESTS ENDED WITH A RESULT OF: ";
    if (overralResult) std::cout << "PASSED\n";
    else std::cout << "FAILURE\n";
    std::cout << "TOTAL TIME EXECUTION: " << totalTime.count() << " milliseconds\n";
}
//

// Compile File
std::string removeLastDir(const std::string& s) {
    std::string output = s;
    while (output.size() >= 1) {
        if (output[output.size() - 1] == '\\' || output[output.size() - 1] == '/')
            break;
        output.pop_back();
    }
    return output;
}
std::string compileFile(const std::string& cppFile) {
    std::string binaryDir = removeLastDir(cppFile);
    binaryDir += "testBinary.exe";
    binaryDir = removeEOF(binaryDir);
    std::string command = COMPILER_COMMAND_LINE;
    command += " " + cppFile + COMPILER_COMMAND_LINE_ARGUMENT + " -o " + binaryDir;
    command = removeEOF(command);
    int result = system(command.c_str());
    if (result != 0) return "";
    return removeEOF(binaryDir);
}
//

// Environment variable setup
bool hasExtention(const std::string& file, const std::string& extention) {
    int a = 0;
    for (int i = file.size() - extention.size(); i < file.size(); i++)
        if (file[i] != extention[a++]) return false;
    return true;
}

std::string getEnvironmentVariable(int argc, char* argv[], char variable) {
    for (int i = 0; i < argc; i++) {
        char first = *argv[i];
        if (first != '-') continue;
        if (*(++argv[i]) == variable) {
            std::string output = "";
            if (++i >= argc) return "";
            while (*argv[i] != '\0') output += *argv[i]++;
            return output;
        } else --argv[i];
    }
    return "";
}
//

// main
const std::string INVALID_INPUT = "Invalid input -> -f \"Program file (.cpp/.cxx/.exe/.out)\" -t \"Test file directory\"\n";
int main(int argc, char* argv[]) {
    std::string fileName = getEnvironmentVariable(argc, argv, 'f');
    std::string testFileDirectory = getEnvironmentVariable(argc, argv, 't');

    if (fileName == "" || testFileDirectory == "") {
        std::cout << INVALID_INPUT;
        std::cout << "\t*Environment variables not set.\nMake sure to use -f and -t\n";
        return -1;
    }
    if (!fileExists(fileName)) {
        std::cout << INVALID_INPUT;
        std::cout << "\t*File in path: \"" << fileName << "\" does not exist.\n";
        return -1;
    }
    if (!std::filesystem::exists(testFileDirectory)) {
        std::cout << INVALID_INPUT;
        std::cout << "\t*Directory: \"" << testFileDirectory << "\" does not exist.\n";
        return -1;
    }

    if (hasExtention(fileName, ".exe") || hasExtention(fileName, ".out")) {
        testFiles(fileName, testFileDirectory);
        return 0;
    }
    if (hasExtention(fileName, ".cpp") || hasExtention(fileName, ".cxx")) {
        fileName = compileFile(fileName);
        testFiles(fileName, testFileDirectory);
        return 0;
    }
    std::cout << INVALID_INPUT;
    std::cout << "\t*Extension for file name: \"" << fileName << "\" is not .exe/.out/.cpp/.cxx\n";
    return -1;
}
//