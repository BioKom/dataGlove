
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
#include "cMessageGetIdToDataGlove.h"

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
 * @param iDataGloveStreamFileDescriptor the data glove file identifer
 * @return true if the data glove file could be closed, else false
 */
bool cDataGloveDGTechVHand::closeDataGloveFile( const int iDataGloveStreamFileDescriptor ) {
	
	if ( iDataGloveStreamFileDescriptor != -1 ) {
		//close file
		close( iDataGloveStreamFileDescriptor );
		return true;
	}//else no valid file descriptor
	return false;
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
	if ( iDataGloveFileDescriptor == -1 ) {
		return false;
	}
	
	
	//write the get ID message
	cMessageGetIdToDataGlove messageGetIdToDataGlove;
	const bool bMessageWritten =
		messageGetIdToDataGlove.writeMessage( iDataGloveFileDescriptor );
	
	if ( ! bMessageWritten ) {
		return false;
	}
	
/*	
	char getIDRequest[ 8 ];

	getIDRequest[ 0 ] = '$'; //0x24
	getIDRequest[ 1 ] = 0x0C;
	getIDRequest[ 2 ] = 0x02;   //num bytes
	getIDRequest[ 3 ] = 0x50;  //CRC
	getIDRequest[ 4 ] = '#';
	getIDRequest[ 5 ] = 0x0;
	
	write( iDataGloveFileDescriptor, &getIDRequest, 5 );
*/

	time_t tmEndTime = time( NULL ) + 3;
	
	unsigned int uiByte = 1;
	unsigned char cReaded = 0;
	unsigned int uiConvertedNumber = 0;
	printf( "Reading:\n" );
	while ( time( NULL ) < tmEndTime ) {
		// if new data is available on the serial port, print it out
		if ( read( iDataGloveFileDescriptor, &cReaded, 1 ) > 0 ) {
			uiConvertedNumber = cReaded;
			printf( "Byte %ud: '%1c'  0x%02X  %3u\n", uiByte, cReaded,
				uiConvertedNumber, uiConvertedNumber );
			uiByte++;
		}
	}
	printf( "done reading\n" );

	return closeDataGloveFile( iDataGloveFileDescriptor );
}



/**
 * This method tries to get the identifier data from the data glove.
 *
 * @see TDataGloveID
 * @param inPDataGloveStraem the pointer to the stream for the
 * 	communication with the data glove
 * @return the identifier data from the data glove
 */
TDataGloveID cDataGloveDGTechVHand::getDataGloveIDStatic(
		iostream * inPDataGloveStraem ) {
	//TODO
	
	TDataGloveID dataGloveID;
	dataGloveID.deviceType = TDataGloveID::NONE;
	if ( ( inPDataGloveStraem == NULL ) ||
			( ! inPDataGloveStraem->good() ) ) {
		//no good data stream -> can't send data
		return dataGloveID;
	}
	
	char getIDRequest[ 8 ];
	getIDRequest[ 0 ] = '$';
	getIDRequest[ 1 ] = 0x0C;
	getIDRequest[ 2 ] = 0x02;
	getIDRequest[ 3 ] = cMessageDataGlove::evalueCRC( getIDRequest, 2 );
	getIDRequest[ 4 ] = '#';
	getIDRequest[ 5 ] = 0x0;
	
	inPDataGloveStraem->write( getIDRequest, 5 );
	//inPDataGloveStraem->flush();
	shortSleep();
	if ( ! inPDataGloveStraem->good() ){
		perror( "write() of 'GET ID' request faild failed! - " );
		printf( "   %s%s%s\n", (inPDataGloveStraem->fail()?"":"fail bit set, "),
			(inPDataGloveStraem->bad()?"":"bad bit set, "),
			(inPDataGloveStraem->eof()?"":"eof bit set ")
		);
		return dataGloveID;
	}
	//TODO
	//get the returned message
	
	//
	return dataGloveID;
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



