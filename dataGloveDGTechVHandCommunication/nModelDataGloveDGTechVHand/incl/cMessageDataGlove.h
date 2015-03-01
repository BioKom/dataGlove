/**
 * @file cMessageDataGlove
 * file name: cMessageDataGlove.h
 * @author Betti Oesterholz
 * @date 11.07.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies the parent class for all DGTech VHand data glove messages.
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
 * This file specifies the parent class for all DGTech VHand data glove messages.
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


#ifndef ___N_DATA_GLOVE__N_MODEL_DATA_GLOVE_D_G_TECH_V_HAND__C_MESSAGE_DATA_GLOVE_H__
#define ___N_DATA_GLOVE__N_MODEL_DATA_GLOVE_D_G_TECH_V_HAND__C_MESSAGE_DATA_GLOVE_H__


#include "version.h"

#include <string>


namespace nDataGlove{

namespace nModelDataGloveDGTechVHand{

//forward declarations
class cMessageFromDataGlove;

#define DATA_GLOVE_D_G_TECH_V_HAND__HEADER '$'
#define DATA_GLOVE_D_G_TECH_V_HAND__ENDCHAR '#'

/**
 * Data glove commands.
 * @see cMessageDataGlove::cCommand
 */
#define DATA_GLOVE_D_G_TECH_V_HAND__UNKNOWN  0x00
#define DATA_GLOVE_D_G_TECH_V_HAND__CMD_START_SAMPLING 0x0A
#define DATA_GLOVE_D_G_TECH_V_HAND__CMD_STOP_SAMPLING  0x0B
#define DATA_GLOVE_D_G_TECH_V_HAND__CMD_GET_ID         0x0C
#define DATA_GLOVE_D_G_TECH_V_HAND__CMD_SET_ID         0x0D
#define DATA_GLOVE_D_G_TECH_V_HAND__CMD_GET_LABEL      0x11
#define DATA_GLOVE_D_G_TECH_V_HAND__CMD_SET_LABEL      0x10
#define DATA_GLOVE_D_G_TECH_V_HAND__CMD_GET_FIRMWARE_VERSION 0x13
#define DATA_GLOVE_D_G_TECH_V_HAND__CMD_START_CALIBRATION    0x31
//wifi commands (just for the Wifi data glove)
#define DATA_GLOVE_D_G_TECH_V_HAND__CMD_SET_WIFI_INFORMATION 0x1A
#define DATA_GLOVE_D_G_TECH_V_HAND__CMD_GET_WIFI_INFORMATION 0x1B
#define DATA_GLOVE_D_G_TECH_V_HAND__CMD_GET_MAC_ADDRESS      0x1D
#define DATA_GLOVE_D_G_TECH_V_HAND__CMD_SET_ACCESS_POINT_SSID     0X21
#define DATA_GLOVE_D_G_TECH_V_HAND__CMD_SET_ACCESS_POINT_PASSWORD 0x22
#define DATA_GLOVE_D_G_TECH_V_HAND__CMD_GET_ACCESS_POINT_PASSWORD 0x24
#define DATA_GLOVE_D_G_TECH_V_HAND__CMD_STORE_WIFI_SETTINGS       0x1E




class cMessageDataGlove{
public:
	
	/**
	 * The type for the type of the data glove message.
	 */
	enum typeMessageDataGlove{
		UNKNOWN,
		///requests:
		START_SAMPLINT,
		STOP_SAMPLING,
		GET_ID,
		SET_ID,
		GET_LABEL,
		SET_LABEL,
		GET_FIRMWARE_VERSION,
		START_CALIBRATION,
		SET_WIFI_INFORMATION,
		GET_WIFI_INFORMATION,
		GET_MAC_ADRESS,
		SET_ACCESS_POINT_SSID,
		GET_ACCESS_POINT_SSID,
		SET_ACCESS_POINT_PASSWORD,
		GET_ACCESS_POINT_PASSWORD,
		STORE_WIFI_SETTINGS,
		
		///results:
		SAMPLING_DATA,
		GET_ID_RESULT,
		SET_ID_RESULT,
		GET_LABEL_RESULT,
		SET_LABEL_RESULT,
		GET_FIRMWARE_VERSION_RESULT,
		START_CALIBRATION_RESULT,
		SET_WIFI_INFORMATION_RESULT,
		GET_WIFI_INFORMATION_RESULT,
		GET_MAC_ADRESS_RESULT,
		SET_ACCESS_POINT_SSID_RESULT,
		GET_ACCESS_POINT_SSID_RESULT,
		SET_ACCESS_POINT_PASSWORD_RESULT,
		GET_ACCESS_POINT_PASSWORD_RESULT,
		STORE_WIFI_SETTINGS_RESULT,
		
	};  //end typeMessageDataGlove
	
	/**
	 * The standard constructor for the DGTech VHand data glove message.
	 */
	cMessageDataGlove();
	
	/**
	 * The destructor.
	 */
	virtual ~cMessageDataGlove();
	
	
	/**
	 * @return the name of this class "cMessageDataGlove"
	 */
	virtual std::string getName() const;
	
	
	/**
	 * This method reads a message from the file/port with the given data
	 * glove file identifer.
	 *
	 * @see writeMessage()
	 * @param iDataGloveFileDescriptor the data glove file identifer
	 * @param uiMsTimeout milli seconds to wait for a new message
	 * @param bHeaderRead if true the header was read from the stream, else not
	 * @param bReadTillNextHeader if the first character in the file is not
	 * 	a header character, try to read till the next header
	 * 	just works with ( bHeaderRead = false )
	 * @return the data glove message read (please delete after usage),
	 * 	or NULL, if non could be read
	 */
	static cMessageFromDataGlove * readMessage(
			const int iDataGloveFileDescriptor,
			const unsigned int uiMsTimeout = 3000,
			const bool bHeaderRead = false,
			const bool bReadTillNextHeader = false);
	
	/**
	 * This method writes this message to the file/port with the given data
	 * glove file identifer.
	 *
	 * @see readMessage()
	 * @param iDataGloveFileDescriptor the data glove file identifer
	 * @return true if the message was writen, else false
	 */
	virtual bool writeMessage( const int iDataGloveFileDescriptor );
	
	/**
	 * @return the type of the message
	 * 	@see typeMessageDataGlove
	 */
	inline typeMessageDataGlove getType() const {
		
		return type;
	}
	
	/**
	 * @return the type character of this message
	 * 	@see cCommand
	 */
	inline char getCommand() const {
		
		return cCommand;
	}
	
	/**
	 * @return a pointer to the message/package buffer, with the message,
	 * 	or NULL if non exists.
	 * 	@see szMessage
	 */
	inline unsigned char  * getMessage() {
		
		return szMessage;
	}
	
	/**
	 * @return a pointer to the message/package buffer, with the message,
	 * 	or NULL if non exists.
	 * 	@see szMessage
	 */
	inline const unsigned char  * getMessage() const {
		
		return szMessage;
	}

	/**
	 * @return the number of bytes/characters in the message szMessage
	 * 	@see uiMessageSize
	 * 	@see szMessage
	 */
	inline unsigned int getMessageSize() const {
		
		return uiMessageSize;
	}
	
	
	/**
	 * @param pData the data, for which the CRC is to be evaluated
	 * @param iDataLength the number of bytes of the data, for which the
	 * 	CRC is to be evaluated
	 * @return the CRC for the given data (sum of the bytes modulo 256)
	 */
	inline static unsigned char evalueCRC( const unsigned char * pData,
			const int iDataLength ) {
		
		unsigned char CRC = 0;
		for ( int iActualByte = 0; iActualByte < iDataLength;
				++iActualByte ) {
			
			CRC += pData[ iActualByte ];
		}
		
		return CRC;
	}
	
	/**
	 * @param pData the data, for which the CRC is to be evaluated
	 * @param iDataLength the number of bytes of the data, for which the
	 * 	CRC is to be evaluated
	 * @return the CRC for the given data (sum of the bytes modulo 256)
	 */
	inline static unsigned char evalueCRC( const char * pData,
			const int iDataLength ) {
		
		unsigned char CRC = 0;
		const unsigned char * pUcData = (unsigned char *)(pData);
		for ( int iActualByte = 0; iActualByte < iDataLength;
				++iActualByte ) {
			
			CRC += pUcData[ iActualByte ];
		}
		
		return CRC;
	}
	
	/**
	 * Reads a 2 byte signed integer number from the szMessage at the given offset.
	 *
	 * @see szMessage
	 * @param uiOffset the offset, where the number begins in the szMessage
	 * 	data (counting starts with 0)
	 * @return the readed number
	 */
	inline int read2ByteInt( const unsigned int uiOffset ) const {
		
		if ( ( szMessage == NULL ) || ( uiMessageSize < uiOffset + 2 ) ) {
			return 0;
		}
		if ( ( szMessage[ uiOffset ] & 0x80 ) == 0 ) {
			//highest bit not set -> positive number
			int iReaded = static_cast<int>( szMessage[ uiOffset ] );
			iReaded = iReaded << 8;
			iReaded |= static_cast<int>( szMessage[ uiOffset + 1 ] );
			return iReaded;
		}// else highest bit set -> negative number
		int iReaded = -1;
		unsigned char * cReaded = ((unsigned char *)(&iReaded));
		cReaded[ 1 ] &= szMessage[ uiOffset ];
		cReaded[ 0 ] &= szMessage[ uiOffset + 1 ];
		return iReaded;
	}

	/**
	 * Reads a unsigned 2 byte integer number from the szMessage at the
	 * given offset.
	 *
	 * @see szMessage
	 * @param uiOffset the offset, where the number begins in the szMessage
	 * 	data (counting starts with 0)
	 * @return the readed number
	 */
	inline int read2ByteUInt( const unsigned int uiOffset ) const {
		
		if ( ( szMessage == NULL ) || ( uiMessageSize < uiOffset + 2 ) ) {
			return 0;
		}
		int iReaded = static_cast<int>( szMessage[ uiOffset ] );
		iReaded = iReaded << 8;
		iReaded |= static_cast<int>( szMessage[ uiOffset + 1 ] );
		
		return iReaded;
	}
	
	
	/**
	 * Reads a long number from the szMessage at the given offset.
	 *
	 * @see szMessage
	 * @param uiOffset the offset, where the number begins in the szMessage
	 * 	data (counting starts with 0)
	 * @return the readed number
	 */
	inline long read4ByteLong( const unsigned int uiOffset ) const {
		
		if ( ( szMessage == NULL ) || ( uiMessageSize < uiOffset + 4 ) ) {
			return 0;
		}
		if ( ( szMessage[ uiOffset ] & 0x80 ) == 0 ) {
			//highest bit not set -> positive number
			long lReaded = static_cast<int>( szMessage[ uiOffset ] );
			lReaded = lReaded << 8;
			lReaded |= static_cast<int>( szMessage[ uiOffset + 1 ] );
			lReaded = lReaded << 8;
			lReaded |= static_cast<int>( szMessage[ uiOffset + 2 ] );
			lReaded = lReaded << 8;
			lReaded |= static_cast<int>( szMessage[ uiOffset + 3 ] );
			return lReaded;
		}// else highest bit set -> negative number
		long lReaded = -1;
		unsigned char * cReaded = ((unsigned char *)(&lReaded));
		cReaded[ 3 ] &= szMessage[ uiOffset ];
		cReaded[ 2 ] &= szMessage[ uiOffset + 1 ];
		cReaded[ 1 ] &= szMessage[ uiOffset + 2 ];
		cReaded[ 0 ] &= szMessage[ uiOffset + 3 ];
		
		return lReaded;
	}

	/**
	 * Reads a unsigned long number from the szMessage at the given offset.
	 *
	 * @see szMessage
	 * @param uiOffset the offset, where the number begins in the szMessage
	 * 	data (counting starts with 0)
	 * @return the readed number
	 */
	inline unsigned long read4ByteULong( const unsigned int uiOffset ) const {
		
		if ( ( szMessage == NULL ) || ( uiMessageSize < uiOffset + 4 ) ) {
			return 0;
		}
		unsigned long ulReaded = static_cast<int>( szMessage[ uiOffset ] );
		ulReaded = ulReaded << 8;
		ulReaded |= static_cast<int>( szMessage[ uiOffset + 1 ] );
		ulReaded = ulReaded << 8;
		ulReaded |= static_cast<int>( szMessage[ uiOffset + 2 ] );
		ulReaded = ulReaded << 8;
		ulReaded |= static_cast<int>( szMessage[ uiOffset + 3 ] );
		
		return ulReaded;
	}
	
	
	/**
	 * Prints the given message.
	 *
	 * @param szMessage a pointer to the message to print
	 * @param uiNumberOfByte the number of bytes to print from te message
	 */
	static void printMessage( const unsigned char * szMessage,
		unsigned int uiNumberOfByte );
	
	/**
	 * Sleeps a smaal time. (If you want to wait for more data.)
	 */
	static inline void shortSleep() {
		
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
	
protected:
	
	/**
	 * The type of the message.
	 * @see typeMessageDataGlove
	 */
	typeMessageDataGlove type;
	
	/**
	 * The type character of this message.
	 */
	char cCommand;
	
	/**
	 * A pointer to the message/package buffer, with the message,
	 * or NULL if non exists.
	 *
	 * @see uiMessageSize
	 *
	 * The packages have the following structure:
	 * HEADER CMD PKGLEN PACKAGE_DATA CRC ENDCAR
	 * where:
	 * 	* HEADER = '$'; signals start of package
	 * 	* CMD = command character for the command to be executed
	 * 	  @see cCommand
	 * 	* PKGLEN: the number of byte in the package (without CRC and
	 * 	  ENDCAR; byte in "HEADER CMD PKGLEN PACKAGE_DATA")
	 * 	* PACKAGE_DATA: the (additional) data of the package
	 * 	* CRC = sum of bytes/chars modulo 256 of the previous bytes in the
	 * 	  package (byte in "HEADER CMD PKGLEN PACKAGE_DATA")
	 * 	* ENDCAR = '#'; signals end of package
	 *
	 * See DG5 VHand 3.0 OEM Technical Datasheet.
	 * @see www.dg-tech.it
	 * @see www.dg-tech.it/vhand3/
	 */
	unsigned char * szMessage;
	
	/**
	 * Number of bytes/characters in the message szMessage.
	 *
	 * @see szMessage
	 */
	unsigned int uiMessageSize;
	
};//end class cMessageDataGlove


};//end namespace nDataGlove
};//end namespace nModelDataGloveDGTechVHand

#endif //___N_DATA_GLOVE__N_MODEL_DATA_GLOVE_D_G_TECH_V_HAND__C_MESSAGE_DATA_GLOVE_H__





