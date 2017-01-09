
//TODO check

/**
 * @file cThreadReaderMessageGloveStates
 * file name: cThreadReaderMessageGloveStates.h
 * @author Betti Oesterholz
 * @date 17.08.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++, Qt4
 *
 * This file specifies a class for handling data glove state messages.
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
 * This file specifies a class for handling data glove state messages.
 * It is a thread, so it won't block the main application.
 * Handeled are the state messages from the data glove
 *
 * @see QThread
 * @see cMessageGloveStates
 * @see cDataGloveDGTechVHand
 */
/*
History:
17.08.2014  Oesterholz  created
*/

#ifndef ___N_DATA_GLOVE__N_INFO___C_THREAD_READER_MESSAGE_GLOVE_STATE_H__
#define ___N_DATA_GLOVE__N_INFO___C_THREAD_READER_MESSAGE_GLOVE_STATE_H__


#include "version.h"

#include <string>
#include <list>

#include <QMutex>
#include <QThread>

#include "cMessageGloveStates.h"
#include "iSetMessageGloveStates.h"


namespace nDataGlove{

namespace nInfo{


class cThreadReaderMessageGloveStates: public QThread{
public:
	
	/**
	 * The standard constructor for data glove message handler.
	 *
	 * @see QThread
	 */
	explicit cThreadReaderMessageGloveStates();
	
	cThreadReaderMessageGloveStates ( const cThreadReaderMessageGloveStates& ) = delete;
	cThreadReaderMessageGloveStates & operator=( const cThreadReaderMessageGloveStates& ) = delete;
	
	/**
	 * destructor
	 */
	virtual ~cThreadReaderMessageGloveStates();
	
	
	/**
	 * @return the name of this class "cThreadReaderMessageGloveStates"
	 */
	virtual std::string getName() const;
	
	/**
	 * @return a const pointer to the data glove state file, which messages are
	 * 	handeled by this object
	 * 	@see FFileMessageGloveStates
	 */
	std::string getFileMessageGloveStates();
	
	/**
	 * @return a pointer to the data glove state file, which messages are
	 * 	handeled by this object
	 * 	@see FFileMessageGloveStates
	 */
	const std::string getFileMessageGloveStates() const;
	
	/**
	 * Sets the data glove state file.
	 *
	 * @see FFileMessageGloveStates
	 * @param pInFileMessageGloveStates a pointer to the data glove state file, which
	 * 	messages should handeled by this object
	 */
	void setFileMessageGloveStates( const std::string inFileMessageGloveStates );
	
	
	/**
	 * Signals this thread to quit at a good moment.
	 * @see bStop
	 */
	void quit();
	
	/**
	 * Runs the data glove message handler.
	 * It will read all messages from the data glove and process them accordingly.
	 *
	 * @see bStop
	 */
	virtual void run();
	
	
	/**
	 * Registers a listeners for new data glove state messages.
	 *
	 * @see LiSamplingMessageListeners
	 * @see unregisterSamplingMessageListener()
	 * @param inPSetMessageGloveStates a pointer to a listener for new data
	 * 	glove state messages
	 * @return true if the listener was registered, else false
	 */
	bool registerSamplingMessageListener( iSetMessageGloveStates * inPSetMessageGloveStates );
	
	/**
	 * Unregisters a listeners for new data glove state messages.
	 *
	 * @see LiSamplingMessageListeners
	 * @see registerSamplingMessageListener()
	 * @param inPSetMessageGloveStates a pointer to a listener for new data
	 * 	glove state messages
	 * @return true if the listener was unregistered, else false
	 */
	bool unregisterSamplingMessageListener( iSetMessageGloveStates * inPSetMessageGloveStates );
	
	
protected:
	
	/**
	 * A pointer to the data glove state file object, or NULL if no data glove
	 * is connected.
	 */
	std::string FFileMessageGloveStates;
	
	/**
	 * A pointer to the last received sampling data from the data glove or
	 * NULL if non was received.
	 */
	nDataGlove::nMapper::cMessageGloveStates *
		pLastMessageSamplingDataFromDataGlove;
	
	/**
	 * Mutex to lock access to the members of this class.
	 * Lock the mutex if you use one of the following containers:
	 * @see FFileMessageGloveStates
	 */
	mutable QMutex MutexMembers;
	
	
//members to handel more listeners for new data glove state messages
	/**
	 * A list with more listeners for new data glove state messages.
	 * @see registerSamplingMessageListener()
	 * @see unregisterSamplingMessageListener()
	 * @see MutexSamplingMessageListeners
	 */
	std::list< iSetMessageGloveStates * > LiSamplingMessageListeners;
	
	
	/**
	 * Mutex to lock access to the list with more listeners for new data
	 * glove state messages.
	 * Lock the mutex if you use one of the following containers:
	 * 	 @see LiSamplingMessageListeners
	 */
	mutable QMutex MutexSamplingMessageListeners;
	
	
	/**
	 * True if this thread shoud be stoped, else false.
	 */
	bool bStop = false;
	
};//end class cThreadReaderMessageGloveStates

};//end namespace nInfo
};//end namespace nDataGlove

#endif //___N_DATA_GLOVE__N_INFO___C_THREAD_READER_MESSAGE_GLOVE_STATE_H__




