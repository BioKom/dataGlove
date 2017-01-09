/**
 * @file cMessageGetIdFromDataGlove
 * file name: cMessageGetIdFromDataGlove.h
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
 * If the the get Id message is send, this message should be the answer.
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
04.02.2016  Oesterholz  copy constructor and clone() added
*/


#ifndef ___N_DATA_GLOVE__N_MODEL_DATA_GLOVE_D_G_TECH_V_HAND__C_MESSAGE_GET_ID_FROM_DATA_GLOVE_H__
#define ___N_DATA_GLOVE__N_MODEL_DATA_GLOVE_D_G_TECH_V_HAND__C_MESSAGE_GET_ID_FROM_DATA_GLOVE_H__


#include "version.h"

#include <string>

#include "cMessageFromDataGlove.h"


namespace nDataGlove{

namespace nModelDataGloveDGTechVHand{

class cMessageGetIdFromDataGlove : public cMessageFromDataGlove {
public:

	/**
	 * The standard constructor for the DGTech VHand data glove message.
	 */
	explicit cMessageGetIdFromDataGlove();
	
	/**
	 * The copy constructor for the DGTech VHand data glove message.
	 *
	 * @param inMessageDataGlove the message to copy
	 */
	cMessageGetIdFromDataGlove( const cMessageGetIdFromDataGlove & inMessageDataGlove );
	
	
	/**
	 * The destructor.
	 */
	virtual ~cMessageGetIdFromDataGlove();
	
	/**
	 * Clones this object.
	 *
	 * @return the clone of this object
	 */
	virtual cMessageGetIdFromDataGlove * clone() const;
	
	
	/**
	 * @return the name of this class "cMessageGetIdFromDataGlove"
	 */
	virtual std::string getName() const;
	
	/**
	 * @return a number for the device type:
	 * 	* 0 -> unkown
	 * 	* 1 -> USB dataglove
	 * 	* 2 -> Wifi Dataglove
	 */
	int getDeviceType() const;
	
	/**
	 * @return a string for the device type
	 */
	std::string getDeviceTypeString() const;
	
	/**
	 * @return a number for the microchip type (important for firmware update):
	 * 	* 1 -> X32 micro device
	 * 	* 2 -> X128 micro device
	 */
	int getMicroType() const;
	
	/**
	 * @return a string for the microchip type:
	 */
	std::string getMicroTypeString() const;

	/**
	 * @return the micro controller identifier/ID
	 */
	int getId() const;
	
	/**
	 * @return if a wifi device: the IP address received from DHCP of
	 * 		the device
	 * 	else: 0
	 */
	long getIp() const;
	
	/**
	 * @return if a for wifi device: the IP address received from DHCP of
	 * 		the device as a string (e.g. 127.0.0.1)
	 * 	else: "0"
	 */
	std::string getIpString() const;
	
	/**
	 * @return if a for wifi device: the netmask of the device
	 */
	long getMask() const;
	
	/**
	 * @return if a for wifi device: the netmask of the device as a string
	 */
	std::string getMaskString() const;
	
	/**
	 * @return if a for wifi device: the gateway IP address of the device
	 */
	long getGateway() const;
	
	/**
	 * @return if a for wifi device: the gateway IP address of the device as a string
	 */
	std::string getGatewayString() const;
	
	/**
	 * @returns true if the DHCP is on, alse false
	 */
	bool getDHCP() const;
	
	//future use getStatus()
	
	
};//end class cMessageGetIdFromDataGlove


};//end namespace nDataGlove
};//end namespace nModelDataGloveDGTechVHand


#endif //___N_DATA_GLOVE__N_MODEL_DATA_GLOVE_D_G_TECH_V_HAND__C_MESSAGE_GET_ID_FROM_DATA_GLOVE_H__





