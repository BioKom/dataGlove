
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
 * This file specifies the interface for classes to evaluate a
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
 * This file specifies the interface for classes to evaluate a
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


#ifndef ___N_DATA_GLOVE__N_MAPPER__I_EVALUATE_MESSAGE_GLOVE_STATES__
#define ___N_DATA_GLOVE__N_MAPPER__I_EVALUATE_MESSAGE_GLOVE_STATES__


#include "version.h"

#include "cMessageGloveStates.h"
#include "cMessageSamplingDataFromDataGlove.h"

#include <string>


namespace nDataGlove{
namespace nMapper{


class iEvaluateMessageGloveStates{
public:

	iEvaluateMessageGloveStates();
	
	
	virtual ~iEvaluateMessageGloveStates();
	
	/**
	 * @return the name of this class "iEvaluateMessageGloveStates"
	 */
	virtual std::string getName() const;
	
	/**
	 * This function evaluates a cMessageGloveStates message for the
	 * given cMessageSamplingDataFromDataGlove object.
	 *
	 * @param inPMessageSamplingDataFromDataGlove the data glove sampling
	 * 	message, for which to evaluate the cMessageGloveStates message
	 * @param inNumberOfMessage the number of the message, which is evaluated
	 * @return the cMessageGloveStates message evaluated, or NULL if non
	 * 	could be evaluated
	 * 	(please delete returned object)
	 */
	virtual cMessageGloveStates * evaluate(
		nDataGlove::nModelDataGloveDGTechVHand::
			cMessageSamplingDataFromDataGlove * inPMessageSamplingDataFromDataGlove,
		const unsigned long inNumberOfMessage = 0  ) = 0;
	
};//end class iEvaluateMessageGloveStates


};//end namespace nMapper
};//end namespace nDataGlove

#endif //___N_DATA_GLOVE__N_MAPPER__I_EVALUATE_MESSAGE_GLOVE_STATES__





