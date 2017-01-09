
//TODO check

/**
 * @file cThreadSamplingMessageWriter
 * file name: cThreadSamplingMessageWriter.h
 * @author Betti Oesterholz
 * @date 05.02.2016
 * @mail webmaster@BioKom.info
 *
 * System: C++, Qt4
 *
 * This file specifies a class for writing the actual data glove sampling
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
 * This file specifies a class for writing the actual data glove sampling
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


#ifndef ___N_DATA_GLOVE__N_MAPPER___C_THREAD_SAMPLING_MESSAGE_WRITER_H__
#define ___N_DATA_GLOVE__N_MAPPER___C_THREAD_SAMPLING_MESSAGE_WRITER_H__


#include "version.h"

#include <string>
#include <ctime>
#include <mutex>

#include "cMessageSamplingDataFromDataGlove.h"

#include "cThread.h"
#include "iSetNewSamplingMessage.h"
#include "iEvaluateMessageGloveStates.h"


namespace nDataGlove{
namespace nMapper{

//forward declarations
class cEvaluateDataGloveState;


class cThreadSamplingMessageWriter: public cThread, public iSetNewSamplingMessage{
public:
	
	
	/**
	 * The standard constructor for data glove message handler.
	 *
	 * @see cThread
	 * @param inSzFilePath The path (inclusive file name) where to write the
	 * 	data glove sampling message to.
	 * @param inPEvaluateMessageGloveStates a pointer to the object which
	 * 	evaluates the message to send for the data glove data
	 * @see SzFilePath
	 */
	cThreadSamplingMessageWriter( const std::string inSzFilePath,
		iEvaluateMessageGloveStates * inPEvaluateMessageGloveStates );
	
	/**
	 * destructor
	 */
	virtual ~cThreadSamplingMessageWriter();
	
	
	/**
	 * @return the name of this class "cThreadSamplingMessageWriter"
	 */
	virtual std::string getName() const;
	
	/**
	 * Returns the path (inclusive file name) where to write the data
	 * glove sampling message to.
	 *
	 * @see SzFilePath
	 * @returns The path (inclusive file name) where to write the
	 * 	data glove sampling message to.
	 */
	std::string getFilePath() const;
	
	/**
	 * Sets the path (inclusive file name) where to write the data
	 * glove sampling message to.
	 *
	 * @see SzFilePath
	 * @param inSzFilePath The path (inclusive file name) where to write the
	 * 	data glove sampling message to.
	 */
	void setFilePath( const std::string inSzFilePath );
	
	
	/**
	 * @return true the thread is running, else false
	 * @see run()
	 */
	bool isRunning() const;
	

	/**
	 * Sets the new sampling data message.
	 * Also cares that the given message is deleted.
	 * (Do not delete pInNewMessageSamplingDataFromDataGlove.)
	 *
	 * @see pNewMessageSamplingDataFromDataGlove
	 * @param pInNewMessageSamplingDataFromDataGlove a pointer the new
	 * 	sampling data message
	 */
	void setNewSamplingMessage( nModelDataGloveDGTechVHand::cMessageSamplingDataFromDataGlove *
		pInNewMessageSamplingDataFromDataGlove );
	
	/**
	 * @return The object which evaluates the message to send for the data glove data.
	 * 	@see pEvaluateMessageGloveStates
	 */
	iEvaluateMessageGloveStates * getEvaluateDataGloveState();

	/**
	 * Sets the object which evaluates the message to send for the data glove data.
	 *
	 * @see pEvaluateMessageGloveStates
	 * @param inPEvaluateMessageGloveStates a pointer to the object which
	 * 	evaluates the message to send for the data glove data
	 */
	void setEvaluateDataGloveState( iEvaluateMessageGloveStates * inPEvaluateMessageGloveStates );
	
	/**
	 * Runs the data glove message handler.
	 * It will read all messages from the data glove  and process them accordingly.
	 *
	 * @see bStop
	 */
	virtual bool run();
	
	
protected:
	
	/**
	 * The path (inclusive file name) where to write the data glove sampling
	 * message to.
	 */
	std::string SzFilePath;

	/**
	 * A pointer to the new received sampling data from the data glove or
	 * NULL if non was received.
	 */
	nModelDataGloveDGTechVHand::cMessageSamplingDataFromDataGlove *
		pNewMessageSamplingDataFromDataGlove;
	
	/**
	 * The pNewMessageSamplingDataFromDataGlove has changed after the last
	 * run() call.
	 * @see pNewMessageSamplingDataFromDataGlove
	 * @see run()
	 */
	bool bNewMessageSamplingDataFromDataGloveChangedTillLastRunEval;
	
	/**
	 * Messages wich are received (with setNewSamplingMessage) but not
	 * evaluated (by run()).
	 * @see setNewSamplingMessage()
	 * @see run()
	 */
	unsigned long ulMessageInQueue;
	
	/**
	 * The number of the message evaluated.
	 */
	unsigned long ulMessageCounter;
	
	/**
	 * The count of all messages set.
	 * (Count of setNewSamplingMessage() calls.)
	 * @see setNewSamplingMessage()
	 */
	//unsigned long ulMessageCounter;
	
	/**
	 * The object which evaluates the message to send for the data glove data.
	 * @see iEvaluateMessageGloveStates.evaluate()
	 */
	iEvaluateMessageGloveStates * pEvaluateMessageGloveStates;

#ifdef CPP_2011
	
	/**
	 * Mutex to lock access to the members of this class.
	 * Lock the mutex if you use one of the following containers:
	 * 	@see pNewMessageSamplingDataFromDataGlove
	 */
	mutable std::mutex mutexMembers;
	
#endif  //CPP_2011
	
	
};//end class cThreadSamplingMessageWriter

};//end namespace nMapper
};//end namespace nDataGlove

#endif //___N_DATA_GLOVE__N_MAPPER___C_THREAD_SAMPLING_MESSAGE_WRITER_H__




