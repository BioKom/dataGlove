
//TODO check

/**
 * @file cByteOStream
 * file name: cByteOStream.h
 * @author Betti Oesterholz
 * @date 08.12.2016
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies a class output bytes to a stream.
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
 * This file specifies a class output bytes to a stream.
 *
 * Functionality:
 * * it writes a checksum at the end of the file
 * * it uses a special escape char '\'=0x5C ; translations
 * ** 0x5C 0x5C -> 0x5C
 * ** 0x5C 0x01 -> transfer block start
 * ** 0x5C 0x17 -> transfer block end
 *
 * Nice to have: it should inherit from ostream and work like a normal
 * output stream.
 *
 * @see cByteStreamChecksum
 * @see cByteIStream
 */
/*
History:
08.12.2016  Oesterholz  created
*/

#ifndef ___C_BYTE_O_STREAM__
#define ___C_BYTE_O_STREAM__


#include "version.h"

#include "cByteStreamChecksum.h"

#include <ostream>

/**
 * The class encapsults a ostream.
 */
class cByteOStream{
public:

	/**
	 * Constructor
	 *
	 * @param inOstream the ostream to which to write the data
	 */
	cByteOStream( std::ostream & inOstream );
	
	/**
	 * Inserts the first n characters of the array pointed by inCharArray
	 * into the stream.
	 *
	 * @param inCharArray Pointer to an array of at least n characters.
	 * @param inWriteChars Number of characters to insert.
	 * @return The cByteOStream object (*this).
	 */
	cByteOStream & write(const char* inCharArray, std::streamsize inWriteChars );
	
	
	/**
	 * Inserts the one character inChar into the stream.
	 *
	 * @param inCharArray Character to write.
	 * @return The cByteOStream object (*this).
	 */
	cByteOStream & put( char inChar );
	
	/**
	 * Synchronizes the associated stream buffer with its controlled output sequence.
	 * @return The cByteOStream object (*this).
	 */
	cByteOStream & flush();
	
	/**
	 * Writes a transfer block end, if a block was started.
	 *
	 * @return True if a transfer block end was writen
	 */
	bool write_end();
	
	
	/**
	 * Check whether state of stream is good
	 *
	 * @return Returns true if none of the stream's error state flags
	 * 	(eofbit, failbit and badbit) is set.
	 */
	bool good() const;
	
	
	
protected:
	
	/**
	 * The ostream to which this objekt writes.
	 */
	std::ostream & OStream;
	
	
	/**
	 * true if a transfer block was started, else false.
	 */
	bool TransferBlockStarted;
	
	
	/**
	 * The checksum of the actual transfer block.
	 */
	cByteStreamChecksum checksum;
	
};//cByteOStream

#endif //___C_BYTE_O_STREAM__


