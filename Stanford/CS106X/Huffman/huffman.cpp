/**
 * File: huffman.cpp 
 * -------------------
 * Huffman main module.  Build it!
 */

#include <iostream>
#include "console.h"
#include "encoding.h"
#include "filelib.h"
#include "simpio.h"
using namespace std;

// Function prototypes
void simpleTest();
void huffman();
static bool responseIsAffirmative(const string& prompt);
static string selectFileToCompress();
static string createCompressedFile();
static string selectFileToDecompress();
static string createDecompressedFile();
int compareFiles(string fileOne, string fileTwo);

int main() {
    // Remove the following function once your Encoding class is complete.
    //simpleTest();
    huffman();


    return 0;
}

/**
 * Function: simpleTest
 * --------------------
 * This function performs a simple compression and decompression
 * test. It compresses, then decompresses a test file.
 */
static const string kTestFile = "testfile.txt";
static const string kTestCompressed = "testfile-compressed.txt";
static const string kTestDecompressed = "testfile-decompressed.txt";
void simpleTest() {
    cout << "This is a simple test function to test the functionality of "
            "your Encoding class. It compresses, then decompresses " << kTestFile << ". "
            "Note that the files you output will be in your build directory "
            "(something like build-Huffman-*). "
            "If you just started, this will pass the test because compress "
            "and decompress both faithfully copy the original."
         << endl << endl
         << "Once you've implemented Encoding, replace this with the main "
            "huffman module, which prompts users for files to compress/decompress"
         << endl << endl;

    // Set up the encoding object to power compression/decompression
    Encoding encoding;

    // Compress kTestFile to kTestCompressed
    ibstream testin;
    testin.open(kTestFile.c_str());
    obstream compressedout;
    compressedout.open(kTestCompressed.c_str());
    encoding.compress(testin, compressedout);
    cout << "Original file: " << testin.size() << " bytes, "
         << "Compressed file: " << compressedout.size() << " bytes." << endl;
    testin.close();
    compressedout.close();

    // Decompress kTestCompressed to kTestDecompressed
    ibstream compressedin;
    compressedin.open(kTestCompressed.c_str());
    obstream decompressedout;
    decompressedout.open(kTestDecompressed.c_str());
    encoding.decompress(compressedin, decompressedout);
    compressedin.close();
    decompressedout.close();

    // Compare original input to decompressed output
    int errors = compareFiles(kTestFile, kTestDecompressed);
    if(errors == 0) cout << "Hooray! ";
    else cout << "Uh oh. " << endl;
    cout << "Files differ in " << errors << " bits." << endl;
}

/**
 * Function: huffman
 * --------------------
 * This function allows the user to compress or decompress a file.  When compressing,
 * it prints the original file size, the new file size, and the percent size decrease.
 */
void huffman() {
    cout << "This program uses the Huffman coding algorithm for compression. Any file can be compressed with this ";
    cout << "method, often with substantial savings. Decompression will faithfully reproduce the original." << endl << endl;
    Encoding encoding;
    while (true){
        if (responseIsAffirmative("Would you like to compress? (no will default to decompress) ")){
            string fileToCompress = selectFileToCompress(); /* compresses fileIn to compressedOut */
            ibstream fileIn;
            fileIn.open(fileToCompress.c_str());
            string compressedFile = createCompressedFile();
            obstream compressedOut;
            compressedOut.open(compressedFile.c_str());
            encoding.compress(fileIn, compressedOut);
            cout << "Original file: " << fileIn.size() << " bytes, " << endl;
            cout << "Compressed file: " << compressedOut.size() << " bytes." << endl;
            if (compressedOut.size() > fileIn.size()) cout << "Compressed file is actually bigger!" << endl;
            else cout << "Compression: " << 100.- 100. * (float) compressedOut.size()/fileIn.size() << "%" << endl;
        }else{
            string fileToDecompress = selectFileToDecompress(); /* decompresses compressedIn to decompressedOut */
            ibstream compressedIn;
            compressedIn.open(fileToDecompress.c_str());
            string decompressedFile = createDecompressedFile();
            obstream decompressedOut;
            decompressedOut.open(decompressedFile.c_str());
            encoding.decompress(compressedIn, decompressedOut);
            cout << "Decompression finished! " << endl;
        }
        if (!responseIsAffirmative("Would you like to compress or decompress another file? ")) break;
    }
    cout << endl << endl << endl << "Thanks for compressing! " << endl;
}


/*
 * function: responseIsAffirmative(const string& prompt)
 * usage: if (responseIsAffirmative(prompt)) ...
 * -------------------
 * Prints prompt to the screen, and asks for user input of yes or no.
 * If yes, returns true, if no, returns false.
 */
static bool responseIsAffirmative(const string& prompt) {
    while (true) {
        string answer = getLine(prompt);
        if (!answer.empty()) {
            switch (toupper(answer[0])) {
                case 'Y': return true;
                case 'N': return false;
            }
        }
        cout << "Please answer yes or no." << endl;
    }
}


/*
 * function: selectFileToCompress()
 * usage: string inputFile = selectFileToCompress();
 * -------------------
 * Asks user for the file wanted, and checks to see if it exists.  If it exists, returns
 * file name, if not, asks again.
 */
static string selectFileToCompress(){
    string inputFile;
    bool fileExists = false;
    while (!fileExists){
        inputFile = getLine("Please enter the file to be compressed:");
        ifstream fileName(inputFile);
        if (fileName){
            fileExists = true;
        }else{
            cout << "That file could not be found." << endl;
        }
    }
    return inputFile;
}

/*
 * function: createCompressedFile()
 * usage: string compressedFile = createCompressedFile();
 * -------------------
 * Asks user for the file name wanted, and checks to see if it exists.  If it exists,
 * ask again, otherwise create the file.
 */
static string createCompressedFile(){
    string compressedFile;
    bool fileExists = true;
    while (fileExists) {
        compressedFile = getLine("Enter the name to give the compressed file: ");
        ifstream fileName(compressedFile);
        if (!fileName) {
            fileExists = false;
        }else{
            cout << "That file already exists! No overwriting existing files!" << endl;
        }
    }
    return compressedFile;
}

/*
 * function: selectFileToDecompress()
 * usage: string inputFile = selectFileToDecompress();
 * -------------------
 * Asks user for the file wanted, and checks to see if it exists.  If it exists, returns
 * file name, if not, asks again.
 */
static string selectFileToDecompress(){
    string inputFile;
    bool fileExists = false;
    while (!fileExists){
        inputFile = getLine("Please enter the file to be decompressed:");
        ifstream fileName(inputFile);
        if (fileName){
            fileExists = true;
        }else{
            cout << "That file could not be found." << endl;
        }
    }
    return inputFile;
}

/*
 * function: createDecompressedFile()
 * usage: string decompressedFile = createDecompressedFile();
 * -------------------
 * Asks user for the file name wanted, and checks to see if it exists.  If it exists,
 * ask again, otherwise create the file.
 */
static string createDecompressedFile(){
    string decompressedFile;
    bool fileExists = true;
    while (fileExists) {
        decompressedFile = getLine("Enter the name to give the decompressed file: ");
        ifstream fileName(decompressedFile);
        if (!fileName) {
            fileExists = false;
        }else{
            cout << "That file already exists! No overwriting existing files!" << endl;
        }
    }
    return decompressedFile;
}

int compareFiles(string fileOne, string fileTwo) {
    cout << "Comparing " << fileOne << " to " << fileTwo << "." << endl;
    int errors = 0;
    ibstream one;
    one.open(fileOne.c_str());
    ibstream two;
    two.open(fileTwo.c_str());
    while(true) {
        int obit = one.readbit();
        int dbit = two.readbit();
        if(obit != dbit) errors++;
        if(obit == EOF && dbit == EOF) break;
    }
    return errors;
}
