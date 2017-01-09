/**
 * @class iSetMessageGloveStates
 * file name: iSetMessageGloveStates.h
 * @author Betti Oesterholz
 * @date 04.02.2016
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file contains the interface for to set a new data glove state message.
 *
 * Copyright (C) @c LGPL3 2016 Betti Oesterholz
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation, either version 3 of the
 * License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * This file contains the interface for to set a new data glove state message.
 * @see cMessageGloveStates
 */
/*
History:
04.02.2016  Oesterholz  created
*/


#ifndef ___N_DATA_GLOVE__N_INFO___I_SET_MESSAGE_GLOVE_STATES_H__
#define ___N_DATA_GLOVE__N_INFO___I_SET_MESSAGE_GLOVE_STATES_H__

#include "version.h"

#include "cMessageGloveStates.h"



namespace nDataGlove{
namespace nInfo{


class iSetMessageGloveStates{
public:
	
	/**
	 * Sets the new data glove state message.
	 * Also cares that the given message is deleted.
	 *
	 * @param pInMessageGloveStates a pointer the new data glove state message
	 */
	virtual void setMessageGloveStates(
		nDataGlove::nMapper::cMessageGloveStates *
		pInMessageGloveStates ) = 0;

};//class iSetMessageGloveStates

};  //namespace nInfo
};  //namespace nDataGlove

#endif


