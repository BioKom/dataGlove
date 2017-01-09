/**
 * @file cMessageDataGlove
 * file name: cMessageDataGlove.cpp
 * @author Betti Oesterholz
 * @date 11.07.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file implements the parent class for all DGTech VHand data glove messages.
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
 * This file implements the parent class for all DGTech VHand data glove messages.
 * All Messages which the  DGTech VHand data glove can send or receive
 * should inherit this class.
 * See DG5 VHand 3.0 OEM Technical Datasheet.
 * @see www.dg-tech.it
 * @see www.dg-tech.it/vhand3/
 *
 * Tested with:
 * 	*  DGTech VHand data glove 3.0 left handed
 *
 */
/*
History:
11.07.2014  Oesterholz  created
04.02.2016  Oesterholz  copy constructor and clone() added
*/


//switches for test proposes:
//#define DEBUG

//switch for debugging:
//#define DEBUG_PRINT_MESSAGE( MSG, NUM_BYTE ) printMessage( MSG, NUM_BYTE )
#define DEBUG_PRINT_MESSAGE( MSG, NUM_BYTE )


#include "cMessageDataGlove.h"

#include <cstdlib>
#include <cstdio>  //printf()
#include <unistd.h>


#include "cMessageFromDataGlove.h"
#include "cMessageGetIdFromDataGlove.h"
#include "cMessageSamplingDataFromDataGlove.h"



using namespace nDataGlove::nModelDataGloveDGTechVHand;
using namespace std;


/**
 * The standard constructor for the DGTech VHand data glove message.
 */
cMessageDataGlove::cMessageDataGlove():
		type( UNKNOWN ), cCommand( DATA_GLOVE_D_G_TECH_V_HAND__UNKNOWN ),
		szMessage( NULL ),
		uiMessageSize( 0 ) {
	//nothing to do
}


/**
 * The copy constructor for the DGTech VHand data glove message.
 *
 * @param inMessageDataGlove the message to copy
 */
cMessageDataGlove::cMessageDataGlove( const cMessageDataGlove & inMessageDataGlove ):
		type( UNKNOWN ), cCommand( DATA_GLOVE_D_G_TECH_V_HAND__UNKNOWN ),
		szMessage( NULL ),
		uiMessageSize( 0 ) {
	
	copyData( inMessageDataGlove );
}


/**
 * The destructor.
 */
cMessageDataGlove::~cMessageDataGlove() {
	
	if ( szMessage != NULL ) {
		delete [] szMessage;
	}
}


/**
 * Clones this object.
 *
 * @return the clone of this object
 */
cMessageDataGlove * cMessageDataGlove::clone() const {
	
	return new cMessageDataGlove( *this );
}


/**
 * @return the name of this class "cMessageDataGlove"
 */
std::string cMessageDataGlove::getName() const {
	
	return string( "cMessageDataGlove" );
}


/**
 * This method reads a message from the file/port with the given data
 * glove file identifer.
 *
 * @see closeDataGloveFile()
 * @param iDataGloveFileDescriptor the data glove file identifer
 * @param uiMsTimeout milli seconds to wait for a new message
 * @param bHeaderRead if true the header was read from the stream, else not
 * @param bReadTillNextHeader if the first character in the file is not
 * 	a header character, try to read till the next header
 * 	just works with ( bHeaderRead = false )
 * @return the data glove message read (please delete after usage),
 * 	or NULL, if non could be read
 */
cMessageFromDataGlove * cMessageDataGlove::readMessage(
	const int iDataGloveFileDescriptor,
		const unsigned int uiMsTimeout,
		const bool bHeaderRead,
		const bool bReadTillNextHeader ) {
	
	DEBUG_OUT_L2(<<"cMessageDataGlove::readMessage( iDataGloveFileDescriptor="<<iDataGloveFileDescriptor<<", uiMsTimeout="<<uiMsTimeout<<", bHeaderRead="<<(bHeaderRead?"yes":"no")<<", bReadTillNextHeader="<<(bReadTillNextHeader?"yes":"no")<<" ) called"<<endl<<flush);
	if ( iDataGloveFileDescriptor == -1 ) {
		//no valid file -> can't read the message
		DEBUG_OUT_L2(<<"   not a valid file descriptor"<<endl<<flush);
		return NULL;
	}
	//TODO change to milli secounds
	const time_t tmEndTime = time( NULL ) + (
		( uiMsTimeout == 0 ) ? 0 : (uiMsTimeout / 1000) );
	
	unsigned char charReaded = 0;
	
	int iReadStatus = 0;
	if ( ! bHeaderRead ) {
		//read the header
		do {// if new data is available on the serial port, read it
			iReadStatus = read( iDataGloveFileDescriptor, &charReaded, 1 );
			if ( iReadStatus == - 1 ) {
				//Error occured while reading
				DEBUG_OUT_L2(<<"   Error occured while reading"<<endl<<flush);
				return NULL;
			}//else
			if ( ( 0 < iReadStatus ) &&
					( ( ! bReadTillNextHeader ) ||
						( charReaded == DATA_GLOVE_D_G_TECH_V_HAND__HEADER ) ) ) {
				//if char read and it should not read till the next header or
				//the char is the header char -> done
				break;
			}
		} while ( time( NULL ) < tmEndTime );
		
		if ( charReaded != DATA_GLOVE_D_G_TECH_V_HAND__HEADER ) {
			//no valid header
#ifdef DEBUG
			if ( 0 < iReadStatus ) {
				DEBUG_OUT_L4(<<"   Error no valid header (readed: "<<((int)(charReaded))<<"; last bytes readed: "<<iReadStatus<<" )"<<endl<<flush);
			}else if ( iReadStatus < 0 ) {
				DEBUG_OUT_L4(<<"    Error occured while reading header (readed: "<<((int)(charReaded))<<"; last bytes readed: "<<iReadStatus<<" )"<<endl<<flush);
			}/* else {
				DEBUG_OUT_L4(<<"    Error while reading header nothing to read (readed: "<<((int)(charReaded))<<"; last bytes readed: "<<iReadStatus<<" )"<<endl<<flush);
			}*/
#endif //DEBUG
			return NULL;
		}
	}
#define FEATURE_FAST_DATA_GLOVE_MESSAGE_READ
#ifdef FEATURE_FAST_DATA_GLOVE_MESSAGE_READ
	//read first tree characters + read number of bytes in message
	
	//read command char
	char cCommandChar = 0x0;
	do {// if new data is available on the serial port, read it
		iReadStatus = read( iDataGloveFileDescriptor, &cCommandChar, 1 );
		if ( iReadStatus == - 1 ) {
			//Error occured while reading
			DEBUG_OUT_L2(<<"   Error occured while reading"<<endl<<flush);
			return NULL;
		}//else
		if ( 0 < iReadStatus ) {
			break;
		}
	} while ( time( NULL ) < tmEndTime );
	//check if cCommandChar is valid
	if ( ( cCommandChar != DATA_GLOVE_D_G_TECH_V_HAND__CMD_GET_ID ) &&
			( cCommandChar != DATA_GLOVE_D_G_TECH_V_HAND__CMD_START_SAMPLING )
		//TODO more cCommandChar types
		) {
		//no valid from message
		DEBUG_OUT_L2(<<"   no valid from message"<<endl<<flush);
		return NULL;
	}
	
	//read size char
	do {// if new data is available on the serial port, read it
		iReadStatus = read( iDataGloveFileDescriptor, &charReaded, 1 );
		if ( iReadStatus == - 1 ) {
			//Error occured while reading
			DEBUG_OUT_L2(<<"   Error occured while reading"<<endl<<flush);
			return NULL;
		}//else
		if ( 0 < iReadStatus ) {
			break;
		}
	} while ( time( NULL ) < tmEndTime );
	unsigned int uiBytesToRead = static_cast<unsigned int>(charReaded);
	if ( uiBytesToRead < 2 ) {
		//CRC and endchar should be still to read
		//-> one or more are missing -> no valid message
		DEBUG_OUT_L2(<<"   Error: "<<uiBytesToRead<<" Bytes to read to low, CRC and endchar should be still to read"<<endl<<flush);
		return NULL;
	}
	unsigned int uiReadedMessageSize = uiBytesToRead + 3;
	unsigned char * szReadedMessage = new unsigned char[ uiReadedMessageSize + 2 ];
	szReadedMessage[ 0 ] = DATA_GLOVE_D_G_TECH_V_HAND__HEADER;
	szReadedMessage[ 1 ] = cCommandChar;
	szReadedMessage[ 2 ] = charReaded;
	szReadedMessage[ uiReadedMessageSize ] = 0x0;  //add Null character behind message
	unsigned int uiByte = 3;  //index of the next to read byte
	//uiBytesToRead -= 1;
	
	DEBUG_OUT_L2(<<"   Reading remaining "<<uiBytesToRead<<" byte of message with size "<<uiReadedMessageSize<<endl<<flush);
	
	while ( true ) {// if new data is available on the serial port, read it
		if ( uiBytesToRead == 0 ) {
			break;  //entire message read
		}
		iReadStatus = read( iDataGloveFileDescriptor, &( szReadedMessage[ uiByte ] ),
			uiBytesToRead );
		if ( iReadStatus == - 1 ) {
			//Error occured while reading
			DEBUG_OUT_L2(<<"   Error occured while reading"<<endl<<flush);
			return NULL;
		}//else
		/*if ( uiBytesToRead < iReadStatus ) {
			//Error to much bytes read
			return NULL;
		}*/
		if ( 0 < iReadStatus ) {
			uiByte += iReadStatus;
			uiBytesToRead -= iReadStatus;
			
			continue;
		}//else ( 0 == iReadStatus ) -> no bytes read
		if ( tmEndTime <= time( NULL ) ) {
			//times is up
			break;
		}
		shortSleep();
	}
	
#else //FEATURE_FAST_DATA_GLOVE_MESSAGE_READ
	
	//allocate memory for the message buffer
	unsigned int uiByte = 0;
	unsigned int uiMaxSize = 64;
	unsigned char szReadedMessage = new unsigned char[ uiMaxSize ];
	szReadedMessage[ uiByte ] = DATA_GLOVE_D_G_TECH_V_HAND__HEADER;
	uiByte++;
	
	int iReadStatus = 0;
	//read the message
	while ( true ) {// if new data is available on the port, read it
		iReadStatus = read( iDataGloveFileDescriptor, &charReaded, 1 );
		if ( iReadStatus == - 1 ) {
			//Error occured while reading
			break;
		}//else
		if ( 0 < iReadStatus ) {
			if ( uiMaxSize <= uiByte ) {
				//dynamic increase the message size
				uiMaxSize *= 2;  //double size
				char * szTmpMessage = malloc( uiMaxSize );
				memcpy( szTmpMessage, szReadedMessage, uiMaxSize / 2 );
				delete szReadedMessage;  //free old memory
				szReadedMessage = szTmpMessage;
			}
			//store message character
			szReadedMessage[ uiByte ] = charReaded;
			uiByte++;
			
			continue;
		}
		if ( tmEndTime <= time( NULL ) ) {
			break;  //done;
		}
	};
	const unsigned char  cCommandChar = szReadedMessage[ 1 ];
	unsigned int uiReadedMessageSize = uiByte;
#endif //FEATURE_FAST_DATA_GLOVE_MESSAGE_READ
	
	if ( ( uiByte < 5 ) ||
			( szReadedMessage[ uiByte - 1 ] != DATA_GLOVE_D_G_TECH_V_HAND__ENDCHAR ) ||
			( szReadedMessage[ uiByte - 2 ] != evalueCRC( szReadedMessage, uiReadedMessageSize - 2 ) ) ) {
		//no valid message readed
		DEBUG_OUT_L2(<<"   no valid message readed ("<<uiByte<<" byte read, end char '"<<szReadedMessage[ uiByte - 1 ]<<"', CRC read "<<((int)(szReadedMessage[ uiByte - 2 ]))<<" CRC correct "<<((int)(evalueCRC( szReadedMessage, uiReadedMessageSize - 2 ) ))<<" )"<<endl<<flush);
		
		DEBUG_PRINT_MESSAGE( szReadedMessage, uiReadedMessageSize );
		
		delete [] szReadedMessage;
		szReadedMessage = NULL;
		uiReadedMessageSize = 0;
		return NULL;
	}
	//create the correct message
	cMessageFromDataGlove * pReadedMessageFromDataGlove = NULL;
	switch ( cCommandChar ) {
		case DATA_GLOVE_D_G_TECH_V_HAND__CMD_GET_ID: {
			DEBUG_OUT_L2(<<"   cMessageGetIdFromDataGlove readed"<<endl<<flush);
			pReadedMessageFromDataGlove = new cMessageGetIdFromDataGlove();
		}; break;
		case DATA_GLOVE_D_G_TECH_V_HAND__CMD_START_SAMPLING: {
			DEBUG_OUT_L2(<<"   cMessageSamplingDataFromDataGlove readed"<<endl<<flush);
			pReadedMessageFromDataGlove = new cMessageSamplingDataFromDataGlove();
		}; break;
		//TODO more message types
		
	};  //end switch ( cCommandChar )
	
	if ( pReadedMessageFromDataGlove != NULL ) {
		//set data for created message
		pReadedMessageFromDataGlove->uiMessageSize = uiReadedMessageSize;
		if ( pReadedMessageFromDataGlove->szMessage != NULL ) {
			//delete the old message
			delete (pReadedMessageFromDataGlove->szMessage);
		}
		pReadedMessageFromDataGlove->szMessage = szReadedMessage;
	} else {  //no correct message could be created
		DEBUG_OUT_L2(<<"   no correct message could be created"<<endl<<flush);
		delete [] szReadedMessage;
		szReadedMessage = NULL;
		uiReadedMessageSize = 0;
		return NULL;
	}
	
	return pReadedMessageFromDataGlove;
}


/**
 * This method writes this message to the file/port with the given data
 * glove file identifer.
 *
 * @see readMessage()
 * @param iDataGloveFileDescriptor the data glove file identifer
 * @return true if the message was writen, else false
 */
bool cMessageDataGlove::writeMessage( const int iDataGloveFileDescriptor ) {
	
	if ( ( iDataGloveFileDescriptor == -1 ) ||
			( szMessage == NULL ) || ( uiMessageSize == 0 ) ) {
		//can't write the message
		return false;
	}
	if ( write( iDataGloveFileDescriptor, szMessage, uiMessageSize ) !=
			uiMessageSize ) {
		//error while writing data
		return false;
	}
	
	return true;
}


/**
 * Prints the given message.
 *
 * @param szMessage a pointer to the message to print
 * @param uiNumberOfByte the number of bytes to print from te message
 */
void cMessageDataGlove::printMessage( const unsigned char * szMessage,
		unsigned int uiNumberOfByte ) {
	
	printf( "\nPrinting message with %u bytes:\n", uiNumberOfByte );
	printf( "message: \"" );
	for ( unsigned int index = 0; index < uiNumberOfByte; ++index ) {
		printf( "%u", szMessage[ index ] );
	}
	printf( "\"\nhex: \"" );
	unsigned int uiConvertedNumber = 0;
	for ( unsigned int index = 0; index < uiNumberOfByte; ++index ) {
		uiConvertedNumber = szMessage[ index ];
		printf( "0x%02X", uiConvertedNumber );
	}
	printf( "\"\nhex2: \"" );
	for ( unsigned int index = 0; index < uiNumberOfByte; ++index ) {
		uiConvertedNumber = szMessage[ index ];
		printf( "%02X", uiConvertedNumber );
	}
	printf( "\"\n" );
}


/**
 * The copies the data of the DGTech VHand data glove message into this
 * message.
 *
 * @see type
 * @see cCommand
 * @see szMessage
 * @see uiMessageSize
 * @param inMessageDataGlove the message from which to copy the data
 */
void cMessageDataGlove::copyData( const cMessageDataGlove & inMessageDataGlove  ) {
	
	type = inMessageDataGlove.type;
	cCommand = inMessageDataGlove.cCommand;
	uiMessageSize = inMessageDataGlove.uiMessageSize;
	szMessage = new unsigned char[ uiMessageSize + 2 ];
	memcpy( szMessage, inMessageDataGlove.szMessage, uiMessageSize + 1 );
}


