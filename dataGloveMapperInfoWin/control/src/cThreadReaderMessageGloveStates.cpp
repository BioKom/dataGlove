
//TODO check

/**
 * @file cThreadReaderMessageGloveStates
 * file name: cThreadReaderMessageGloveStates.cpp
 * @author Betti Oesterholz
 * @date 28.07.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++, Qt4
 *
 * This file implements a class for handling data glove state messages.
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
 * This file implements a class for handling data glove state messages.
 * It is a thread, so it won't block the main application.
 * Handeled are the state messages from the data glove
 *
 * @see QThread
 * @see cMessageGloveStates
 * @see cDataGloveDGTechVHand
 */
/*
History:
28.07.2014  Oesterholz  created
*/


//#define DEBUG



#include "cThreadReaderMessageGloveStates.h"


#include <map>
#include <iostream>
#include <fstream>


using namespace std;
using namespace nDataGlove::nInfo;
using namespace nDataGlove::nMapper;


/**
 * The standard constructor for data glove message handler.
 *
 * @see QThread
 */
cThreadReaderMessageGloveStates::cThreadReaderMessageGloveStates() :
		QThread(),
		FFileMessageGloveStates( "dataGloveSamplinOutput.dat" ),
		pLastMessageSamplingDataFromDataGlove( nullptr ) {
	//nothing to do
}

/**
 * destructor
 */
cThreadReaderMessageGloveStates::~cThreadReaderMessageGloveStates() {
	
	if ( isRunning() ) {
		//stop this thread and wait til it is no more running
		quit();
		while ( isRunning() ) {
			msleep( 10 );
		}
	}
}


/**
 * @return the name of this class "cThreadReaderMessageGloveStates"
 */
std::string cThreadReaderMessageGloveStates::getName() const {
	
	return "cThreadReaderMessageGloveStates";
}


/**
 * @return a const pointer to the data glove state file, which messages are
 * 	handeled by this object
 * 	@see FFileMessageGloveStates
 */
std::string cThreadReaderMessageGloveStates::getFileMessageGloveStates()  {
	
	MutexMembers.lock();
	const std::string tmpFileMessageGloveStates = FFileMessageGloveStates;
	MutexMembers.unlock();
	return tmpFileMessageGloveStates;
}


/**
 * @return a pointer to the data glove state file, which messages are
 * 	handeled by this object
 * 	@see FFileMessageGloveStates
 */
const std::string cThreadReaderMessageGloveStates::getFileMessageGloveStates() const {
	
	MutexMembers.lock();
	const std::string tmpFileMessageGloveStates = FFileMessageGloveStates;
	MutexMembers.unlock();
	return tmpFileMessageGloveStates;
}


/**
 * Sets the data glove state file.
 *
 * @see FFileMessageGloveStates
 * @param pInFileMessageGloveStates a pointer to the data glove state file, which
 * 	messages should handeled by this object
 */
void cThreadReaderMessageGloveStates::setFileMessageGloveStates(
		const std::string inFileMessageGloveStates ) {
	
	MutexMembers.lock();
	FFileMessageGloveStates = inFileMessageGloveStates;
	MutexMembers.unlock();
}

/**
 * Signals this thread to quit at a good moment.
 * @see bStop
 */
void cThreadReaderMessageGloveStates::quit() {
	
	bStop = true;
	
	QThread::quit();
}

/**
 * Runs the data glove message handler.
 * It will read all messages from the data glove  and process them accordingly.
 *
 * @see bStop
 */
void cThreadReaderMessageGloveStates::run() {
	
	DEBUG_OUT_L2(<<"cThreadReaderMessageGloveStates("<<this<<")::run() started"<<endl<<flush);
	
	bStop = false;
	nMapper::cMessageGloveStates * pMessageGloveStates = nullptr;
	unsigned long ulMilliSecondsToWait = 100;
	int iReaderOutStatus;
	unsigned long ulLastClockValue = -1;
	unsigned long ulNewClockValue  = 0;
	
	while ( ! bStop ) {
		//read message
		MutexMembers.lock();
		
		/*
			1. check if file exists
			1.1 if file dosn't exists -> wait
			1.2 if file exists
			1.2.1 read data from file
			1.2.2 check if data changed (clock)
			1.2.4 send data of the file
		*/
		iReaderOutStatus = 0;
		
		if ( ! FFileMessageGloveStates.empty() ) {
			ifstream isMessageFile;
			isMessageFile.open( FFileMessageGloveStates, ios::in | ios::binary );
			if ( isMessageFile.is_open() ) {
				//try to read the data glove state message
				pMessageGloveStates = nMapper::cMessageGloveStates::restore(
					isMessageFile, &iReaderOutStatus );
				isMessageFile.close();
			}
			
			DEBUG_OUT_L3(<<"cThreadReaderMessageGloveStates("<<this<<")::run() readed message: "<<pMessageGloveStates<<endl<<flush);
		}
		MutexMembers.unlock();
		
		if ( pMessageGloveStates ) {
			if ( iReaderOutStatus != 0 ) {
				//error or warning while reading
				delete pMessageGloveStates;
				pMessageGloveStates = nullptr;
			} else {
				//check if a new message was read
				ulNewClockValue = pMessageGloveStates->getClock();
				
				if ( ulNewClockValue == ulLastClockValue ) {
					//message was readed before -> don't evaluate again
					delete pMessageGloveStates;
					pMessageGloveStates = nullptr;
				} else {  //new message read
					ulLastClockValue = ulNewClockValue;
				}
			}
		}
		
		if ( pMessageGloveStates == nullptr ) {
			//wait a short while till try to read new messages
			if ( 0 < ulMilliSecondsToWait ) {
				msleep( ulMilliSecondsToWait );
			}
			ulMilliSecondsToWait += 10;
			if ( 1000 < ulMilliSecondsToWait ) {
				//wait maximal 1 second
				ulMilliSecondsToWait = 1000;
			}
			continue;
		}  //else pMessageGloveStates != nullptr
		ulMilliSecondsToWait = 100;
		//handle/send message
		
		MutexSamplingMessageListeners.lock();
		
		if ( not LiSamplingMessageListeners.empty() ) {
			DEBUG_OUT_L4(<<"cThreadMessageHandler("<<this<<")::run() sending message to more listeners"<<endl<<flush);
			//set pMessageGloveStates for the first listener
			//(copy (and delete) as less as possible)
			if ( 1 < LiSamplingMessageListeners.size() ) {
				std::list< iSetMessageGloveStates * >::iterator itrSetNewSamplingMessage =
						LiSamplingMessageListeners.begin();
				++itrSetNewSamplingMessage;
				//set a clone of pMessageGloveStates for
				//every listener after the first listener
				for ( ; itrSetNewSamplingMessage != LiSamplingMessageListeners.end();
						++itrSetNewSamplingMessage ) {
					
					(*itrSetNewSamplingMessage)->setMessageGloveStates(
						pMessageGloveStates->clone() );
				}
			}
			//set pMessageGloveStates for the first listener
			(*(LiSamplingMessageListeners.begin()))->setMessageGloveStates(
				pMessageGloveStates );
		} else {  //no listeners exists
			delete pMessageGloveStates;
		}
		
		MutexSamplingMessageListeners.unlock();
	}  //while more messages to read
	
	bStop = false;
	DEBUG_OUT_L2(<<"cThreadReaderMessageGloveStates("<<this<<")::run() done"<<endl<<flush);
}


/**
 * Registers a listeners for new data glove state messages.
 *
 * @see LiSamplingMessageListeners
 * @see unregisterSamplingMessageListener()
 * @param inPSetMessageGloveStates a pointer to a listener for new data
 * 	glove state messages
 * @return true if the listener was registered, else false
 */
bool cThreadReaderMessageGloveStates::registerSamplingMessageListener(
		iSetMessageGloveStates * inPSetMessageGloveStates ) {
	
	if ( inPSetMessageGloveStates == nullptr ) {
		return false;
	}
	MutexSamplingMessageListeners.lock();
	DEBUG_OUT_L2(<<"cThreadReaderMessageGloveStates("<<this<<")::registerSamplingMessageListener("<<inPSetMessageGloveStates<<")"<<endl<<flush);
	LiSamplingMessageListeners.push_back( inPSetMessageGloveStates );
	
	MutexSamplingMessageListeners.unlock();
	return true;
}


/**
 * Unregisters a listeners for new data glove state messages.
 *
 * @see LiSamplingMessageListeners
 * @see registerSamplingMessageListener()
 * @param inPSetMessageGloveStates a pointer to a listener for new data
 * 	glove state messages
 * @return true if the listener was unregistered, else false
 */
bool cThreadReaderMessageGloveStates::unregisterSamplingMessageListener(
		iSetMessageGloveStates * inPSetMessageGloveStates ) {
	
	MutexSamplingMessageListeners.lock();
	DEBUG_OUT_L2(<<"cThreadReaderMessageGloveStates("<<this<<")::unregisterSamplingMessageListener("<<inPSetMessageGloveStates<<")"<<endl<<flush);
	LiSamplingMessageListeners.remove( inPSetMessageGloveStates );
	
	MutexSamplingMessageListeners.unlock();
	return true;
}


















