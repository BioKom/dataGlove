
//TODO check

/**
 * @file iEvaluateMessageGloveStates
 * file name: iEvaluateMessageGloveStates.h
 * @author Betti Oesterholz
 * @date 07.03.2016
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file implements the interface for classes to evaluate a
 * cMessageGloveStates from a cMessageSamplingDataFromDataGlove.
 *
 *
 * Copyright (C) @c GPL3 2016 Betti Oesterholz
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
 * This file implements the interface for classes to evaluate a
 * cMessageGloveStates from a cMessageSamplingDataFromDataGlove.
 *
 * @pattern interface
 * @see cMessageSamplingDataFromDataGlove
 * @see cMessageGloveStates
 * @see cThreadSamplingMessageWriter
 */
/*
History:
07.03.2016  Oesterholz  created
*/


#include "iEvaluateMessageGloveStates.h"



using namespace nDataGlove::nMapper;


iEvaluateMessageGloveStates::iEvaluateMessageGloveStates() {
	//nothing to do
}


iEvaluateMessageGloveStates::~iEvaluateMessageGloveStates() {
	//nothing to do
}


/**
 * @return the name of this class "iEvaluateMessageGloveStates"
 */
std::string iEvaluateMessageGloveStates::getName() const {
	
	return "iEvaluateMessageGloveStates";
}





