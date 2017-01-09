
//TODO check

/**
 * @file cEvaluateMessageGloveStatesEach2Neighbors
 * file name: cEvaluateMessageGloveStatesEach2Neighbors.cpp
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



#include "cEvaluateMessageGloveStatesEach2Neighbors.h"

#include "nMessageGloveStates_cMessageGloveDigit.h"
#include "nMessageGloveStates_cDigitState.h"

#include <string>

#include "cCallChangeModus.h"
#include "cCallPrepareChangeModus.h"

#ifdef DEBUG_EVALUATE_MESSAGE_GLOVE_STATES_EACH_2_NEIGHBORS
	#include <iostream>
#endif  //DEBUG_EVALUATE_MESSAGE_GLOVE_STATES_EACH_2_NEIGHBORS


using namespace nDataGlove::nMapper::nMessageGloveStates;
using namespace nDataGlove::nMapper;
using namespace nDataGlove::nModelDataGloveDGTechVHand;
using namespace std;


cEvaluateMessageGloveStatesEach2Neighbors::cEvaluateMessageGloveStatesEach2Neighbors() :
		pEvaluateDataGloveState( NULL ) {
	//nothing to do
}


cEvaluateMessageGloveStatesEach2Neighbors::~cEvaluateMessageGloveStatesEach2Neighbors() {
	//nothing to do
}


/**
 * @return the name of this class "cEvaluateMessageGloveStatesEach2Neighbors"
 */
std::string cEvaluateMessageGloveStatesEach2Neighbors::getName() const {
	
	return "cEvaluateMessageGloveStatesEach2Neighbors";
}


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
cMessageGloveStates * cEvaluateMessageGloveStatesEach2Neighbors::evaluate(
		cMessageSamplingDataFromDataGlove * inPMessageSamplingDataFromDataGlove,
		const unsigned long inNumberOfMessage ) {
	
	if ( ( inPMessageSamplingDataFromDataGlove == NULL ) ||
			( pEvaluateDataGloveState == NULL ) ) {
		return NULL;
	}
	
	//copy message
	cMessageSamplingDataFromDataGlove * pMessageSamplingDataFromDataGlove =
		inPMessageSamplingDataFromDataGlove->clone();
	//create message to send
	cMessageGloveStates * pMessageGloveStates = new cMessageGloveStates();
	
	pMessageGloveStates->setClock(
		pMessageSamplingDataFromDataGlove->getClock() );
	pMessageGloveStates->setNumberReceivedMessage( inNumberOfMessage );
	
	//evaluate possible digits
	const unsigned int samplinTypeActual =
			pMessageSamplingDataFromDataGlove->getSamplingType();
	
	if ( samplinTypeActual != SamplinType ) {
		//new type of sampling message
		SamplinType = samplinTypeActual;
		LiDigits = pMessageSamplingDataFromDataGlove->
			getHandSamplingValuesTypesForMessageType( SamplinType ) ;
	}
	//get the state for the message (with the intervalls to it)
	cDataGloveState * pOriginalDataGloveState =
		pEvaluateDataGloveState->evalueDataGloveState(
				pMessageSamplingDataFromDataGlove );
	cDataGloveState * pLowerDataGloveState;
	cDataGloveState * pHigherDataGloveState;
	cDataGloveState * pActualDataGloveState;
	cInterval * pActualIntervall;
	cInterval * pTmpInterval;
	long lActualValue, lDeltaValue, lDeltaValueAdapted,
		lLowerBorderStart, lHigherBorderStart, lOldValue, lCorrectionValue;
	iCallFunction * pCallFunction;
	
	int digitTyp;
	std::string digitName;
	std::pair< int, std::string > stateTypeAndName;
	
	//for each digit:
	for ( list< nModelDataGloveDGTechVHand::cMessageSamplingDataFromDataGlove::
			tTypeSamplingValue >::const_iterator itrDigtType = LiDigits.begin();
			itrDigtType != LiDigits.end(); ++itrDigtType ) {
		//remember if there are any intervalls for the digit, if not: don't
		//search for neighbour state
		if ( ! pEvaluateDataGloveState->isSamplingValueRelevant( *itrDigtType ) ) {
			continue;
		}  //else
		//remember old value
		lOldValue = pMessageSamplingDataFromDataGlove->getValue(
			*itrDigtType );
		
		//evaluate neighbour states (use data glove sampling message with new value)
		if ( pOriginalDataGloveState != NULL ) {
			pActualIntervall = pOriginalDataGloveState->getIntervalForType( *itrDigtType );
		} else {
			pActualIntervall = NULL;
		}
		if ( pActualIntervall != NULL ) {
			//if intervall is given: search for neighbour state at:
			//		(intervall border) + i * intervall / 16
			lLowerBorderStart  = pActualIntervall->getMinimum();
			lHigherBorderStart = pActualIntervall->getMaximum();
			lDeltaValue = ( ( lHigherBorderStart - lHigherBorderStart ) >> 4 ) + 1;  //min 1
		} else {  //if not intervall is given: search for neighbour state at:
			  //(actual value) + i * ( ((actual value) / 256) + 1)
			lDeltaValue = ( lOldValue >> 8 ) + 1;  //min 1
			lLowerBorderStart  = lOldValue - 1;
			lHigherBorderStart = lOldValue + 1;
		}
		//search for lower state
		lActualValue = lLowerBorderStart;
		lDeltaValueAdapted = lDeltaValue;
		pLowerDataGloveState = NULL;
		for ( unsigned int iteration = 0; iteration < 256; ++iteration ) {
			
			lActualValue -= lDeltaValueAdapted;
			if ( lActualValue < -1000000000 ) {
				break;  //Value gets to small
			}
			
			pMessageSamplingDataFromDataGlove->setValue(
				*itrDigtType, lActualValue );
			//is there a lower state for the value
			pActualDataGloveState = pEvaluateDataGloveState->evalueDataGloveState(
				pMessageSamplingDataFromDataGlove );
			
			if ( ( pActualDataGloveState != NULL ) &&
					( pActualDataGloveState != pOriginalDataGloveState ) ) {
				//lower state found -> done
				pLowerDataGloveState = pActualDataGloveState;
				break;
			}
			lDeltaValueAdapted += ( lDeltaValueAdapted >> 4 ) + 1;
		}
		//search for higher state
		lActualValue = lHigherBorderStart;
		lDeltaValueAdapted = lDeltaValue;
		pHigherDataGloveState = NULL;
		for ( unsigned int iteration = 0; iteration < 256; ++iteration ) {
			
			lActualValue += lDeltaValueAdapted;
			if ( 1000000000 < lActualValue ) {
				break;  //Value gets to big
			}
			
			pMessageSamplingDataFromDataGlove->setValue(
				*itrDigtType, lActualValue );
			//is there a lower state for the value
			pActualDataGloveState = pEvaluateDataGloveState->evalueDataGloveState(
				pMessageSamplingDataFromDataGlove );
			
			if ( ( pActualDataGloveState != NULL ) &&
					( pActualDataGloveState != pOriginalDataGloveState ) ) {
				//higher state found -> done
				pHigherDataGloveState = pActualDataGloveState;
				break;
			}
			lDeltaValueAdapted += ( lDeltaValueAdapted >> 4 ) + 1;
		}
		//restore old value
		pMessageSamplingDataFromDataGlove->setValue(
			*itrDigtType, lOldValue );
		//add digit state data to message
		switch ( *itrDigtType ) {
			case cMessageSamplingDataFromDataGlove::tTypeSamplingValue::FINGER_1 : {
				digitTyp  = 5;
				digitName = "thumb";
			}; break;
			case cMessageSamplingDataFromDataGlove::tTypeSamplingValue::FINGER_2 : {
				digitTyp  = 4;
				digitName = "index finger";
			}; break;
			case cMessageSamplingDataFromDataGlove::tTypeSamplingValue::FINGER_3 : {
				digitTyp  = 3;
				digitName = "middle finger";
			}; break;
			case cMessageSamplingDataFromDataGlove::tTypeSamplingValue::FINGER_4 : {
				digitTyp  = 2;
				digitName = "ring finger";
			}; break;
			case cMessageSamplingDataFromDataGlove::tTypeSamplingValue::FINGER_5 : {
				digitTyp  = 1;
				digitName = "pinky";
			}; break;
			case cMessageSamplingDataFromDataGlove::tTypeSamplingValue::QUATERNION_1 : {
				digitTyp  = 11;
				digitName = "quaterion 1";
			}; break;
			case cMessageSamplingDataFromDataGlove::tTypeSamplingValue::QUATERNION_2 : {
				digitTyp  = 12;
				digitName = "quaterion 2";
			}; break;
			case cMessageSamplingDataFromDataGlove::tTypeSamplingValue::QUATERNION_3 : {
				digitTyp  = 13;
				digitName = "quaterion 3";
			}; break;
			case cMessageSamplingDataFromDataGlove::tTypeSamplingValue::QUATERNION_4 : {
				digitTyp  = 14;
				digitName = "quaterion 4";
			}; break;
			case cMessageSamplingDataFromDataGlove::tTypeSamplingValue::GYROSCOPE_X : {
				digitTyp  = 21;
				digitName = "gyroscop 1";
			}; break;
			case cMessageSamplingDataFromDataGlove::tTypeSamplingValue::GYROSCOPE_Y : {
				digitTyp  = 22;
				digitName = "gyroscop 2";
			}; break;
			case cMessageSamplingDataFromDataGlove::tTypeSamplingValue::GYROSCOPE_Z : {
				digitTyp  = 23;
				digitName = "gyroscop 3";
			}; break;
			case cMessageSamplingDataFromDataGlove::tTypeSamplingValue::MAGNETOMETER_X : {
				digitTyp  = 41;
				digitName = "magnetometer X";
			}; break;
			case cMessageSamplingDataFromDataGlove::tTypeSamplingValue::MAGNETOMETER_Y : {
				digitTyp  = 42;
				digitName = "magnetometer Y";
			}; break;
			case cMessageSamplingDataFromDataGlove::tTypeSamplingValue::MAGNETOMETER_Z : {
				digitTyp  = 43;
				digitName = "magnetometer Z";
			}; break;
			case cMessageSamplingDataFromDataGlove::tTypeSamplingValue::ACCELEROMETER_X : {
				digitTyp  = 31;
				digitName = "accelerometer X";
			}; break;
			case cMessageSamplingDataFromDataGlove::tTypeSamplingValue::ACCELEROMETER_Y : {
				digitTyp  = 32;
				digitName = "accelerometer Y";
			}; break;
			case cMessageSamplingDataFromDataGlove::tTypeSamplingValue::ACCELEROMETER_Z : {
				digitTyp  = 33;
				digitName = "accelerometer Z";
			}; break;
			case cMessageSamplingDataFromDataGlove::tTypeSamplingValue::CLOCK : {
				digitTyp  = 100;
				digitName = "clock";
			}; break;
			
			default : {
				digitTyp  = 0;
				digitName = "unknown";
			}; break;
		
		};  //end switch ( itrDigtType )
		
		cMessageGloveDigit messageGloveDigit( digitTyp, digitName, lOldValue );
		//evaluate the correction value to write
		lCorrectionValue = pEvaluateDataGloveState->getCorrectionValue( *itrDigtType );
		if ( lCorrectionValue != 0 ) {
			messageGloveDigit.setCorrection( lCorrectionValue );
		}
		
		//write lower state
		if ( pLowerDataGloveState != NULL ) {
			pTmpInterval = pLowerDataGloveState->getIntervalForType( *itrDigtType );
			pCallFunction = pLowerDataGloveState->getCallFunction();
			stateTypeAndName = getStateTypeAndName( pCallFunction );
			
			if ( pTmpInterval != NULL ) {
				cDigitState digitState( stateTypeAndName.first,
					stateTypeAndName.second,
					getStateParameter( pCallFunction ),
					pTmpInterval->getTarget(),
					pTmpInterval->getMinimum(), pTmpInterval->getMaximum() );
				
				messageGloveDigit.addNeighbourState( digitState );
			} else {
				cDigitState digitState( stateTypeAndName.first,
					stateTypeAndName.second,
					getStateParameter( pCallFunction ),
					0, 0, 0 );
				
				messageGloveDigit.addNeighbourState( digitState );
			}
		}
		//write actual state
		if ( pOriginalDataGloveState != NULL ) {
			pCallFunction = pOriginalDataGloveState->getCallFunction();
			stateTypeAndName = getStateTypeAndName( pCallFunction );
			
			if ( pActualIntervall != NULL ) {
				cDigitState digitState( stateTypeAndName.first,
					stateTypeAndName.second,
					getStateParameter( pCallFunction ),
					pActualIntervall->getTarget(),
					pActualIntervall->getMinimum(), pActualIntervall->getMaximum() );
				
				messageGloveDigit.addNeighbourState( digitState );
			} else {
				cDigitState digitState( stateTypeAndName.first,
					stateTypeAndName.second,
					getStateParameter( pCallFunction ),
					0, 0, 0 );
				
				messageGloveDigit.addNeighbourState( digitState );
			}
		}
		//write higher state
		if ( pHigherDataGloveState != NULL ) {
			pTmpInterval = pHigherDataGloveState->getIntervalForType( *itrDigtType );
			pCallFunction = pHigherDataGloveState->getCallFunction();
			stateTypeAndName = getStateTypeAndName( pCallFunction );
			
			if ( pTmpInterval != NULL ) {
				cDigitState digitState( stateTypeAndName.first,
					stateTypeAndName.second,
					getStateParameter( pCallFunction ),
					pTmpInterval->getTarget(),
					pTmpInterval->getMinimum(), pTmpInterval->getMaximum() );
				
				messageGloveDigit.addNeighbourState( digitState );
			} else {
				cDigitState digitState( stateTypeAndName.first,
					stateTypeAndName.second,
					getStateParameter( pCallFunction ),
					0, 0, 0 );
				
				messageGloveDigit.addNeighbourState( digitState );
			}
		}
		messageGloveDigit.setIndexActiveNeighbour(
			( pOriginalDataGloveState == NULL ) ? -1 :
				( ( pLowerDataGloveState == NULL ) ? 0 : 1 ) );
		//add the digit
		pMessageGloveStates->addDigit( messageGloveDigit );
	}
#ifdef DEBUG_EVALUATE_MESSAGE_GLOVE_STATES_EACH_2_NEIGHBORS
	pMessageGloveStates->print( cout );
#endif  //DEBUG_EVALUATE_MESSAGE_GLOVE_STATES_EACH_2_NEIGHBORS
	delete pMessageSamplingDataFromDataGlove;
	return pMessageGloveStates;
}


/**
 * Returns the state type number and name for the call function.
 * (The state is for the call function.)
 *
 * @param pCallFunction a pointer to the to call function
 * @return The state/call function type number and name for the call function as a pair.
 */
std::pair< int, std::string > cEvaluateMessageGloveStatesEach2Neighbors::
		getStateTypeAndName( const iCallFunction * pCallFunction ) {
	
	if ( pCallFunction == NULL ) {
		return std::pair< int, std::string >( 0, "non" );
	}
	//TODO implement + use iCallFunction->getClassId() instead of getName()
	const string strCallFunctionName = pCallFunction->getName();
	if ( strCallFunctionName == "cCallChangeModus" ) {
		//modus
		return std::pair< int, std::string >( 1, std::string("change modus ").append(
			std::to_string( ((cCallChangeModus*)(pCallFunction))->getModus() ) ) );
	}  //else
	if ( strCallFunctionName == "cCallKeybordFunction" ) {
		//keyboard
		return std::pair< int, std::string >( 2, "keyboard");
	}  //else
	if ( strCallFunctionName == "cCallMouseFunction" ) {
		//mouse
		return std::pair< int, std::string >( 3, "mouse");
	}  //else
	if ( strCallFunctionName == "cCallPrepareChangeModus" ) {
		//prepare modus
		return std::pair< int, std::string >( 101, std::string("prepare change modus ").append(
			std::to_string( ((cCallPrepareChangeModus*)(pCallFunction))->getModus() ) ) );
	}  //else
	if ( strCallFunctionName == "cCallPrepareKeybordFunction" ) {
		//prepare keyboard
		return std::pair< int, std::string >( 102, "prepare keyboard");
	}  //else
	if ( strCallFunctionName == "cCallPrepareMouseFunction" ) {
		//modus
		return std::pair< int, std::string >( 103, "prepare mouse");
	}  //else
	if ( strCallFunctionName == "cCallSetBoolFlag" ) {
		//modus
		return std::pair< int, std::string >( 1001, "set a bool flag");
	}  //else
	if ( strCallFunctionName == "cCallSimulation" ) {
		//modus
		return std::pair< int, std::string >( 1002, "call simulation");
	}  //else
	
	
	
	return std::pair< int, std::string >( 0, strCallFunctionName );
}



/**
 * Returns the state parameter for the call function.
 * (The state is for the call function.)
 *
 * @param pCallFunction a pointer to the to call function
 * @return The state/call function parameter for the call function as a pair.
 */
std::string cEvaluateMessageGloveStatesEach2Neighbors::getStateParameter(
		const iCallFunction * pCallFunction ) {
	
	if ( pCallFunction == NULL ) {
		return "";
	}
	return pCallFunction->getParameter();
}


/**
 * @return the object to evaluate the data glove state
 * 	@see pEvaluateDataGloveState
 */
cEvaluateDataGloveState * cEvaluateMessageGloveStatesEach2Neighbors::
		getEvaluateDataGloveState() {
	
	return pEvaluateDataGloveState;
}


/**
 * Sets the object to evaluate the data glove state.
 *
 * @see pEvaluateDataGloveState
 * @param pInEvaluateDataGloveState a pointer to the object to evaluate the
 * 	data glove state
 */
void cEvaluateMessageGloveStatesEach2Neighbors::setEvaluateDataGloveState(
		cEvaluateDataGloveState * pInEvaluateDataGloveState ) {
	
	pEvaluateDataGloveState = pInEvaluateDataGloveState;
}






