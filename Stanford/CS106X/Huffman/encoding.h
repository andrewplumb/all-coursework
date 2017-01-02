/**
 * File: encoding.h
 * ----------------
 * Defines the Encoding class to manage all aspects of the
 * Huffman compression and decompression algorithms.
 */

#ifndef _encoding_
#define _encoding_

#include "bstream.h"
#include "string.h"

/*
 * Class: Encoding
 * ---------------
 * The Encoding class manages an encoding tree mapping each character to a unique
 * bit pattern encoding using the classic Huffman algorithm for building an optimal
 * tree. It includes methods for compressing and decompressing files.
 */
class Encoding {
public:
    /*
     * Constructor: Encoding()
     * usage: Encoding encoding;
     * --------------------------------
     * Initializes a new encoding class.
     */
    Encoding();

    /*
     * Destructor: ~Encoding()
     * usage: (usually implicit)
     * --------------------------------
     * Frees any heap storage associated with the class.
     */
    ~Encoding();

    /*
     * Method: compress
     * usage: encoding.compress(filein, compressedout);
     * ----------------------------------
     * Compresses a file, and stores the result in compressedout.
     */
    void compress(ibstream& infile, obstream& outfile);

    /*
     * Method: decompress
     * usage: encoding.decompress(compressedin, decompressedout);
     * ----------------------------------
     * Deompresses compressedin, and stores the result in decompressedout.
     */
    void decompress(ibstream& infile, obstream& outfile);


private:
    /* Private Structures */

    struct LinkedNode {
        int letter;
        LinkedNode *left;
        LinkedNode *right;

        LinkedNode() {
            letter = 257; /* 258 is larger than any ASCII value, so is a good representative of  */
                          /* no value.  -1 would also be a good choice. */
            left = NULL;
            right = NULL;
        }
        LinkedNode(int l, LinkedNode* lft, LinkedNode* r){
            letter = l;
            left = lft;
            right = r;
        }
    };

    struct entry {
        LinkedNode * elem;
        double priority;
    };

    /* Instance Variables */
    LinkedNode * head;

    std::string *array;


    /* private function prototypes */
    void fillArray(LinkedNode* head, std::string (&array)[257]);
    void traverseTree(LinkedNode * head, std::string (&array)[257], std::string path);
};


#endif
