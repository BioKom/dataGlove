
//TODO check

/**
 * @file cByteStreamChecksum
 * file name: cByteStreamChecksum.h
 * @author Betti Oesterholz
 * @date 27.12.2016
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies a class for byte checksums.
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
 * This file specifies a class for byte checksums.
 */
/*
History:
27.12.2016  Oesterholz  created
*/

#ifndef ___C_BYTE_STREAM_CHECKSUM__
#define ___C_BYTE_STREAM_CHECKSUM__


#include "version.h"

#include <ios>
#include <cstdio>
#include <string.h>


/**
 * The class encapsults a ostream.
 */
class cByteStreamChecksum{
public:

	/**
	 * Constructor
	 */
	cByteStreamChecksum();
	
	
	/**
	 * Compares this checksum to the given Checksum (as readed).
	 * Beware: The given string should have at minimum checksumWidth() chars.
	 *
	 * @see checksumWidth()
	 * @param inChar the character to add to the checksum
	 */
	bool equalChecksum( const char * inChecksum );
	
protected:
	
	/**
	 * The bytes writen in the actual transfer block.
	 */
	std::streamsize BytsInBlock;
	
	/**
	 * Number of bytes in the checksum.
	 */
	static const size_t CHECKSUM_WIDTH = 4;
	
	/**
	 * The checksum to write.
	 */
	char Checksum[ CHECKSUM_WIDTH ];
	
public:
	
	/**
	 * Clears this checksum.
	 */
	inline void clear() {
		
		BytsInBlock = 0;
		memset( Checksum, 0x00, CHECKSUM_WIDTH );
	}
	
	/**
	 * Adds the given character to the checksum
	 *
	 * @param inChar the character to add to the checksum
	 */
	inline void addToChecksum( const char inChar ) {
		
		++BytsInBlock;
		//use prime numers for the checksum bytes
		Checksum[ 0 ] = Checksum[ 0 ] ^ inChar;
		if ( ( BytsInBlock & 0x01 ) != 0 ) {  // BytsInBlock % 2 = 1
			Checksum[ 1 ] = Checksum[ 1 ] ^ inChar;
		}
		if ( ( BytsInBlock % 3 ) != 0 ) {
			Checksum[ 2 ] = Checksum[ 2 ] ^ inChar;
		}
		if ( ( BytsInBlock % 5 ) != 0 ) {
			Checksum[ 3 ] = Checksum[ 3 ] ^ inChar;
		}
	}
	
	/**
	 * @return the number of bytes in the checksum
	 */
	static constexpr std::size_t size() {
		
		return CHECKSUM_WIDTH;
	}
	
	/**
	 * @return the string of the checksum with checksumWidth() chars
	 * @see checksumWidth()
	 */
	inline const char * c_str() const {
		
		return Checksum;
	}
	
};//cByteStreamChecksum

#endif //___C_BYTE_STREAM_CHECKSUM__


