
//TODO check

/**
 * @file cThreadMessageHandler
 * file name: cThreadMessageHandler.h
 * @author Betti Oesterholz
 * @date 17.08.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++, Qt4
 *
 * This file specifies a class for handling data glove messages.
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
 * This file specifies a class for handling data glove messages.
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
17.08.2014  Oesterholz  created
*/


#ifndef ___N_DATA_GLOVE__N_MAPPER___C_THREAD_MESSAGE_HANDLER_H__
#define ___N_DATA_GLOVE__N_MAPPER___C_THREAD_MESSAGE_HANDLER_H__


#include "version.h"

#include <string>
#include <list>

#include <mutex>

#include "cThread.h"
#include "cMessageSamplingDataFromDataGlove.h"
#include "iSetNewSamplingMessage.h"


namespace nDataGlove{

//forward declarations
class cDataGloveDGTechVHand;

namespace nMapper{

//forward declarations
class cEvaluateDataGloveState;
class cThreadSamplingMessageEvaluator;


class cThreadMessageHandler: public cThread{
public:
	
	/**
	 * The standard constructor for data glove message handler.
	 *
	 * @see cThread
	 */
	cThreadMessageHandler();
	
	/**
	 * destructor
	 */
	virtual ~cThreadMessageHandler();
	
	
	/**
	 * @return the name of this class "cThreadMessageHandler"
	 */
	virtual std::string getName() const;
	
	/**
	 * @return true the thread is running, else false
	 * @see run()
	 */
	bool isRunning() const;
	
	/**
	 * @return a const pointer to the data glove device, which messages are
	 * 	handeled by this object
	 * 	@see pDataGloveDevice
	 */
	cDataGloveDGTechVHand * getDataGloveDevice();
	
	/**
	 * @return a pointer to the data glove device, which messages are
	 * 	handeled by this object
	 * 	@see pDataGloveDevice
	 */
	const cDataGloveDGTechVHand * getDataGloveDevice() const;
	
	/**
	 * Sets the data glove device.
	 * 
	 * @see pDataGloveDevice
	 * @param pInDataGloveDevice a pointer to the data glove device, which
	 * 	messages should handeled by this object
	 */
	void setDataGloveDevice( cDataGloveDGTechVHand * pInDataGloveDevice );
	
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
	 * Signals this thread to quit at a good moment.
	 * @see bStop
	 */
	bool quit();
	
	/**
	 * Runs the data glove message handler.
	 * It will read all messages from the data glove  and process them accordingly.
	 *
	 * @see bStop
	 */
	virtual bool run();
	
	
	/**
	 * Registers a listeners for new data glove sampling messages.
	 *
	 * @see LiSamplingMessageListeners
	 * @see unregisterSamplingMessageListener()
	 * @param inPSetNewSamplingMessage a pointer to a listener for new data
	 * 	glove sampling messages
	 * @return true if the listener was registered, else false
	 */
	bool registerSamplingMessageListener( iSetNewSamplingMessage * inPSetNewSamplingMessage );
	
	/**
	 * Unregisters a listeners for new data glove sampling messages.
	 *
	 * @see LiSamplingMessageListeners
	 * @see registerSamplingMessageListener()
	 * @param inPSetNewSamplingMessage a pointer to a listener for new data
	 * 	glove sampling messages
	 * @return true if the listener was unregistered, else false
	 */
	bool unregisterSamplingMessageListener( iSetNewSamplingMessage * inPSetNewSamplingMessage );
	
	
protected:
	
	/**
	 * A pointer to the data glove device object, or NULL if no data glove
	 * is connected.
	 */
	cDataGloveDGTechVHand * pDataGloveDevice;
	
	/**
	 * A pointer to the last received sampling data from the data glove or
	 * NULL if non was received.
	 */
	nModelDataGloveDGTechVHand::cMessageSamplingDataFromDataGlove *
		pLastMessageSamplingDataFromDataGlove;
	
	/**
	 * The widget for the sampling data.
	 */
	cEvaluateDataGloveState * pEvaluateDataGloveState;
	
	/**
	 * The thread to evaluate sampling data.
	 */
	cThreadSamplingMessageEvaluator * pThreadSamplingMessageEvaluator;

#ifdef CPP_2011
	/**
	 * Mutex to lock access to the members of this class.
	 * Lock the mutex if you use one of the following containers:
	 * @see pDataGloveDevice
	 */
	mutable std::mutex mutexMembers;
#endif  //CPP_2011
	
	
//members to handel more listeners for new data glove sampling messages
	/**
	 * A list with more listeners for new data glove sampling messages.
	 * @see registerSamplingMessageListener()
	 * @see unregisterSamplingMessageListener()
	 * @see mutexSamplingMessageListeners
	 * @see BMoreSamplingMessageListenersExists
	 */
	std::list< iSetNewSamplingMessage * > LiSamplingMessageListeners;

	/**
	 * True if more listeners for new data glove sampling messages exists.
	 * ( not LiSamplingMessageListeners.empty() )
	 * @see LiSamplingMessageListeners
	 */
	bool BMoreSamplingMessageListenersExists;
	
	
#ifdef CPP_2011
	/**
	 * Mutex to lock access to the list with more listeners for new data
	 * glove sampling messages.
	 * Lock the mutex if you use one of the following containers:
	 * 	 @see LiSamplingMessageListeners
	 */
	mutable std::mutex mutexSamplingMessageListeners;
#endif  //CPP_2011	
	
};//end class cThreadMessageHandler

};//end namespace nMapper
};//end namespace nDataGlove

#endif //___N_DATA_GLOVE__N_MAPPER___C_THREAD_MESSAGE_HANDLER_H__




