
//TODO check

/**
 * @file cMessageGloveStates
 * file name: cMessageGloveStates.cpp
 * @author Betti Oesterholz
 * @date 23.01.2016
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file implements a class for a message of the actual data glove state.
 *
 *
 * Copyright (C) @c GPL3 2016 Betti Oesterholz
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
 * This file implements a class for a message of the actual data glove state.
 * It also send the neighbour states if existing.
 *
 * @see cThreadMessageHandler
 * @see cMessageDataGlove
 * @see cEvaluateDataGloveState
 * @see cMessageSamplingDataFromDataGlove
 */
/*
History:
23.01.2016  Oesterholz  created
*/


#include "nMessageGloveStates_cMessageGloveDigit.h"

#ifdef USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES
	#include <boost/archive/binary_oarchive.hpp>
	#include <boost/archive/binary_iarchive.hpp>
	//include this header to serialize vectors
	#include <boost/serialization/vector.hpp>

	using namespace boost::archive;
#endif  // USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES


using namespace nDataGlove::nMapper::nMessageGloveStates;
using namespace nDataGlove::nMapper;
using namespace std;




/**
 * The standard constructor for data glove state digit message.
 *
 * @param inDigitTyp Typ of this digit.
 * 	@see DigitTyp
 * @param inDigitName Name of this digit.
 * 	@see DigitName
 * @param inValue Actual data glove value for this digit.
 * 	@see Value
 */
cMessageGloveDigit::cMessageGloveDigit( const int inDigitTyp,
		const string inDigitName, const int inValue ) :
		DigitTyp( inDigitTyp ), DigitName( inDigitName ), Value( inValue ),
		Correction( 0 ) {
	
	IndexActiveNeighbour = -1;
}


/**
 * The copy constructor for data glove state digit message.
 *
 * @param gloveDigit the to copy data glove state digit message
 *
 */
cMessageGloveDigit::cMessageGloveDigit( const cMessageGloveDigit & gloveDigit ) :
		DigitTyp( gloveDigit.DigitTyp ), DigitName( gloveDigit.DigitName ),
		Value( gloveDigit.Value ), Correction( gloveDigit.Correction ),
		VecNeighbourStates( gloveDigit.VecNeighbourStates ),
		IndexActiveNeighbour( gloveDigit.IndexActiveNeighbour ){
	//nothing to do
}


/**
 * destructor
 */
cMessageGloveDigit::~cMessageGloveDigit() {
	
	//nothing to do
}



/**
 * This method clones this data glove state digit message.
 *
 */
cMessageGloveDigit * cMessageGloveDigit::clone() {
	
	return new cMessageGloveDigit( *this );
}



/**
 * @return the name of this class "cMessageGloveDigit"
 */
std::string cMessageGloveDigit::getName() const {
	
	return "cMessageGloveDigit";
}


/**
 * Checks if this cMessageGloveDigit is equal to the given cMessageGloveDigit
 * @param inDigitState the to compare cMessageGloveDigit
 * @return true if this cMessageGloveDigit is equal to the given cMessageGloveDigit
 */
bool cMessageGloveDigit::equal( const cMessageGloveDigit & inDigitState ) const {
	
	return ( DigitTyp == inDigitState.DigitTyp ) &&
		( DigitName == inDigitState.DigitName ) &&
		( Value == inDigitState.Value ) &&
		( Correction == inDigitState.Correction ) &&
		( VecNeighbourStates == inDigitState.VecNeighbourStates ) &&
		( IndexActiveNeighbour == inDigitState.IndexActiveNeighbour );
}


/**
 * This method stores this message into the given stream.
 *
 * @param stream the stream where this message should be stored to
 * @return true if this message is stored, else false
 */
bool cMessageGloveDigit::store( ostream & inStream ) const {
	
#ifdef USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES
	try {
		binary_oarchive outputArchive{ inStream };
		outputArchive << (*this);
		
	} catch ( archive_exception ) {
		
		return false;
	}
	return true;
}
#else  // USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES
	cByteOStream byteOStream( inStream );
	const bool bStoreOk = store( byteOStream );
	
	if ( bStoreOk && byteOStream.good() ) {
		byteOStream.write_end();
		byteOStream.flush();
	}
	return bStoreOk && byteOStream.good();
}


/**
 * This method stores this message into the given stream.
 *
 * @param stream the stream where this message should be stored to
 * @return true if this message is stored, else false
 */
bool cMessageGloveDigit::store( cByteOStream & inStream ) const {
	const char * pChar;
	//write 2 B digit Type
	pChar = ((const char *)(&DigitTyp));
	inStream.write( pChar, 2 );
	
	//2 B: digit name legth
	unsigned int uiBytesToWrite = DigitName.length();
	if ( 0xFFFF < uiBytesToWrite ) {
		//write max length of 2^16-1
		uiBytesToWrite = 0xFFFF;
	}
	pChar = ((const char *)(&uiBytesToWrite));
	inStream.write( pChar, 2 );
	//write legth B: digit name string
	pChar = DigitName.c_str();
	inStream.write( pChar, uiBytesToWrite );
	
	//write 4 B digit value
	pChar = ((const char *)(&Value));
	inStream.write( pChar, 4 );
	
	//write 4 B digit correction
	pChar = ((const char *)(&Correction));
	inStream.write( pChar, 4 );
	
	
	//write 1 B: index active neigbour
	pChar = ((const char *)(&IndexActiveNeighbour));
	inStream.write( pChar, 1 );
	
	//1 B: unsigned number neigbour states
	unsigned int uiStatesToWrite = VecNeighbourStates.size();
	
	if ( MAX_LOAD_NUMBER_VECTOR_STATES < uiStatesToWrite ) {
		//don't write more than MAX_LOAD_NUMBER_VECTOR_STATES states
		uiStatesToWrite = MAX_LOAD_NUMBER_VECTOR_STATES;
	}
	if ( 255 < uiStatesToWrite ) {
		//don't write more than 255 states
		uiStatesToWrite = 255;
	}
	pChar = ((const char *)(&uiStatesToWrite));
	inStream.write( pChar, 1 );
	//store the states
	bool bReturnValue = true;
	for ( std::vector< cDigitState >::const_iterator
			itrState = VecNeighbourStates.begin();
			itrState != VecNeighbourStates.end(); ++itrState ) {
		if ( uiStatesToWrite <= 0 ) {
			break;
		}
		--uiStatesToWrite;
		bReturnValue = bReturnValue && itrState->store( inStream );
	}
	return bReturnValue && inStream.good();
}
#endif  // USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES


/**
 * This method restores the message from the stream.
 *
 * @param stream the stream where this message is stored to in
 * @param outStatus An pointer to an integervalue where the error value
 * 	can be stored to. If the pointer is nullptr no error value will be
 * 	given back.
 * 	possible errorvalues are:
 * 		- 0  loading successful
 * 		- -1 loading error, invalid stream
 * 		- -2 loading error, invalid data in stream
 * 		- 1 loading warning, invalid data in stream, error could be corrected
 * 		- 2 loading warning, invalid data in stream, maybe the loaded
 * 			object is wrong
 * @return the readed message or the Null pointer nullptr, if reading
 * 	was not possible
 */
cMessageGloveDigit * cMessageGloveDigit::restore(
		istream & inStream, int * outStatus ) {
	
	cMessageGloveDigit * pNewGloveDigit = new cMessageGloveDigit();
	
#ifdef USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES
	try {
		binary_iarchive inputArchive{ inStream };
		inputArchive >> (*pNewGloveDigit);
		
	} catch ( boost::archive::archive_exception& archivException ) {
		
		if ( outStatus != nullptr ) {
			switch ( archivException.code ) {
			
				case archive_exception::exception_code::unregistered_class: {
					// attempt to serialize a pointer of an an unregistered class
					(*outStatus) = -2;
					break;
				};
				case archive_exception::exception_code::invalid_signature: {
					// first line of archive does not contain expected string
					(*outStatus) = -2;
					break;
				};
				case archive_exception::exception_code::unsupported_version: {
					// archive created with library version subsequent to this one
					(*outStatus) = -2;
					break;
				};
				case archive_exception::exception_code::pointer_conflict: {
					// an attempt has been made to directly serialize
					// an object after having already serialized the same
					// object through a pointer
					(*outStatus) = -2;
					break;
				};
				case archive_exception::exception_code::incompatible_native_format: {
					// attempt to read native binary format on incompatible platform
					(*outStatus) = -2;
					break;
				};
				case archive_exception::exception_code::array_size_too_short: {
					// array being loaded doesn't fit in array allocated
					(*outStatus) = -2;
					break;
				};
				case archive_exception::exception_code::invalid_class_name: {
					// class name greater than the maximum permitted.
					// most likely a corrupted archive or an attempt
					// to insert virus via buffer overrun method.
					(*outStatus) = -2;
					break;
				};
				case archive_exception::exception_code::unregistered_cast: {
					// base - derived relationship not registered with 
					// void_cast_register
					(*outStatus) = -2;
					break;
				};
				case archive_exception::exception_code::unsupported_class_version: {
					// type saved with a version # greater than the 
					// one used by the program.  This indicates that the proggram
					// needs to be rebuilt.
					(*outStatus) = -2;
					break;
				};
/*TODO not defined?
				case archive_exception::exception_code::stream_error: {
					// i/o error on stream
					(*outStatus) = -1;
					break;
				};
*/
				default: {
					(*outStatus) = -1;
					break;
				};
			};
		}
	}
#else //USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES
	const int readStatus = pNewGloveDigit->read( inStream );
	
	if ( outStatus != nullptr ) {
		(*outStatus) = readStatus;
	}
	if ( readStatus < 0 ) {
		//error occured while reading -> return nullptr
		delete pNewGloveDigit;
		return nullptr;
	}
#endif //USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES
	return pNewGloveDigit;
}

/**
 * This method reads the message from the stream into this object.
 *
 * @param stream the stream where this message is stored to in
 * @return An integervalue for the error value
 * 	possible errorvalues are:
 * 		- 0  loading successful
 * 		- -1 loading error, invalid stream
 * 		- -2 loading error, invalid data in stream
 * 		- 1 loading warning, invalid data in stream, error could be corrected
 * 		- 2 loading warning, invalid data in stream, maybe the loaded
 * 			object is wrong
 */
int cMessageGloveDigit::read( istream & inStream ) {
	
#ifdef USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES
	try {
		binary_iarchive inputArchive{ inStream };
		inputArchive >> (*this);
		
	} catch ( boost::archive::archive_exception& archivException ) {
		
		switch ( archivException.code ) {
		
			case archive_exception::exception_code::unregistered_class: {
				// attempt to serialize a pointer of an an unregistered class
				return -2;
			};
			case archive_exception::exception_code::invalid_signature: {
				// first line of archive does not contain expected string
				return -2;
			};
			case archive_exception::exception_code::unsupported_version: {
				// archive created with library version subsequent to this one
				return -2;
			};
			case archive_exception::exception_code::pointer_conflict: {
				// an attempt has been made to directly serialize
				// an object after having already serialized the same
				// object through a pointer
				return -2;
			};
			case archive_exception::exception_code::incompatible_native_format: {
				// attempt to read native binary format on incompatible platform
				return -2;
			};
			case archive_exception::exception_code::array_size_too_short: {
				// array being loaded doesn't fit in array allocated
				return -2;
			};
			case archive_exception::exception_code::invalid_class_name: {
				// class name greater than the maximum permitted.
				// most likely a corrupted archive or an attempt
				// to insert virus via buffer overrun method.
				return -2;
			};
			case archive_exception::exception_code::unregistered_cast: {
				// base - derived relationship not registered with 
				// void_cast_register
				return -2;
			};
			case archive_exception::exception_code::unsupported_class_version: {
				// type saved with a version # greater than the 
				// one used by the program.  This indicates that the proggram
				// needs to be rebuilt.
				return -2;
			};
/*TODO not defined?
			case archive_exception::exception_code::stream_error: {
				// i/o error on stream
				(*outStatus) = -1;
				break;
			};
*/
			default: {
				return -1;
			};
		};
	}
	return 0;
}
#else //USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES
	cByteIStream byteIStream( inStream );
	int iReadOk = read( byteIStream );
	
	if ( ! byteIStream.good() ) {
		if ( iReadOk != 0 ) {
			return iReadOk;
		}
		return -2;
	}
	
	if ( 0 <= iReadOk) {
		//try to read the transfer block end
		if ( ! byteIStream.read_end() ) {
			//error while reading transfer block end
			if ( iReadOk == 0 ) {
				iReadOk = 2;
			}
		}
		
		if ( byteIStream.dataError() && ( iReadOk == 0 ) ) {
			iReadOk = 2;
		}
	}
	if ( ! byteIStream.good() ) {
		if ( iReadOk != 0 ) {
			return iReadOk;
		}
		return -2;
	}
	return iReadOk;
}



/**
 * This method reads the message from the stream into this object.
 *
 * @param stream the stream where this message is stored to in
 * @return An integervalue for the error value
 * 	possible errorvalues are:
 * 		- 0  loading successful
 * 		- -1 loading error, invalid stream
 * 		- -2 loading error, invalid data in stream
 * 		- 1 loading warning, invalid data in stream, error could be corrected
 * 		- 2 loading warning, invalid data in stream, maybe the loaded
 * 			object is wrong
 */
int cMessageGloveDigit::read( cByteIStream & inStream ) {
	
	if ( ! inStream.good() ) {
		return -1;
	}
	
	char * pBuffer = new char[ 256 ];
	//read 2 Byte digit type
	inStream.read( pBuffer, 2 );
	
	if ( ! inStream.good() ) {
		delete[] pBuffer;
		return -2;
	}
	DigitTyp = 0;
	((char *)(&DigitTyp))[ 0 ] = pBuffer[ 0 ];
	((char *)(&DigitTyp))[ 1 ] = pBuffer[ 1 ];
	
	
	//2 Byte: digit name legth
	inStream.read( pBuffer, 2 );
	if ( ! inStream.good() ) {
		delete[] pBuffer;
		return -2;
	}
	unsigned int uiBytesToRead = 0;
	((char *)(&uiBytesToRead))[ 0 ] = pBuffer[ 0 ];
	((char *)(&uiBytesToRead))[ 1 ] = pBuffer[ 1 ];
	
	//read legth Byte: state name string
	DigitName = "";
	//read blocks with 256 chars each
	unsigned int uiBytesReaded = 256;
	while ( 0 < uiBytesToRead ) {
		if ( uiBytesToRead < uiBytesReaded ) {
			uiBytesReaded = uiBytesToRead;
		}
		inStream.read( pBuffer, uiBytesReaded );
		
		if ( ! inStream.good() ) {
			delete[] pBuffer;
			return -2;
		}
		DigitName.append( pBuffer, uiBytesReaded );
		uiBytesToRead -= uiBytesReaded;
	}
	
	//read 4 B digit value
	inStream.read( pBuffer, 4 );
	
	if ( ! inStream.good() ) {
		delete[] pBuffer;
		return -2;
	}
	if ( (pBuffer[ 3 ] & 0x80) == 0 ) {
		//first bit is 0 -> positiv value
		Value = 0;
	} else {  //first bit is 1 -> negativ value
		Value = -1;
	}
	((char *)(&Value))[ 0 ] = pBuffer[ 0 ];
	((char *)(&Value))[ 1 ] = pBuffer[ 1 ];
	((char *)(&Value))[ 2 ] = pBuffer[ 2 ];
	((char *)(&Value))[ 3 ] = pBuffer[ 3 ];
	
	//read 4 B digit correction
	inStream.read( pBuffer, 4 );
	
	if ( ! inStream.good() ) {
		delete[] pBuffer;
		return -2;
	}
	if ( (pBuffer[ 3 ] & 0x80) == 0 ) {
		//first bit is 0 -> positiv correction
		Correction = 0;
	} else {  //first bit is 1 -> negativ correction
		Correction = -1;
	}
	((char *)(&Correction))[ 0 ] = pBuffer[ 0 ];
	((char *)(&Correction))[ 1 ] = pBuffer[ 1 ];
	((char *)(&Correction))[ 2 ] = pBuffer[ 2 ];
	((char *)(&Correction))[ 3 ] = pBuffer[ 3 ];
	
	
	//read 2 B  index active neigbour
	inStream.read( pBuffer, 1 );
	
	if ( ! inStream.good() ) {
		delete[] pBuffer;
		return -2;
	}
	IndexActiveNeighbour = 0;
	((char *)(&IndexActiveNeighbour))[ 0 ] = pBuffer[ 0 ];
	if ( IndexActiveNeighbour == 255 ) {
		//-1 was stored
		IndexActiveNeighbour = -1;
	}
	
	//read 1 B: unsigned number neigbour states
	inStream.read( pBuffer, 1 );
	
	if ( ! inStream.good() ) {
		delete[] pBuffer;
		return -2;
	}
	unsigned int uiStatesToRead = 0;
	((char *)(&uiStatesToRead))[ 0 ] = pBuffer[ 0 ];
	
	int iReturnValue = 0;
	if ( MAX_LOAD_NUMBER_VECTOR_STATES < uiStatesToRead ) {
		//don't read more than MAX_LOAD_NUMBER_VECTOR_STATES states
		uiStatesToRead = MAX_LOAD_NUMBER_VECTOR_STATES;
		iReturnValue = 2;
	}
	if ( 255 < uiStatesToRead ) {
		//don't read more than 255 states
		uiStatesToRead = 255;
		iReturnValue = 2;
	}
	//read the neigbour states
	int iSubReturnValue = 0;
	
	VecNeighbourStates.resize(uiStatesToRead );
	for ( unsigned int uiIndexState = 0; uiIndexState < uiStatesToRead;
			++uiIndexState ) {
		
		iSubReturnValue = VecNeighbourStates[ uiIndexState ].read( inStream );
	
		if ( iSubReturnValue != 0 ) {
			//data of the subelement not readed correctly
			if ( iSubReturnValue < 0 ) {
				//reading error
				iReturnValue = iSubReturnValue;
				break;
			}  //else ( 0 < iSubReturnValue ) -> warning
			if ( iReturnValue == 0 ) {
				iReturnValue = iSubReturnValue;
			}
		}
	}
	delete[] pBuffer;
	
	if ( ! inStream.good() ) {
		return -2;
	}
	return iReturnValue;
}
#endif //USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES


/**
 * This method prints this message into the given stream.
 * The output is in a human readable form.
 *
 * @param stream the stream where this message should be printed to
 * @return true if this message is printed, else false
 */
void cMessageGloveDigit::print( std::ostream & inStream ) const {
	
	inStream<<DigitTyp<<"."<<getDigitTypName()<<"->"<<DigitName<<"=";
	if ( Correction == 0 ) {
		inStream<<Value;
	}else{
		inStream<<Value<<"+"<<Correction<<"="<<(Value+Correction);
	}
	inStream<<" index: "<<IndexActiveNeighbour<<" ->"<<std::endl;
	for ( const auto & itrState : VecNeighbourStates ) {
		
		itrState.print( inStream );
	}
}


/**
 * @see DigitTyp
 * @return Typ of this digit.
 *
 * Types of digits:
 * 	1-5 digits left hand 1=pinky, ..., 5=thumb
 * 	6-10 digits right hand 1=thumb, ..., 5=pinky
 * 	11-15 quaterion left hand
 * 	16-20 quaterion right hand
 * 	21-25 gyroscop left hand: instantaneous rotation of the hand
 * 	26-30 gyroscop right hand: instantaneous rotation of the hand
 * 	31-35 accelerometer left hand
 * 	36-40 accelerometer right hand
 * 	41-45 magnetometer left hand
 * 	46-50 magnetometer right hand
 */
int cMessageGloveDigit::getDigitTyp() const {
	
	return DigitTyp;
}


/**
 * @see DigitTyp
 * @return a name for the typ of this digit.
 *
 * Types of digits:
 * 	1-5 digits left hand 1=pinky, ..., 5=thumb
 * 	6-10 digits right hand 1=thumb, ..., 5=pinky
 * 	11-15 quaterion left hand
 * 	16-20 quaterion right hand
 * 	21-25 gyroscop left hand: instantaneous rotation of the hand
 * 	26-30 gyroscop right hand: instantaneous rotation of the hand
 * 	31-35 accelerometer left hand
 * 	36-40 accelerometer right hand
 * 	41-45 magnetometer left hand
 * 	46-50 magnetometer right hand
 */
string cMessageGloveDigit::getDigitTypName() const {
	
	switch ( DigitTyp ) {
		//finger
		case 1: return "left pinky";
		case 2: return "left index";
		case 3: return "left middle";
		case 4: return "left ring";
		case 5: return "left thumb";
		case 6: return "right thumb";
		case 7: return "right ring";
		case 8: return "right middle";
		case 9: return "right index";
		case 10: return "right pinky";
		//quaterion
		case 11:
		case 12:
		case 13:
		case 14:
		case 15: return string("quaterion left ").append( std::to_string( DigitTyp - 10 ) );
		case 16:
		case 17:
		case 18:
		case 19:
		case 20: return string("quaterion right ").append( std::to_string( DigitTyp - 15 ) );
		//gyroscop
		case 21:
		case 22:
		case 23:
		case 24:
		case 25: return string("gyroscop left ").append( std::to_string( DigitTyp - 20 ) );
		case 26:
		case 27:
		case 28:
		case 29:
		case 30: return string("gyroscop right ").append( std::to_string( DigitTyp - 25 ) );
		//accelerometer
		case 31:
		case 32:
		case 33:
		case 34:
		case 35: return string("accelerometer left ").append( std::to_string( DigitTyp - 30 ) );
		case 36:
		case 37:
		case 38:
		case 39:
		case 40: return string("accelerometer right ").append( std::to_string( DigitTyp - 35 ) );
		//magnetometer
		case 41:
		case 42:
		case 43:
		case 44:
		case 45: return string("magnetometer left ").append( std::to_string( DigitTyp - 40 ) );
		case 46:
		case 47:
		case 48:
		case 49:
		case 50: return string("magnetometer right ").append( std::to_string( DigitTyp - 45 ) );
		
	};
	return "unknown";
}


/**
 * @see DigitTyp
 * @param inDigitTyp Typ of this digit.
 *
 * Types of digits:
 * 	1-5 digits left hand 1=pinky, ..., 5=thumb
 * 	6-10 digits right hand 1=thumb, ..., 5=pinky
 * 	11-15 quaterion left hand
 * 	16-20 quaterion right hand
 * 	21-25 gyroscop left hand: instantaneous rotation of the hand
 * 	26-30 gyroscop right hand: instantaneous rotation of the hand
 * 	31-35 accelerometer left hand
 * 	36-40 accelerometer right hand
 * 	41-45 magnetometer left hand
 * 	46-50 magnetometer right hand
 */
void cMessageGloveDigit::setDigitTyp( const int inDigitTyp ) {
	
	if ( DigitTyp == inDigitTyp ) {
		return;
	}
	DigitTyp = inDigitTyp;
}


/**
 * @see DigitName
 * @return Name of this digit.
 * 	e.g. tumb, finger 1
 */
string cMessageGloveDigit::getDigitName() const {
	
	return DigitName;
}


/**
 * @see DigitName
 * @param inDigitName Name of this digit.
 * 	e.g. tumb, finger 1
 */
void cMessageGloveDigit::setDigitName( const string inDigitName ) {
	
	if ( DigitName == inDigitName ) {
		return;
	}
	DigitName = inDigitName;
}


/**
 * @see Value
 * @return Actual data glove value for this digit.
 */
long cMessageGloveDigit::getValue() const {
	
	return Value;
}


/**
 * @see Value
 * @param Actual data glove value for this digit.
 */
void cMessageGloveDigit::setValue( const long inValue ) {
	
	if ( Value == inValue ) {
		return;
	}
	Value = inValue;
}


/**
 * @see Correction
 * @return The value with which the data glove value is corrected.
 */
long cMessageGloveDigit::getCorrection() const {
	
	return Correction;
}


/**
 * @see Correction
 * @param inCorrection The value with which the data glove value is corrected.
 */
void cMessageGloveDigit::setCorrection( const long inCorrection ) {
	
	if ( Correction == inCorrection ) {
		return;
	}
	Correction = inCorrection;
}


/**
 * @see VecNeighbourStates
 * @return the number of Dataglove neighbour states for the digit
 */
unsigned int cMessageGloveDigit::getNeighbourStateCount() const {
	
	return VecNeighbourStates.size();
}


/**
 * @see VecNeighbourStates
 * @return the vector with the data glove neighbour states for the digit
 */
vector< cDigitState > & cMessageGloveDigit::getDataGloveNeighbourStates() {
	
	return VecNeighbourStates;
}


/**
 * @see VecNeighbourStates
 * @return the const vector with the data glove neighbour states for the digit
 */
const vector< cDigitState > & cMessageGloveDigit::
		getDataGloveNeighbourStatesConst() const {
	
	return VecNeighbourStates;
}


/**
 * Sets the vector with the data glove neighbour states for the digit.
 *
 * @see VecNeighbourStates
 * @param inVecNeighbourStates the const vector with the data glove
 * 	neighbour states to copy
 */
void cMessageGloveDigit::setDataGloveNeighbourStates(
		const vector< cDigitState > & inVecNeighbourStates ) {
	
	VecNeighbourStates = inVecNeighbourStates;
}


/**
 * @see VecNeighbourStates
 * @param inNeighbourStateIndex the index of the data glove neighbour
 * 	state for the digit to return
 * @return the dataglove neighbour state for the digit with the index
 * 	inNeighbourStateIndex (count starts with 0)
 */
cDigitState * cMessageGloveDigit::getNeighbourState(
		const int inNeighbourStateIndex ) {
	
	if ( ( ((const int)(VecNeighbourStates.size())) <= inNeighbourStateIndex ) ||
			( inNeighbourStateIndex < 0 ) ) {
		
		return nullptr;
	}
	return &(VecNeighbourStates[ inNeighbourStateIndex ]);
}


/**
 * @see VecNeighbourStates
 * @param inNeighbourStateIndex the index of the data glove neighbour
 * 	state for the digit to return
 * @return the dataglove neighbour state for the digit with the index
 * 	inNeighbourStateIndex (count starts with 0)
 */
const cDigitState * cMessageGloveDigit::getNeighbourState(
		const int inNeighbourStateIndex ) const {
	
	if ( ( ((const int)(VecNeighbourStates.size())) <= inNeighbourStateIndex ) ||
			( inNeighbourStateIndex < 0 ) ) {
		
		return nullptr;
	}
	return &(VecNeighbourStates[ inNeighbourStateIndex ]);
}


/**
 * Sets the the data glove neighbour state for the digit with the index
 * inNeighbourStateIndex'th.
 *
 * @see VecNeighbourStates
 * @param inNeighbourStateIndex the index of the data glove neighbour
 * 	state for the digit to set
 * @param inMessageGloveNeighbourState the data glove neighbour state
 * 	for the digit to set
 */
void cMessageGloveDigit::setNeighbourState( const int inNeighbourStateIndex,
		cDigitState & inMessageGloveNeighbourState ) {
	
	if ( ((const int)(VecNeighbourStates.size())) <= inNeighbourStateIndex ) {
		if ( ((const int)(VecNeighbourStates.size())) == inNeighbourStateIndex ) {
			//add to end
			VecNeighbourStates.push_back( inMessageGloveNeighbourState );
			return;
		}  //else enlarge the vector
		VecNeighbourStates.resize( inNeighbourStateIndex + 1 );
	}
	VecNeighbourStates[ inNeighbourStateIndex ] = inMessageGloveNeighbourState;
}


/**
 * Adds the the data glove neighbour state for the digit to the end of
 * the data glove neighbour state vector for the digit.
 *
 * @see VecNeighbourStates
 * @param inMessageGloveNeighbourState the data glove neighbour state to
 * 	add for the digit
 */
void cMessageGloveDigit::addNeighbourState(
		cDigitState & inMessageGloveNeighbourState ) {
	
	VecNeighbourStates.push_back( inMessageGloveNeighbourState );
}


/**
 * Removes the the data glove neighbour state for the digit with the
 * index inNeighbourStateIndex'th.
 *
 * @see VecNeighbourStates
 * @param inNeighbourStateIndex the index of the data glove neighbour
 * 	state to remove from the digit
 * @return true if a data glove neighbour state was removed, else false
 */
bool cMessageGloveDigit::removeNeighbourState( const int inNeighbourStateIndex ) {
	
	if ( ( inNeighbourStateIndex < 0 ) ||
			( ((const int)(VecNeighbourStates.size())) <= inNeighbourStateIndex ) ) {
		
		return false;
	}
	vector< cDigitState >::iterator itrElementToRemove =
		VecNeighbourStates.begin();
	for ( int itrActualElement = 0; ( itrActualElement < inNeighbourStateIndex ) &&
			( itrElementToRemove != VecNeighbourStates.end() );
			++itrActualElement, ++itrElementToRemove ) {
		//nothing to do
	}
	if ( itrElementToRemove != VecNeighbourStates.end() ) {
		//element found -> remove element
		VecNeighbourStates.erase( itrElementToRemove );
		return true;
	}  //else no such element
	return false;
}


/**
 * @see IndexActiveNeighbour
 * @see VecNeighbourStates
 * @return Index of the neighbour state, which is activ.
 * 	if -1 no state is active.
 */
int cMessageGloveDigit::getIndexActiveNeighbour() const {
	
	return IndexActiveNeighbour;
}


/**
 * @see IndexActiveNeighbour
 * @see VecNeighbourStates
 * @param inIndexActiveNeighbour Index of the neighbour state, which is activ.
 * 	if -1 no state is active.
 */
void cMessageGloveDigit::setIndexActiveNeighbour( const int inIndexActiveNeighbour ) {
	
	if ( IndexActiveNeighbour == inIndexActiveNeighbour ) {
		return;
	}//else
	if ( inIndexActiveNeighbour < -1 ) {
		//-1 for no neigbour (lowest number)
		IndexActiveNeighbour = -1;
		return;
	}//else
	if ( VecNeighbourStates.size() <= ((size_t)(inIndexActiveNeighbour)) ) {
		//index should be index of vector VecNeighbourStates
		IndexActiveNeighbour = VecNeighbourStates.size() - 1;
		return;
	}//else
	IndexActiveNeighbour = inIndexActiveNeighbour;
}




