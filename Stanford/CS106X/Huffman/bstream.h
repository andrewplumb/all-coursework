/*
 * File: bstream.h
 * Last modified on Tue Nov 14 16:06:35 PST 2006 by jzelenski
 *                  Sat Nov 03 16:33:00 PST 2012 by jcain
 * ----------------------------------------------------------
 * Defines the ibstream and obstream classes which are basically
 * same as the ordinary ifstream and ofstream classes, but add the 
 * functionality to read and write one bit at a time. 
 * 
 * The idea is that you can substitute an ibstream in place of an
 * ifstream and use the same operations (open, get, fail, >>, etc.) 
 * along with added member functions of readbit, rewind, and size.
 *
 * Similarly, the obstream can be used in place of ofstream, and has
 * same operations (open, put, fail, <<, etc.) along with additional
 * member functions writebit and size.
 */
 
#ifndef _bstream_
#define _bstream_

#include <fstream>

/*
 * Class: ibstream
 * ---------------
 * Defines a class for reading files with all the functionality of ifstream
 * along with an added member function for reading a single bit and convenience
 * functions for rewinding the stream back to the beginning and getting the file
 * size. You use this class the same way you do ifstream.
 */

class ibstream: public std::ifstream {
public:
/*
 * Constructor: ibstream
 * Usage: ibstream infile;
 * -----------------------
 * Initializes a new ibstream that is not attached to any file.  Use the
 * open member function to attach the stream to a file.
 */
    ibstream();
		
/*
 * Member function: open
 * Usage: in.open(name.c_str());
 * -----------------------------
 * Attempts to open the named file and attach to the receiving ibstream.
 * Check fail state of stream afterwards to see if was successful.
 */
	void open(const char *filename);
	
/*
 * Member function: readbit
 * Usage: bit = in.readbit();
 * --------------------------
 * Reads a single bit from the ibstream and returns 0 or 1 depending on
 * the bit value.  If the stream is exhausted, EOF (-1) is returned.
 * Raises an error if this ibstream has not been properly opened.
 */
	int readbit();
	
/*
 * Member function: rewind
 * Usage: in.rewind();
 * -------------------
 * Rewinds the ibstream back to the beginning so that subsequent reads
 * start again from the beginning. 	Raises an error if this ibstream
 * has not been properly opened.
 */
	void rewind();

/*
 * Member function: size
 * Usage: sz = in.size();
 * ----------------------
 * Returns the size in bytes of the file attached to this stream.
 * Raises an error if this ibstream has not been properly opened.
 */
	long size();

private:
    std::streampos lastTell;
	int curByte, pos;
};


/*
 * Class: obstream
 * ---------------
 * Defines a class for writing files with all the functionality of ofstream
 * along with an added member function for writing a single bit and a convenience
 * function for getting the file size. You use this class the same way you
 * do ofstream.
 */

class obstream: public std::ofstream {
public:
/*
 * Constructor: obstream
 * Usage: obstream outfile;
 * ------------------------
 * Initializes a new obstream that is not attached to any file.  Use the
 * open member function from ofstream to attach the stream to a file.
 */
  	obstream();
  	
/*
 * Member function: open
 * Usage: out.open(name.c_str());
 * ------------------------------
 * Attempts to open the named file and attach to the receiving obstream.
 * Check fail state of stream afterwards to see if was successful.
 */
    void open(const char *filename);

/*
 * Member function: writebit
 * Usage: out.writebit(1);
 * -----------------------
 * Writes a single bit to the obstream.
 * Raises an error if this ibstream has not been properly opened.
 */
    void writebit(int bit);
	
 /*
  * Member function: size
  * Usage: sz = in.size();
  * ----------------------
  * Returns the size in bytes of the file attached to this stream.
  * Raises an error if this obstream has not been properly opened.
  */
	long size();

private:
    std::streampos lastTell;
	int curByte, pos;
};

#endif