
//TODO check

/**
 * @file cDataGloveDGTechVHand
 * file name: cDataGloveDGTechVHand.h
 * @author Betti Oesterholz
 * @date 09.07.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies the class for the DGTech VHand data glove.
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
 * This file specifies the class for the DGTech VHand data glove.
 *
 * Tested with:
 * 	*  DGTech VHand data glove 3.0 left handed
 *
 */
/*
History:
06.07.2014  Oesterholz  created
*/


#ifndef ___N_DATA_GLOVE__DATA_GLOVE__D_G_TECH_V_HAND_H__
#define ___N_DATA_GLOVE__DATA_GLOVE__D_G_TECH_V_HAND_H__


#include "version.h"

#include <iostream>
#include <string>

#include "cMessageFromDataGlove.h"


namespace nDataGlove{


namespace nModelDataGloveDGTechVHand{
	//forward declarations
	class cMessageGetIdFromDataGlove;
};  //end nModelDataGloveDGTechVHand

class cDataGloveDGTechVHand{
public:

	/**
	 * The standard constructor for the DGTech VHand data glove handler.
	 *
	 * @param inPDataGloveFile the pointer to the file name for the
	 * 	communication with the data glove
	 */
	cDataGloveDGTechVHand( const char * inPDataGloveFile );
	
	/**
	 * The destructor.
	 */
	virtual ~cDataGloveDGTechVHand();
	
	
	/**
	 * @return the name of this class "cDataGloveDGTechVHand"
	 */
	virtual std::string getName() const;
	
	
	/**
	 * This method opens a file to communicate with the data glove.
	 *
	 * @see closeDataGloveFile()
	 * @param inPDataGloveFile the pointer to the file name for the
	 * 	communication with the data glove
	 * @return the data glove file identifer or -1 if the data glove file could
	 * 	not be opened
	 */
	static int openDataGloveFile( const char * inPDataGloveFile );

	/**
	 * This method closes a file to communicate with the data glove.
	 *
	 * @see openDataGloveFile()
	 * @param iDataGloveFileDescriptor the data glove file identifer
	 * @return true if the data glove file could be closed, else false
	 */
	static bool closeDataGloveFile( const int iDataGloveFileDescriptor );
	
	/**
	 * @return true if this is a valid data glove device, else false
	 */
	bool isValid();
	
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
	bool clearDataGlove();

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
	static bool clearDataGloveStatic( const int iDataGloveFileDescriptor );
	
	
	/**
	 * This method checks if the file is for a valid and live data glove.
	 *
	 * @see TDataGloveID
	 * @param inPDataGloveFile the pointer to the file name for the
	 * 	communication with the data glove
	 * @return true if the file is connected to a valid and live DGTech
	 * 	VHand data glove, else false
	 */
	static bool isLiveDataGlove( const char * inPDataGloveFile );
	
	
	
	/**
	 * This method tries to get the identifier data from the data glove.
	 *
	 * @see TDataGloveID
	 * @param iDataGloveFileDescriptor the data glove file identifer for
	 * 	the communication with the data glove
	 * @return the identifier data message from the data glove (please delete it),
	 * 	or NULL if non could be loaded
	 */
	static nModelDataGloveDGTechVHand::cMessageGetIdFromDataGlove *
		getDataGloveIDStatic( const int iDataGloveFileDescriptor );
	
	/**
	 * This method tries to get the identifier data from the data glove.
	 *
	 * @see iDataGloveStreamFileDescriptor
	 * @return the identifier data message from the data glove (please delete it),
	 * 	or NULL if non could be loaded
	 */
	nModelDataGloveDGTechVHand::cMessageGetIdFromDataGlove * getDataGloveID();
	
	
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
	bool startSampling( const int iSamplingType = 1 );
	
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
	static bool startSamplingStatic( const int iDataGloveFileDescriptor,
		 const int iSamplingType = 1 );
	
	/**
	 * This method tries to stop the sampling of data from the data glove.
	 * Use clearDataGloveStatic() if all send messages from the data glove
	 * should also be read.
	 *
	 * @see iDataGloveStreamFileDescriptor
	 * @see clearDataGlove()
	 * @return true if the stop sampling was successful, else false
	 */
	bool stopSampling();
	
	/**
	 * This method tries to stop the sampling of data from the data glove.
	 * Use clearDataGloveStatic() if all send messages from the data glove
	 * should also be read.
	 *
	 * @see clearDataGloveStatic()
	 * @param iDataGloveFileDescriptor the data glove file identifer for
	 * 	the communication with the data glove
	 * @return true if the stop sampling was successful, else false
	 */
	static bool stopSamplingStatic( const int iDataGloveFileDescriptor );
	
	
	/**
	 * This method reads a message from the data glove.
	 *
	 * @param uiMsTimeout milli seconds to wait for a new message
	 * @param bHeaderRead if true the header was read from the stream, else not
	 * @param bReadTillNextHeader if the first character in the file is not
	 * 	a header character, try to read till the next header
	 * 	just works with ( bHeaderRead = false )
	 * @return the data glove message read (please delete after usage),
	 * 	or NULL, if non could be read
	 */
	inline nModelDataGloveDGTechVHand::cMessageFromDataGlove * readMessage(
			const unsigned int uiMsTimeout = 3000,
			const bool bHeaderRead = false,
			const bool bReadTillNextHeader = false) {
	
		if ( iDataGloveStreamFileDescriptor <= 0 ) {
			return NULL;
		}
		
		return nModelDataGloveDGTechVHand::cMessageDataGlove::readMessage(
			iDataGloveStreamFileDescriptor,
			uiMsTimeout, bHeaderRead, bReadTillNextHeader );
	}
	
	
	/**
	 * @param pData the data, for which the CRC is to evaluate
	 * @param iDataLength the number of bytes of the data, for which the
	 * 	CRC is to evaluate
	 * @return the CRC for the given data (sum of the bytes modulo 256)
	 */
	static char evalueCRC( const unsigned char * pData, const int iDataLength );
	
	/**
	 * @param pData the data, for which the CRC is to evaluate
	 * @param iDataLength the number of bytes of the data, for which the
	 * 	CRC is to evaluate
	 * @return the CRC for the given data (sum of the bytes modulo 256)
	 */
	static char evalueCRC( const char * pData, const int iDataLength );
	
	
	/**
	 * Reads the data form the data glove and writes it to standard output
	 * (printf) for debugging purpose.
	 *
	 * @param iSeconds the seconds input data should be read
	 */
	void debugReadData( const int iSeconds = 3 );

	/**
	 * Reads the data and writes it to standard output (printf) for debugging
	 * purpose.
	 *
	 * @param inDataGloveFile the pointer to the file name for the
	 * 	communication with the data glove
	 * @param iSeconds the seconds input data should be read
	 */
	static void debugReadDataStatic(
		const int inDataGloveFile, const int iSeconds = 3 );
	
	
protected:
	
	/**
	 * Sleeps a smaal time. (If you want to wait for more data.)
	 */
	static void shortSleep();

//members
	
	/**
	 * The file descriptor for the file for the communication with data glove.
	 * If -1 no file is open or no data glove could be found.
	 */
	int iDataGloveStreamFileDescriptor;

	
};//end class cDataGloveDGTechVHand


};//end namespace nDataGlove

#endif //___N_DATA_GLOVE__DATA_GLOVE__D_G_TECH_V_HAND_H__





