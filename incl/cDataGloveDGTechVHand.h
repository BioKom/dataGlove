
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

//TODO rework



namespace nDataGlove{

/**
 * The structure with the data glove identifier data.
 */
struct TDataGloveID{
	
	
	/**
	 * The divice type of the data glove.
	 */
	enum TDeviceType {
		NONE, ///no valid data glove device found
		USB,  ///the data glove is a Usb device
		WIFI  ///the data glove is a Wifi device
	} deviceType;
	
	//TODO more info
	
}; //end TDataGloveID


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
	 * @see closeDataGloveFile()
	 * @param iDataGloveStreamFileDescriptor the data glove file identifer
	 * @return true if the data glove file could be closed, else false
	 */
	static bool closeDataGloveFile( const int iDataGloveStreamFileDescriptor );
	
	
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
	 * @param inPDataGloveStraem the pointer to the stream for the
	 * 	communication with the data glove
	 * @return the identifier data from the data glove
	 */
	static TDataGloveID getDataGloveIDStatic( std::iostream * inPDataGloveStraem );
	
	/**
	 * This method tries to get the identifier data from the data glove.
	 *
	 * @see TDataGloveID
	 * @see pDataGloveStraem
	 * @return the identifier data from the data glove
	 */
	TDataGloveID getDataGloveID();
	
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





