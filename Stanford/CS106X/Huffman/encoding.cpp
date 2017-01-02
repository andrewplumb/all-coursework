/**
 * File: encoding.cpp
 * ------------------
 * Place your Encoding class implementation here.
 */

#include "encoding.h"
#include "bstream.h"
#include "map.h"
#include "foreach.h"
#include "pqueue.h"
#include "string.h"
#include "strlib.h"
#include <iostream>
using namespace std;

Encoding::Encoding() {
    head = NULL;
}
Encoding::~Encoding() {}

/**
 * Function: copy
 * --------------
 * Performs a bit-by-bit copy from infile to outfile.
 * Note that infile.close() and outfile.close() must be
 * properly called to ensure that any trailing bits are
 * properly flushed to the output file.
 */
static void copy(ibstream &infile, obstream &outfile) {
    while(true) {
        int bit = infile.readbit();
        if(bit == EOF) break;
        outfile.writebit(bit);
    }
}

/**
 * Function: fillArray
 * -------------------
 * Fills the ith index of an array with the string of bits that will
 * represent the unicode character for i in the compressed file. This
 * is done by recursively traversing the tree, and keeping track of each turn
 * throught the tree.  Going to the left child represents the bit 0, going to the
 * right child represents the bit 1. fillArray is a wrapper function for
 * traverseTree.
 */
void Encoding::fillArray(LinkedNode* head, string (&array)[257]){
    string path = "0";
    traverseTree(head->left, array, path);
    path = "1";
    traverseTree(head->right, array, path);
}

/**
 * Function: traverseTree
 * ----------------------
 * This recursively traverses through the tree.  If it comes to a node that has a value for
 * the letter, it populates the correct index of the array with the bit path
 * taken to get to that node.
 */
void Encoding::traverseTree(LinkedNode * head, string (&array)[257], string path){
    if (head->letter < 257){
        array[head->letter] = path;
        return;
    }
    while (head->letter == 257){
       string newPath = path + "0";
       traverseTree(head->left, array, newPath);
       newPath = path + "1";
       traverseTree(head->right, array, newPath);
       return;
    }
}

/**
 * Function: compress
 * ------------------
 * compress compresses infile using Huffman Encoding and stores the result
 * in outfile.
 */
void Encoding::compress(ibstream &infile, obstream &outfile) {
    Map<int, int> counts; /* the number of times each character appears in the  */
    int num;              /* file to be compressed will initially be stored in a */
    while ((num = infile.get()) != EOF){ /* map. */
        if (!counts.containsKey(num)){ /* if the character hasn't been seen, a new key */
            counts.put(num, 1);        /* is added to the map. */
        }else{                         /* otherwise the value is just incremented */
            counts[num]++;
        }
    }
    counts.put(256, 1);                 /* this adds a pseudo-EOF character to the map */

    PQueue<entry> trees;                /* trees is a priority queue of all the subtrees that
                                           will eventually make up our final tree.  entry is a
                                           struct that contains both an element value and a
                                           priority. This is so priorities can be combined as
                                           trees are combined. */
    outfile << counts.size()-1 << " "; /* adds to the header of the output file the number of
                                          unique characters in the file, with the exception of
                                          the pseudo-EOF character, as we know it occurs exactly
                                          once. */
    foreach(int key in counts){ /* adds each character and the number of times it occurs to the
                                   header of the file, and enqueues a single node tree with that
                                   character and the number of times it occurs as it's priority. */
        if (key != 256) outfile << (char)key << counts[key] << " ";
        head = new LinkedNode(key, NULL, NULL);
        trees.enqueue({head, (double) counts[key]}, (double) counts[key]);
    }
    while (trees.size() > 1){ /* combines trees in the priority queue using the huffman algorithm
                                 until onlya  single, final tree remains. */
        entry first = trees.extractMin();
        entry second = trees.extractMin();
        double newPriority = first.priority+second.priority; /* the new priority is the sum of the combining trees priorities.*/
        head = new LinkedNode(257, first.elem, second.elem);
        trees.enqueue({head, newPriority}, first.priority+second.priority);
    }
    head = trees.extractMin().elem; /* head points to the head of the tree. */
    string array[257]; /* the 257 index array that will be used  for quick lookup. */
    fillArray(head, array);
    infile.rewind();
    while ((num = infile.get()) != EOF){                                /* writes the bits to outfile that represent */
        for (int i = 0; i < array[num].length(); i++){                  /* each character in infile. */
            outfile.writebit(stringToInteger(array[num].substr(i,1)));
        }
    }
    for (int i = 0; i < array[256].length(); i++){                      /* writes the pseudo-EOF bits to the end of the file. */
        outfile.writebit(stringToInteger(array[256].substr(i,1)));
    }
}




/**
 * Function: decompress
 * --------------------
 * decompresses a ompressed file to its previous, readable state. Characters are determined
 * by traversing through the encoding tree according to the bits in the compressed file until a character is
 * found.
 */
void Encoding::decompress(ibstream &infile, obstream &outfile) {
    char ch;
    string numberUniqueChars = "";      /* determines the number of unique characters within the  */
    while ((ch = infile.get()) != ' '){ /* decompressed file, so it knows how many to add to the  */
        numberUniqueChars += ch;        /* encoding tree. */
    }
    int numUniqueChars = stringToInteger(numberUniqueChars);
    Map<int, int> counts;
    for (int i = 0; i < numUniqueChars; i++){   /* populates a map of characters and their frequency using the */
        int letter = infile.get();              /* header of the file.  characters are followed by their frequency */
        string numberOfChars = "";              /* in the header file. */
        while ((ch = infile.get()) != ' '){
            numberOfChars += ch;
        }
        int numOfChars = stringToInteger(numberOfChars);
        counts.put(letter, numOfChars);
    }
    counts.put(256, 1);
    PQueue<entry> trees;                        /* populates a queue with trees, and combines trees, as is done in  */
    foreach(int key in counts){                 /* the compress method. */
        head = new LinkedNode(key, NULL, NULL);
        trees.enqueue({head, (double) counts[key]}, (double) counts[key]);
    }
    while (trees.size() > 1){
        entry first = trees.extractMin();
        entry second = trees.extractMin();
        double newPriority = first.priority+second.priority;
        head = new LinkedNode(257, first.elem, second.elem);
        trees.enqueue({head, newPriority}, first.priority+second.priority);
    }
    head = trees.extractMin().elem;
    LinkedNode * current = head;
    int bit;
    while (true){ /* adds the characters represented by bits in the compressed file to outfile. */
        if (current->letter == 256) return; /* pseudo-EOF, stops looking through infile for more characters */
        if (current->letter < 257){
            outfile << (char) current->letter; /* adds the represented character to outfile, */
            current = head;                    /* and points current back to the head. */
        }
        bit = infile.readbit();                 /* reads the next bit */
        if (bit == 0){ /* this is left as bit == 0 instead of !bit as it implies why current points to the left child. */
            current = current->left;             /* if it's a 0, point current to the left child. */
        }
        if (bit == 1){                          /* if it's a 1, point current to the right child. */
            current = current->right;
        }

    }
}
