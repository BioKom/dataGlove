
//TODO check

/**
 * @file cMessageSamplingDataFromDataGlove
 * file name: cMessageSamplingDataFromDataGlove.h
 * @author Betti Oesterholz
 * @date 13.07.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies the class for the DGTech VHand data glove
 * messages, which is the sampling data from the data glove.
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
 * This file specifies the class for the DGTech VHand data glove
 * messages, which is the sampling data from the data glove.
 * If the get the get Id message is send, this message should be the answer.
 * See DG5 VHand 3.0 OEM Technical Datasheet.
 * @see www.dg-tech.it
 * @see www.dg-tech.it/vhand3/
 *
 * Tested with:
 * 	*  DGTech VHand data glove 3.0 left handed
 *
 * @see cMessageStartSamplingToDataGlove
 * @see cMessageStopSamplingToDataGlove
 */
/*
History:
13.07.2014  Oesterholz  created
*/


#ifndef ___N_DATA_GLOVE__N_MODEL_DATA_GLOVE_D_G_TECH_V_HAND__C_MESSAGE_SAMPLING_DATA_FROM_DATA_GLOVE_H__
#define ___N_DATA_GLOVE__N_MODEL_DATA_GLOVE_D_G_TECH_V_HAND__C_MESSAGE_SAMPLING_DATA_FROM_DATA_GLOVE_H__

///pi divided 18  (to convert cent degrees to rad)
#define PI_D_18 (3.141592653 / 18.0)


#include "version.h"

#include <string>

#include "cMessageFromDataGlove.h"


namespace nDataGlove{

namespace nModelDataGloveDGTechVHand{

	enum tDirection{
		DIM_X = 0,
		DIM_Y = 1,
		DIM_Z = 2
	};

class cMessageSamplingDataFromDataGlove : public cMessageFromDataGlove {
public:

	/**
	 * The standard constructor for the DGTech VHand data glove message.
	 */
	explicit cMessageSamplingDataFromDataGlove();
	
	
	/**
	 * The destructor.
	 */
	virtual ~cMessageSamplingDataFromDataGlove();
	
	
	/**
	 * @return the name of this class "cMessageSamplingDataFromDataGlove"
	 */
	virtual std::string getName() const;
	
	/**
	 * @return a number for the device type:
	 * 	1: quaternion orientation and finger sensors values
	 * 	2: only quaternion values
	 * 	3: raw gyroscope data, raw accelerometer data,
	 * 		raw magnetometer data and finger sensor values
	 * 	4: only raw data
	 * 	5: only finger data
	 */
	inline int getType() const {
	
		if ( ( szMessage == NULL ) || ( uiMessageSize <= 4 ) ) {
			return 0;
		}
		return static_cast<int>( szMessage[ 3 ] );
	}
	
	/**
	 * @return the micro controller identifier/ID
	 */
	inline int getId() const {
		
		return read2ByteUInt( 4 );
	}
	
	/**
	 * @return the data glove clock (CLK; so the messages can be ordered)
	 */
	inline unsigned long getClock() const {
		
		return read4ByteULong( 7 );
	}
	
	/**
	 * @return the status of the data glove
	 * 	TODO???
	 */
	inline int getStatus() const {
		
		const unsigned int iType = getType();
		switch ( iType ) {
			case 0: return read2ByteUInt( 10 );
			case 1: return read2ByteUInt( 36 );
			case 2: return read2ByteUInt( 26 );
			case 3: return read2ByteUInt( 38 );
			case 4: return read2ByteUInt( 28 );
			case 5: return read2ByteUInt( 20 );
		};
	}
	
	
	/**
	 * @return true if there is finger information, else false
	 * @see getFinger()
	 */
	inline bool isFinger() const {
		
		const unsigned int iType = getType();
		return ( iType == 1 ) || ( iType ==  3 ) || ( iType ==  5 );
	}
	
	/**
	 * Returns the percentage of flexion in thousandths of percentage of
	 * the uiNumber'th finger/digit.
	 *
	 * @param uiNumber the number of the finger/digit, for which to return
	 * 	the flexion
	 * @return the percentage of flexion in thousandths of percentage
	 * 	(0 = no flexion, 1000 = maximum flexion)
	 */
	inline int getFinger( const unsigned int uiNumber ) const {
		
		if ( ( uiNumber < 1 ) || ( 5 < uiNumber ) ) {
			return 0;
		}
		const unsigned int iType = getType();
		switch ( iType ) {  //uiNumber * 2 = (uiNumber << 1)
			case 1: return read2ByteUInt( 24 + (uiNumber << 1) );
			case 3: return read2ByteUInt( 26 + (uiNumber << 1) );
			case 5: return read2ByteUInt( 8 + (uiNumber << 1) );
		};
		return 0;
	}
	
	
	/**
	 * Returns the fraction of flexion of the uiNumber'th finger/digit.
	 *
	 * @param uiNumber the number of the finger/digit, for which to return
	 * 	the flexion
	 * @return the fraction of flexion (0 = no flexion, 1.0 = maximum flexion)
	 */
	inline double getFingerDouble( const unsigned int uiNumber ) const {
		
		return static_cast<double>( getFinger( uiNumber ) ) / 1000.0;
	}
	
	/**
	 * Returns the percentage of flexion in thousandths of percentage of
	 * the first finger/digit.
	 *
	 * @return the percentage of flexion in thousandths of percentage
	 * 	(0 = no flexion, 1000 = maximum flexion) of the first finger/digit
	 */
	inline int getFinger1() const {
		
		const unsigned int iType = getType();
		switch ( iType ) {
			case 1: return read2ByteUInt( 26 );
			case 3: return read2ByteUInt( 28 );
			case 5: return read2ByteUInt( 10 );
		};
	}
	
	/**
	 * Returns the fraction of flexion of the first finger/digit.
	 *
	 * @return the fraction of flexion (0 = no flexion, 1.0 = maximum flexion)
	 *   of the first finger/digit
	 */
	inline double getFinger1Double() const {
		
		return static_cast<double>(getFinger1()) / 1000.0;
	}
	
	/**
	 * Returns the percentage of flexion in thousandths of percentage of
	 * the second finger/digit.
	 *
	 * @return the percentage of flexion in thousandths of percentage
	 * 	(0 = no flexion, 1000 = maximum flexion) of the second finger/digit
	 */
	inline int getFinger2() const {
		
		const unsigned int iType = getType();
		switch ( iType ) {
			case 1: return read2ByteUInt( 28 );
			case 3: return read2ByteUInt( 30 );
			case 5: return read2ByteUInt( 12 );
		};
	}
	
	/**
	 * Returns the fraction of flexion of the second finger/digit.
	 *
	 * @return the fraction of flexion (0 = no flexion, 1.0 = maximum flexion)
	 *   of the second finger/digit
	 */
	inline double getFinger2Double() const {
		
		return static_cast<double>(getFinger2()) / 1000.0;
	}
	
	/**
	 * Returns the percentage of flexion in thousandths of percentage of
	 * the third finger/digit.
	 *
	 * @return the percentage of flexion in thousandths of percentage
	 * 	(0 = no flexion, 1000 = maximum flexion) of the third finger/digit
	 */
	inline int getFinger3() const {
		
		const unsigned int iType = getType();
		switch ( iType ) {
			case 1: return read2ByteUInt( 30 );
			case 3: return read2ByteUInt( 32 );
			case 5: return read2ByteUInt( 14 );
		};
	}
	
	/**
	 * Returns the fraction of flexion of the third finger/digit.
	 *
	 * @return the fraction of flexion (0 = no flexion, 1.0 = maximum flexion)
	 *   of the third finger/digit
	 */
	inline double getFinger3Double() const {
		
		return static_cast<double>(getFinger3()) / 1000.0;
	}
	
	/**
	 * Returns the percentage of flexion in thousandths of percentage of
	 * the fourth finger/digit.
	 *
	 * @return the percentage of flexion in thousandths of percentage
	 * 	(0 = no flexion, 1000 = maximum flexion) of the fourth finger/digit
	 */
	inline int getFinger4() const {
		
		const unsigned int iType = getType();
		switch ( iType ) {
			case 1: return read2ByteUInt( 32 );
			case 3: return read2ByteUInt( 34 );
			case 5: return read2ByteUInt( 16 );
		};
	}
	
	/**
	 * Returns the fraction of flexion of the fourth finger/digit.
	 *
	 * @return the fraction of flexion (0 = no flexion, 1.0 = maximum flexion)
	 *   of the fourth finger/digit
	 */
	inline double getFinger4Double() const {
		
		return static_cast<double>(getFinger4()) / 1000.0;
	}
	
	/**
	 * Returns the percentage of flexion in thousandths of percentage of
	 * the fifth finger/digit.
	 *
	 * @return the percentage of flexion in thousandths of percentage
	 * 	(0 = no flexion, 1000 = maximum flexion) of the fifth finger/digit
	 */
	inline int getFinger5() const {
		
		const unsigned int iType = getType();
		switch ( iType ) {
			case 1: return read2ByteUInt( 34 );
			case 3: return read2ByteUInt( 36 );
			case 5: return read2ByteUInt( 18 );
		};
	}
	
	/**
	 * Returns the fraction of flexion of the fifth finger/digit.
	 *
	 * @return the fraction of flexion (0 = no flexion, 1.0 = maximum flexion)
	 *   of the fifth finger/digit
	 */
	inline double getFinger5Double() const {
		
		return static_cast<double>(getFinger5()) / 1000.0;
	}
	
	

	/**
	 * @return true if there is quaternion data, else false
	 * @see getFinger()
	 */
	inline bool isQuaternion() const {
		
		const unsigned int iType = getType();
		return ( iType == 1 ) || ( iType ==  2 );
	}
	
	/**
	 * Returns the value representing the value of the quaternion data
	 * (signed integer maximum 32768).
	 *
	 * @param uiNumber the number of the data value, for which to return
	 * 	the quaternion data
	 * @return value representing the value of the quaternion data
	 * 	(signed integer maximum 32768)
	 */
	inline long getQuaternion( const unsigned int uiNumber ) const {
		
		if ( ( uiNumber < 1 ) || ( 4 < uiNumber ) ) {
			return 0;
		}
		const unsigned int iType = getType();
		switch ( iType ) {  //uiNumber * 4 = (uiNumber << 2)
			case 1:
			case 2: return read4ByteLong( 6 + (uiNumber << 2) );
		};
		return 0;
	}
	
	
	/**
	 * Returns the fraction value representing the value of the quaternion
	 * data (betwean -1.0 and 1.0).
	 *
	 * @param uiNumber the number of the data value, for which to return
	 * 	the quaternion data
	 * @return the fraction value representing the value of the quaternion
	 * 	data (betwean -1.0 and 1.0)
	 */
	inline double getQuaternionDouble( const unsigned int uiNumber ) const {
		
		return static_cast<double>( getQuaternion( uiNumber ) ) / 32768.0;
	}

	
	/**
	 * Returns the value representing the value of the first quaternion data
	 * (signed integer maximum 32768).
	 *
	 * @return value representing the value of the first quaternion data
	 * 	(signed integer maximum 32768)
	 */
	inline long getQuaternion1() const {
		const unsigned int iType = getType();
		switch ( iType ) {  //uiNumber * 2 = (uiNumber << 1)
			case 1:
			case 2: return read4ByteLong( 10 );
		};
		return 0;
	}
	
	/**
	 * Returns the fraction value representing the value of the first
	 * quaternion data (betwean -1.0 and 1.0).
	 *
	 * @return the fraction value representing the value of the first
	 * 	quaternion data (betwean -1.0 and 1.0)
	 */
	inline double getQuaternion1Double() const {
		
		return static_cast<double>( getQuaternion1() ) / 32768.0;
	}
	
	/**
	 * Returns the value representing the value of the second quaternion data
	 * (signed integer maximum 32768).
	 *
	 * @return value representing the value of the second quaternion data
	 * 	(signed integer maximum 32768)
	 */
	inline long getQuaternion2() const {
		
		const unsigned int iType = getType();
		switch ( iType ) {  //uiNumber * 2 = (uiNumber << 1)
			case 1:
			case 2: return read4ByteLong( 14 );
		};
		return 0;
	}
	
	/**
	 * Returns the fraction value representing the value of the second
	 * quaternion data (betwean -1.0 and 1.0).
	 *
	 * @return the fraction value representing the value of the second
	 * 	quaternion data (betwean -1.0 and 1.0)
	 */
	inline double getQuaternion2Double() const {
		
		return static_cast<double>( getQuaternion2() ) / 32768.0;
	}
	
	/**
	 * Returns the value representing the value of the third quaternion data
	 * (signed integer maximum 32768).
	 *
	 * @return value representing the value of the third quaternion data
	 * 	(signed integer maximum 32768)
	 */
	inline long getQuaternion3() const {
		const unsigned int iType = getType();
		switch ( iType ) {  //uiNumber * 2 = (uiNumber << 1)
			case 1:
			case 2: return read4ByteLong( 18 );
		};
		return 0;
	}
	
	/**
	 * Returns the fraction value representing the value of the third
	 * quaternion data (betwean -1.0 and 1.0).
	 *
	 * @return the fraction value representing the value of the third
	 * 	quaternion data (betwean -1.0 and 1.0)
	 */
	inline double getQuaternion3Double() const {
		
		return static_cast<double>( getQuaternion3() ) / 32768.0;
	}
	
	/**
	 * Returns the value representing the value of the fourth quaternion data
	 * (signed integer maximum 32768).
	 *
	 * @return value representing the value of the fourth quaternion data
	 * 	(signed integer maximum 32768)
	 */
	inline long getQuaternion4() const {
		
		const unsigned int iType = getType();
		switch ( iType ) {  //uiNumber * 2 = (uiNumber << 1)
			case 1:
			case 2: return read4ByteLong( 22 );
		};
		return 0;
	}
	
	/**
	 * Returns the fraction value representing the value of the fourth
	 * quaternion data (betwean -1.0 and 1.0).
	 *
	 * @return the fraction value representing the value of the fourth
	 * 	quaternion data (betwean -1.0 and 1.0)
	 */
	inline double getQuaternion4Double() const {
		
		return static_cast<double>( getQuaternion4() ) / 32768.0;
	}
	
	
	/**
	 * @return true if there is gyroscope data, else false
	 * @see getQuaternion()
	 * @see getFinger()
	 */
	inline bool isGyroscope() const {
		
		const unsigned int iType = getType();
		return ( iType == 3 ) || ( iType ==  4 );
	}
	
	/**
	 * Returns the value representing the value of the gyroscope data in the
	 * given direction (instantaneous rotation of the hand in cents of
	 * degrees per second).
	 *
	 * @param direction the direction of the gyroscope data value, for which
	 * 	to return the gyroscope data
	 * @return value representing the value of the gyroscope data in the
	 * 	given direction (instantaneous rotation of the hand in cents of
	 * 	degrees per second)
	 */
	inline int getGyroscope( const tDirection direction ) const {
		
		const unsigned int iType = getType();
		switch ( iType ) {  //direction * 2 = (direction << 1)
			case 3:
			case 4: return read2ByteInt( 10 + (direction << 1) );
		};
		return 0;
	}
	
	
	/**
	 * Returns the value representing the value of the gyroscope data in the
	 * given direction (instantaneous rotation of the hand in radian per
	 * second; getGyroscope( direction ) * Pi / 180  ).
	 *
	 * @param direction the direction of the gyroscope data value, for which
	 * 	to return the gyroscope data
	 * @return value representing the value of the gyroscope data in the
	 * 	given direction (instantaneous rotation of the hand in radian per
	 * 	second)
	 */
	inline int getGyroscopeDouble( const tDirection direction ) const {
		
		return static_cast<double>( getGyroscope( direction ) ) * PI_D_18;
	}
	
	/**
	 * Returns the value representing the value of the gyroscope data in the
	 * X direction (instantaneous rotation of the hand in cents of degrees
	 * per second).
	 *
	 * @return value representing the value of the gyroscope data in the
	 * 	X direction (instantaneous rotation of the hand in cents of
	 * 	degrees per second)
	 */
	inline int getGyroscopeX() const {
		
		const unsigned int iType = getType();
		switch ( iType ) {
			case 3:
			case 4: return read2ByteInt( 10 );
		};
		return 0;
	}
	
	/**
	 * Returns the value representing the value of the gyroscope data in the
	 * X direction (instantaneous rotation of the hand in radian per second;
	 * getGyroscope( direction ) * Pi / 180  ).
	 *
	 * @return value representing the value of the gyroscope data in the
	 * 	X direction (instantaneous rotation of the hand in radian per
	 * 	second)
	 */
	inline int getGyroscopeDoubleX() const {
		
		return static_cast<double>( getGyroscopeX() ) * PI_D_18;
	}
	
	/**
	 * Returns the value representing the value of the gyroscope data in the
	 * Y direction (instantaneous rotation of the hand in cents of degrees
	 * per second).
	 *
	 * @return value representing the value of the gyroscope data in the
	 * 	Y direction (instantaneous rotation of the hand in cents of
	 * 	degrees per second)
	 */
	inline int getGyroscopeY() const {
		
		const unsigned int iType = getType();
		switch ( iType ) {
			case 3:
			case 4: return read2ByteInt( 12 );
		};
		return 0;
	}
	
	/**
	 * Returns the value representing the value of the gyroscope data in the
	 * Y direction (instantaneous rotation of the hand in radian per second;
	 * getGyroscope( direction ) * Pi / 180  ).
	 *
	 * @return value representing the value of the gyroscope data in the
	 * 	Y direction (instantaneous rotation of the hand in radian per
	 * 	second)
	 */
	inline int getGyroscopeDoubleY() const {
		
		return static_cast<double>( getGyroscopeZ() ) * PI_D_18;
	}
	
	/**
	 * Returns the value representing the value of the gyroscope data in the
	 * Z direction (instantaneous rotation of the hand in cents of degrees
	 * per second).
	 *
	 * @return value representing the value of the gyroscope data in the
	 * 	Z direction (instantaneous rotation of the hand in cents of
	 * 	degrees per second)
	 */
	inline int getGyroscopeZ() const {
		
		const unsigned int iType = getType();
		switch ( iType ) {
			case 3:
			case 4: return read2ByteInt( 14 );
		};
		return 0;
	}
	
	/**
	 * Returns the value representing the value of the gyroscope data in the
	 * Z direction (instantaneous rotation of the hand in radian per second;
	 * getGyroscope( direction ) * Pi / 180  ).
	 *
	 * @return value representing the value of the gyroscope data in the
	 * 	Z direction (instantaneous rotation of the hand in radian per
	 * 	second)
	 */
	inline int getGyroscopeDoubleZ() const {
		
		return static_cast<double>( getGyroscopeZ() ) * PI_D_18;
	}

	
	
	/**
	 * @return true if there is magnetometer data, else false
	 * @see getQuaternion()
	 * @see getFinger()
	 */
	inline bool isMagnetometer() const {
		
		const unsigned int iType = getType();
		return ( iType == 3 ) || ( iType ==  4 );
	}
	
	/**
	 * Returns the value representing the value of the magnetometer data in
	 * the given direction.
	 *
	 * @param direction the direction of the magnetometer data value, for
	 * 	which to return the magnetometer data
	 * @return value representing the value of the magnetometer data in the
	 * 	given direction
	 */
	inline int getMagnetometer( const tDirection direction ) const {
		
		const unsigned int iType = getType();
		switch ( iType ) {  //direction * 2 = (direction << 1)
			case 3:
			case 4: return read2ByteInt( 16 + (direction << 1) );
		};
		return 0;
	}
	
	/**
	 * Returns the value representing the value of the magnetometer data in
	 * the X direction.
	 *
	 * @return value representing the value of the magnetometer data in the
	 * 	X direction
	 */
	inline int getMagnetometerX() const {
		
		const unsigned int iType = getType();
		switch ( iType ) {
			case 3:
			case 4: return read2ByteInt( 16 );
		};
		return 0;
	}
	
	/**
	 * Returns the value representing the value of the magnetometer data in
	 * the Y direction.
	 *
	 * @return value representing the value of the magnetometer data in the
	 * 	Y direction
	 */
	inline int getMagnetometerY() const {
		
		const unsigned int iType = getType();
		switch ( iType ) {
			case 3:
			case 4: return read2ByteInt( 18 );
		};
		return 0;
	}
	
	/**
	 * Returns the value representing the value of the magnetometer data in
	 * the Z direction.
	 *
	 * @return value representing the value of the magnetometer data in the
	 * 	Z direction
	 */
	inline int getMagnetometerZ() const {
		
		const unsigned int iType = getType();
		switch ( iType ) {
			case 3:
			case 4: return read2ByteInt( 20 );
		};
		return 0;
	}
	
	
	/**
	 * @return true if there is accelerometer data, else false
	 * @see getQuaternion()
	 * @see getFinger()
	 */
	inline bool isAccelerometer() const {
		
		const unsigned int iType = getType();
		return ( iType == 3 ) || ( iType ==  4 );
	}
	
	/**
	 * Returns the value representing the value of the accelerometer data in
	 * the given direction.
	 *
	 * @param direction the direction of the accelerometer data value, for
	 * 	which to return the accelerometer data
	 * @return value representing the value of the accelerometer data in the
	 * 	given direction
	 */
	inline int getAccelerometer( const tDirection direction ) const {
		
		const unsigned int iType = getType();
		switch ( iType ) {  //direction * 2 = (direction << 1)
			case 3:
			case 4: return read2ByteInt( 22 + (direction << 1) );
		};
		return 0;
	}
	
	/**
	 * Returns the value representing the value of the accelerometer data in
	 * the X direction.
	 *
	 * @return value representing the value of the accelerometer data in the
	 * 	X direction
	 */
	inline int getAccelerometerX() const {
		
		const unsigned int iType = getType();
		switch ( iType ) {
			case 3:
			case 4: return read2ByteInt( 22 );
		};
		return 0;
	}
	
	/**
	 * Returns the value representing the value of the accelerometer data in
	 * the Y direction.
	 *
	 * @return value representing the value of the accelerometer data in the
	 * 	Y direction
	 */
	inline int getAccelerometerY() const {
		
		const unsigned int iType = getType();
		switch ( iType ) {
			case 3:
			case 4: return read2ByteInt( 24 );
		};
		return 0;
	}
	
	/**
	 * Returns the value representing the value of the accelerometer data in
	 * the Z direction.
	 *
	 * @return value representing the value of the accelerometer data in the
	 * 	Z direction
	 */
	inline int getAccelerometerZ() const {
		
		const unsigned int iType = getType();
		switch ( iType ) {
			case 3:
			case 4: return read2ByteInt( 26 );
		};
		return 0;
	}
	
	
};//end class cMessageSamplingDataFromDataGlove


};//end namespace nDataGlove
};//end namespace nModelDataGloveDGTechVHand


#endif //___N_DATA_GLOVE__N_MODEL_DATA_GLOVE_D_G_TECH_V_HAND__C_MESSAGE_SAMPLING_DATA_FROM_DATA_GLOVE_H__





