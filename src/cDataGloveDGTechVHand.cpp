
//TODO check

/**
 * @file cDataGloveDGTechVHand
 * file name: cDataGloveDGTechVHand.cpp
 * @author Betti Oesterholz
 * @date 09.07.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file implements the class for the DGTech VHand data glove.
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
 * This file implements the class for the DGTech VHand data glove.
 *
 * Tested with:
 * 	*  DGTech VHand data glove 3.0 left handed
 *
 */
/*
History:
06.07.2014  Oesterholz  created
*/


#include "cDataGloveDGTechVHand.h"

#include <cstdio>
#include <ctime>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>


#include "cMessageDataGlove.h"
#include "cMessageFromDataGlove.h"
#include "cMessageGetIdToDataGlove.h"
#include "cMessageGetIdFromDataGlove.h"
#include "cMessageStartSamplingToDataGlove.h"
#include "cMessageStopSamplingToDataGlove.h"

//TODO rework



using namespace nDataGlove::nModelDataGloveDGTechVHand;
using namespace nDataGlove;
using namespace std;



/**
 * The standard constructor for the DGTech VHand data glove handler.
 *
 * @param inPDataGloveFile the pointer to the file name for the
 * 	communication with the data glove
 */
cDataGloveDGTechVHand::cDataGloveDGTechVHand( const char * inPDataGloveFile ) :
		iDataGloveStreamFileDescriptor( -1 ) {
	
	iDataGloveStreamFileDescriptor = openDataGloveFile( inPDataGloveFile );
	clearDataGloveStatic( iDataGloveStreamFileDescriptor );
}


/**
 * The destructor.
 */
cDataGloveDGTechVHand::~cDataGloveDGTechVHand() {
	
	closeDataGloveFile( iDataGloveStreamFileDescriptor );
	
}


/**
 * @return the name of this class "cDataGloveDGTechVHand"
 */
std::string cDataGloveDGTechVHand::getName() const {
	
	return string( "cDataGloveDGTechVHand" );
}


/**
 * This method opens a file to communicate with the data glove.
 *
 * @see closeDataGloveFile()
 * @param inPDataGloveFile the pointer to the file name for the
 * 	communication with the data glove
 * @return the data glove file identifer or -1 if the data glove file could
 * 	not be opened
 */
int cDataGloveDGTechVHand::openDataGloveFile( const char * inPDataGloveFile ) {

	struct termios termiosPortParameter;

	memset( &termiosPortParameter, 0, sizeof(termiosPortParameter) );
	termiosPortParameter.c_oflag &= ~OPOST;  //?raw output
	termiosPortParameter.c_cflag &= ~PARENB; //no parity
	termiosPortParameter.c_cflag &= ~CSTOPB; //1 stop bit
	termiosPortParameter.c_cflag=CS8 | CREAD | CLOCAL; // 8n1, see termios.h for more informatermiosPortParametern
	cfsetospeed( &termiosPortParameter, B115200 );     // 115200 baud
	
	const int iDataGloveFileDescriptor = open( inPDataGloveFile , O_RDWR | O_NONBLOCK );
	if ( iDataGloveFileDescriptor == -1 ) {
		return false;
	}
	
	tcsetattr( iDataGloveFileDescriptor, TCSANOW, &termiosPortParameter );
	
	return iDataGloveFileDescriptor;
}


/**
 * This method closes a file to communicate with the data glove.
 *
 * @see closeDataGloveFile()
 * @param iDataGloveFileDescriptor the data glove file identifer
 * @return true if the data glove file could be closed, else false
 */
bool cDataGloveDGTechVHand::closeDataGloveFile( const int iDataGloveFileDescriptor ) {
	
	if ( iDataGloveFileDescriptor != -1 ) {
		//close file
		clearDataGloveStatic( iDataGloveFileDescriptor );
		close( iDataGloveFileDescriptor );
		return true;
	}//else no valid file descriptor
	return false;
}


/**
 * This method clears the (file to communicate with) the data glove.
 * This includes:
 * 	* if running the sampling is stoped
 * 	* all to read Data is readed
 *
 * @see iDataGloveStreamFileDescriptor
 * @see openDataGloveFile()
 * @see closeDataGloveFile()
 * @return true if the data glove file could be cleard, else false
 */
bool cDataGloveDGTechVHand::clearDataGlove()  {
	
	return clearDataGloveStatic( iDataGloveStreamFileDescriptor );
}


/**
 * This method clears the (file to communicate with) the data glove.
 * This includes:
 * 	* if running the sampling is stoped
 * 	* all to read Data is readed
 *
 * @see openDataGloveFile()
 * @see closeDataGloveFile()
 * @param iDataGloveFileDescriptor the data glove file identifer
 * @return true if the data glove file could be cleard, else false
 */
bool cDataGloveDGTechVHand::clearDataGloveStatic(
		const int iDataGloveFileDescriptor ) {
	
	if ( iDataGloveFileDescriptor <= 0 ) {
		return false;
	}
	unsigned char cReaded = 0;
	if ( 0 < read( iDataGloveFileDescriptor, &cReaded, 1 ) ) {
		//data to read -> stop sampling + try read all data
		//write the stop sampling message
		cMessageStopSamplingToDataGlove messageStopSamplingToDataGlove;
		const bool bMessageWritten =
			messageStopSamplingToDataGlove.writeMessage( iDataGloveFileDescriptor );
		
		if ( ! bMessageWritten ) {
			return false;
		}
		
		time_t tmEndTime = time( NULL ) + 3;  //read maximal 3 seconds
		
		while ( time( NULL ) < tmEndTime ) {
			// if new data is available on the serial port, print it out
			if ( read( iDataGloveFileDescriptor, &cReaded, 1 ) == 0 ) {
				//all data read
				return true;
			}
		}
		return false;  //not all data read
	}
	//no data to read -> data glove file cleared
	return true;
}


/**
 * This method checks if the file is for a valid and live data glove.
 *
 * @see TDataGloveID
 * @param inPDataGloveFile the pointer to the file name for the
 * 	communication with the data glove
 * @return true if the file is connected to a valid and live DGTech
 * 	VHand data glove, else false
 */
bool cDataGloveDGTechVHand::isLiveDataGlove( const char * inPDataGloveFile ) {
	
	int iDataGloveFileDescriptor = openDataGloveFile( inPDataGloveFile );
	if ( iDataGloveFileDescriptor <= 0 ) {
		return false;
	}
	if ( ! clearDataGloveStatic( iDataGloveFileDescriptor ) ) {
		return false;  //data glove can not be cleared
	}
	
	//write the get ID message
	cMessageGetIdToDataGlove messageGetIdToDataGlove;
	const bool bMessageWritten =
		messageGetIdToDataGlove.writeMessage( iDataGloveFileDescriptor );
	
	if ( ! bMessageWritten ) {
		return false;
	}
	
	//read the result message
	const cMessageFromDataGlove * pResultMessage =
		cMessageDataGlove::readMessage( iDataGloveFileDescriptor, 3000, false );
	
	if ( pResultMessage == 0 ) {
		//no result could be read -> invalid data glove file
		return false;
	}
	if ( pResultMessage->getType() != DATA_GLOVE_D_G_TECH_V_HAND__CMD_GET_ID ) {
		//wrong message read -> invalid data glove file
		delete pResultMessage;
		return false;
	}
	delete pResultMessage;

	return closeDataGloveFile( iDataGloveFileDescriptor );
}



/**
 * This method tries to get the identifier data from the data glove.
 *
 * @see TDataGloveID
 * @param iDataGloveFileDescriptor the data glove file identifer for
 * 	the communication with the data glove
 * @return the identifier data message from the data glove (please delete it),
 * 	or NULL if non could be loaded
 */
cMessageGetIdFromDataGlove * cDataGloveDGTechVHand::getDataGloveIDStatic(
		const int iDataGloveFileDescriptor ) {
	
	if ( iDataGloveFileDescriptor <= 0 ) {
		return NULL;
	}
	if ( ! clearDataGloveStatic( iDataGloveFileDescriptor ) ) {
		return NULL;  //data glove can not be cleared
	}
	
	//write the get ID message
	cMessageGetIdToDataGlove messageGetIdToDataGlove;
	const bool bMessageWritten =
		messageGetIdToDataGlove.writeMessage( iDataGloveFileDescriptor );
	
	if ( ! bMessageWritten ) {
		return NULL;
	}
	
	//read the result message
	const cMessageFromDataGlove * pResultMessage =
		cMessageDataGlove::readMessage( iDataGloveFileDescriptor, 3000, false );
	
	if ( pResultMessage == 0 ) {
		//no result could be read -> invalid data glove file
		return NULL;
	}
	if ( ( pResultMessage->getType() != DATA_GLOVE_D_G_TECH_V_HAND__CMD_GET_ID ) ||
			( pResultMessage->getName() != "cMessageGetIdFromDataGlove" ) ) {
		//wrong message read -> invalid data glove file
		delete pResultMessage;
		return NULL;
	}
	return ((cMessageGetIdFromDataGlove*)(pResultMessage));
}


/**
 * This method tries to get the identifier data from the data glove.
 *
 * @see iDataGloveStreamFileDescriptor
 * @return the identifier data message from the data glove (please delete it),
 * 	or NULL if non could be loaded
 */
cMessageGetIdFromDataGlove * cDataGloveDGTechVHand::getDataGloveID() {
	
	return getDataGloveIDStatic( iDataGloveStreamFileDescriptor );
}


/**
 * This method tries to start the sampling of data from the data glove.
 *
 * @see iDataGloveStreamFileDescriptor
 * @param iSamplingType a number for the Sampling package format
 * 	possible values are:
 * 		0: stop comunicating
 * 		1: send quaternion orientation and finger sensors values
 * 		2: send only quaternion values
 * 		3: send raw gyroscope data, raw accelerometer data,
 * 			raw magnetometer data and finger sensor values
 * 		4: send only raw data
 * 		5: send only finger data
 * @return true if the start sampling was successful, else false
 */
bool cDataGloveDGTechVHand::startSampling( const int iSamplingType ) {
	
	return startSamplingStatic( iDataGloveStreamFileDescriptor, iSamplingType );
}


/**
 * This method tries to start the sampling of data from the data glove.
 *
 * @param iDataGloveFileDescriptor the data glove file identifer for
 * 	the communication with the data glove
 * @param iSamplingType a number for the Sampling package format
 * 	possible values are:
 * 		0: stop comunicating
 * 		1: send quaternion orientation and finger sensors values
 * 		2: send only quaternion values
 * 		3: send raw gyroscope data, raw accelerometer data,
 * 			raw magnetometer data and finger sensor values
 * 		4: send only raw data
 * 		5: send only finger data
 * @return true if the start sampling was successful, else false
 */
bool cDataGloveDGTechVHand::startSamplingStatic(
		const int iDataGloveFileDescriptor,
		const int iSamplingType ) {
	
	if ( iDataGloveFileDescriptor <= 0 ) {
		return NULL;
	}
	
	//write the start sampling message
	cMessageStartSamplingToDataGlove messageStartSamplingToDataGlove( iSamplingType );
	const bool bMessageWritten =
		messageStartSamplingToDataGlove.writeMessage( iDataGloveFileDescriptor );
	
	if ( ! bMessageWritten ) {
		return false;
	}
	
	return true;
}


/**
 * This method tries to stop the sampling of data from the data glove.
 *
 * @see iDataGloveStreamFileDescriptor
 * @return true if the stop sampling was successful, else false
 */
bool cDataGloveDGTechVHand::stopSampling() {
	
	return stopSamplingStatic( iDataGloveStreamFileDescriptor );
}


/**
 * This method tries to stop the sampling of data from the data glove.
 *
 * @param iDataGloveFileDescriptor the data glove file identifer for
 * 	the communication with the data glove
 * @return true if the stop sampling was successful, else false
 */
bool cDataGloveDGTechVHand::stopSamplingStatic(
		const int iDataGloveFileDescriptor ) {
	
	if ( iDataGloveFileDescriptor <= 0 ) {
		return NULL;
	}
	
	//write the start sampling message
	cMessageStopSamplingToDataGlove messageStopSamplingToDataGlove;
	const bool bMessageWritten =
		messageStopSamplingToDataGlove.writeMessage( iDataGloveFileDescriptor );
	
	if ( ! bMessageWritten ) {
		return false;
	}
	
	return true;
}



/**
 * Reads the data form the data glove and writes it to standard output
 * (printf) for debugging purpose.
 *
 * @param iSeconds the seconds input data should be read
 */
void cDataGloveDGTechVHand::debugReadData( const int iSeconds ) {
	
	debugReadDataStatic( iDataGloveStreamFileDescriptor, iSeconds );
}


/**
 * Reads the data and writes it to standard output (printf) for debugging
 * purpose.
 *
 * @param inDataGloveFile the pointer to the file name for the
 * 	communication with the data glove
 * @param iSeconds the seconds input data should be read
 */
void cDataGloveDGTechVHand::debugReadDataStatic(
		const int inDataGloveFile, const int iSeconds ) {
	
	if ( inDataGloveFile <= 0 ) {
		return;
	}
	time_t tmEndTime = time( NULL ) + iSeconds;
	
	unsigned int uiByte = 1;
	unsigned char cReaded = 0;
	unsigned int uiConvertedNumber = 0;
	printf( "Reading:\n" );
	while ( time( NULL ) < tmEndTime ) {
		// if new data is available on the serial port, print it out
		if ( read( inDataGloveFile, &cReaded, 1 ) > 0 ) {
			uiConvertedNumber = cReaded;
			printf( "Byte %u: '%1c'  0x%02X  %3u\n", uiByte, cReaded,
				uiConvertedNumber, uiConvertedNumber );
			uiByte++;
		}
	}
	printf( "done reading\n" );
}




/**
 * Sleeps a smaal time. (If you want to wait for more data.)
 */
void cDataGloveDGTechVHand::shortSleep() {
	
#ifdef WINDOWS
	Sleep( 10 ); //= 10 ms
#else//WINDOWS
	static struct timespec timeToWait;
	timeToWait.tv_sec  = 0;
	timeToWait.tv_nsec = 10000000L; //= 10 ms
	static struct timespec remainingTime;
	nanosleep( &timeToWait, &remainingTime );
#endif//WINDOWS
}



