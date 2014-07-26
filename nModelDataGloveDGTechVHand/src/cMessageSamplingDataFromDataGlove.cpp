
//TODO check

/**
 * @file cMessageSamplingDataFromDataGlove
 * file name: cMessageSamplingDataFromDataGlove.cpp
 * @author Betti Oesterholz
 * @date 13.07.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file implements the class for the DGTech VHand data glove
 * messages, which is the sampling data from the data glove.
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
 * messages, which is the sampling data from the data glove.
 * If the get the get Id message is send, this message should be the answer.
 * See DG5 VHand 3.0 OEM Technical Datasheet.
 * @see www.dg-tech.it
 * @see www.dg-tech.it/vhand3/
 *
 * Tested with:
 * 	*  DGTech VHand data glove 3.0 left handed
 *
 * @see cMessageStartSamplingToDataGlove
 * @see cMessageStopSamplingToDataGlove
 */
/*
History:
13.07.2014  Oesterholz  created
*/



#include "cMessageSamplingDataFromDataGlove.h"


using namespace nDataGlove::nModelDataGloveDGTechVHand;
using namespace std;


/**
 * The standard constructor for the DGTech VHand data glove message.
 */
cMessageSamplingDataFromDataGlove::cMessageSamplingDataFromDataGlove() {
	
	cType = DATA_GLOVE_D_G_TECH_V_HAND__CMD_START_SAMPLING;
}


/**
 * The destructor.
 */
cMessageSamplingDataFromDataGlove::~cMessageSamplingDataFromDataGlove() {
	
	//nothing to do
}


/**
 * @return the name of this class "cMessageSamplingDataFromDataGlove"
 */
std::string cMessageSamplingDataFromDataGlove::getName() const {
	
	return string( "cMessageSamplingDataFromDataGlove" );
}










