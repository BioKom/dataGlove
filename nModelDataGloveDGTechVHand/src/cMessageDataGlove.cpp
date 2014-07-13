
//TODO check

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
*/


#include "cMessageDataGlove.h"

#include <cstdlib>
#include <unistd.h>


#include "cMessageFromDataGlove.h"



using namespace nDataGlove::nModelDataGloveDGTechVHand;
using namespace std;



/**
 * The standard constructor for the DGTech VHand data glove message.
 */
cMessageDataGlove::cMessageDataGlove():
		cType( DATA_GLOVE_D_G_TECH_V_HAND__UNKNOWN ), szMessage( NULL ),
		uiMessageSize( 0 ) {
	
	//nothing to do
}


/**
 * The destructor.
 */
cMessageDataGlove::~cMessageDataGlove() {
	
	if ( szMessage != NULL ) {
		delete szMessage;
	}
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
 * @return the data glove message read (please delete after usage),
 * 	or Null, if non could be read
 */
cMessageFromDataGlove * cMessageDataGlove::readMessage(
	const int iDataGloveFileDescriptor,
		const unsigned int uiMsTimeout,
		const bool bHeaderRead ) {
	
	if ( iDataGloveFileDescriptor == -1 ) {
		//no valid file -> can't read the message
		return NULL;
	}
	//TODO change to milli secounds
	const time_t tmEndTime = time( NULL ) + (
		( uiMsTimeout == 0 ) ? 0 : (uiMsTimeout / 1000) );
	
	char charReaded = 0;
	
	int iReadStatus = 0;
	if ( ! bHeaderRead ) {
		//read the header
		do {// if new data is available on the serial port, read it
			iReadStatus = read( iDataGloveFileDescriptor, &charReaded, 1 );
			if ( iReadStatus == - 1 ) {
				//Error occured while reading
				return NULL;
			}//else
			if ( 0 < iReadStatus ) {
				if ( charReaded != DATA_GLOVE_D_G_TECH_V_HAND__HEADER ) {
					//no valid header
					return NULL;
				}
				break;
			}
		} while ( time( NULL ) < tmEndTime );
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
			return NULL;
		}//else
		if ( 0 < iReadStatus ) {
			break;
		}
	} while ( time( NULL ) < tmEndTime );
	//TODO check if cCommandChar is valid
	//if ( cCommandChar != )
	
	//read size char
	do {// if new data is available on the serial port, read it
		iReadStatus = read( iDataGloveFileDescriptor, &charReaded, 1 );
		if ( iReadStatus == - 1 ) {
			//Error occured while reading
			return NULL;
		}//else
		if ( 0 < iReadStatus ) {
			break;
		}
	} while ( time( NULL ) < tmEndTime );
	unsigned int uiBytesToRead = static_cast<unsigned int>((unsigned char)(
		charReaded));
	if ( uiBytesToRead < 2 ) {
		//CRC and endchar should be still to read
		//-> one or more are missing -> no valid message
		return NULL;
	}
	unsigned int uiReadedMessageSize = uiBytesToRead + 2;
	char * szReadedMessage = static_cast<char *>(
		malloc( uiReadedMessageSize + 2 ) );
	szReadedMessage[ 0 ] = DATA_GLOVE_D_G_TECH_V_HAND__HEADER;
	szReadedMessage[ 1 ] = cCommandChar;
	szReadedMessage[ 2 ] = charReaded;
	szReadedMessage[ uiReadedMessageSize ] = 0x0;  //add Null character behind message
	unsigned int uiByte = 3;
	uiBytesToRead -= 1;
	
	do {// if new data is available on the serial port, read it
		if ( uiBytesToRead == 0 ) {
			break;  //entire message read
		}
		iReadStatus = read( iDataGloveFileDescriptor, &( szReadedMessage[ uiByte ] ),
			uiBytesToRead );
		if ( iReadStatus == - 1 ) {
			//Error occured while reading
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
		}
	} while ( time( NULL ) < tmEndTime );
	
	
#else //FEATURE_FAST_DATA_GLOVE_MESSAGE_READ
	
	//allocate memory for the message buffer
	unsigned int uiByte = 0;
	unsigned int uiMaxSize = 64;
	unsigned int szReadedMessage = malloc( uiMaxSize );
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
	const char cCommandChar = szReadedMessage[ 1 ];
	unsigned int uiReadedMessageSize = uiByte;
#endif //FEATURE_FAST_DATA_GLOVE_MESSAGE_READ
	
	if ( ( uiByte < 5 ) ||
			( szReadedMessage[ uiByte - 1 ] != DATA_GLOVE_D_G_TECH_V_HAND__ENDCHAR ) ||
			( szReadedMessage[ uiByte - 2 ] != evalueCRC( szReadedMessage, uiReadedMessageSize ) ) ) {
		//no valid message readed
		delete szReadedMessage;
		szReadedMessage = NULL;
		uiReadedMessageSize = 0;
		return NULL;
	}
	//TODO create the correct message
	
	cMessageFromDataGlove * pReadedMessageFromDataGlove = NULL;
	
	//TODO
	
	if ( pReadedMessageFromDataGlove != NULL ) {
		//set data for created message
		pReadedMessageFromDataGlove->uiMessageSize = uiReadedMessageSize;
		pReadedMessageFromDataGlove->szMessage = szReadedMessage;
		pReadedMessageFromDataGlove->cType = cCommandChar;
	} else {  //no correct message could be created
		delete szReadedMessage;
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
 * @return the type character of this message
 * 	@see cType
 */
char cMessageDataGlove::getType() const {
	
	return cType;
}


/**
 * @return a pointer to the message/package buffer, with the message,
 * 	or NULL if non exists.
 * 	@see szMessage
 */
char * cMessageDataGlove::getMessage() {
	
	return szMessage;
}


/**
 * @return a pointer to the message/package buffer, with the message,
 * 	or NULL if non exists.
 * 	@see szMessage
 */
const char * cMessageDataGlove::getMessage() const {
	
	return szMessage;
}


/**
 * @return the number of bytes/characters in the message szMessage
 * 	@see uiMessageSize
 * 	@see szMessage
 */
unsigned int cMessageDataGlove::getMessageSize() const {
	
	return uiMessageSize;
}


/**
 * @param pData the data, for which the CRC is to evaluate
 * @param iDataLength the number of bytes of the data, for which the
 * 	CRC is to evaluate
 * @return the CRC for the given data (sum of the bytes modulo 256)
 */
char cMessageDataGlove::evalueCRC( const unsigned char * pData,
		const int iDataLength ) {
	
	unsigned char CRC = 0;
	for ( int iActualByte = 0; iActualByte < iDataLength;
			iActualByte++ ) {
		
		CRC += pData[ iActualByte ];
	}
	
	return CRC;
}


/**
 * @param pData the data, for which the CRC is to evaluate
 * @param iDataLength the number of bytes of the data, for which the
 * 	CRC is to evaluate
 * @return the CRC for the given data (sum of the bytes modulo 256)
 */
char cMessageDataGlove::evalueCRC( const char * pData, const int iDataLength ) {
	
	unsigned char CRC = 0;
	const unsigned char * pUcData = (unsigned char *)(pData);
	for ( int iActualByte = 0; iActualByte < iDataLength;
			iActualByte++ ) {
		
		CRC += pUcData[ iActualByte ];
	}
	
	return CRC;
}




