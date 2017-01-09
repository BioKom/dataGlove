
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


#include "nMessageGloveStates_cDigitState.h"

#ifdef USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES
	#include <boost/archive/binary_oarchive.hpp>
	#include <boost/archive/binary_iarchive.hpp>
	using namespace boost::archive;
#endif  // USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES


using namespace nDataGlove::nMapper::nMessageGloveStates;
using namespace nDataGlove::nMapper;
using namespace std;



/**
 * The constructor for data glove state message.
 *
 * @param inStateTyp The typ of the state.
 * @param inStateName The name of the state.
 * @param inParameter Parameter of the state.
 * @param inTargetValue Target value of the state digit.
 * @param inMinValue Minimum value of the state digit.
 * @param inMaxValue Maximum value of the state digit.
 *
 */
cDigitState::cDigitState( int inStateTyp, const std::string inStateName,
		const std::string inParameter,
		const long inTargetValue, const long inMinValue, const long inMaxValue ) :
		StateTyp( inStateTyp ), StateName( inStateName ),
		Parameter( inParameter ), TargetValue( inTargetValue ),
		MinValue( inMinValue ), MaxValue( inMaxValue ) {
	//nothing to do
}


/**
 * The copy constructor for data glove digit state message.
 *
 * @param digitState the to copy digit state
 *
 */
cDigitState::cDigitState( const cDigitState & digitState ) :
		StateTyp( digitState.StateTyp ), StateName( digitState.StateName ),
		Parameter( digitState.Parameter ), TargetValue( digitState.TargetValue ),
		MinValue( digitState.MinValue ), MaxValue( digitState.MaxValue ) {
	//nothing to do
}


/**
 * destructor
 */
cDigitState::~cDigitState() {
	//nothing to do
}


/**
 * This method clones this data glove digit state message.
 *
 */
cDigitState * cDigitState::clone() {
	
	return new cDigitState( *this );
}



/**
 * @return the name of this class "cDigitState"
 */
std::string cDigitState::getName() const {
	
	return "cDigitState";
}


/**
 * Checks if this cDigitState is equal to the given cDigitState
 * @param inDigitState the to compare cDigitState
 * @return true if this cDigitState is equal to the given cDigitState
 */
bool cDigitState::equal( const cDigitState & inDigitState ) const {
	
	return ( StateTyp == inDigitState.StateTyp ) &&
		( StateName == inDigitState.StateName ) &&
		( Parameter == inDigitState.Parameter ) &&
		( TargetValue == inDigitState.TargetValue ) &&
		( MinValue == inDigitState.MinValue ) &&
		( MaxValue == inDigitState.MaxValue );
}



/**
 * This method stores this message into the given stream.
 *
 * @param stream the stream where this message should be stored to
 * @return true if this message is stored, else false
 */
bool cDigitState::store( ostream & inStream ) const {
	
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
bool cDigitState::store( cByteOStream & inStream ) const {
	
	const char * pChar;
	//write 2 B State Type
	pChar = ((const char *)(&StateTyp));
	inStream.write( pChar, 2 );
	
	//2 B: digit state name legth
	unsigned int uiBytesToWrite = StateName.length();
	if ( 0xFFFF < uiBytesToWrite ) {
		//write max length of 2^16-1
		uiBytesToWrite = 0xFFFF;
	}
	pChar = ((const char *)(&uiBytesToWrite));
	inStream.write( pChar, 2 );
	//write legth B: digit state name string
	pChar = StateName.c_str();
	inStream.write( pChar, uiBytesToWrite );
	
	//2 B: digit state parameter leght
	uiBytesToWrite = Parameter.length();
	if ( 0xFFFF < uiBytesToWrite ) {
		//write max length of 2^16-1
		uiBytesToWrite = 0xFFFF;
	}
	pChar = ((const char *)(&uiBytesToWrite));
	inStream.write( pChar, 2 );
	//write legth B: digit state parameter string
	pChar = Parameter.c_str();
	inStream.write( pChar, uiBytesToWrite );
	
	//write 4 B target value
	pChar = ((const char *)(&TargetValue));
	inStream.write( pChar, 4 );
	//write 4 B min value
	pChar = ((const char *)(&MinValue));
	inStream.write( pChar, 4 );
	//write 4 B max value
	pChar = ((const char *)(&MaxValue));
	inStream.write( pChar, 4 );
	
	return inStream.good();
}
#endif //USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES


/**
 * This method restores the message from the stream.
 *
 * @param stream the stream where this message is stored to in
 * @param outStatus An pointer to an integervalue where the error value
 * 	can be stored to. If the pointer is NULL no error value will be
 * 	given back.
 * 	possible errorvalues are:
 * 		- 0  loading successful
 * 		- -1 loading error, invalid stream
 * 		- -2 loading error, invalid data in stream
 * 		- 1 loading warning, invalid data in stream, error could be corrected
 * 		- 2 loading warning, invalid data in stream, maybe the loaded
 * 			object is wrong
 * @return the readed message or the Null pointer NULL, if reading
 * 	was not possible
 */
cDigitState * cDigitState::restore( istream & inStream, int * outStatus ) {
	
	cDigitState * pNewDigitState = new cDigitState();
	
#ifdef USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES
	try {
		binary_iarchive inputArchive{ inStream };
		
		inputArchive >> (*pNewDigitState);
		
	} catch ( boost::archive::archive_exception& archivException ) {
		
		if ( outStatus != NULL ) {
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
	const int readStatus = pNewDigitState->read( inStream );
	
	if ( outStatus != nullptr ) {
		(*outStatus) = readStatus;
	}
	if ( readStatus < 0 ) {
		//error occured while reading -> return nullptr
		delete pNewDigitState;
		return nullptr;
	}
#endif //USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES
	return pNewDigitState;
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
int cDigitState::read( istream & inStream ) {
	
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
int cDigitState::read( cByteIStream & inStream ) {
	
	if ( ! inStream.good() ) {
		return -1;
	}
	
	char * pBuffer = new char[ 256 ];
	//read 2 Byte State Type
	inStream.read( pBuffer, 2 );
	
	if ( ! inStream.good() ) {
		delete[] pBuffer;
		return -2;
	}
	StateTyp = 0;
	((char *)(&StateTyp))[ 0 ] = pBuffer[ 0 ];
	((char *)(&StateTyp))[ 1 ] = pBuffer[ 1 ];
	
	//2 Byte: digit state name legth
	inStream.read( pBuffer, 2 );
	if ( ! inStream.good() ) {
		delete[] pBuffer;
		return -2;
	}
	unsigned int uiBytesToRead = 0;
	((char *)(&uiBytesToRead))[ 0 ] = pBuffer[ 0 ];
	((char *)(&uiBytesToRead))[ 1 ] = pBuffer[ 1 ];
	
	//read legth Byte: digit state name string
	StateName = "";
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
		StateName.append( pBuffer, uiBytesReaded );
		uiBytesToRead -= uiBytesReaded;
	}
	
	//2 Byte: digit state parameter legth
	inStream.read( pBuffer, 2 );
	
	if ( ! inStream.good() ) {
		delete[] pBuffer;
		return -2;
	}
	uiBytesToRead = 0;
	((char *)(&uiBytesToRead))[ 0 ] = pBuffer[ 0 ];
	((char *)(&uiBytesToRead))[ 1 ] = pBuffer[ 1 ];
	
	//read legth B: digit state parameter string
	Parameter = "";
	//read blocks with 256 chars each
	uiBytesReaded = 256;
	while ( 0 < uiBytesToRead ) {
		if ( uiBytesToRead < uiBytesReaded ) {
			uiBytesReaded = uiBytesToRead;
		}
		inStream.read( pBuffer, uiBytesReaded );
		
		if ( ! inStream.good() ) {
			delete[] pBuffer;
			return -2;
		}
		Parameter.append( pBuffer, uiBytesReaded );
		uiBytesToRead -= uiBytesReaded;
	}
	
	//read 4 Byte target value
	inStream.read( pBuffer, 4 );
	
	if ( ! inStream.good() ) {
		delete[] pBuffer;
		return -2;
	}
	if ( (pBuffer[ 3 ] & 0x80) == 0 ) {
		//first bit is 0 -> positiv value
		TargetValue = 0;
	} else {  //first bit is 1 -> negativ value
		TargetValue = -1;
	}
	((char *)(&TargetValue))[ 0 ] = pBuffer[ 0 ];
	((char *)(&TargetValue))[ 1 ] = pBuffer[ 1 ];
	((char *)(&TargetValue))[ 2 ] = pBuffer[ 2 ];
	((char *)(&TargetValue))[ 3 ] = pBuffer[ 3 ];
	
	//read 4 Byte min value
	inStream.read( pBuffer, 4 );
	
	if ( ! inStream.good() ) {
		delete[] pBuffer;
		return -2;
	}
	if ( (pBuffer[ 3 ] & 0x80) == 0 ) {
		//first bit is 0 -> positiv value
		MinValue = 0;
	} else {  //first bit is 1 -> negativ value
		MinValue = -1;
	}
	((char *)(&MinValue))[ 0 ] = pBuffer[ 0 ];
	((char *)(&MinValue))[ 1 ] = pBuffer[ 1 ];
	((char *)(&MinValue))[ 2 ] = pBuffer[ 2 ];
	((char *)(&MinValue))[ 3 ] = pBuffer[ 3 ];
	
	//read 4 Byte max value
	inStream.read( pBuffer, 4 );
	
	if ( ! inStream.good() ) {
		delete[] pBuffer;
		return -2;
	}
	if ( (pBuffer[ 3 ] & 0x80) == 0 ) {
		//first bit is 0 -> positiv value
		MaxValue = 0;
	} else {  //first bit is 1 -> negativ value
		MaxValue = -1;
	}
	((char *)(&MaxValue))[ 0 ] = pBuffer[ 0 ];
	((char *)(&MaxValue))[ 1 ] = pBuffer[ 1 ];
	((char *)(&MaxValue))[ 2 ] = pBuffer[ 2 ];
	((char *)(&MaxValue))[ 3 ] = pBuffer[ 3 ];
	
	delete[] pBuffer;
	
	if ( ! inStream.good() ) {
		return -2;
	}
	return 0;
}
#endif //USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES


/**
 * This method prints this message into the given stream.
 * The output is in a human readable form.
 *
 * @param stream the stream where this message should be printed to
 * @return true if this message is printed, else false
 */
void cDigitState::print( std::ostream & inStream ) const {
	
	inStream<<"	"<<StateTyp<<"."<<getStateTypName()<<"->"<<StateName<<"("<<Parameter<<"): "<<
		MinValue<<"<"<<TargetValue<<"<"<<MaxValue<<";"<<endl;
}


/**
 * @see StateTyp
 * @return The typ of the state.
 */
int cDigitState::getStateTyp() const {
	
	return StateTyp;
}


/**
 * @see StateTyp
 * @return The name of the typ of the state.
 */
std::string cDigitState::getStateTypName() const {
	
	switch ( StateTyp ) {
		case 0: return "undefined";
		case 1: return "modus";
		case 2: return "keyboard";
		case 3: return "mouse";
		case 101: return "prepare modus";
		case 102: return "prepare keyboard";
		case 103: return "prepare mouse";
		case 1000: return "user defined (see name and parameter)";
	};
	return "unknown";
}


/**
 * Sets the typ of the state.
 *
 * @see StateTyp
 * @param inStateTyp The typ of the state.
 */
void cDigitState::setStateTyp( const int inStateTyp ) {
	
	if ( StateTyp == inStateTyp ) {
		return;
	}
	StateTyp = inStateTyp;
}


/**
 * @see StateName
 * @return The name of the state.
 */
string cDigitState::getStateName() const {
	
	return StateName;
}


/**
 * Sets the name of the state.
 *
 * @see StateName
 * @param inStateName The name of the state.
 */
void cDigitState::setStateName( const string inStateName ) {
	
	if ( StateName == inStateName ) {
		return;
	}
	StateName = inStateName;
}


/**
 * @see Parameter
 * @return Parameter of the state.
 */
string cDigitState::getParameter() const {
	
	return Parameter;
}


/**
 * Sets the Parameter of the state.
 *
 * @see Parameter
 * @param inParameter Parameter of the state.
 */
void cDigitState::setParameter( const string inParameter ) {
	
	if ( Parameter == inParameter ) {
		return;
	}
	Parameter = inParameter;
}


/**
 * @see TargetValue
 * @return Target value of the state digit.
 */
long cDigitState::getTargetValue() const {
	
	return TargetValue;
}


/**
 * Sets the Target value of the state digit.
 *
 * @see TargetValue
 * @param inTargetValue Target value of the state digit.
 */
void cDigitState::setTargetValue( const long inTargetValue ) {
	
	if ( TargetValue == inTargetValue ) {
		return;
	}
	TargetValue = inTargetValue;
}


/**
 * @see MinValue
 * @return Minimum value of the state digit.
 */
long cDigitState::getMinValue() const {
	
	return MinValue;
}


/**
 * Sets the Minimum value of the state digit.
 *
 * @see MinValue
 * @param inMinValue Minimum value of the state digit.
 */
void cDigitState::setMinValue( const long inMinValue ) {
	
	if ( MinValue == inMinValue ) {
		return;
	}
	MinValue = inMinValue;
}


/**
 * @see MaxValue
 * @return Maximum value of the state digit.
 */
long cDigitState::getMaxValue() const {
	
	return MaxValue;
}


/**
 * Sets the Maximum value of the state digit.
 *
 * @see MaxValue
 * @param inMaxValue Maximum value of the state digit.
 */
void cDigitState::setMaxValue( const long inMaxValue ) {
	
	if ( MaxValue == inMaxValue ) {
		return;
	}
	MaxValue = inMaxValue;
}





