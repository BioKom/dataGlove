
//TODO check

/**
 * @file cThreadSamplingMessageEvaluator
 * file name: cThreadSamplingMessageEvaluator.h
 * @author Betti Oesterholz
 * @date 17.08.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++, Qt4
 *
 * This file specifies a class for evaluating data glove sampling messages.
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
 * This file specifies a class for evaluating data glove sampling messages.
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


#ifndef ___N_DATA_GLOVE__N_MAPPER___C_THREAD_SAMPLING_MESSAGE_EVALUATOR_H__
#define ___N_DATA_GLOVE__N_MAPPER___C_THREAD_SAMPLING_MESSAGE_EVALUATOR_H__


#include "version.h"

#include <string>
#include <ctime>
#include <mutex>

#include "cMessageSamplingDataFromDataGlove.h"

#include "cThread.h"


namespace nDataGlove{
namespace nMapper{

//forward declarations
class cEvaluateDataGloveState;


class cThreadSamplingMessageEvaluator: public cThread{
public:
	
	
	/**
	 * The standard constructor for data glove message handler.
	 *
	 * @see cThread
	 */
	cThreadSamplingMessageEvaluator();
	
	/**
	 * destructor
	 */
	virtual ~cThreadSamplingMessageEvaluator();
	
	
	/**
	 * @return the name of this class "cThreadSamplingMessageEvaluator"
	 */
	virtual std::string getName() const;
	
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
	 * Runs the data glove message handler.
	 * It will read all messages from the data glove  and process them accordingly.
	 *
	 * @see bStop
	 */
	virtual bool run();
	
	
protected:
	
	/**
	 * A pointer to the last received sampling data from the data glove or
	 * NULL if non was received.
	 */
//TODO weg: 	nModelDataGloveDGTechVHand::cMessageSamplingDataFromDataGlove *
//		pLastMessageSamplingDataFromDataGlove;

	/**
	 * A pointer to the new received sampling data from the data glove or
	 * NULL if non was received.
	 */
	nModelDataGloveDGTechVHand::cMessageSamplingDataFromDataGlove *
		pNewMessageSamplingDataFromDataGlove;

	
	/**
	 * The count of all messages set.
	 * (Count of setNewSamplingMessage() calls.)
	 * @see setNewSamplingMessage()
	 */
	unsigned long ulMessageCounter;
	
	/**
	 * The number of messages in the second tActualSecond.
	 * @see Actual
	 */
	unsigned int uiNumberOfMessagesInActualSecond;
	
	/**
	 * The actual second, for which the messages are counted.
	 * @see uiNumberOfMessagesInActualSecond
	 */
	time_t tActualSecond;
	
	/**
	 * The average number of messages per second.
	 * Evalued with the help of:
	 * 	@see uiNumberOfMessagesInActualSecond
	 * 	@see tActualSecond
	 */
	double dMessagesPerSecond;
	
	
	/**
	 * The widget for the sampling data.
	 */
	cEvaluateDataGloveState * pEvaluateDataGloveState;

#ifdef CPP_2011
	
	/**
	 * Mutex to lock access to the members of this class.
	 * Lock the mutex if you use one of the following containers:
	 * 	@see pNewMessageSamplingDataFromDataGlove
	 */
	mutable std::mutex mutexMembers;
	
#endif  //CPP_2011
	
};//end class cThreadSamplingMessageEvaluator

};//end namespace nMapper
};//end namespace nDataGlove

#endif //___N_DATA_GLOVE__N_MAPPER___C_THREAD_SAMPLING_MESSAGE_EVALUATOR_H__




