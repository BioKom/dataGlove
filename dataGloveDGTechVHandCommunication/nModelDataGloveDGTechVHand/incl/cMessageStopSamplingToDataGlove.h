
//TODO check

/**
 * @file cMessageStopSamplingToDataGlove
 * file name: cMessageStopSamplingToDataGlove.h
 * @author Betti Oesterholz
 * @date 11.07.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies the class for the DGTech VHand data glove
 * messages, to stop sampling the hand data.
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
 * messages, to stop sampling the hand data.
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
11.07.2014  Oesterholz  created
04.02.2016  Oesterholz  copy constructor and clone() added
*/


#ifndef ___N_DATA_GLOVE__N_MODEL_DATA_GLOVE_D_G_TECH_V_HAND__C_MESSAGE_STOP_SAMPLING_TO_DATA_GLOVE_H__
#define ___N_DATA_GLOVE__N_MODEL_DATA_GLOVE_D_G_TECH_V_HAND__C_MESSAGE_STOP_SAMPLING_TO_DATA_GLOVE_H__


#include "version.h"

#include <string>

#include "cMessageToDataGlove.h"


namespace nDataGlove{

namespace nModelDataGloveDGTechVHand{

class cMessageStopSamplingToDataGlove : public cMessageToDataGlove {
public:

	/**
	 * The standard constructor for the DGTech VHand data glove message.
	 *
	 * @param bCreateMessage if true the message will be created, else
	 * 	this message will just contain an empty message
	 */
	explicit cMessageStopSamplingToDataGlove( const bool bCreateMessage = true );
	
	/**
	 * The copy constructor for the DGTech VHand data glove message.
	 *
	 * @param inMessageDataGlove the message to copy
	 */
	cMessageStopSamplingToDataGlove( const cMessageStopSamplingToDataGlove & inMessageDataGlove );
	
	/**
	 * The destructor.
	 */
	virtual ~cMessageStopSamplingToDataGlove();
	
	/**
	 * Clones this object.
	 *
	 * @return the clone of this object
	 */
	virtual cMessageStopSamplingToDataGlove * clone() const;
	
	
	/**
	 * @return the name of this class "cMessageStopSamplingToDataGlove"
	 */
	virtual std::string getName() const;
	
	
};//end class cMessageStopSamplingToDataGlove


};//end namespace nDataGlove
};//end namespace nModelDataGloveDGTechVHand

#endif //___N_DATA_GLOVE__N_MODEL_DATA_GLOVE_D_G_TECH_V_HAND__C_MESSAGE_STOP_SAMPLING_TO_DATA_GLOVE_H__





