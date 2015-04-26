/**
 * @file cMessageSamplingDataFromDataGlove
 * file name: cMessageSamplingDataFromDataGlove.cpp
 * @author Betti Oesterholz
 * @date 13.07.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file implements the class for the DGTech VHand data glove
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
 * This file implements the class for the DGTech VHand data glove
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



#include "cMessageSamplingDataFromDataGlove.h"


using namespace nDataGlove::nModelDataGloveDGTechVHand;
using namespace std;


/**
 * The types of the message bytes.
 * For every byte in the message an entry with the type of the element for
 * the byte is given.
 * (For every message type there is an entry.)
 */
cMessageSamplingDataFromDataGlove::tTypeSamplingValue const
	cMessageSamplingDataFromDataGlove::mapByteToElementType[ 6 ][ 64 ] = {
		//sampling message type 0
		{ BEGIN, TYPE_MSG, LENGTH, TYPE, ID, ID, CLOCK, CLOCK, CLOCK, CLOCK,
			STATUS, STATUS, CRC, END
		},
		//sampling message type 1
		{ BEGIN, TYPE_MSG, LENGTH, TYPE, ID, ID, CLOCK, CLOCK, CLOCK, CLOCK,
			QUATERNION_1, QUATERNION_1, QUATERNION_1, QUATERNION_1,
			QUATERNION_2, QUATERNION_2, QUATERNION_2, QUATERNION_2,
			QUATERNION_3, QUATERNION_3, QUATERNION_3, QUATERNION_3,
			QUATERNION_4, QUATERNION_4, QUATERNION_4, QUATERNION_4,
			FINGER_1, FINGER_1,
			FINGER_2, FINGER_2,
			FINGER_3, FINGER_3,
			FINGER_4, FINGER_4,
			FINGER_5, FINGER_5,
			STATUS, STATUS, CRC, END
		},
		//sampling message type 2
		{ BEGIN, TYPE_MSG, LENGTH, TYPE, ID, ID, CLOCK, CLOCK, CLOCK, CLOCK,
			QUATERNION_1, QUATERNION_1, QUATERNION_1, QUATERNION_1,
			QUATERNION_2, QUATERNION_2, QUATERNION_2, QUATERNION_2,
			QUATERNION_3, QUATERNION_3, QUATERNION_3, QUATERNION_3,
			QUATERNION_4, QUATERNION_4, QUATERNION_4, QUATERNION_4,
			STATUS, STATUS, CRC, END
		},
		//sampling message type 3
		{ BEGIN, TYPE_MSG, LENGTH, TYPE, ID, ID, CLOCK, CLOCK, CLOCK, CLOCK,
			GYROSCOPE_X, GYROSCOPE_X,
			GYROSCOPE_Y, GYROSCOPE_Y,
			GYROSCOPE_Z, GYROSCOPE_Z,
			MAGNETOMETER_X, MAGNETOMETER_X,
			MAGNETOMETER_Y, MAGNETOMETER_Y,
			MAGNETOMETER_Z, MAGNETOMETER_Z,
			ACCELEROMETER_X, ACCELEROMETER_X,
			ACCELEROMETER_Y, ACCELEROMETER_Y,
			ACCELEROMETER_Z, ACCELEROMETER_Z,
			FINGER_1, FINGER_1,
			FINGER_2, FINGER_2,
			FINGER_3, FINGER_3,
			FINGER_4, FINGER_4,
			FINGER_5, FINGER_5,
			STATUS, STATUS, CRC, END
		},
		//sampling message type 4
		{ BEGIN, TYPE_MSG, LENGTH, TYPE, ID, ID, CLOCK, CLOCK, CLOCK, CLOCK,
			GYROSCOPE_X, GYROSCOPE_X,
			GYROSCOPE_Y, GYROSCOPE_Y,
			GYROSCOPE_Z, GYROSCOPE_Z,
			MAGNETOMETER_X, MAGNETOMETER_X,
			MAGNETOMETER_Y, MAGNETOMETER_Y,
			MAGNETOMETER_Z, MAGNETOMETER_Z,
			ACCELEROMETER_X, ACCELEROMETER_X,
			ACCELEROMETER_Y, ACCELEROMETER_Y,
			ACCELEROMETER_Z, ACCELEROMETER_Z,
			STATUS, STATUS, CRC, END
		},
		//sampling message type 4
		{ BEGIN, TYPE_MSG, LENGTH, TYPE, ID, ID, CLOCK, CLOCK, CLOCK, CLOCK,
			FINGER_1, FINGER_1,
			FINGER_2, FINGER_2,
			FINGER_3, FINGER_3,
			FINGER_4, FINGER_4,
			FINGER_5, FINGER_5,
			STATUS, STATUS, CRC, END
		}
	};  //end mapByteToElementType


/**
 * This map indicates the beginning byte of the next elements at the given
 * byte.
 * Some message elements need more than one byte. If you want to evaluate,
 * where the next message element begins, look at the byte in this map.
 * (For every message type there is an entry.)
 */
int const cMessageSamplingDataFromDataGlove::mapByteToNextElementByte[ 6 ][ 64 ] =
	{
		//sampling message type 0
		{ 1, 2, 3, 4, 6, 6, 10, 10, 10, 10,
			12, 12, 13 },
		//sampling message type 1
		{ 1, 2, 3, 4, 6, 6, 10, 10, 10, 10,
			14, 14, 14, 14,  //QUATERNION_1
			18, 18, 18, 18,  //QUATERNION_2
			22, 22, 22, 22,  //QUATERNION_3
			26, 26, 26, 26,  //QUATERNION_4
			28, 28,  //FINGER_1
			30, 30,  //FINGER_2
			32, 32,  //FINGER_3
			34, 34,  //FINGER_4
			36, 36,  //FINGER_5
			38, 38, 39
		},
		//sampling message type 2
		{ 1, 2, 3, 4, 6, 6, 10, 10, 10, 10,
			14, 14, 14, 14,  //QUATERNION_1
			18, 18, 18, 18,  //QUATERNION_2
			22, 22, 22, 22,  //QUATERNION_3
			26, 26, 26, 26,  //QUATERNION_4
			28, 28, 29
		},
		//sampling message type 3
		{ 1, 2, 3, 4, 6, 6, 10, 10, 10, 10,
			12, 12,  //GYROSCOPE_X
			14, 14,  //GYROSCOPE_Y
			16, 16,  //GYROSCOPE_Z
			18, 18,  //MAGNETOMETER_X
			20, 20,  //MAGNETOMETER_Y
			22, 22,  //MAGNETOMETER_Z
			24, 24,  //ACCELEROMETER_X
			26, 26,  //ACCELEROMETER_Y
			28, 28,  //ACCELEROMETER_Z
			30, 30,  //FINGER_1
			32, 32,  //FINGER_2
			34, 34,  //FINGER_3
			36, 36,  //FINGER_4
			38, 38,  //FINGER_5
			40, 40, 41
		},
		//sampling message type 4
		{ 1, 2, 3, 4, 6, 6, 10, 10, 10, 10,
			12, 12,  //GYROSCOPE_X
			14, 14,  //GYROSCOPE_Y
			16, 16,  //GYROSCOPE_Z
			18, 18,  //MAGNETOMETER_X
			20, 20,  //MAGNETOMETER_Y
			22, 22,  //MAGNETOMETER_Z
			24, 24,  //ACCELEROMETER_X
			26, 26,  //ACCELEROMETER_Y
			28, 28,  //ACCELEROMETER_Z
			30, 30, 31
		},
		//sampling message type 5
		{ 1, 2, 3, 4, 6, 6, 10, 10, 10, 10,
			12, 12,  //FINGER_1
			14, 14,  //FINGER_2
			16, 16,  //FINGER_3
			18, 18,  //FINGER_4
			20, 20,  //FINGER_5
			22, 22, 23
		}
	};  //end mapByteToNextElementByte


/**
 * The standard constructor for the DGTech VHand data glove message.
 */
cMessageSamplingDataFromDataGlove::cMessageSamplingDataFromDataGlove() {
	
	type = SAMPLING_DATA;
	cCommand = DATA_GLOVE_D_G_TECH_V_HAND__CMD_START_SAMPLING;
}


/**
 * The destructor.
 */
cMessageSamplingDataFromDataGlove::~cMessageSamplingDataFromDataGlove() {
	
	//nothing to do
}


/**
 * @return the name of this class "cMessageSamplingDataFromDataGlove"
 */
std::string cMessageSamplingDataFromDataGlove::getName() const {
	
	return string( "cMessageSamplingDataFromDataGlove" );
}










