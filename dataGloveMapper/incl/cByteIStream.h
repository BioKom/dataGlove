
//TODO check

/**
 * @file cByteIStream
 * file name: cByteIStream.h
 * @author Betti Oesterholz
 * @date 28.12.2016
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies a class input bytes to a stream.
 *
 *
 * Copyright (C) @c GPL3 2016 Betti Oesterholz
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License (GPL) as
 * published by the Free Software Foundation, either version 3 of the
 * License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * This file specifies a class input bytes to a stream.
 *
 * Functionality:
 * * it reads a checksum at the end of the file
 * * it uses a special escape char '\'=0x5C ; translations
 * ** 0x5C 0x5C -> 0x5C
 * ** 0x5C 0x01 -> transfer block start
 * ** 0x5C 0x17 -> transfer block end
 *
 * Nice to have: it should inherit from istream and work like a normal
 * input stream.
 *
 * @see cByteStreamChecksum
 * @see cByteIStream
 */
/*
History:
28.12.2016  Oesterholz  created
*/

#ifndef ___C_BYTE_I_STREAM__
#define ___C_BYTE_I_STREAM__


#include "version.h"

#include "cByteStreamChecksum.h"

#include <istream>

/**
 * The class encapsults a istream.
 */
class cByteIStream{
public:

	/**
	 * Constructor
	 *
	 * @param inIstream the istream from which to read the data
	 */
	cByteIStream( std::istream & inIstream );
	
	/**
	 * Inserts the first n characters of the array pointed by inCharArray
	 * into the stream.
	 *
	 * @param inCharArray Pointer to an array of at least n characters.
	 * @param inReadChars Number of characters to read.
	 * @return The cByteIStream object (*this).
	 */
	cByteIStream & read( char* inCharArray, std::streamsize inReadChars );
	
	
	/**
	 * Reads the one character inChar into the stream.
	 *
	 * @param inCharArray Character to read.
	 * @return The cByteIStream object (*this).
	 */
	cByteIStream & get( char inChar );
	
	/**
	 * Writes a transfer block end, if a block was started.
	 *
	 * @return True if a transfer block end was read correctly
	 */
	bool read_end();
	
	
	/**
	 * Check whether state of stream is good
	 *
	 * @return Returns true if none of the stream's error state flags
	 * 	(eofbit, failbit and badbit) is set.
	 */
	bool good() const;
	
	/**
	 * Check whether data readed of stream is good
	 *
	 * @return Returns true data readed of stream is good, else false.
	 */
	bool dataError() const;
	
	
protected:
	
	/**
	 * The istream to which this objekt reads.
	 */
	std::istream & IStream;
	
	
	/**
	 * true if a transfer block was started, else false.
	 */
	bool TransferBlockStarted;
	
	/**
	 * true if a transfer block was started, else false.
	 */
	bool DataReadedError;
	
	/**
	 * The checksum of the actual transfer block.
	 */
	cByteStreamChecksum checksum;
	
};//cByteIStream

#endif //___C_BYTE_I_STREAM__


