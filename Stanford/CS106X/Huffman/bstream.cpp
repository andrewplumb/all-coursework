/**
 * File: bstream.cpp
 * -----------------
 * Implementation of ibstream and obstream classes.  These classes are patterned after
 * (and, in fact, inherit from) the standard ifstream and ofstream classes.  Please
 * see bstream.h for information about how a client properly uses these classes.
 *
 * Last modified on Tue Nov 14 16:07:09 PST 2006 by jzelenski
 */

#include <iostream>
#include "bstream.h"
#include "error.h"
using namespace std;

static const int kNumBitsPerByte = 8;

inline int getNthBit(int n, int fromByte) { return ((fromByte & (1 << n)) != 0) ? 1 : 0; }
inline void setNthBit(int n, int& inByte) { inByte |= (1 << n); }

/**
 * Constructor ibstream::ibstream
 * ------------------------------
 * Each ibstream tracks 3 integers as private data.  
 * "lastTell" is streampos of the last byte that was read (this is used
 * to detect when other non-readbit activity has changed the tell)
 * "curByte" contains contents of byte currently being read
 * "pos" is the bit position within curByte that is next to read
 * We set initial state for lastTell and curByte to 0, then pos is
 * set at 8 so that next readbit will trigger a fresh read.
 */
ibstream::ibstream() : lastTell(0), curByte(0), pos(kNumBitsPerByte) {}

/**
 * Member function ibstream::open
 * ------------------------------
 * Overrides open member function to be sure that stream is opened
 * in binary mode (default is text, which would play havoc w/ line-endings)
 */
void ibstream::open(const char * filename) {
	ifstream::open(filename, ios::binary);
}

/**
 * Member function ibstream::readbit
 * ---------------------------------
 * If bits remain in curByte, retrieve next and increment pos
 * Else if end of curByte (or some other read happened), then read next byte
 * and start reading from bit position 0 of that byte.
 * If read byte from file at EOF, return EOF.
 */
int ibstream::readbit() {
	if (!is_open()) error("Cannot readbit from stream which is not open.");
	// if just finished bits from curByte or if data read from stream after last readbit()
	if (lastTell != tellg() || pos == kNumBitsPerByte) { 
		if ((curByte = get()) == EOF) // read next single byte from file
			return EOF;           
		pos = 0; // start reading from first bit of new byte
	    lastTell = tellg();
	}
	int result = getNthBit(pos, curByte);
	pos++; // advance bit position for next call to readbit
	return result;
}

/** 
 * Member function ibstream::rewind
 * ---------------------------------
 * Simply seeks back to beginning of file, so reading begins again
 * from start.
 */
void ibstream::rewind() {
	if (!is_open()) error("Cannot rewind stream which is not open.");
	clear();
	seekg(0, ios::beg);
}

/**
 * Member function ibstream::size
 * ------------------------------
 * Seek to file end and use tell to retrieve position.
 * In order to not disrupt reading, we also record cur streampos and
 * re-seek to there before returning.
 */
long ibstream::size() {
	if (!is_open()) error("Cannot get size of stream which is not open.");
	clear();					// clear any error state
	streampos cur = tellg();	// save current streampos
	seekg(0, ios::end);			// seek to end
	streampos end = tellg();	// get offset
	seekg(cur);					// seek back to original pos
	return long(end);
}

/**
 * Constructor obstream::obstream
 * ----------------------------------
 * Each obstream tracks 3 integers as private data.  
 * "lastTell" is streampos of the last byte that was written (this is used
 * to detect when other non-writebit activity has changed the tell)
 * "curByte" contains contents of byte currently being written
 * "pos" is the bit position within curByte that is next to write
 * We set initial state for lastTell and curByte to 0, then pos is
 * set at 8 so that next writebit will start a new byte.
 */
obstream::obstream() : lastTell(0), curByte(0), pos(kNumBitsPerByte) {}

/** 
 * Member function obstream::open
 * ------------------------------
 * Overrides open member function to be sure that stream is opened
 * in binary mode (default is text, which would play havoc w/ line-endings)
 */
void obstream::open(const char * filename) {
	ofstream::open(filename, ios::binary);
}

/**
 * Member function obstream::writebit
 * ----------------------------------
 * If bits remain to be written in curByte, add bit into byte and increment pos
 * Else if end of curByte (or some other write happened), then start a fresh
 * byte at position 0.
 * We write the byte out for each bit (backing up to overwrite as needed), rather
 * than waiting for 8 bits.  This is because the client might make
 * 3 writebit calls and then start using << so we can't wait til full-byte
 * boundary to flush any partial-byte bits.
 */
void obstream::writebit(int bit) {
	if (bit != 0 && bit != 1) error("writebit expects argument which can be only 0 or 1.");
	if (!is_open()) error("Cannot writebit to stream which is not open.");
	
		// if just filled curByte or if data written to stream after last writebit()
	if (lastTell != tellp() || pos == kNumBitsPerByte) { 
		curByte = 0; // zero out byte for next writes
		pos = 0;	// start writing to first bit of new byte	
	}
		
	if (bit == 1) // only need to change if bit needs to be 1 (byte starts already zeroed)
		setNthBit(pos, curByte);
		
	if (pos == 0 || bit) { // only write if first bit in byte or changing 0 to 1
	    if (pos != 0) seekp(-1, ios::cur); // back up to overwite if pos > 0
	    put(curByte);
	}
	    
	pos++; // advance to next bit position for next write
	lastTell = tellp();
}

/**
 * Member function obstream::size
 * ------------------------------
 * Seek to file end and use tell to retrieve position.
 * In order to not disrupt writing, we also record cur streampos and
 * re-seek to there before returning.
 */
long obstream::size() {
	if (!is_open()) error("Cannot get size of stream which is not open.");
	clear();					// clear any error state
	streampos cur = tellp();	// save current streampos
	seekp(0, ios::end);			// seek to end
	streampos end = tellp();	// get offset
	seekp(cur);					// seek back to original pos
	return long(end);
}

/**
 * Function: bstream_test()
 * --------------------------
 * This function is just a very simple unit-test for the bstream classes.
 */
void bstream_test() {
	obstream outfile;
	outfile.open("binarytestfile");
	
	outfile << "TenAlternate: ";
	for (int i = 0; i < 10; i++)
		outfile.writebit(i % 2);
	outfile << "Zero ";
	outfile.writebit(0);
    outfile << "One ";
    outfile.writebit(1);
    outfile << "ToEOF ";
	for (int i = 0; i < 5; i++)
		outfile.writebit(i % 2);
	outfile.close();
    
	ibstream infile;
	infile.open("binarytestfile");
	string str;
	infile >> str;
	infile.get(); // read space
	cout << endl << str << " ";
	for (int i = 0; i < 10; i++)
		cout << char(infile.readbit() + '0');
	infile >> str;
	infile.get(); // read space
	cout << endl << str << " ";
	cout << char(infile.readbit() + '0');
	infile >> str;
	infile.get(); // read space
	cout << endl << str << " ";
	cout << char(infile.readbit() + '0');
	infile >> str;
	infile.get(); // read space
	cout << endl << str << " ";
	int bit;
	while ((bit = infile.readbit()) != EOF)
		cout << char(bit + '0');
	cout << bit << endl;
	infile.close();
}
