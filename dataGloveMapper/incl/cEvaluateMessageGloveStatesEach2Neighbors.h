
//TODO check

/**
 * @file cEvaluateMessageGloveStatesEach2Neighbors
 * file name: cEvaluateMessageGloveStatesEach2Neighbors.h
 * @author Betti Oesterholz
 * @date 07.03.2016
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies the class for classes to evaluate a
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
 * This file specifies the class for classes to evaluate a
 * cMessageGloveStates from a cMessageSamplingDataFromDataGlove.
 * It will generate a message with the actual message and For each data
 * glove value the two direct neighbour states (if existing).
 *
 * @see iEvaluateMessageGloveStates
 * @see cMessageSamplingDataFromDataGlove
 * @see cMessageGloveStates
 * @see cThreadSamplingMessageWriter
 */
/*
History:
07.03.2016  Oesterholz  created
*/


#ifndef ___N_DATA_GLOVE__N_MAPPER__C_EVALUATE_MESSAGE_GLOVE_STATES_EACH_2_NEIGHBORS__
#define ___N_DATA_GLOVE__N_MAPPER__C_EVALUATE_MESSAGE_GLOVE_STATES_EACH_2_NEIGHBORS__


#include "version.h"

#include "iEvaluateMessageGloveStates.h"
#include "cMessageGloveStates.h"
#include "cEvaluateDataGloveState.h"

#include <string>
#include <utility>


namespace nDataGlove{
namespace nMapper{


class cEvaluateMessageGloveStatesEach2Neighbors : public iEvaluateMessageGloveStates{
public:

	cEvaluateMessageGloveStatesEach2Neighbors();
	
	virtual ~cEvaluateMessageGloveStatesEach2Neighbors();
	
	/**
	 * @return the name of this class "cEvaluateMessageGloveStatesEach2Neighbors"
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
		const unsigned long inNumberOfMessage = 0 );
	
	
	
	/**
	 * @return the object to evaluate the data glove state
	 * 	@see pEvaluateDataGloveState
	 */
	cEvaluateDataGloveState * getEvaluateDataGloveState();

	/**
	 * Sets the object to evaluate the data glove state.
	 *
	 * @see pEvaluateDataGloveState
	 * @param pInEvaluateDataGloveState a pointer to the widget with the
	 * 	sampling data
	 */
	void setEvaluateDataGloveState( cEvaluateDataGloveState * pInEvaluateDataGloveState );
	
	
	
	/**
	 * Returns the state type number and name for the call function.
	 * (The state is for the call function.)
	 *
	 * @param pCallFunction a pointer to the to call function
	 * @return The state/call function type number and name for the call function as a pair.
	 */
	static std::pair< int, std::string > getStateTypeAndName( const iCallFunction * pCallFunction );

	/**
	 * Returns the state parameter for the call function.
	 * (The state is for the call function.)
	 *
	 * @param pCallFunction a pointer to the to call function
	 * @return The state/call function parameter for the call function as a pair.
	 */
	static std::string getStateParameter( const iCallFunction * pCallFunction );
	
protected:
	
	
	/**
	 * The object which evaluates the data glove data.
	 */
	cEvaluateDataGloveState * pEvaluateDataGloveState;
	
	
	/**
	 * The last data glove message samplin type.
	 * liDigits was evaluated for this sampling type.
	 * @see LiDigits
	 */
	unsigned int SamplinType;
	
	/**
	 * A list with the hand sampling value types for the message type
	 * SamplinType.
	 * @see SamplinType
	 */
	std::list< nModelDataGloveDGTechVHand::cMessageSamplingDataFromDataGlove::
		tTypeSamplingValue > LiDigits;
	
};//end class cEvaluateMessageGloveStatesEach2Neighbors


};//end namespace nMapper
};//end namespace nDataGlove

#endif //___N_DATA_GLOVE__N_MAPPER__C_EVALUATE_MESSAGE_GLOVE_STATES_EACH_2_NEIGHBORS__





