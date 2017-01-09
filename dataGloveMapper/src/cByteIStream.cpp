
//TODO check

/**
 * @file cByteIStream
 * file name: cByteIStream.cpp
 * @author Betti Oesterholz
 * @date 28.12.2016
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file implements a class input bytes to a stream.
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
 * This file implements a class input bytes to a stream.
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

#include "cByteIStream.h"

#include <string.h>


using namespace std;


/**
 * Constructor
 *
 * @param inIstream the istream from which to read the data
 */
cByteIStream::cByteIStream( istream & inIstream ) :
		IStream( inIstream ), TransferBlockStarted( false ), DataReadedError( false ) {
	//nothing to do
}


/**
 * Inserts the first n characters of the array pointed by inCharArray
 * into the stream.
 *
 * @param inCharArray Pointer to an array of at least n characters.
 * @param inReadChars Number of characters to read.
 * @return The cByteIStream object (*this).
 */
cByteIStream & cByteIStream::read( char* inCharArray, streamsize inReadChars ) {
	
	char cBuffer;
	if ( ! TransferBlockStarted ) {
		while ( ( not TransferBlockStarted ) && good() ) {
			//read 0x5C 0x01 -> transfer block start
			IStream.get( cBuffer );
			if ( cBuffer != 0x5C ) {
				DataReadedError = true;
				continue;
			}
			IStream.get( cBuffer );
			if ( cBuffer != 0x01 ) {
				DataReadedError = true;
				continue;
			}
			//0x5C 0x01 readed
			TransferBlockStarted = true;
		}
		if ( ! TransferBlockStarted ) {
			//no transfer block start found
			return *this;
		}
		checksum.clear();
		checksum.addToChecksum( 0x5C );
		checksum.addToChecksum( 0x01 );
	}
	
	//read char for char of the inReadChars chars till a 0x5C char,
	//which needs to be translated to 0x5C 0x5C
	while ( 0 < inReadChars ) {
		
		IStream.get( cBuffer );
		checksum.addToChecksum( cBuffer );
		
		if ( cBuffer == 0x5C ) {
			 //0x5C char found
			//-> read char after 0x5C (which should be 0x5C)
			IStream.get( cBuffer );
			checksum.addToChecksum( cBuffer );
			
			if ( cBuffer != 0x5C ) {
				//the char after 0x5C is not 0x5C -> data error
				DataReadedError = true;
			}
		}
		*inCharArray = cBuffer;
		++inCharArray;
		--inReadChars;
	};
	return *this;
}



/**
 * Reads the one character inChar into the stream.
 *
 * @param inCharArray Character to read.
 * @return The cByteIStream object (*this).
 */
cByteIStream & cByteIStream::get( char inChar ) {
	
	return read( &inChar, 1 );
}


/**
 * Writes a transfer block end, if a block was started.
 *
 * @return True if a transfer block end was read correctly
 */
bool cByteIStream::read_end() {
	if ( ! TransferBlockStarted ) {
		
		return false;
	}  //else
	
	while ( TransferBlockStarted && good() ) {
		//read 0x5C 0x01 -> transfer block start
		char cBuffer;
		IStream.get( cBuffer );
		checksum.addToChecksum( cBuffer );
		if ( cBuffer != 0x5C ) {
			DataReadedError = true;
			continue;
		}
		IStream.get( cBuffer );
		checksum.addToChecksum( cBuffer );
		if ( cBuffer != 0x17 ) {
			DataReadedError = true;
			continue;
		}
		//0x5C 0x01 readed
		TransferBlockStarted = false;
	}
	if ( TransferBlockStarted ) {
		//no transfer block end found
		return false;
	}
	
	//reads a checksum at the end
	char cChecksumBuffer[ checksum.size() ];
	IStream.read( cChecksumBuffer, checksum.size() );
	
	//check if the checksum is correct
	const bool bChecksumOK = checksum.equalChecksum( cChecksumBuffer );
	if ( ( ! DataReadedError ) && ( ! bChecksumOK ) ) {
		//checksum not correct -> data error
		DataReadedError = true;
	}
	
	return bChecksumOK;
}


/**
 * Check whether state of stream is good
 *
 * @return Returns true if none of the stream's error state flags
 * 	(eofbit, failbit and badbit) is set.
 */
bool cByteIStream::good() const {
	
	return IStream.good();
}


/**
 * Check whether data readed of stream is good
 *
 * @return Returns true data readed of stream is good, else false.
 */
bool cByteIStream::dataError() const {
	
	return DataReadedError;
}


