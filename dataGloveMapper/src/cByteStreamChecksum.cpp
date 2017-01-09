
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

#include "cByteStreamChecksum.h"



using namespace std;


/**
 * Constructor
 */
cByteStreamChecksum::cByteStreamChecksum() {
	clear();
}


/**
 * Compares this checksum to the given Checksum (as readed).
 * Beware: The given string should have at minimum checksumWidth() chars.
 *
 * @see checksumWidth()
 * @param inChar the character to add to the checksum
 */
bool cByteStreamChecksum::equalChecksum( const char * inChecksum ) {
	
	return ( memcmp( Checksum, inChecksum, CHECKSUM_WIDTH ) == 0 );
}




