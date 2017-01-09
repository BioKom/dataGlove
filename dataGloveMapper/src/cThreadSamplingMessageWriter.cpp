
//TODO check

/**
 * @file cThreadSamplingMessageWriter
 * file name: cThreadSamplingMessageWriter.cpp
 * @author Betti Oesterholz
 * @date 17.08.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++, Qt4
 *
 * This file implements a class for writing the actual data glove sampling
 * messages into a file.
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
 * This file implements a class for writing the actual data glove sampling
 * messages into a file.
 *
 * @see cThreadMessageHandler
 * @see cThread
 * @see cMessageDataGlove
 * @see cDataGloveDGTechVHand
 * @see cEvaluateDataGloveState
 * @see cMessageSamplingDataFromDataGlove
 */
/*
History:
05.02.2016  Oesterholz  created
*/


//#define DEBUG


#include "cThreadSamplingMessageWriter.h"


#include <map>
#include <fstream>

#include "cEvaluateDataGloveState.h"
#include "cDataGloveState.h"
#include "iCallFunction.h"



using namespace nDataGlove::nMapper;
using namespace nDataGlove::nModelDataGloveDGTechVHand;
using namespace nDataGlove;

using namespace std;

/**
 * The standard constructor for data glove message handler.
 *
 * @see cThread
 * @param inSzFilePath The path (inclusive file name) where to write the
 * 	data glove sampling message to.
 * @param inPEvaluateMessageGloveStates a pointer to the object which
 * 	evaluates the message to send for the data glove data
 */
cThreadSamplingMessageWriter::cThreadSamplingMessageWriter(
		const std::string inSzFilePath,
		iEvaluateMessageGloveStates * inPEvaluateMessageGloveStates ) :
		cThread(),
		SzFilePath( inSzFilePath ),
		pNewMessageSamplingDataFromDataGlove( NULL ),
		bNewMessageSamplingDataFromDataGloveChangedTillLastRunEval( false ),
		ulMessageInQueue( 0 ),
		ulMessageCounter( 0 ),
		pEvaluateMessageGloveStates( inPEvaluateMessageGloveStates ) {
}


/**
 * destructor
 */
cThreadSamplingMessageWriter::~cThreadSamplingMessageWriter() {
	
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
 * @return the name of this class "cThreadSamplingMessageWriter"
 */
std::string cThreadSamplingMessageWriter::getName() const {
	
	return "cThreadSamplingMessageWriter";
}


/**
 * @return true the thread is running, else false
 * @see run()
 */
bool cThreadSamplingMessageWriter::isRunning() const {
	
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
void cThreadSamplingMessageWriter::setNewSamplingMessage(
	nModelDataGloveDGTechVHand::cMessageSamplingDataFromDataGlove *
		pInNewMessageSamplingDataFromDataGlove ) {
	
	DEBUG_OUT_L3(<<"cThreadSamplingMessageWriter("<<this<<")::setNewSamplingMessage(new="<<pInNewMessageSamplingDataFromDataGlove<<")"<<endl<<flush);
	
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
	ulMessageCounter++;
	DEBUG_OUT_L4(<<"cThreadSamplingMessageWriter("<<this<<")::setNewSamplingMessage() bNewMessageSamplingDataFromDataGloveChangedTillLastRunEval set"<<endl<<flush);
#ifdef CPP_2011
	mutexMembers.unlock();
#endif  //CPP_2011
	if ( pToDeleteMessageSamplingDataFromDataGlove != NULL ) {
		delete pToDeleteMessageSamplingDataFromDataGlove;
	}
}



/**
 * @return The object which evaluates the message to send for the data glove data.
 * 	@see pEvaluateMessageGloveStates
 */
iEvaluateMessageGloveStates * cThreadSamplingMessageWriter::getEvaluateDataGloveState() {
	
	return pEvaluateMessageGloveStates;
}


/**
 * Sets the object which evaluates the message to send for the data glove data.
 *
 * @see pEvaluateMessageGloveStates
 * @param inPEvaluateMessageGloveStates a pointer to the object which
 * 	evaluates the message to send for the data glove data
 */
void cThreadSamplingMessageWriter::setEvaluateDataGloveState(
		iEvaluateMessageGloveStates * inPEvaluateMessageGloveStates ) {
	
#ifdef CPP_2011
	mutexMembers.lock();
#endif  //CPP_2011
	pEvaluateMessageGloveStates = inPEvaluateMessageGloveStates;
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
bool cThreadSamplingMessageWriter::run() {
	
	DEBUG_OUT_L2(<<"cThreadSamplingMessageWriter("<<this<<")::run() called"<<endl<<flush);
	
	bIsRunning = true;
	bStop = false;
	cMessageSamplingDataFromDataGlove * pMessageToEvaluate = NULL;
	
	cMessageGloveStates * pMessageGloveStates = NULL;
	
	long lMilliSecondsToWait = -20;
	bool bNothingHappend = false;
	bool bSomethingMissed = false;
	
	while ( ! bStop ) {
		DEBUG_OUT_L4(<<"cThreadSamplingMessageWriter("<<this<<")::run() if something happend"<<endl<<flush);
		
		if ( bSomethingMissed ) {
			//something was missed -> reevaluate the delay time
			lMilliSecondsToWait = -20;
			bSomethingMissed = false;
			
		} else if ( bNothingHappend ) {
			//nothing happend
			DEBUG_OUT_L4(<<"cThreadSamplingMessageWriter("<<this<<")::run() nothing happend"<<endl<<flush);
			if ( 0 < lMilliSecondsToWait ) {
				DEBUG_OUT_L4(<<"cThreadSamplingMessageWriter("<<this<<")::run() wait millisecounds"<<endl<<flush);
				msleep( lMilliSecondsToWait );
			}
			//nothing happend -> wait longer
			lMilliSecondsToWait++;
		} else {  //wait less
			lMilliSecondsToWait--;
		}
		
		bNothingHappend = true;
		
		if ( bNewMessageSamplingDataFromDataGloveChangedTillLastRunEval ) {
			DEBUG_OUT_L4(<<"cThreadSamplingMessageWriter("<<this<<")::run() evaluate new message"<<endl<<flush);
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
			DEBUG_OUT_L4(<<"cThreadSamplingMessageWriter("<<this<<")::run() bNewMessageSamplingDataFromDataGloveChangedTillLastRunEval unset"<<endl<<flush);
#ifdef CPP_2011
			mutexMembers.unlock();
#endif  //CPP_2011
			if ( pMessageToEvaluate == NULL ) {
				DEBUG_OUT_L4(<<"cThreadSamplingMessageWriter("<<this<<")::run() no new message"<<endl<<flush);
				//no new message -> nothing to do
				continue;
			} else { //else pReadedMessage != NULL
				//-> new message happend
				DEBUG_OUT_L4(<<"cThreadSamplingMessageWriter("<<this<<")::run() new message"<<endl<<flush);
				bNothingHappend = false;
			}
		} else {
			//pMessageToEvaluate = NULL;
			DEBUG_OUT_L4(<<"cThreadSamplingMessageWriter("<<this<<")::run() bNewMessageSamplingDataFromDataGloveChangedTillLastRunEval not set"<<endl<<flush);
			//no new message -> nothing to do
			continue;
		}
		
		//handle message
		DEBUG_OUT_L3(<<"cThreadSamplingMessageWriter("<<this<<")::run() processing next message: "<<pMessageToEvaluate<<endl<<flush);
		
		if ( ( pEvaluateMessageGloveStates != NULL ) &&
				( pMessageToEvaluate != NULL ) ) {
			
			pMessageGloveStates =
				pEvaluateMessageGloveStates->evaluate(
					pMessageToEvaluate, ulMessageCounter );
			
			if ( pMessageGloveStates != NULL ) {
				//write the message to the message file
				DEBUG_OUT_L3(<<"cThreadSamplingMessageWriter("<<this<<")::run() write the message to the message file: "<<SzFilePath<<endl<<flush);
				ofstream osMessageFile;
				osMessageFile.open( SzFilePath, ios::out | ios::binary | ios::trunc );
				if ( osMessageFile.is_open() ) {
					DEBUG_OUT_L3(<<"cThreadSamplingMessageWriter("<<this<<")::run() writing"<<endl<<flush);
					pMessageGloveStates->store( osMessageFile );
					osMessageFile.close();
				}
				delete pMessageGloveStates;
			}else{  //Error occured: delete the message file
				DEBUG_OUT_L3(<<"cThreadSamplingMessageWriter("<<this<<")::run() delete"<<endl<<flush);
				remove( SzFilePath.c_str() );
			}
		}
	
		if ( pMessageGloveStates != NULL ) {
			delete pMessageToEvaluate;
			pMessageToEvaluate = NULL;
		}
	}  //while more messages to read
	
	bStop = false;
	bIsRunning = false;
	DEBUG_OUT_L2(<<"cThreadSamplingMessageWriter("<<this<<")::run() done"<<endl<<flush);
	return true;
}
















