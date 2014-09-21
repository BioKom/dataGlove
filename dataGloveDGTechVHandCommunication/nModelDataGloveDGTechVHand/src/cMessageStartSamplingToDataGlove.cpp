
//TODO check

/**
 * @file cMessageStartSamplingToDataGlove
 * file name: cMessageStartSamplingToDataGlove.cpp
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
 * @see cMessageSamplingDataFromDataGlove
 * @see cMessageStopSamplingToDataGlove
 */
/*
History:
19.07.2014  Oesterholz  created
*/


#include "cMessageStartSamplingToDataGlove.h"



using namespace nDataGlove::nModelDataGloveDGTechVHand;
using namespace std;


/**
 * The standard constructor for the DGTech VHand data glove message.
 *
 * @param bCreateMessage if true the message will be created, else
 * 	this message will just contain an empty message
 */
cMessageStartSamplingToDataGlove::cMessageStartSamplingToDataGlove(
		const bool bCreateMessage ) {
	
	type = START_SAMPLINT;
	cCommand = DATA_GLOVE_D_G_TECH_V_HAND__CMD_GET_ID;
	if ( bCreateMessage ) {
		uiMessageSize = 6;
		szMessage = new unsigned char[ uiMessageSize + 2 ];
		szMessage[ 0 ] = '$'; //0x24
		szMessage[ 1 ] = DATA_GLOVE_D_G_TECH_V_HAND__CMD_START_SAMPLING;
		szMessage[ 2 ] = 0x03;  //num bytes
		szMessage[ 3 ] = 0x01;  //send quaternion orientation and finger sensors values
		szMessage[ 4 ] = 0x50;  //CRC
		szMessage[ 5 ] = '#';
		szMessage[ 6 ] = 0x0;
	}
}

/**
 * The standard constructor for the DGTech VHand data glove message.
 *
 * @param iSamplingType a number for the Sampling package format
 * 	possible values are:
 * 		0: stop comunicating
 * 		1: send quaternion orientation and finger sensors values
 * 		2: send only quaternion values
 * 		3: send raw gyroscope data, raw accelerometer data,
 * 			raw magnetometer data and finger sensor values
 * 		4: send only raw data
 * 		5: send only finger data
 */
cMessageStartSamplingToDataGlove::cMessageStartSamplingToDataGlove(
		const int iSamplingType ) {

	type = START_SAMPLINT;
	cCommand = DATA_GLOVE_D_G_TECH_V_HAND__CMD_START_SAMPLING;
	uiMessageSize = 6;
	szMessage = new unsigned char[ uiMessageSize + 2 ];
	szMessage[ 0 ] = '$'; //0x24
	szMessage[ 1 ] = DATA_GLOVE_D_G_TECH_V_HAND__CMD_START_SAMPLING; //0x0A
	szMessage[ 2 ] = 0x03;  //num bytes
	switch ( iSamplingType ) {
		case 1: {
			szMessage[ 3 ] = 0x01;  //send quaternion orientation and finger sensors values
		}; break;
		case 2: {
			szMessage[ 3 ] = 0x02;  //send only quaternion values
		}; break;
		case 3: {
			szMessage[ 3 ] = 0x03;  //send raw gyroscope data, raw accelerometer data,
			//raw magnetometer data and finger sensor values
		}; break;
		case 4: {
			szMessage[ 3 ] = 0x04;  //send only raw data
		}; break;
		case 5: {
			szMessage[ 3 ] = 0x05;  //send only finger data
		}; break;
		default: {
			szMessage[ 3 ] = 0x00;  //stop comunicating
		};
	};  //end switch ( iSamplingType )
	szMessage[ 4 ] = evalueCRC( szMessage, 3 );  //CRC
	szMessage[ 5 ] = '#';
	szMessage[ 6 ] = 0x0;
}


/**
 * The destructor.
 */
cMessageStartSamplingToDataGlove::~cMessageStartSamplingToDataGlove() {
	
	//nothing to do
}


/**
 * @return the name of this class "cMessageStartSamplingToDataGlove"
 */
std::string cMessageStartSamplingToDataGlove::getName() const {
	
	return string( "cMessageStartSamplingToDataGlove" );
}





