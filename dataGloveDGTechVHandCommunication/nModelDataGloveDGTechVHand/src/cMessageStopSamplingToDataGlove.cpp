
//TODO check

/**
 * @file cMessageStopSamplingToDataGlove
 * file name: cMessageStopSamplingToDataGlove.cpp
 * @author Betti Oesterholz
 * @date 19.07.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file implements the class for the DGTech VHand data glove
 * messages, to start sampling the hand data.
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
 * This file implements the class for the DGTech VHand data glove
 * messages, to start sampling the hand data.
 * All Messages which the  DGTech VHand data glove can receive should
 * inherit this class.
 * See DG5 VHand 3.0 OEM Technical Datasheet.
 * @see www.dg-tech.it
 * @see www.dg-tech.it/vhand3/
 *
 * Tested with:
 * 	*  DGTech VHand data glove 3.0 left handed
 *
 * @see cMessageStartSamplingToDataGlove
 * @see cMessageSamplingDataFromDataGlove
 */
/*
History:
19.07.2014  Oesterholz  created
*/


#include "cMessageStopSamplingToDataGlove.h"



using namespace nDataGlove::nModelDataGloveDGTechVHand;
using namespace std;


/**
 * The standard constructor for the DGTech VHand data glove message.
 *
 * @param bCreateMessage if true the message will be created, else
 * 	this message will just contain an empty message
 */
cMessageStopSamplingToDataGlove::cMessageStopSamplingToDataGlove(
		const bool bCreateMessage ) {
	
	type = STOP_SAMPLING;
	cCommand = DATA_GLOVE_D_G_TECH_V_HAND__CMD_STOP_SAMPLING;
	if ( bCreateMessage ) {
		uiMessageSize = 5;
		szMessage = new unsigned char[ uiMessageSize + 2 ];
		szMessage[ 0 ] = '$'; //0x24
		szMessage[ 1 ] = 0x0B;  //DATA_GLOVE_D_G_TECH_V_HAND__CMD_STOP_SAMPLING
		szMessage[ 2 ] = 0x02;  //num bytes
		szMessage[ 3 ] = 0x31;  //CRC
		szMessage[ 4 ] = '#';
		szMessage[ 5 ] = 0x0;
	}
}

/**
 * The destructor.
 */
cMessageStopSamplingToDataGlove::~cMessageStopSamplingToDataGlove() {
	
	//nothing to do
}


/**
 * @return the name of this class "cMessageStopSamplingToDataGlove"
 */
std::string cMessageStopSamplingToDataGlove::getName() const {
	
	return string( "cMessageStopSamplingToDataGlove" );
}





