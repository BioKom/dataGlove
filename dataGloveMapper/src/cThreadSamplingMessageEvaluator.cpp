
//TODO check

/**
 * @file cThreadSamplingMessageEvaluator
 * file name: cThreadSamplingMessageEvaluator.cpp
 * @author Betti Oesterholz
 * @date 17.08.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++, Qt4
 *
 * This file implements a class for evaluating data glove sampling messages.
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
 * This file implements a class for evaluating data glove sampling messages.
 * It is a thread, so it won't block the main application and did not
 * delay the reading of the messages.
 * Handeled are the sampling messages from the data glove.
 * Helper class of cThreadMessageHandler .
 *
 * @see cThreadMessageHandler
 * @see cThread
 * @see cMessageDataGlove
 * @see cDataGloveDGTechVHand
 * @see iWidgetSamplingValue
 * @see cEvaluateDataGloveState
 * @see cMessageSamplingDataFromDataGlove
 */
/*
History:
17.08.2014  Oesterholz  created
*/


//#define DEBUG


#include "cThreadSamplingMessageEvaluator.h"


#include <map>


#include "cEvaluateDataGloveState.h"
#include "cDataGloveState.h"
#include "iCallFunction.h"



using namespace nDataGlove::nMapper;
using namespace nDataGlove::nModelDataGloveDGTechVHand;
using namespace nDataGlove;


/**
 * The standard constructor for data glove message handler.
 *
 * @see cThread
 */
cThreadSamplingMessageEvaluator::cThreadSamplingMessageEvaluator() :
		cThread(),
		pNewMessageSamplingDataFromDataGlove( NULL ),
		bNewMessageSamplingDataFromDataGloveChangedTillLastRunEval( false ),
		ulMessageInQueue( 0 ),
		ulMessageCounter( 0 ),
		uiNumberOfMessagesInActualSecond( 0 ),
		tActualSecond( time( NULL) ),
		dMessagesPerSecond( 0 ),
		pEvaluateDataGloveState( NULL ) {
}


/**
 * destructor
 */
cThreadSamplingMessageEvaluator::~cThreadSamplingMessageEvaluator() {
	
	//delete old sampling messages
	if ( bIsRunning ) {
		//stop this thread
		quit();
	}
#ifdef CPP_2011
	mutexMembers.lock();
#endif  //CPP_2011
	nModelDataGloveDGTechVHand::cMessageSamplingDataFromDataGlove *
		pToDeleteMessageSamplingDataFromDataGlove =
			pNewMessageSamplingDataFromDataGlove;
	pNewMessageSamplingDataFromDataGlove = NULL;
#ifdef CPP_2011
	mutexMembers.unlock();
#endif  //CPP_2011
	if ( pToDeleteMessageSamplingDataFromDataGlove != NULL ) {
		delete pToDeleteMessageSamplingDataFromDataGlove;
	}
	
	if ( bIsRunning ) {
		//stop this thread and wait till it is no more running
		while ( bIsRunning ) {
			msleep ( 10 );
		}
	}
}


/**
 * @return the name of this class "cThreadSamplingMessageEvaluator"
 */
std::string cThreadSamplingMessageEvaluator::getName() const {
	
	return "cThreadSamplingMessageEvaluator";
}


/**
 * @return true the thread is running, else false
 * @see run()
 */
bool cThreadSamplingMessageEvaluator::isRunning() const {
	
	return bIsRunning;
}


/**
 * Sets the new sampling data message.
 * Also cares that the given message is deleted.
 * (Do not delete pInNewMessageSamplingDataFromDataGlove.)
 *
 * @see pNewMessageSamplingDataFromDataGlove
 * @param pInNewMessageSamplingDataFromDataGlove a pointer the new
 * 	sampling data message
 */
void cThreadSamplingMessageEvaluator::setNewSamplingMessage(
	nModelDataGloveDGTechVHand::cMessageSamplingDataFromDataGlove *
		pInNewMessageSamplingDataFromDataGlove ) {
	
	DEBUG_OUT_L3(<<"cThreadSamplingMessageEvaluator("<<this<<")::setNewSamplingMessage(new="<<pInNewMessageSamplingDataFromDataGlove<<")"<<endl<<flush);
	
	if ( pInNewMessageSamplingDataFromDataGlove == NULL ) {
		return;  //no new message
	}
#ifdef CPP_2011
	mutexMembers.lock();
#endif  //CPP_2011
	nModelDataGloveDGTechVHand::cMessageSamplingDataFromDataGlove *
		pToDeleteMessageSamplingDataFromDataGlove =
			pNewMessageSamplingDataFromDataGlove;
	pNewMessageSamplingDataFromDataGlove = pInNewMessageSamplingDataFromDataGlove;
	bNewMessageSamplingDataFromDataGloveChangedTillLastRunEval = true;
	ulMessageInQueue++;
	DEBUG_OUT_L4(<<"cThreadSamplingMessageEvaluator("<<this<<")::setNewSamplingMessage() bNewMessageSamplingDataFromDataGloveChangedTillLastRunEval set"<<endl<<flush);
#ifdef CPP_2011
	mutexMembers.unlock();
#endif  //CPP_2011
	if ( pToDeleteMessageSamplingDataFromDataGlove != NULL ) {
		delete pToDeleteMessageSamplingDataFromDataGlove;
	}
	ulMessageCounter++;
	if ( tActualSecond == time( NULL ) ) {
		//still actual second
		uiNumberOfMessagesInActualSecond++;
	}else{
		const time_t tNewTime = time( NULL );
		dMessagesPerSecond = dMessagesPerSecond * 9 / 10 +
			( uiNumberOfMessagesInActualSecond / ( tNewTime - tActualSecond ) )
				/ 10;
				
		uiNumberOfMessagesInActualSecond = 0;
		tActualSecond = tNewTime;
	}
}



/**
 * @return the object to evaluate the data glove state
 * 	@see pEvaluateDataGloveState
 */
cEvaluateDataGloveState * cThreadSamplingMessageEvaluator::getEvaluateDataGloveState() {
	
	return pEvaluateDataGloveState;
}


/**
 * Sets the object to evaluate the data glove state.
 *
 * @see pEvaluateDataGloveState
 * @param pInEvaluateDataGloveState a pointer to the object to evaluate the
 * 	data glove state
 */
void cThreadSamplingMessageEvaluator::setEvaluateDataGloveState(
		cEvaluateDataGloveState * pInEvaluateDataGloveState ) {
	
#ifdef CPP_2011
	mutexMembers.lock();
#endif  //CPP_2011
	pEvaluateDataGloveState = pInEvaluateDataGloveState;
#ifdef CPP_2011
	mutexMembers.unlock();
#endif  //CPP_2011
}


/**
 * Runs the data glove message handler.
 * It will read all messages from the data glove  and process them accordingly.
 *
 * @see bStop
 */
bool cThreadSamplingMessageEvaluator::run() {
	
	DEBUG_OUT_L2(<<"cThreadSamplingMessageEvaluator("<<this<<")::run() called"<<endl<<flush);
	
	bIsRunning = true;
	bStop = false;
	cMessageSamplingDataFromDataGlove * pMessageToEvaluate = NULL;
	cDataGloveState * pDataGloveState = NULL;
	int iModusForDataGloveState = 0;
	
	iCallFunction * pCallFunction;
	
	long lMilliSecondsToWait = -20;
	bool bNothingHappend = false;
	bool bSomethingMissed = false;
	
	while ( ! bStop ) {
		DEBUG_OUT_L4(<<"cThreadSamplingMessageEvaluator("<<this<<")::run() if something happend"<<endl<<flush);
		
		if ( bSomethingMissed ) {
			//something was missed -> reevaluate the delay time
			lMilliSecondsToWait = -20;
			bSomethingMissed = false;
			
		} else if ( bNothingHappend ) {
			//nothing happend
			DEBUG_OUT_L4(<<"cThreadSamplingMessageEvaluator("<<this<<")::run() nothing happend"<<endl<<flush);
			if ( 0 < lMilliSecondsToWait ) {
				DEBUG_OUT_L4(<<"cThreadSamplingMessageEvaluator("<<this<<")::run() weit millisecounds"<<endl<<flush);
				msleep( lMilliSecondsToWait );
			}
			//nothing happend -> wait longer
			lMilliSecondsToWait++;
		} else {  //wait less
			lMilliSecondsToWait--;
		}
		
		bNothingHappend = true;
		
		if ( bNewMessageSamplingDataFromDataGloveChangedTillLastRunEval ) {
			DEBUG_OUT_L4(<<"cThreadSamplingMessageEvaluator("<<this<<")::run() evaluate new message"<<endl<<flush);
			//read message
#ifdef CPP_2011
			mutexMembers.lock();
#endif  //CPP_2011
			pMessageToEvaluate = pNewMessageSamplingDataFromDataGlove;
			pNewMessageSamplingDataFromDataGlove = NULL;
			bNewMessageSamplingDataFromDataGloveChangedTillLastRunEval = false;
			
			if ( 1 < ulMessageInQueue ) {
				//one message missed
				bSomethingMissed = true;
			}
			ulMessageInQueue = 0;
			DEBUG_OUT_L4(<<"cThreadSamplingMessageEvaluator("<<this<<")::run() bNewMessageSamplingDataFromDataGloveChangedTillLastRunEval unset"<<endl<<flush);
#ifdef CPP_2011
			mutexMembers.unlock();
#endif  //CPP_2011
			if ( pMessageToEvaluate == NULL ) {
				DEBUG_OUT_L4(<<"cThreadSamplingMessageEvaluator("<<this<<")::run() no new message"<<endl<<flush);
				//go on for repeater of state
			} else { //else pReadedMessage != NULL
				//-> new message happend
				DEBUG_OUT_L4(<<"cThreadSamplingMessageEvaluator("<<this<<")::run() new message"<<endl<<flush);
				bNothingHappend = false;
			}
		} else {
			//pMessageToEvaluate = NULL;
			DEBUG_OUT_L4(<<"cThreadSamplingMessageEvaluator("<<this<<")::run() bNewMessageSamplingDataFromDataGloveChangedTillLastRunEval not set"<<endl<<flush);
			//go on for repeater of state
		}
		
		//handle message
		DEBUG_OUT_L3(<<"cThreadSamplingMessageEvaluator("<<this<<")::run() processing next message: "<<pMessageToEvaluate<<endl<<flush);
		
		if ( pDataGloveState != NULL ) {
			
			if ( ( pEvaluateDataGloveState != NULL ) &&
					( iModusForDataGloveState != pEvaluateDataGloveState->getActualModus() ) ) {
				//modus changed -> old data glove state invalide
				//pDataGloveState = NULL;
			} else if ( ( pMessageToEvaluate == NULL ) ||
					( pDataGloveState->isIn( pMessageToEvaluate ) ) ) {
				//state not changed
				DEBUG_OUT_L3(<<"cThreadSamplingMessageEvaluator("<<this<<")::run() state not changed"<<endl<<flush);
				if ( pCallFunction != NULL ) {
					//if it is time to call the function again, do it
					const unsigned int uiNumberOfRepeats =
						pCallFunction->checkAndRepeatCall();
					DEBUG_OUT_L4(<<"cThreadSamplingMessageEvaluator("<<this<<")::run() function was called again "<<uiNumberOfRepeats<<" times"<<endl<<flush);
					if ( 0 < uiNumberOfRepeats ) {
						//function was repeated
						bNothingHappend = false;
						
						if ( 1 < uiNumberOfRepeats ) {
							//function was repeated more than once
							//-> repeat call was missed
							bSomethingMissed = true;
						}
					}
				}
				if ( pMessageToEvaluate != NULL ) {
					delete pMessageToEvaluate;
					pMessageToEvaluate = NULL;
				}
				continue;  //no message nothing to evaluate
			}//else state changed -> evaluate new state
			pDataGloveState = NULL;
			if ( pCallFunction != NULL ) {
				pCallFunction->end();
				pCallFunction = NULL;
			}
		}
		if  ( pMessageToEvaluate != NULL ) {
			DEBUG_OUT_L3(<<"cThreadSamplingMessageEvaluator("<<this<<")::run() evaluate new state"<<endl<<flush);
			if ( pEvaluateDataGloveState != NULL ) {
				
				pDataGloveState = pEvaluateDataGloveState->
					evalueDataGloveState( pMessageToEvaluate );
				
				if ( pDataGloveState != NULL ) {
					pCallFunction = pDataGloveState->getCallFunction();
					
					if ( pCallFunction != NULL ) {
						//call the function
						(*pCallFunction)();
					//TODO remove cDataGloveState->getRepeatAllMilliSeconds()
					/*TODO weg:
						if ( pDataGloveState->getRepeatAllMilliSeconds() != 0 ) {
							//TODO if repeat delay start repeat delay function
							//handle in this method
							
						}
					*/
					}
					iModusForDataGloveState = pEvaluateDataGloveState->getActualModus();
				}
			}
			delete pMessageToEvaluate;
			pMessageToEvaluate = NULL;
		}
	}  //while more messages to read
	
	bStop = false;
	bIsRunning = false;
	DEBUG_OUT_L2(<<"cThreadSamplingMessageEvaluator("<<this<<")::run() done"<<endl<<flush);
	return true;
}




















