
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


#include "cMessageGloveStates.h"

#ifdef USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES
	#include <boost/archive/binary_oarchive.hpp>
	#include <boost/archive/binary_iarchive.hpp>
	// include this header to serialize vectors
	#include <boost/serialization/vector.hpp>
	
	using namespace boost::archive;
#endif  // USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES


using namespace nDataGlove::nMapper::nMessageGloveStates;
using namespace nDataGlove::nMapper;
using namespace std;



/**
 * The standard constructor for data glove state message.
 *
 */
cMessageGloveStates::cMessageGloveStates() : Clock( 0 ),
		 NumberReceivedMessage( 0 ) {
	//nothing to do
}


/**
 * The copy constructor for data glove state message.
 *
 * @param gloveStates the to copy data glove state message
 *
 */
cMessageGloveStates::cMessageGloveStates( const cMessageGloveStates & gloveStates ) :
		VecDataGloveDigits( gloveStates.VecDataGloveDigits ),
		Clock( gloveStates.Clock ),
		NumberReceivedMessage( gloveStates.NumberReceivedMessage ) {
	//nothing to do
}


/**
 * destructor
 */
cMessageGloveStates::~cMessageGloveStates() {
	//nothing to do
}


/**
 * This method clones this data glove state message.
 */
cMessageGloveStates * cMessageGloveStates::clone() {
	
	return new cMessageGloveStates( *this );
}



/**
 * @return the name of this class "cMessageGloveStates"
 */
std::string cMessageGloveStates::getName() const {
	
	return "cMessageGloveStates";
}


/**
 * Checks if this cMessageGloveStates is equal to the given cMessageGloveStates
 * @param inMessageGloveStates the to compare cMessageGloveStates
 * @return true if this cMessageGloveStates is equal to the given cMessageGloveStates
 */
bool cMessageGloveStates::equal( const cMessageGloveStates & inMessageGloveStates ) const {
	
	return ( VecDataGloveDigits == inMessageGloveStates.VecDataGloveDigits) &&
		( Clock == inMessageGloveStates.Clock) &&
		( NumberReceivedMessage == inMessageGloveStates.NumberReceivedMessage);
}


/**
 * This method stores this message into the given stream.
 *
 * @param stream the stream where this message should be stored to
 * @return true if this message is stored, else false
 */
bool cMessageGloveStates::store( ostream & inStream ) const {
	
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
bool cMessageGloveStates::store( cByteOStream & inStream ) const {

	char * pChar;
	//write 4 Bytes: unsigned Clock
	pChar = ((char *)(&Clock));
	inStream.write( pChar, 4 );
	
	//write 4 Bytes: unsigned number of message
	pChar = ((char *)(&NumberReceivedMessage));
	inStream.write( pChar, 4 );

	//1 B: unsigned number of diggits
	unsigned int uiStatesToWrite = VecDataGloveDigits.size();
	
	if ( MAX_LOAD_NUMBER_GLOVE_DIGITS < uiStatesToWrite ) {
		//don't write more than MAX_LOAD_NUMBER_GLOVE_DIGITS glove digits
		uiStatesToWrite = MAX_LOAD_NUMBER_GLOVE_DIGITS;
	}
	if ( 255 < uiStatesToWrite ) {
		//don't write more than 255 glove digits
		uiStatesToWrite = 255;
	}
	pChar = ((char *)(&uiStatesToWrite));
	inStream.write( pChar, 1 );
	bool bReturnValue = true;
	//store the glove digits
	for ( std::vector< cMessageGloveDigit >::const_iterator
			itrDigit = VecDataGloveDigits.begin();
			itrDigit != VecDataGloveDigits.end(); ++itrDigit ) {
		if ( uiStatesToWrite <= 0 ) {
			break;
		}
		--uiStatesToWrite;
		bReturnValue = bReturnValue && itrDigit->store( inStream );
		
	}
	return bReturnValue && inStream.good();
}
#endif  // USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES


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
cMessageGloveStates * cMessageGloveStates::restore(
		istream & inStream, int * outStatus ) {
	
	cMessageGloveStates * pNewGloveState = new cMessageGloveStates();
	
#ifdef USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES
	try {
		binary_iarchive inputArchive{ inStream };
		inputArchive >> (*pNewGloveState);
		
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
#else  // USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES
	const int readStatus = pNewGloveState->read( inStream );
	
	if ( outStatus != nullptr ) {
		(*outStatus) = readStatus;
	}
	if ( readStatus < 0 ) {
		//error occured while reading -> return nullptr
		delete pNewGloveState;
		return nullptr;
	}
#endif //USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES
	return pNewGloveState;
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
int cMessageGloveStates::read( istream & inStream ) {
	
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
	};
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
int cMessageGloveStates::read( cByteIStream & inStream ) {
	
	if ( ! inStream.good() ) {
		return -1;
	}
	char * pBuffer = new char[ 4 ];
	//read 4 Bytes: unsigned Clock
	inStream.read( pBuffer, 4 );
	
	if ( ! inStream.good() ) {
		delete[] pBuffer;
		return -2;
	}
	Clock = 0;
	((char *)(&Clock))[ 0 ] = pBuffer[ 0 ];
	((char *)(&Clock))[ 1 ] = pBuffer[ 1 ];
	((char *)(&Clock))[ 2 ] = pBuffer[ 2 ];
	((char *)(&Clock))[ 3 ] = pBuffer[ 3 ];
	
	//read 4 Bytes: unsigned number of message
	inStream.read( pBuffer, 4 );
	
	if ( ! inStream.good() ) {
		delete[] pBuffer;
		return -2;
	}
	NumberReceivedMessage = 0;
	((char *)(&NumberReceivedMessage))[ 0 ] = pBuffer[ 0 ];
	((char *)(&NumberReceivedMessage))[ 1 ] = pBuffer[ 1 ];
	((char *)(&NumberReceivedMessage))[ 2 ] = pBuffer[ 2 ];
	((char *)(&NumberReceivedMessage))[ 3 ] = pBuffer[ 3 ];
	
	//read 1 B: unsigned number of diggits
	inStream.read( pBuffer, 1 );
	
	if ( ! inStream.good() ) {
		delete[] pBuffer;
		return -2;
	}
	unsigned int uiDigitsToRead = 0;
	((char *)(&uiDigitsToRead))[ 0 ] = pBuffer[ 0 ];
	
	int iReturnValue = 0;
	if ( MAX_LOAD_NUMBER_GLOVE_DIGITS < uiDigitsToRead ) {
		//don't read more than MAX_LOAD_NUMBER_GLOVE_DIGITS states
		uiDigitsToRead = MAX_LOAD_NUMBER_GLOVE_DIGITS;
		iReturnValue = 2;
	}
	if ( 255 < uiDigitsToRead ) {
		//don't read more than 255 states
		uiDigitsToRead = 255;
		iReturnValue = 2;
	}
	//read the neigbour states
	int iSubReturnValue = 0;
	
	VecDataGloveDigits.resize( uiDigitsToRead );
	for ( unsigned int uiIndexDigit = 0; uiIndexDigit < uiDigitsToRead;
			++uiIndexDigit ) {
		
		iSubReturnValue = VecDataGloveDigits[ uiIndexDigit ].read( inStream );
	
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
void cMessageGloveStates::print( std::ostream & inStream ) const {
	
	inStream<<"Message glove states -> clock: "<<Clock<<
		" number: "<<NumberReceivedMessage<<endl;
	//print the digits
	for ( const auto & itrGloveDigit : VecDataGloveDigits ) {
		
		itrGloveDigit.print( inStream );
	}
	inStream<<endl;
}


/**
 * @see VecDataGloveDigits
 * @return the number of Dataglove digits
 */
unsigned int cMessageGloveStates::getDigitCount() const {
	
	return VecDataGloveDigits.size();
}


/**
 * @see VecDataGloveDigits
 * @return the vector with the data glove digits
 */
vector< cMessageGloveDigit > & cMessageGloveStates::getDataGloveDigits() {
	
	return VecDataGloveDigits;
}


/**
 * @see VecDataGloveDigits
 * @return the const vector with the data glove digits
 */
const vector< cMessageGloveDigit > & cMessageGloveStates::getDataGloveDigitsConst() const {
	
	return VecDataGloveDigits;
}


/**
 * Sets the vector with the data glove digits.
 *
 * @see VecDataGloveDigits
 * @param inVecDataGloveDigits the const vector with the data glove
 * 	digits to copy
 */
void cMessageGloveStates::setDataGloveDigits(
	const vector< cMessageGloveDigit > & inVecDataGloveDigits ) {
	
	VecDataGloveDigits = inVecDataGloveDigits;
}


/**
 * @see VecDataGloveDigits
 * @param inDigitIndex the index of the data glove digit to return
 * @return the dataglove digit with the index inDigitIndex (count starts with 0)
 */
cMessageGloveDigit * cMessageGloveStates::getDigit( const int inDigitIndex ) {
	
	if ( ( ((const int)(VecDataGloveDigits.size())) <= inDigitIndex ) ||
			( inDigitIndex < 0 ) ) {
		
		return NULL;
	}
	return &(VecDataGloveDigits[ inDigitIndex ]);
}


/**
 * Sets the the data glove digit with the index inDigitIndex'th.
 *
 * @see VecDataGloveDigits
 * @param inDigitIndex the index of the data glove digit to set
 * @param inMessageGloveDigit the data glove digit to set
 */
void cMessageGloveStates::setDigit( const int inDigitIndex,
		cMessageGloveDigit & inMessageGloveDigit ) {
	
	if ( ((const int)(VecDataGloveDigits.size())) <= inDigitIndex ) {
		if ( ((const int)(VecDataGloveDigits.size())) == inDigitIndex ) {
			//add to end
			VecDataGloveDigits.push_back( inMessageGloveDigit );
			return;
		}  //else enlarge the vector
		VecDataGloveDigits.resize( inDigitIndex + 1 );
	}
	VecDataGloveDigits[ inDigitIndex ] = inMessageGloveDigit;
}


/**
 * Adds the the data glove digit to the end of the data glove digit vector.
 *
 * @see VecDataGloveDigits
 * @param inMessageGloveDigit the data glove digit to add
 */
void cMessageGloveStates::addDigit( cMessageGloveDigit & inMessageGloveDigit ) {
	
	VecDataGloveDigits.push_back( inMessageGloveDigit );
}


/**
 * Removes the the data glove digit with the index inDigitIndex'th.
 *
 * @see VecDataGloveDigits
 * @param inDigitIndex the index of the data glove digit to remove
 * @return true if a data glove digit was removed, else false
 */
bool cMessageGloveStates::removeDigit( const int inDigitIndex ) {
	
	if ( ( inDigitIndex < 0 ) || ( ((int)(VecDataGloveDigits.size())) <= inDigitIndex ) ) {
		return false;
	}
	vector< cMessageGloveDigit >::iterator itrElementToRemove =
		VecDataGloveDigits.begin();
	for ( int itrActualElement = 0; ( itrActualElement < inDigitIndex ) &&
			( itrElementToRemove != VecDataGloveDigits.end() );
			++itrActualElement, ++itrElementToRemove ) {
		//nothing to do
	}
	if ( itrElementToRemove != VecDataGloveDigits.end() ) {
		//element found -> remove element
		VecDataGloveDigits.erase( itrElementToRemove );
		return true;
	}  //else no such element
	return false;
}



/**
 * @see Clock
 * @return The clock number of the message.
 */
unsigned long cMessageGloveStates::getClock() {
	
	return Clock;
}


/**
 * @see Clock
 * @param inClock The clock number of the message.
 */
void cMessageGloveStates::setClock( const unsigned long inClock ) {
	
	Clock = inClock;
}


/**
 * @see NumberReceivedMessage
 * @return The number of the received message.
 */
unsigned long cMessageGloveStates::getNumberReceivedMessage() {
	
	return NumberReceivedMessage;
}


/**
 * @see NumberReceivedMessage
 * @param inNumberReceivedMessage The number of the received message.
 */
void cMessageGloveStates::setNumberReceivedMessage(
		const unsigned long inNumberReceivedMessage ) {
	
	NumberReceivedMessage = inNumberReceivedMessage;
}





