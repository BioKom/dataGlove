
//TODO check

/**
 * @file cThreadMessageHandler
 * file name: cThreadMessageHandler.cpp
 * @author Betti Oesterholz
 * @date 28.07.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++, Qt4
 *
 * This file implements a class for handling data glove messages.
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
 * This file implements a class for handling data glove messages.
 * It is a thread, so it won't block the main application.
 * Handeled are the messages from the data glove
 *
 * @see cThread
 * @see cMessageDataGlove
 * @see cDataGloveDGTechVHand
 * @see cEvaluateDataGloveState
 * @see cMessageSamplingDataFromDataGlove
 */
/*
History:
28.07.2014  Oesterholz  created
*/


//#define DEBUG



#include "cThreadMessageHandler.h"


#include <map>

#include "cDataGloveDGTechVHand.h"

#include "cMessageGetIdFromDataGlove.h"


#include "cEvaluateDataGloveState.h"
#include "cThreadSamplingMessageEvaluator.h"



using namespace nDataGlove::nMapper;
using namespace nDataGlove::nModelDataGloveDGTechVHand;
using namespace nDataGlove;


/**
 * The standard constructor for data glove message handler.
 *
 * @see cThread
 */
cThreadMessageHandler::cThreadMessageHandler() :
		cThread(),
		pDataGloveDevice( NULL ),
		pLastMessageSamplingDataFromDataGlove( NULL ),
		pEvaluateDataGloveState( NULL ) {
	
	pThreadSamplingMessageEvaluator = new cThreadSamplingMessageEvaluator();
}

/**
 * destructor
 */
cThreadMessageHandler::~cThreadMessageHandler() {
	
	if ( bIsRunning ) {
		//stop this thread and wait til it is no more running
		quit();
		while ( bIsRunning ) {
			msleep( 10 );
		}
	}
	delete pThreadSamplingMessageEvaluator;
}


/**
 * @return the name of this class "cThreadMessageHandler"
 */
std::string cThreadMessageHandler::getName() const {
	
	return "cThreadMessageHandler";
}


/**
 * @return true the thread is running, else false
 * @see run()
 */
bool cThreadMessageHandler::isRunning() const {
	
	return bIsRunning || pThreadSamplingMessageEvaluator->isRunning();
}


/**
 * @return a const pointer to the data glove device, which messages are
 * 	handeled by this object
 * 	@see pDataGloveDevice
 */
cDataGloveDGTechVHand * cThreadMessageHandler::getDataGloveDevice() {
	
	return pDataGloveDevice;
}


/**
 * @return a pointer to the data glove device, which messages are
 * 	handeled by this object
 * 	@see pDataGloveDevice
 */
const cDataGloveDGTechVHand * cThreadMessageHandler::getDataGloveDevice() const {
	
	return pDataGloveDevice;
}


/**
 * Sets the data glove device.
 * 
 * @see pDataGloveDevice
 * @param pInDataGloveDevice a pointer to the data glove device, which
 * 	messages should handeled by this object
 */
void cThreadMessageHandler::setDataGloveDevice(
		cDataGloveDGTechVHand * pInDataGloveDevice ) {
	
#ifdef CPP_2011
	mutexMembers.lock();
#endif  //CPP_2011
	pDataGloveDevice = pInDataGloveDevice;
#ifdef CPP_2011
	mutexMembers.unlock();
#endif  //CPP_2011
}


/**
 * @return the object to evaluate the data glove state
 * 	@see pEvaluateDataGloveState
 */
cEvaluateDataGloveState * cThreadMessageHandler::getEvaluateDataGloveState() {
	
	return pEvaluateDataGloveState;
}

/**
 * Sets the object to evaluate the data glove state.
 *
 * @see pEvaluateDataGloveState
 * @param pInEvaluateDataGloveState a pointer to the object to evaluate the
 * 	data glove state
 */
void cThreadMessageHandler::setEvaluateDataGloveState(
		cEvaluateDataGloveState * pInEvaluateDataGloveState ) {
	
#ifdef CPP_2011
	mutexMembers.lock();
#endif  //CPP_2011
	pEvaluateDataGloveState = pInEvaluateDataGloveState;
#ifdef CPP_2011
	mutexMembers.unlock();
#endif  //CPP_2011
	pThreadSamplingMessageEvaluator->setEvaluateDataGloveState(
		pInEvaluateDataGloveState );
}



/**
 * Signals this thread to quit at a good moment.
 * @see bStop
 */
bool cThreadMessageHandler::quit() {
	
	pThreadSamplingMessageEvaluator->quit();
	
	return cThread::quit();
}


/**
 * Runs the data glove message handler.
 * It will read all messages from the data glove  and process them accordingly.
 *
 * @see bStop
 */
bool cThreadMessageHandler::run() {
	
	DEBUG_OUT_L2(<<"cThreadMessageHandler("<<this<<")::run() started"<<endl<<flush);
	
	bIsRunning = true;
	bStop = false;
	cMessageFromDataGlove * pReadedMessage = NULL;
	unsigned long ulMilliSecondsToWait = 0;
	nModelDataGloveDGTechVHand::cMessageSamplingDataFromDataGlove *
		pMessageSamplingDataFromDataGlove;
	
	pThreadSamplingMessageEvaluator->start();
	
	
	while ( ! bStop ) {
		//read message
		pReadedMessage = NULL;
#ifdef CPP_2011
		mutexMembers.lock();
#endif  //CPP_2011
		if ( pDataGloveDevice != NULL ) {
			//TODO if true milli seconds are implemented
			//pReadedMessage = pDataGloveDevice->readMessage( 10, false, true );
			pReadedMessage = pDataGloveDevice->readMessage( 2000, false, true );
			
			DEBUG_OUT_L3(<<"cThreadMessageHandler("<<this<<")::run() readed message: "<<pReadedMessage<<endl<<flush);
		}
#ifdef CPP_2011
		mutexMembers.unlock();
#endif  //CPP_2011
		
		if ( pReadedMessage == NULL ) {
			//wait a short while till try to read new messages
			if ( 0 < ulMilliSecondsToWait ) {
				msleep( ulMilliSecondsToWait );
			}
			ulMilliSecondsToWait += 1;
			if ( 20 < ulMilliSecondsToWait ) {
				//wait maximal 1/10 second
				ulMilliSecondsToWait = 20;
			}
			continue;
		}  //else pReadedMessage != NULL
		ulMilliSecondsToWait = 0;
		//handle/send message
		
		
		
		if ( pReadedMessage->getType() == cMessageDataGlove::SAMPLING_DATA ) {
			if ( BMoreSamplingMessageListenersExists ) {
				pMessageSamplingDataFromDataGlove =
					static_cast<nModelDataGloveDGTechVHand::cMessageSamplingDataFromDataGlove *>(
						pReadedMessage )->clone();
			} else {
				pMessageSamplingDataFromDataGlove = NULL;
			}
			
			//sampling data read -> evaluate it
			pThreadSamplingMessageEvaluator->setNewSamplingMessage(
				static_cast<nModelDataGloveDGTechVHand::cMessageSamplingDataFromDataGlove *>(
					pReadedMessage ) );
			
			if ( pMessageSamplingDataFromDataGlove != NULL ) {
#ifdef CPP_2011
				mutexSamplingMessageListeners.lock();
#endif  //CPP_2011
				if ( not LiSamplingMessageListeners.empty() ) {
					DEBUG_OUT_L4(<<"cThreadMessageHandler("<<this<<")::run() sending message to more listeners"<<endl<<flush);
					//set pMessageSamplingDataFromDataGlove for the first listener
					//(copy (and delete) as less as possible)
					if ( 1 < LiSamplingMessageListeners.size() ) {
						std::list< iSetNewSamplingMessage * >::iterator itrSetNewSamplingMessage =
								LiSamplingMessageListeners.begin();
						++itrSetNewSamplingMessage;
						//set a clone of pMessageSamplingDataFromDataGlove for
						//every listener after the first listener
						for ( ; itrSetNewSamplingMessage != LiSamplingMessageListeners.end();
								++itrSetNewSamplingMessage ) {
							
							(*itrSetNewSamplingMessage)->setNewSamplingMessage(
								pMessageSamplingDataFromDataGlove->clone() );
						}
					}
					//set pMessageSamplingDataFromDataGlove for the first listener
					(*(LiSamplingMessageListeners.begin()))->setNewSamplingMessage(
						pMessageSamplingDataFromDataGlove );
				} else {  //Error: no listeners exists
					BMoreSamplingMessageListenersExists = false;
					delete pMessageSamplingDataFromDataGlove;
				}
#ifdef CPP_2011
				mutexSamplingMessageListeners.unlock();
#endif  //CPP_2011
			}
			
		} else {  //not sampling data
			//TODO
			//cThreadMessageEvaluator
			//TODO weg
			delete pReadedMessage;
		}
	}  //while more messages to read
	
	bStop = false;
	bIsRunning = false;
	DEBUG_OUT_L2(<<"cThreadMessageHandler("<<this<<")::run() done"<<endl<<flush);
	return true;
}


/**
 * Registers a listeners for new data glove sampling messages.
 *
 * @see LiSamplingMessageListeners
 * @see unregisterSamplingMessageListener()
 * @param inPSetNewSamplingMessage a pointer to a listener for new data
 * 	glove sampling messages
 * @return true if the listener was registered, else false
 */
bool cThreadMessageHandler::registerSamplingMessageListener(
		iSetNewSamplingMessage * inPSetNewSamplingMessage ) {
	
	if ( inPSetNewSamplingMessage == NULL ) {
		return false;
	}
#ifdef CPP_2011
	mutexSamplingMessageListeners.lock();
#endif  //CPP_2011
	DEBUG_OUT_L2(<<"cThreadMessageHandler("<<this<<")::registerSamplingMessageListener("<<inPSetNewSamplingMessage<<")"<<endl<<flush);
	LiSamplingMessageListeners.push_back( inPSetNewSamplingMessage );
	
	BMoreSamplingMessageListenersExists =
		not LiSamplingMessageListeners.empty();
#ifdef CPP_2011
	mutexSamplingMessageListeners.unlock();
#endif  //CPP_2011
	return true;
}

/**
 * Unregisters a listeners for new data glove sampling messages.
 *
 * @see LiSamplingMessageListeners
 * @see registerSamplingMessageListener()
 * @param inPSetNewSamplingMessage a pointer to a listener for new data
 * 	glove sampling messages
 * @return true if the listener was unregistered, else false
 */
bool cThreadMessageHandler::unregisterSamplingMessageListener(
		iSetNewSamplingMessage * inPSetNewSamplingMessage ) {
	
#ifdef CPP_2011
	mutexSamplingMessageListeners.lock();
#endif  //CPP_2011
	DEBUG_OUT_L2(<<"cThreadMessageHandler("<<this<<")::unregisterSamplingMessageListener("<<inPSetNewSamplingMessage<<")"<<endl<<flush);
	LiSamplingMessageListeners.remove( inPSetNewSamplingMessage );
	
	BMoreSamplingMessageListenersExists =
		not LiSamplingMessageListeners.empty();
#ifdef CPP_2011
	mutexSamplingMessageListeners.unlock();
#endif  //CPP_2011
	return true;
}


















