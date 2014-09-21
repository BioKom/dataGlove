/**
 * @file cThread
 * file name: cThread.h
 * @author Betti Oesterholz
 * @date 17.08.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This header specifies the abstract basis class for threads.
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
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * This header specifies the abstract basis class for threads.
 * Overwrite the start() or run() method to implement the operation.
 *
 * Beware: If the variable bStop is true, quit the operation.
 */
/*
History:
17.08.2014  Oesterholz  created
*/


#ifndef ___C_THREAD_H__
#define ___C_THREAD_H__

#include "version.h"

#include <string>
#include <set>
#include <ctime>


#ifdef CPP_2011
	#include <thread>
	#include <mutex>
#else  //CPP_2011

	#ifdef WINDOWS
		#include <windows.h>
	#endif //WINDOWS

#endif   //CPP_2011


class cThread;

/**
 * This is the interface for objects, which want's to be notified about
 * changes in the operation running status.
 */
class iOperationStatusChange{
public:
	/**
	 * This method is called, if the running status of the operation is changed.
	 *
	 * @see cThread::bIsRunning
	 * @see cThread::isRunning()
	 * @param pOperation a pointer to the operation, which status has changed
	 * @param isRunning the new running status for the operation
	 */
	virtual void operationStatusChange( cThread * pOperation, bool isRunning ) = 0;
};


class cThread{
protected:
	
#ifdef CPP_2011
	
	/**
	 * Mutex variable for handling the set with the operation status change
	 * listeners ( @see setOperationListeners ).
	 */
	std::mutex mutexOperationStatusChange;
	
	/**
	 * Mutex variable for changing the operation run status in one step.
	 * @see bIsRunning
	 * @see bHasRun
	 */
	std::mutex mutexRunStatusChange;
	
	/**
	 * The tread for running the operation.
	 * @see start()
	 * @see run()
	 */
	std::thread * pThreadOperator;
	
#else  //CPP_2011

#ifdef WINDOWS
	/**
	 * Mutex variable for handling the set with the operation status change
	 * listeners ( @see setOperationListeners ).
	 */
	HANDLE mutexOperationStatusChange;
	
	/**
	 * Mutex variable for changing the operation run status in one step.
	 * @see bIsRunning
	 * @see bHasRun
	 */
	HANDLE mutexRunStatusChange;
	
	/**
	 * The tread for running the operation.
	 * @see start()
	 * @see run()
	 */
	HANDLE pThreadOperator;

#else //WINDOWS
	/**
	 * Mutex variable for handling the set with the operation status change
	 * listeners ( @see setOperationListeners ).
	 */
	pthread_mutex_t mutexOperationStatusChange;
	
	/**
	 * Mutex variable for changing the operation run status in one step.
	 * @see bIsRunning
	 * @see bHasRun
	 */
	pthread_mutex_t mutexRunStatusChange;
	
	/**
	 * The tread for running the operation.
	 * @see start()
	 * @see run()
	 */
	pthread_t * pThreadOperator;
#endif //WINDOWS
	
#endif   //CPP_2011
	
	/**
	 * If true the operation is running, else not.
	 */
	bool bIsRunning;
	
	/**
	 * If true the operation has run, else not.
	 */
	bool bHasRun;
	
	
	/**
	 * If true the operation should be quited, else not.
	 * Beware: Check this variable in your operation implementation and
	 * 	quit the execution if the variable is true.
	 */
	bool bStop;
	
	/**
	 * A value for the cpu -time this operation runs.
	 */
	double dRunningTime;
	
private:
	/**
	 * The last time the cpu -time was evalued.
	 * This is needed to evalue the @see dRunningTime .
	 */
	clock_t tmLastCpuRunTime;
	
public:

	/**
	 * The standardconstructor for the operation.
	 * It will create the operation, but won't start it.
	 */
	cThread();
	
	/**
	 * Destructor of the class cThread.
	 */
	virtual ~cThread();

	/**
	 * This method starts the operation and returns.
	 * It dosn't wait till the operation is ended for returning.
	 * Overwrite this or the run() method to implement the operation.
	 * Beware: If overwriten call setIsRunning( false ), when the operation
	 * is done.
	 *
	 * @see run()
	 * @see quit()
	 * @see setIsRunning()
	 * @return true if the operation was started
	 */
	virtual bool start();
	
	/**
	 * This method runs the operation.
	 * It will wait till the operation is ended befor returning.
	 * Overwrite this or the start() method to implement the operation.
	 * Beware: If overwriten call setIsRunning( false ), when the operation
	 * is done.
	 *
	 * @see start()
	 * @see quit()
	 * @see setIsRunning()
	 * @return true if the operation was started
	 */
	virtual bool run();
	
	
	/**
	 * This method quits the operation and returns.
	 *
	 * @see bStop
	 * @see run()
	 * @see start()
	 * @return true if this operation is quited, else false
	 */
	virtual bool quit();
	
	/**
	 * @see bIsRunning
	 * @see run()
	 * @see start()
	 * @see quit()
	 * @return true if this operation is running, else false
	 */
	virtual bool isRunning() const;
	
	/**
	 * @see bHasRun
	 * @see bIsRunning
	 * @see run()
	 * @see start()
	 * @see quit()
	 * @return true the operation has run, else not
	 */
	virtual bool hasRun() const;
	
	
	/**
	 * @return A value for the cpu -time this operation runs.;
	 * 	@see isRunning() gives back true for this time
	 * @see dRunningTime
	 */
	double getCpuRunTime() const;

	/**
	 * @return the (class-)name of the operation
	 */
	virtual std::string getName() const;
	
	/**
	 * This method adds the given operation running status listener object
	 * to the set of running status listener objects. Listeners of the set
	 * gets notified if the running status of this object/ operation changes.
	 *
	 * @see unregisterOperationRunStatusListener()
	 * @see setOperationListeners
	 * @see iOperationStatusChange
	 * @param pOperationRunStatusListener a pointer to the operation running
	 * 	status listener object to add to the set of operation running listeners
	 * @return true if the operation running listener was added, else false
	 */
	bool registerOperationRunStatusListener(
		iOperationStatusChange * pOperationRunStatusListener );
	
	/**
	 * This method removes the given operation running status listener object
	 * from the set of running status listener objects. Listeners of the set
	 * gets notified if the running status of this object/ operation changes.
	 *
	 * @see registerOperationRunStatusListener()
	 * @see setOperationListeners
	 * @see iOperationStatusChange
	 * @param pOperationRunStatusListener a pointer to the operation running
	 * 	status listener object to removes from the set of operation
	 * 	running listeners
	 * @return true if the operation running listener was removed, else false
	 */
	bool unregisterOperationRunStatusListener(
		iOperationStatusChange * pOperationRunStatusListener );

	/**
	 * This function sleeps for a short period.
	 */
	static void shortSleep();
	
	/**
	 * This function sleeps for some milli seconds.
	 *
	 * @param ulMilliSeconds the number of milli seconds to sleep
	 */
	static void msleep( const unsigned long ulMilliSeconds );

protected:

	/**
	 * This method sets the value if this operation is running to the given
	 * value. It notifies allso objects which listen to operation status
	 * changes.
	 *
	 * @see isRunning
	 * @param bInIsRunning the value, if this operation is running
	 * @return the new value, if this operation is running
	 */
	bool setIsRunning( bool bInIsRunning );
	
	/**
	 * If the run() or start() method of this class cThread was called
	 * this filed is true.
	 * It should quit cyclic class of run() and start();
	 */
	bool bOperationStartedInRoot;
	
	/**
	 * The set with pointers to objects, which want's to be notified if the
	 * operation running status changes.
	 * @see iOperationStatusChange
	 */
	std::set< iOperationStatusChange * > setOperationListeners;
	
	
	/**
	 * This method runs the operation for a tread.
	 * It will wait till the operation is ended befor returning.
	 * It will simply call run().
	 *
	 * @see run()
	 * @see start()
	 * @see quit()
	 */
	static void * runTread( void * inputArg );

#ifndef CPP_2011
#ifdef WINDOWS
	/**
	 * Wraper function for windows.
	 * Wait till the given mutex is free and than locks it.
	 * @param pMutexHandle pointer to the mutex to lock.
	 */
	static void pthread_mutex_lock( HANDLE * pMutexHandle );

	/**
	 * Wraper function for windows.
	 * Unlocks the given mutex.
	 * @param pMutexHandle pointer to the mutex to unlock.
	 */
	static void pthread_mutex_unlock( HANDLE * pMutexHandle );
#endif //WINDOWS
#endif  //CPP_2011
	
};//end class cThread


#endif //___C_THREAD_H__







