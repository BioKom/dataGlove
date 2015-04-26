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
 * If the start sampling message is send, this message (or a lot of these
 * messages) should be the answer.
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
#include <map>
#include <cstring>
#include <utility>  //pair

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
	 * The typ for the parts of the DGTech VHand data glove message.
	 * (Not every part is available in every message type.)
	 */
	enum tTypeSamplingValue{
		UNKNOWN  = 0,
		BEGIN    = 1,
		TYPE_MSG = 2,
		LENGTH   = 3,
		TYPE  = 4,
		ID    = 5,
		CLOCK = 6,
		STATUS   = 7,
		FINGER_1 = 8,
		FINGER_2 = 9,
		FINGER_3 = 10,
		FINGER_4 = 11,
		FINGER_5 = 12,
		QUATERNION_1 = 13,
		QUATERNION_2 = 14,
		QUATERNION_3 = 15,
		QUATERNION_4 = 16,
		GYROSCOPE_X  = 17,
		GYROSCOPE_Y  = 18,
		GYROSCOPE_Z  = 19,
		MAGNETOMETER_X  = 20,
		MAGNETOMETER_Y  = 21,
		MAGNETOMETER_Z  = 22,
		ACCELEROMETER_X = 23,
		ACCELEROMETER_Y = 24,
		ACCELEROMETER_Z = 25,
		CRC = 26,
		END = 27
	};  //end tTypeSamplingValue
	
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
	inline int getSamplingType() const {
	
		if ( ( szMessage == NULL ) || ( uiMessageSize < 4 ) ) {
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
		
		return read4ByteULong( 6 );
	}
	
	/**
	 * @return the status of the data glove
	 * 	TODO???
	 */
	inline int getStatus() const {
		
		const unsigned int iType = getSamplingType();
		switch ( iType ) {
			case 0: return read2ByteUInt( 10 );
			case 1: return read2ByteUInt( 36 );
			case 2: return read2ByteUInt( 26 );
			case 3: return read2ByteUInt( 38 );
			case 4: return read2ByteUInt( 28 );
			case 5: return read2ByteUInt( 20 );
		};
		return 0;
	}
	
	
	/**
	 * @return true if there is finger information, else false
	 * @see getFinger()
	 */
	inline bool isFinger() const {
		
		const unsigned int iType = getSamplingType();
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
		const unsigned int iType = getSamplingType();
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
		
		const unsigned int iType = getSamplingType();
		switch ( iType ) {
			case 1: return read2ByteUInt( 26 );
			case 3: return read2ByteUInt( 28 );
			case 5: return read2ByteUInt( 10 );
		};
		return 0;
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
		
		const unsigned int iType = getSamplingType();
		switch ( iType ) {
			case 1: return read2ByteUInt( 28 );
			case 3: return read2ByteUInt( 30 );
			case 5: return read2ByteUInt( 12 );
		};
		return 0;
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
		
		const unsigned int iType = getSamplingType();
		switch ( iType ) {
			case 1: return read2ByteUInt( 30 );
			case 3: return read2ByteUInt( 32 );
			case 5: return read2ByteUInt( 14 );
		};
		return 0;
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
		
		const unsigned int iType = getSamplingType();
		switch ( iType ) {
			case 1: return read2ByteUInt( 32 );
			case 3: return read2ByteUInt( 34 );
			case 5: return read2ByteUInt( 16 );
		};
		return 0;
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
		
		const unsigned int iType = getSamplingType();
		switch ( iType ) {
			case 1: return read2ByteUInt( 34 );
			case 3: return read2ByteUInt( 36 );
			case 5: return read2ByteUInt( 18 );
		};
		return 0;
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
		
		const unsigned int iType = getSamplingType();
		return ( iType == 1 ) || ( iType ==  2 );
	}
	
	/**
	 * Returns the value representing the value of the quaternion data
	 * (signed integer, maximum 32768).
	 *
	 * @param uiNumber the number of the data value, for which to return
	 * 	the quaternion data
	 * @return value representing the value of the quaternion data
	 * 	(signed integer, maximum 32768)
	 */
	inline long getQuaternion( const unsigned int uiNumber ) const {
		
		if ( ( uiNumber < 1 ) || ( 4 < uiNumber ) ) {
			return 0;
		}
		const unsigned int iType = getSamplingType();
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
	 * (signed integer, maximum 32768).
	 *
	 * @return value representing the value of the first quaternion data
	 * 	(signed integer, maximum 32768)
	 */
	inline long getQuaternion1() const {
		const unsigned int iType = getSamplingType();
		switch ( iType ) {
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
	 * (signed integer, maximum 32768).
	 *
	 * @return value representing the value of the second quaternion data
	 * 	(signed integer, maximum 32768)
	 */
	inline long getQuaternion2() const {
		
		const unsigned int iType = getSamplingType();
		switch ( iType ) {
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
	 * (signed integer, maximum 32768).
	 *
	 * @return value representing the value of the third quaternion data
	 * 	(signed integer, maximum 32768)
	 */
	inline long getQuaternion3() const {
		const unsigned int iType = getSamplingType();
		switch ( iType ) {
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
	 * (signed integer, maximum 32768).
	 *
	 * @return value representing the value of the fourth quaternion data
	 * 	(signed integer, maximum 32768)
	 */
	inline long getQuaternion4() const {
		
		const unsigned int iType = getSamplingType();
		switch ( iType ) {
			case 1:
			case 2: return read4ByteLong( 22 );
		};
		return 0;
	}

//TODO check

	
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
		
		const unsigned int iType = getSamplingType();
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
		
		const unsigned int iType = getSamplingType();
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
		
		const unsigned int iType = getSamplingType();
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
		
		const unsigned int iType = getSamplingType();
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
		
		const unsigned int iType = getSamplingType();
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
		
		const unsigned int iType = getSamplingType();
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
		
		const unsigned int iType = getSamplingType();
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
		
		const unsigned int iType = getSamplingType();
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
		
		const unsigned int iType = getSamplingType();
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
		
		const unsigned int iType = getSamplingType();
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
		
		const unsigned int iType = getSamplingType();
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
		
		const unsigned int iType = getSamplingType();
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
		
		const unsigned int iType = getSamplingType();
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
		
		const unsigned int iType = getSamplingType();
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
		
		const unsigned int iType = getSamplingType();
		switch ( iType ) {
			case 3:
			case 4: return read2ByteInt( 26 );
		};
		return 0;
	}
	
	
	/**
	 * This function checks if the message has changed.
	 * This message is compared with the given old message.
	 *
	 * @see szMessage
	 * @see uiMessageSize
	 * @param pOldMessageSamplingDataFromDataGlove a pointer to the old
	 * 	message
	 * @return true if the message has changed, else false
	 */
	inline bool hasChanged( const cMessageSamplingDataFromDataGlove *
			pOldMessageSamplingData ) const {
		
		return ( ( uiMessageSize != pOldMessageSamplingData->uiMessageSize ) ||
			( memcmp( szMessage,
				pOldMessageSamplingData->szMessage, uiMessageSize ) != 0 ) );
	}
	
	
	/**
	 * Returns the value of the message element of the given type.
	 *
	 * @see tTypeSamplingValue
	 * @param typeSamplingValue the type of the message value to return
	 * @return the message value of the mesage element of the given type
	 */
	inline long getValue( const tTypeSamplingValue typeSamplingValue ) const {
		
		switch ( typeSamplingValue ) {
			case BEGIN : return static_cast< long >('$');
			case TYPE_MSG : return getType();
			case LENGTH : return static_cast< long >( uiMessageSize - 3 );
			case TYPE  : return getSamplingType();
			case ID    : return getId();
			case CLOCK : return getClock();
			case STATUS   : return getStatus();
			case FINGER_1 : return getFinger1();
			case FINGER_2 : return getFinger2();
			case FINGER_3 : return getFinger3();
			case FINGER_4 : return getFinger4();
			case FINGER_5 : return getFinger5();
			case QUATERNION_1 : return getQuaternion1();
			case QUATERNION_2 : return getQuaternion2();
			case QUATERNION_3 : return getQuaternion3();
			case QUATERNION_4 : return getQuaternion4();
			case GYROSCOPE_X  : return getGyroscopeX();
			case GYROSCOPE_Y  : return getGyroscopeY();
			case GYROSCOPE_Z  : return getGyroscopeZ();
			case MAGNETOMETER_X  : return getMagnetometerX();
			case MAGNETOMETER_Y  : return getMagnetometerY();
			case MAGNETOMETER_Z  : return getMagnetometerZ();
			case ACCELEROMETER_X : return getAccelerometerX();
			case ACCELEROMETER_Y : return getAccelerometerY();
			case ACCELEROMETER_Z : return getAccelerometerZ();
			case END : return static_cast< long >('#');
			case CRC : //TODO?
			case UNKNOWN : return 0;
		};  //end switch typeSamplingValue
		return 0;
	}
	
	
	
	
	
private:
	/**
	 * A array for faster mapping from a byte to the type for the byte.
	 * 
	 * 	array index 1: the type of the message (0 ... 5)
	 * 	array index 2: the byte of the message
	 * 	array value: the type of the element in the message of the type
	 * 		of index 1 and on the byte of index 2
	 */
	static tTypeSamplingValue const mapByteToElementType[ 6 ][ 64 ];
	
	/**
	 * A array for faster mapping from a byte to the starting byte of the
	 * next element in the message.
	 *
	 * 	array index 1: the type of the message (0 ... 5)
	 * 	array index 2: the byte of the message
	 * 	array value: the index of byte of the next element in the message
	 * 		of type index 1 and after the element the byte of index 2
	 */
	static int const mapByteToNextElementByte[ 6 ][ 64 ];
	
public:
	
	/**
	 * This metod returns the value of the message element at the given
	 * message byte.
	 * Beware: iByte and iType won't be checked, don't give invalid values.
	 * 	The array mapByteToElementType will be used to map the iByte and
	 * 	iType to the message type.
	 *
	 * @see mapByteToElementType
	 * @param iByte the byte for which to return the message value
	 * @param iType the type the message has
	 * 	@see getSamplingType()
	 * @return the value, which stands in the sampling massage of type iType
	 * 	at the byte iByte
	 */
	inline long getElementValueForByte( const int iByte, const int iType ) const {
		
		return getValue( mapByteToElementType[ iType ][ iByte ] );
	}
	
	
	/**
	 * This metod returns the value of the message element at the given
	 * message byte.
	 * Beware: iByte and iType won't be checked, don't give invalid values.
	 * 	The array mapByteToElementType will be used to map the iByte and
	 * 	iType to the message type.
	 *
	 * @see mapByteToElementType
	 * @param iByte the byte for which to return the message value
	 * @param iType the type the message has
	 * 	@see getSamplingType()
	 * @return the value, which stands in the sampling massage of type iType
	 * 	at the byte iByte
	 */
	inline long getElementValueForByte( const int iByte ) const {
		
		return getValue( mapByteToElementType[ getSamplingType() ][ iByte ] );
	}
	
	
	/**
	 * This metod returns the value of the message element at the given
	 * message byte.
	 * Beware: iByte and iType won't be checked, don't give invalid values.
	 * 	The array mapByteToElementType will be used to map the iByte and
	 * 	iType to the message type.
	 *
	 * @see mapByteToElementType
	 * @see mapByteToNextElementByte
	 * @param iByte the byte for which to return the message value
	 * @param iType the type the message has
	 * 	@see getSamplingType()
	 * @return the pair, with as first the value, which stands in the sampling
	 * 	massage of type iType at the byte iByte and as second the byte of
	 * 	the next element in the message
	 */
	inline std::pair< long, int > getElementValueForByteAndNextByte(
			const int iByte, const int iType ) const {
		
		return std::pair< long, int >(
				getValue( mapByteToElementType[ iType ][ iByte ] ),
				mapByteToNextElementByte[ iType ][ iByte ]
			);
	}
	
	/**
	 * This metod returns the value of the message element at the given
	 * message byte.
	 * Beware: iByte and iType won't be checked, don't give invalid values.
	 * 	The array mapByteToElementType will be used to map the iByte and
	 * 	iType to the message type.
	 *
	 * @see mapByteToElementType
	 * @see mapByteToNextElementByte
	 * @param iByte the byte for which to return the message value
	 * @param iType the type the message has
	 * 	@see getSamplingType()
	 * @return the pair, with as first the value, which stands in the sampling
	 * 	massage of type iType at the byte iByte and as second the byte of
	 * 	the next element in the message
	 */
	inline std::pair< long, int > getElementValueForByteAndNextByte(
			const int iByte ) const {
		
		const int iType = getSamplingType();
		return std::pair< long, int >(
				getValue( mapByteToElementType[ iType ][ iByte ] ),
				mapByteToNextElementByte[ iType ][ iByte ]
			);
	}
	
	/**
	 * This method returns all sampling values, which differ to the old values.
	 * It will compare the samplin part of this and the other message and
	 * return all values, which are changed.
	 * Ignored elements: BEGIN, TYPE_MSG, LENGTH, CRC, END
	 *
	 * @param pOldMessageSampling a pointer to the message, to which compare
	 * 	this message (evalue the changed values for)
	 * @return a map with all the changes
	 * 	key: the type of the changed element
	 * 	value: the new value for the element
	 */
	inline std::map< tTypeSamplingValue, long > getChangeTo(
			const cMessageSamplingDataFromDataGlove * pOldMessageSampling ) const {
		std::map< tTypeSamplingValue, long > mapChangedValues;
		
		if ( pOldMessageSampling == NULL ) {
			//no old message -> nothing changed
			return mapChangedValues;
		}
		
		const int iType = getSamplingType();
		const int iOldType = pOldMessageSampling->getSamplingType();
		if ( iType != iOldType ) {
			//the message type changed -> check all elements with there values
			mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
				TYPE, iType ) );
			
			if ( getId() != pOldMessageSampling->getId() ) {
				mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
					ID, getId() ) );
			}
			if ( getClock() != pOldMessageSampling->getClock() ) {
				mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
					CLOCK, getClock() ) );
			}
			if ( getStatus() != pOldMessageSampling->getStatus() ) {
				mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
					STATUS, getStatus() ) );
			}
			if ( getFinger1() != pOldMessageSampling->getFinger1() ) {
				mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
					FINGER_1, getFinger1() ) );
			}
			if ( getFinger2() != pOldMessageSampling->getFinger2() ) {
				mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
					FINGER_2, getFinger2() ) );
			}
			if ( getFinger3() != pOldMessageSampling->getFinger3() ) {
				mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
					FINGER_3, getFinger3() ) );
			}
			if ( getFinger4() != pOldMessageSampling->getFinger4() ) {
				mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
					FINGER_4, getFinger4() ) );
			}
			if ( getFinger5() != pOldMessageSampling->getFinger5() ) {
				mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
					FINGER_5, getFinger5() ) );
			}
			
			if ( getQuaternion1() != pOldMessageSampling->getQuaternion1() ) {
				mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
					QUATERNION_1, getQuaternion1() ) );
			}
			if ( getQuaternion2() != pOldMessageSampling->getQuaternion2() ) {
				mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
					QUATERNION_2, getQuaternion2() ) );
			}
			if ( getQuaternion3() != pOldMessageSampling->getQuaternion3() ) {
				mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
					QUATERNION_3, getQuaternion3() ) );
			}
			if ( getQuaternion4() != pOldMessageSampling->getQuaternion4() ) {
				mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
					QUATERNION_4, getQuaternion4() ) );
			}
			
			if ( getGyroscopeX() != pOldMessageSampling->getGyroscopeX() ) {
				mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
					GYROSCOPE_X, getGyroscopeX() ) );
			}
			if ( getGyroscopeY() != pOldMessageSampling->getGyroscopeY() ) {
				mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
					GYROSCOPE_Y, getGyroscopeY() ) );
			}
			if ( getGyroscopeZ() != pOldMessageSampling->getGyroscopeZ() ) {
				mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
					GYROSCOPE_Z, getGyroscopeZ() ) );
			}
			
			if ( getMagnetometerX() != pOldMessageSampling->getMagnetometerX() ) {
				mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
					MAGNETOMETER_X, getMagnetometerX() ) );
			}
			if ( getMagnetometerY() != pOldMessageSampling->getMagnetometerY() ) {
				mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
					MAGNETOMETER_Y, getMagnetometerY() ) );
			}
			if ( getMagnetometerZ() != pOldMessageSampling->getMagnetometerZ() ) {
				mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
					MAGNETOMETER_Z, getMagnetometerZ() ) );
			}
			
			if ( getAccelerometerX() != pOldMessageSampling->getAccelerometerX() ) {
				mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
					ACCELEROMETER_X, getAccelerometerX() ) );
			}
			if ( getAccelerometerY() != pOldMessageSampling->getAccelerometerY() ) {
				mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
					ACCELEROMETER_Y, getAccelerometerY() ) );
			}
			if ( getAccelerometerZ() != pOldMessageSampling->getAccelerometerZ() ) {
				mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
					ACCELEROMETER_Z, getAccelerometerZ() ) );
			}
			return mapChangedValues;
		}  //else the sampling message type is the same
		const int iMaxIndex = uiMessageSize - 2;
		for ( int iItrByte = 4; iItrByte < iMaxIndex; ) {
			
			if ( szMessage[ iItrByte ] == pOldMessageSampling->szMessage[ iItrByte ] ) {
				iItrByte++;  //check next char
			} else {  //value changed
				const std::pair< long, int > paChangedElementType =
					getElementValueForByteAndNextByte( iItrByte, iType );
				mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
					mapByteToElementType[ iType ][ iItrByte ],
					paChangedElementType.first ) );
				//check next value
				iItrByte = paChangedElementType.second;
			}
		}
		return mapChangedValues;
	}
	
	
	/**
	 * This method returns a map of all sampling values.
	 * Ignored elements: BEGIN, TYPE_MSG, LENGTH, CRC, END
	 *
	 * @return a map with all sampling values
	 * 	key: the type of the changed element
	 * 	value: the new value for the element
	 */
	inline std::map< tTypeSamplingValue, long > getAllSamplingData() const {
			std::map< tTypeSamplingValue, long > mapChangedValues;
		
		mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
			TYPE, getSamplingType() ) );
		
		mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
			ID, getId() ) );
		mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
			CLOCK, getClock() ) );
		mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
			STATUS, getStatus() ) );
		mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
			FINGER_1, getFinger1() ) );
		mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
			FINGER_2, getFinger2() ) );
		mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
			FINGER_3, getFinger3() ) );
		mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
			FINGER_4, getFinger4() ) );
		mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
			FINGER_5, getFinger5() ) );
		mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
			QUATERNION_1, getQuaternion1() ) );
		mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
			QUATERNION_2, getQuaternion2() ) );
		mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
			QUATERNION_3, getQuaternion3() ) );
		mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
			QUATERNION_4, getQuaternion4() ) );
		mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
			GYROSCOPE_X, getGyroscopeX() ) );
		mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
			GYROSCOPE_Y, getGyroscopeY() ) );
		mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
			GYROSCOPE_Z, getGyroscopeZ() ) );
		mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
			MAGNETOMETER_X, getMagnetometerX() ) );
		mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
			MAGNETOMETER_Y, getMagnetometerY() ) );
		mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
			MAGNETOMETER_Z, getMagnetometerZ() ) );
		mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
			ACCELEROMETER_X, getAccelerometerX() ) );
		mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
			ACCELEROMETER_Y, getAccelerometerY() ) );
		mapChangedValues.insert( std::pair< tTypeSamplingValue, long >(
			ACCELEROMETER_Z, getAccelerometerZ() ) );
		return mapChangedValues;
	}
	
	
};//end class cMessageSamplingDataFromDataGlove


};//end namespace nDataGlove
};//end namespace nModelDataGloveDGTechVHand


#endif //___N_DATA_GLOVE__N_MODEL_DATA_GLOVE_D_G_TECH_V_HAND__C_MESSAGE_SAMPLING_DATA_FROM_DATA_GLOVE_H__





