
//TODO check

/**
 * @file cMessageGetIdToDataGlove
 * file name: cMessageGetIdToDataGlove.cpp
 * @author Betti Oesterholz
 * @date 11.07.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file implements the parent class for all DGTech VHand data glove
 * messages, which are send to the data glove.
 *
 *
 * Copyright (C) @c GPL3 2014 Betti Oesterholz
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
 * This file implements the parent class for all DGTech VHand data glove
 * messages, which are send to the data glove.
 * All Messages which the  DGTech VHand data glove can receive should
 * inherit this class.
 * See DG5 VHand 3.0 OEM Technical Datasheet.
 * @see www.dg-tech.it
 * @see www.dg-tech.it/vhand3/
 *
 * Tested with:
 * 	*  DGTech VHand data glove 3.0 left handed
 *
 * @see cMessageGetIdFromDataGlove
 */
/*
History:
11.07.2014  Oesterholz  created
*/


#include "cMessageGetIdToDataGlove.h"

#include <cstdlib>



using namespace nDataGlove::nModelDataGloveDGTechVHand;
using namespace std;


/**
 * The standard constructor for the DGTech VHand data glove message.
 *
 * @param bCreateMessage if true the message will be created, else
 * 	this message will just contain an empty message
 */
cMessageGetIdToDataGlove::cMessageGetIdToDataGlove(
		const bool bCreateMessage ) {
	
	cType = DATA_GLOVE_D_G_TECH_V_HAND__CMD_GET_ID;
	if ( bCreateMessage ) {
		uiMessageSize = 5;
		szMessage = static_cast<unsigned char  *>(malloc( uiMessageSize + 2 ));
		szMessage[ 0 ] = '$'; //0x24
		szMessage[ 1 ] = DATA_GLOVE_D_G_TECH_V_HAND__CMD_GET_ID;
		szMessage[ 2 ] = 0x02;  //num bytes
		szMessage[ 3 ] = 0x50;  //CRC
		szMessage[ 4 ] = '#';
		szMessage[ 5 ] = 0x0;
	}
}


/**
 * The destructor.
 */
cMessageGetIdToDataGlove::~cMessageGetIdToDataGlove() {
	
	//nothing to do
}


/**
 * @return the name of this class "cMessageGetIdToDataGlove"
 */
std::string cMessageGetIdToDataGlove::getName() const {
	
	return string( "cMessageGetIdToDataGlove" );
}





