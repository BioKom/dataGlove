
//TODO check

/**
 * @file cByteOStream
 * file name: cByteOStream.cpp
 * @author Betti Oesterholz
 * @date 08.12.2016
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file implements a class output bytes to a stream.
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
 * This file implements a class output bytes to a stream.
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

#include "cByteOStream.h"

#include <string.h>


using namespace std;


/**
 * Constructor
 *
 * @param inOstream the ostream to which to write the data
 */
cByteOStream::cByteOStream( ostream & inOstream ) :
		OStream( inOstream ), TransferBlockStarted( false ) {
	//nothing to do
}


/**
 * Inserts the first n characters of the array pointed by inCharArray
 * into the stream.
 *
 * @param inCharArray Pointer to an array of at least n characters.
 * @param inWriteChars Number of characters to insert.
 * @return The cByteOStream object (*this).
 */
cByteOStream & cByteOStream::write(const char* inCharArray, streamsize inWriteChars ) {
	
	if ( ! TransferBlockStarted ) {
		//write 0x5C 0x01 -> transfer block start
		OStream.put( 0x5C );
		OStream.put( 0x01 );
		TransferBlockStarted = true;
		checksum.clear();
		checksum.addToChecksum( 0x5C );
		checksum.addToChecksum( 0x01 );
	}
	//write block for block of the inWriteChars chars till a 0x5C char,
	//which needs to be translated to 0x5C 0x5C
	streamsize writeCharsNext = 0;
	while ( writeCharsNext < inWriteChars ) {
		
		if ( inCharArray[ writeCharsNext ] != 0x5C ) {
			
			checksum.addToChecksum( inCharArray[ writeCharsNext ] );
			++writeCharsNext;
			continue;
		} else {  //0x5C char found
			//-> write the string till the char before the 0x5C and add the char 0x5C to the end
			if ( 0 < writeCharsNext ) {
				OStream.write( inCharArray, writeCharsNext );
				inCharArray = inCharArray + writeCharsNext;
				inWriteChars = inWriteChars - writeCharsNext;
			}
			//translate 0x5C to 0x5C 0x5C
			OStream.put( 0x5C );
			checksum.addToChecksum( 0x5C );
			checksum.addToChecksum( 0x5C );
			writeCharsNext = 1;  //can write the 0x5C now
		}
	}
	if ( 0 < writeCharsNext ) {
		//write the rest of the to write chars
		OStream.write( inCharArray, writeCharsNext );
	}
	return *this;
}



/**
 * Inserts the one character inChar into the stream.
 *
 * @param inCharArray Character to write.
 * @return The cByteOStream object (*this).
 */
cByteOStream & cByteOStream::put( char inChar ) {
	
	return write( &inChar, 1 );
}


/**
 * Synchronizes the associated stream buffer with its controlled output sequence.
 * @return The cByteOStream object (*this).
 */
cByteOStream & cByteOStream::flush() {
	
	OStream.flush();
	return (*this);
}


/**
 * Writes a transfer block end, if a block was started.
 *
 * @return True if a transfer block end was writen
 */
bool cByteOStream::write_end() {
	if ( ! TransferBlockStarted ) {
		
		return false;
	}  //else
	
	OStream.put( 0x5C );
	OStream.put( 0x17 );
	checksum.addToChecksum( 0x5C );
	checksum.addToChecksum( 0x17 );
	//writes a checksum at the end
	OStream.write( checksum.c_str(), checksum.size() );
	checksum.clear();
	
	TransferBlockStarted = false;
	return true;
}


/**
 * Check whether state of stream is good
 *
 * @return Returns true if none of the stream's error state flags
 * 	(eofbit, failbit and badbit) is set.
 */
bool cByteOStream::good() const {
	
	return OStream.good();
}


