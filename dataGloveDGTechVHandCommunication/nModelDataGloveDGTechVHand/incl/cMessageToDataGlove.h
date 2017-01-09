/**
 * @file cMessageToDataGlove
 * file name: cMessageToDataGlove.h
 * @author Betti Oesterholz
 * @date 11.07.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies the parent class for all DGTech VHand data glove
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
 * This file specifies the parent class for all DGTech VHand data glove
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
 */
/*
History:
11.07.2014  Oesterholz  created
04.02.2016  Oesterholz  copy constructor and clone() added
*/


#ifndef ___N_DATA_GLOVE__N_MODEL_DATA_GLOVE_D_G_TECH_V_HAND__C_MESSAGE_TO_DATA_GLOVE_H__
#define ___N_DATA_GLOVE__N_MODEL_DATA_GLOVE_D_G_TECH_V_HAND__C_MESSAGE_TO_DATA_GLOVE_H__


#include "version.h"

#include <string>

#include "cMessageDataGlove.h"


namespace nDataGlove{

namespace nModelDataGloveDGTechVHand{

class cMessageToDataGlove : public cMessageDataGlove {
public:

	/**
	 * The standard constructor for the DGTech VHand data glove message.
	 */
	cMessageToDataGlove();
	
	/**
	 * The copy constructor for the DGTech VHand data glove message.
	 *
	 * @param inMessageDataGlove the message to copy
	 */
	cMessageToDataGlove( const cMessageToDataGlove & inMessageDataGlove );
	
	/**
	 * The destructor.
	 */
	virtual ~cMessageToDataGlove();
	
	/**
	 * Clones this object.
	 *
	 * @return the clone of this object
	 */
	virtual cMessageToDataGlove * clone() const;
	
	
	/**
	 * @return the name of this class "cMessageToDataGlove"
	 */
	virtual std::string getName() const;
	
	
};//end class cMessageToDataGlove


};//end namespace nDataGlove
};//end namespace nModelDataGloveDGTechVHand

#endif //___N_DATA_GLOVE__N_MODEL_DATA_GLOVE_D_G_TECH_V_HAND__C_MESSAGE_TO_DATA_GLOVE_H__





