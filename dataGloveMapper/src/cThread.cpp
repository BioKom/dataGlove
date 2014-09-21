/**
 * @file cThread
 * file name: cThread.cpp
 * @author Betti Oesterholz
 * @date 17.08.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file implements the abstract basis class for threads.
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
 * This file implement the abstract basis class for threads.
 * Overwrite the start() or run() method to implement the operation.
 *
 * Beware: If the variable bStop is true, stop the operation.
 */
/*
History:
17.08.2014  Oesterholz  created
*/

//TODO weg
//#define DEBUG

#include "cThread.h"

#include <string>

#include <ctime>

#ifndef CPP_2011
	#include <pthread.h>
#endif  //CPP_2011

#ifdef DEBUG
	#include <cerrno>
	
	static long long lTreadsRunning = 0;
#endif

using namespace std;


/**
 * The standardconstructor for the operation.
 * It will create the operation, but won't start it.
 *
 * @param operationId the identifer vor the operation
 * @param szOperationDomain the type of individuals on which the
 * 	operation will run
 */
cThread::cThread(): pThreadOperator( NULL ),
		bIsRunning( false ), bHasRun( false ), bStop( false ),
		dRunningTime( 0.01 ), tmLastCpuRunTime( 0 ),
		bOperationStartedInRoot( false ) {
	
	//init mutex variables
#ifndef CPP_2011

#ifdef WINDOWS
	//close mutex handles
	if ( mutexOperationStatusChange == NULL ) {
		mutexOperationStatusChange = CreateMutex( NULL, FALSE, NULL);
	}
	if ( mutexRunStatusChange == NULL ) {
		mutexRunStatusChange = CreateMutex( NULL, FALSE, NULL);
	}
#else//WINDOWS
	pthread_mutex_init( &mutexOperationStatusChange, NULL);
	pthread_mutex_init( &mutexRunStatusChange, NULL);
#endif//WINDOWS
	
#endif   //CPP_2011
}


/**
 * Destructor of the class cThread.
 */
cThread::~cThread() {
	
#ifdef CPP_2011
	if ( pThreadOperator != NULL ) {
		//wait till tread is done and join it with this tread
		pThreadOperator->join();
		delete pThreadOperator;
	}
#else  //CPP_2011
	
#ifdef WINDOWS
	if ( pThreadOperator != NULL ) {
		//wait till tread is done and join it with this tread
		WaitForMultipleObjects( 1, &pThreadOperator, TRUE, INFINITE );
		//close thread handles
		CloseHandle( pThreadOperator );
	}
	//close mutex handles
	CloseHandle( mutexOperationStatusChange );
	mutexOperationStatusChange = NULL;
	CloseHandle( mutexRunStatusChange );
	mutexRunStatusChange = NULL;

#else //WINDOWS
	if ( pThreadOperator != NULL ) {
		//wait till tread is done and join it with this tread
		pthread_join( *pThreadOperator, NULL);

		delete pThreadOperator;
	}
	//destroy the mutexs
	pthread_mutex_lock( & mutexOperationStatusChange );
	pthread_mutex_unlock( & mutexOperationStatusChange );
	pthread_mutex_destroy( &mutexOperationStatusChange );
	
	pthread_mutex_lock( & mutexRunStatusChange );
	pthread_mutex_unlock( & mutexRunStatusChange );
	pthread_mutex_destroy( &mutexRunStatusChange );
#endif //WINDOWS
	
#endif   //CPP_2011
}


/**
 * This method starts the operation and returns.
 * It dosn't wait till the operation is ended for returning.
 * Overwrite this or the run() method to implement the operation.
 * Beware: If overwriten set bIsRunning to false, when the operation is done.
 *
 * @see run()
 * @see quit()
 * @return true if the operation was started
 */
bool cThread::start() {
	
	if ( isRunning() || hasRun() || bStop || bOperationStartedInRoot ) {
		//the operation runs allready
		DEBUG_OUT_L2(<<"Operation "<<((isRunning())?"is running ":"not running ")<<((hasRun())?"has run ":"has not run ")<<(bStop?"stoped ":"not stopped ")<<(bOperationStartedInRoot?"started in root":"not started in root")<<endl<<flush);
		return false;
	}
	if ( pThreadOperator != NULL ) {
		DEBUG_OUT_L2(<<"Thread was created allready."<<endl<<flush);
		return false;
	}
	bOperationStartedInRoot = true;
	
#ifdef CPP_2011
	//create the thread
	pThreadOperator = new thread( cThread::runTread, this );
#else  //CPP_2011
	
#ifdef WINDOWS
	DWORD   dwThreadId = 0;
	pThreadOperator = CreateThread(
		NULL, //default security attributes
		0, //use default stack size
		(LPTHREAD_START_ROUTINE)&(cThread::runTread), //thread function name
		this, //argument to thread function
		0, //use default creation flags 
		&dwThreadId ); //returns the thread identifier
	
	const int iReturn = ( pThreadOperator == NULL ) ? 1 : 0 ;
#else //WINDOWS
	pThreadOperator = new pthread_t();
	//run in a seperate tread
	int iReturn = pthread_create( pThreadOperator, NULL, &(cThread::runTread), this );
#endif //WINDOWS
	
#endif   //CPP_2011
	
	DEBUG_OUT_L2(<<"Operation treads running:"<<lTreadsRunning<<endl<<flush);
	
	bOperationStartedInRoot = false;
#ifndef CPP_2011
	if ( iReturn != 0 ) {
		DEBUG_OUT_L2(<<"Thread could not started. Return code "<<iReturn<<" ("<<((iReturn==EINVAL)?"attribute invalid":"")<<((iReturn==EAGAIN)?"not enougth resources":"")<<" )."<<endl<<flush);
		return false;
	}
#endif   //CPP_2011
	return true;
}


/**
 * This method runs the operation.
 * It will wait till the operation is ended befor returning.
 * Overwrite this or the start() method to implement the operation.
 * Beware: If overwriten set bIsRunning to false, when the operation is done.
 *
 * @see start()
 * @see quit()
 * @return true if the operation was started
 */
bool cThread::run() {

	if ( isRunning() || bStop || bOperationStartedInRoot ) {
		//the operation runs allready
		return false;
	}
	bOperationStartedInRoot = true;
	//start the operation and wait till it's done
	if ( ! start() ) {
		bOperationStartedInRoot = false;
		return false;
	}
	//wait till the operation is done
	while ( isRunning() ) {
		shortSleep();
	}
	
	bOperationStartedInRoot = false;
	return true;
}


/**
 * This method stops the operation and returns.
 *
 * @see bStop
 * @see run()
 * @see start()
 * @return true if this operation is stoped, else false
 */
bool cThread::quit() {
	
	if ( ! isRunning() ) {
		//nothing to stop
		return false;
	}
	bStop = true;
	
	//wait till the operation is done
	while ( isRunning() ) {
		shortSleep();
	}
	
	bStop = false;
	return true;
}


/**
 * @see bIsRunning
 * @see run()
 * @see start()
 * @see quit()
 * @return true if this operation is running, else false
 */
bool cThread::isRunning() const {
	
	return bIsRunning;
}


/**
 * @see bHasRun
 * @see bIsRunning
 * @see run()
 * @see start()
 * @see quit()
 * @return true the operation has run, else not
 */
bool cThread::hasRun() const {

	return bHasRun;
}


/**
 * @return the (class-)name of the operation
 */
string cThread::getName() const {
	return "cThread";
}


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
bool cThread::registerOperationRunStatusListener(
		iOperationStatusChange * pOperationRunStatusListener ) {
	
	if ( pOperationRunStatusListener == NULL ) {
		//can't insert non object
		return false;
	}
#ifdef CPP_2011
	mutexOperationStatusChange.lock();
	pair< set<iOperationStatusChange*>::iterator, bool > paInserted =
		setOperationListeners.insert( pOperationRunStatusListener );
	mutexOperationStatusChange.unlock();
#else  //CPP_2011
	pthread_mutex_lock( & mutexOperationStatusChange );
	pair< set<iOperationStatusChange*>::iterator, bool > paInserted =
		setOperationListeners.insert( pOperationRunStatusListener );
	pthread_mutex_unlock( & mutexOperationStatusChange );
#endif //CPP_2011

	return paInserted.second;
}


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
bool cThread::unregisterOperationRunStatusListener(
		iOperationStatusChange * pOperationRunStatusListener ) {
	
#ifdef CPP_2011
	mutexOperationStatusChange.lock();
	const size_t iElementsRemoved =
		setOperationListeners.erase( pOperationRunStatusListener );
	mutexOperationStatusChange.unlock();
#else  //CPP_2011
	pthread_mutex_lock( & mutexOperationStatusChange );
	const size_t iElementsRemoved =
		setOperationListeners.erase( pOperationRunStatusListener );
	pthread_mutex_unlock( & mutexOperationStatusChange );
#endif //CPP_2011
	return ( iElementsRemoved == 1);
}


/**
 * This method sets the value if this operation is running to the given
 * value. It notifies allso objects which listen to operation status
 * changes.
 *
 * @see isRunning
 * @param bInIsRunning the value, if this operation is running
 * @return the new value, if this operation is running
 */
bool cThread::setIsRunning( bool bInIsRunning ) {
	
#ifdef CPP_2011
	mutexRunStatusChange.lock();
#else  //CPP_2011
	pthread_mutex_lock( & mutexRunStatusChange );
#endif //CPP_2011
	if ( bIsRunning != bInIsRunning ) {
		//notify the listeners for operation status changes about the change
#ifdef CPP_2011
		mutexOperationStatusChange.lock();
#else  //CPP_2011
		pthread_mutex_lock( & mutexOperationStatusChange );
#endif //CPP_2011
		//copy the listener set, so it can't change while the listeners get notyfied
		set<iOperationStatusChange*> setOperationListenersTmp = setOperationListeners;
#ifdef CPP_2011
		mutexOperationStatusChange.unlock();
#else  //CPP_2011
		pthread_mutex_unlock( & mutexOperationStatusChange );
#endif //CPP_2011
		
		for ( set<iOperationStatusChange*>::iterator
				itrOpRunListener = setOperationListenersTmp.begin();
				itrOpRunListener != setOperationListenersTmp.end(); itrOpRunListener++ ) {
			
			(*itrOpRunListener)->operationStatusChange( this, bInIsRunning );
		}
		{//remember the operation runtime
			if ( bInIsRunning ) {
				//the operation was started
				tmLastCpuRunTime = clock();
			}else{//the operation was stoped
				dRunningTime += clock() - tmLastCpuRunTime;
				tmLastCpuRunTime = 0;
			}
		}
		if ( bIsRunning ) {
			//the operation is running but now ends it run
			bHasRun = true;
		}
		bIsRunning = bInIsRunning;
	}
#ifdef CPP_2011
	mutexRunStatusChange.unlock();
#else  //CPP_2011
	pthread_mutex_unlock( & mutexRunStatusChange );
#endif //CPP_2011
	return bIsRunning;
}


/**
 * @return A value for the cpu -time this operation runs.;
 * 	@see isRunning() gives back true for this time
 * @see dRunningTime
 */
double cThread::getCpuRunTime() const {
	
	if ( isRunning() ) {
		//reevalue the cpu time
		const clock_t tmNewLastCpuRunTime = clock();
		(const_cast<cThread*>(this))->dRunningTime +=
			tmNewLastCpuRunTime - tmLastCpuRunTime;
		(const_cast<cThread*>(this))->tmLastCpuRunTime = tmNewLastCpuRunTime;
	}
	return dRunningTime;
}


/**
 * This method runs the operation for a tread.
 * It will wait till the operation is ended befor returning.
 * It will simply call run().
 *
 * @see run()
 * @see start()
 * @see quit()
 */
void * cThread::runTread( void * inputArg ) {
	
#ifdef DEBUG
	lTreadsRunning++;
#endif
	
	((cThread*)(inputArg))->run();
	
#ifdef DEBUG
	lTreadsRunning--;
#endif

#ifndef CPP_2011
#ifndef WINDOWS
	//exit tread
	pthread_exit(NULL);
#endif //WINDOWS
#endif  //CPP_2011
	
	return NULL;
}


#ifndef CPP_2011
#ifdef WINDOWS
/**
 * Wraper function for windows.
 * Wait till the given mutex is free and than locks it.
 * @param pMutexHandle pointer to the mutex to lock.
 */
void cThread::pthread_mutex_lock( HANDLE * pMutexHandle ) {
	
	WaitForSingleObject( &pMutexHandle, INFINITE);
}

/**
 * Wraper function for windows.
 * Unlocks the given mutex.
 * @param pMutexHandle pointer to the mutex to unlock.
 */
void cThread::pthread_mutex_unlock( HANDLE * pMutexHandle ) {
	
	ReleaseMutex( &pMutexHandle );
}
#endif //WINDOWS
#endif  //CPP_2011


/**
 * This function sleeps for a short period.
 */
void cThread::shortSleep() {
	
	msleep( 10 );
}


/**
 * This function sleeps for some milli seconds.
 *
 * @param ulMilliSeconds the number of milli seconds to sleep
 */
void cThread::msleep( const unsigned long ulMilliSeconds ) {
	
#ifdef WINDOWS
	Sleep( ulMilliSeconds );
#else//WINDOWS
	static struct timespec timeToWait;
	timeToWait.tv_sec  = ( ulMilliSeconds / 1000 );
	timeToWait.tv_nsec = ( ulMilliSeconds % 1000 ) * 1000000L; //1000000L = 1 ms
	static struct timespec remainingTime;
	nanosleep( &timeToWait, &remainingTime );
#endif//WINDOWS
}







