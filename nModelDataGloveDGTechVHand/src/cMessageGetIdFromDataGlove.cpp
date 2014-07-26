
//TODO check

/**
 * @file cMessageGetIdFromDataGlove
 * file name: cMessageGetIdFromDataGlove.cpp
 * @author Betti Oesterholz
 * @date 13.07.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies the class for the DGTech VHand data glove
 * messages, which is the answer to the get the Id message.
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
 * This file specifies the class for the DGTech VHand data glove
 * messages, which is the answer to the get the Id message.
 * If the get the get Id message is send, this message should be the answer.
 * See DG5 VHand 3.0 OEM Technical Datasheet.
 * @see www.dg-tech.it
 * @see www.dg-tech.it/vhand3/
 *
 * Tested with:
 * 	*  DGTech VHand data glove 3.0 left handed
 *
 * @see cMessageGetIdToDataGlove
 */
/*
History:
13.07.2014  Oesterholz  created
*/


#include "cMessageGetIdFromDataGlove.h"

#include <cstdlib>
#include <cstdio>



using namespace nDataGlove::nModelDataGloveDGTechVHand;
using namespace std;


/**
 * The standard constructor for the DGTech VHand data glove message.
 *
 * @param bCreateMessage if true the message will be created, else
 * 	this message will just contain an empty message
 */
cMessageGetIdFromDataGlove::cMessageGetIdFromDataGlove() {
	
	cType = DATA_GLOVE_D_G_TECH_V_HAND__CMD_GET_ID;
}


/**
 * The destructor.
 */
cMessageGetIdFromDataGlove::~cMessageGetIdFromDataGlove() {
	
	//nothing to do
}


/**
 * @return the name of this class "cMessageGetIdFromDataGlove"
 */
std::string cMessageGetIdFromDataGlove::getName() const {
	
	return string( "cMessageGetIdFromDataGlove" );
}



/**
 * @return a number for the device type:
 * 	* 0 -> unkown
 * 	* 1 -> USB dataglove
 * 	* 2 -> Wifi Dataglove
 */
int cMessageGetIdFromDataGlove::getDeviceType() const {
	
	if ( ( szMessage == NULL ) || ( uiMessageSize <= 4 ) ) {
		return 0;
	}
	return static_cast<int>( szMessage[ 3 ] );
}


/**
 * @return a string for the device type
 */
std::string cMessageGetIdFromDataGlove::getDeviceTypeString() const {
	
	if ( ( szMessage == NULL ) || ( uiMessageSize <= 4 ) ) {
		return 0;
	}
	switch ( szMessage[ 3 ] ) {
		case 0x01 : return "USB dataglove";
		case 0x02 : return "Wifi Dataglove";
	};  //end switch ( szMessage[ 3 ] )
	return "unkown";
}


/**
 * @return a number for the microchip type (important for firmware update):
 * 	* 1 -> X32 micro device
 * 	* 2 -> X128 micro device
 */
int cMessageGetIdFromDataGlove::getMicroType() const {
	
	if ( ( szMessage == NULL ) || ( uiMessageSize <= 5 ) ) {
		return 0;
	}
	return static_cast<int>( szMessage[ 4 ] );
}


/**
 * @return a string for the microchip type:
 */
std::string cMessageGetIdFromDataGlove::getMicroTypeString() const {
	
	if ( ( szMessage == NULL ) || ( uiMessageSize <= 5 ) ) {
		return 0;
	}
	switch ( szMessage[ 4 ] ) {
		case 0x01 : return "X32 micro device";
		case 0x02 : return "X128 micro devicee";
	};  //end switch ( szMessage[ 3 ] )
	return "unkown";
}



/**
 * @return the micro controller identifier/ID
 */
int cMessageGetIdFromDataGlove::getId() const {
	
	return read2ByteInt( 5 );
}



/**
 * @return if a for wifi device: the ip address received from DHCP of
 * 		the device
 * 	else: 0
 */
long cMessageGetIdFromDataGlove::getIp() const {
	
	return read4ByteLong( 7 );
}


/**
 * @return if a for wifi device: the ip address received from DHCP of
 * 		the device as a string (e.g. 127.0.0.1)
 * 	else: "0"
 */
std::string cMessageGetIdFromDataGlove::getIpString() const {
	
	if ( ( szMessage == NULL ) || ( uiMessageSize <= 11 ) ) {
		return 0;
	}
	const int iId1 = static_cast<int>( szMessage[ 7 ] );
	const int iId2 = static_cast<int>( szMessage[ 8 ] );
	const int iId3 = static_cast<int>( szMessage[ 9 ] );
	const int iId4 = static_cast<int>( szMessage[ 10 ] );
	
	char szIpString[ 32 ];
	
	sprintf( szIpString, "%d.%d.%d.%d", iId1, iId2, iId3, iId4 );
	
	return std::string( szIpString );
}


/**
 * @return if a for wifi device: the netmask of the device
 */
long cMessageGetIdFromDataGlove::getMask() const {
	
	return read4ByteLong( 11 );
}


/**
 * @return if a for wifi device: the netmask of the device as a string
 */
std::string cMessageGetIdFromDataGlove::getMaskString() const {
	
	if ( ( szMessage == NULL ) || ( uiMessageSize <= 11 ) ) {
		return 0;
	}
	const int iNetmask1 = static_cast<int>( szMessage[ 7 ] );
	const int iNetmask2 = static_cast<int>( szMessage[ 8 ] );
	const int iNetmask3 = static_cast<int>( szMessage[ 9 ] );
	const int iNetmask4 = static_cast<int>( szMessage[ 10 ] );
	
	char szIpString[ 32 ];
	
	sprintf( szIpString, "%d.%d.%d.%d", iNetmask1, iNetmask2, iNetmask3, iNetmask4 );
	
	return std::string( szIpString );
}


/**
 * @return if a for wifi device: the gateway ip address of the device
 */
long cMessageGetIdFromDataGlove::getGateway() const {
	
	return read4ByteLong( 15 );
}


/**
 * @return if a for wifi device: the gateway ip address of the device as a string
 */
std::string cMessageGetIdFromDataGlove::getGatewayString() const {
	
	if ( ( szMessage == NULL ) || ( uiMessageSize <= 19 ) ) {
		return 0;
	}
	const int iNetmask1 = static_cast<int>( szMessage[ 15 ] );
	const int iNetmask2 = static_cast<int>( szMessage[ 16 ] );
	const int iNetmask3 = static_cast<int>( szMessage[ 17 ] );
	const int iNetmask4 = static_cast<int>( szMessage[ 18 ] );
	
	char szIpString[ 32 ];
	
	sprintf( szIpString, "%d.%d.%d.%d", iNetmask1, iNetmask2, iNetmask3, iNetmask4 );
	
	return std::string( szIpString );
}


/**
 * @returns true if the DHCP is on, alse false
 */
bool cMessageGetIdFromDataGlove::getDHCP() const {
	
	if ( ( szMessage == NULL ) || ( uiMessageSize <= 20 ) ) {
		return 0;
	}
	return ( szMessage[ 19 ] == 0x01 );
}






