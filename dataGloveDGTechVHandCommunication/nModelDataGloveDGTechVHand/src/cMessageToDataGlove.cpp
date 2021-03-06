/**
 * @file cMessageToDataGlove
 * file name: cMessageToDataGlove.cpp
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
 * All Messages which the DGTech VHand data glove can receive should
 * inherit this class.
 * See DG5 VHand 3.0 OEM Technical Datasheet.
 * @see www.dg-tech.it
 * @see www.dg-tech.it/vhand3/
 *
 * Tested with:
 * 	*  DGTech VHand data glove 3.0 left handed
 *
 */
/*
History:
11.07.2014  Oesterholz  created
04.02.2016  Oesterholz  copy constructor and clone() added
*/


#include "cMessageToDataGlove.h"



using namespace nDataGlove::nModelDataGloveDGTechVHand;
using namespace std;


/**
 * The standard constructor for the DGTech VHand data glove message.
 */
cMessageToDataGlove::cMessageToDataGlove() :
		cMessageDataGlove() {
	//nothing to do
}


/**
 * The copy constructor for the DGTech VHand data glove message.
 *
 * @param inMessageDataGlove the message to copy
 */
cMessageToDataGlove::cMessageToDataGlove(
		const cMessageToDataGlove & inMessageDataGlove ) :
			cMessageDataGlove( inMessageDataGlove ) {
	//nothing to do
}


/**
 * The destructor.
 */
cMessageToDataGlove::~cMessageToDataGlove() {
	//nothing to do
}


/**
 * Clones this object.
 *
 * @return the clone of this object
 */
cMessageToDataGlove * cMessageToDataGlove::clone() const {
	
	return new cMessageToDataGlove( *this );
}


/**
 * @return the name of this class "cMessageToDataGlove"
 */
std::string cMessageToDataGlove::getName() const {
	
	return string( "cMessageToDataGlove" );
}





