
//TODO check

/**
 * @file cMessageGloveStates
 * file name: cMessageGloveStates.h
 * @author Betti Oesterholz
 * @date 13.01.2016
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies a class for a message of the actual data glove state.
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
 * This file specifies a class for a message of the actual data glove state.
 * It also send the neighbour states if existing.
 *
 * @see cThreadMessageHandler
 * @see cMessageDataGlove
 * @see cEvaluateDataGloveState
 * @see cMessageSamplingDataFromDataGlove
 */
/*
History:
13.01.2016  Oesterholz  created
*/


#ifndef ___N_DATA_GLOVE__N_MAPPER___C_MESSAGE_GLOVE_STATES_H__
#define ___N_DATA_GLOVE__N_MAPPER___C_MESSAGE_GLOVE_STATES_H__


#include "version.h"

#ifndef USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES
	#include "cByteOStream.h"
	#include "cByteIStream.h"
#endif  //USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES

#include <string>
#include <vector>
#include <iostream>

#ifdef USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES
	#include <boost/archive/binary_oarchive.hpp>
	//#include <boost/archive/binary_iarchive.hpp>
#endif  // USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES

#include "nMessageGloveStates_cMessageGloveDigit.h"

#define MAX_LOAD_NUMBER_GLOVE_DIGITS 32

namespace nDataGlove{
namespace nMapper{

/**
 * The class to store state data for the data glove.
 */
class cMessageGloveStates{
public:
	
	
	/**
	 * The standard constructor for data glove state message.
	 *
	 */
	cMessageGloveStates();
	
	/**
	 * The copy constructor for data glove state message.
	 *
	 * @param gloveStates the to copy data glove state message
	 *
	 */
	cMessageGloveStates( const cMessageGloveStates & gloveStates );
	
	/**
	 * destructor
	 */
	virtual ~cMessageGloveStates();
	
	/**
	 * This method clones this data glove state message.
	 *
	 */
	cMessageGloveStates * clone();
	
	
	/**
	 * @return the name of this class "cMessageGloveStates"
	 */
	virtual std::string getName() const;

	/**
	 * Checks if this cMessageGloveStates is equal to the given cMessageGloveStates
	 * @param inMessageGloveStates the to compare cMessageGloveStates
	 * @return true if this cMessageGloveStates is equal to the given cMessageGloveStates
	 */
	bool equal( const cMessageGloveStates & inMessageGloveStates ) const;
	
	/**
	 * Checks if this cMessageGloveStates is equal to the given cMessageGloveStates
	 * @param inMessageGloveStates the to compare cMessageGloveStates
	 * @return true if this cMessageGloveStates is equal to the given cMessageGloveStates
	 */
	inline bool operator==( const cMessageGloveStates & inMessageGloveStates ) const {
		
		return equal( inMessageGloveStates );
	}

	
	/**
	 * This method stores this message into the given stream.
	 *
	 * @param stream the stream where this message should be stored to
	 * @return true if this message is stored, else false
	 */
	virtual bool store( std::ostream & inStream ) const;
	
#ifndef USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES
	/**
	 * This method stores this message into the given stream.
	 *
	 * @param stream the stream where this message should be stored to
	 * @return true if this message is stored, else false
	 */
	virtual bool store( cByteOStream & inStream ) const;
#endif  //USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES
	
	/**
	 * This operator stores this message into the given stream.
	 *
	 * @param stream the stream where this message should be stored to
	 * @return the stream in which this message was stored
	 */
	std::ostream & operator<<( std::ostream & inStream ) const;
	
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
	static cMessageGloveStates * restore( std::istream & inStream, int * outStatus=NULL );
	
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
	int read( std::istream & inStream );
	
#ifndef USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES
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
	int read( cByteIStream & inStream );
#endif  // USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES

	/**
	 * This operator stores this message into the given stream.
	 *
	 * @param stream the stream where this message should be stored to
	 * @return the stream from which the message was restored
	 */
	std::istream & operator>>( std::istream & inStream );
	
	/**
	 * This method prints this message into the given stream.
	 * The output is in a human readable form.
	 *
	 * @param stream the stream where this message should be printed to
	 * @return true if this message is printed, else false
	 */
	void print( std::ostream & inStream ) const;

	/**
	 * @see VecDataGloveDigits
	 * @return the number of Dataglove digits
	 */
	unsigned int getDigitCount() const;
	
	/**
	 * @see VecDataGloveDigits
	 * @return the vector with the data glove digits
	 */
	std::vector< nMessageGloveStates::cMessageGloveDigit > & getDataGloveDigits();
	
	/**
	 * @see VecDataGloveDigits
	 * @return the const vector with the data glove digits
	 */
	const std::vector< nMessageGloveStates::cMessageGloveDigit > & getDataGloveDigitsConst() const;
	
	/**
	 * Sets the vector with the data glove digits.
	 *
	 * @see VecDataGloveDigits
	 * @param inVecDataGloveDigits the const vector with the data glove
	 * 	digits to copy
	 */
	void setDataGloveDigits(
		const std::vector< nMessageGloveStates::cMessageGloveDigit > & inVecDataGloveDigits );

	/**
	 * @see VecDataGloveDigits
	 * @param inDigitIndex the index of the data glove digit to return
	 * @return the dataglove digit with the index inDigitIndex (count starts with 0)
	 */
	nMessageGloveStates::cMessageGloveDigit * getDigit( const int inDigitIndex );
	
	/**
	 * Sets the the data glove digit with the index inDigitIndex'th.
	 *
	 * @see VecDataGloveDigits
	 * @param inDigitIndex the index of the data glove digit to set
	 * @param inMessageGloveDigit the data glove digit to set
	 */
	void setDigit( const int inDigitIndex, nMessageGloveStates::cMessageGloveDigit & inMessageGloveDigit );
	
	/**
	 * Adds the the data glove digit to the end of the data glove digit vector.
	 *
	 * @see VecDataGloveDigits
	 * @param inMessageGloveDigit the data glove digit to add
	 */
	void addDigit( nMessageGloveStates::cMessageGloveDigit & inMessageGloveDigit );
	
	/**
	 * Removes the the data glove digit with the index inDigitIndex'th.
	 *
	 * @see VecDataGloveDigits
	 * @param inDigitIndex the index of the data glove digit to remove
	 * @return true if a data glove digit was removed, else false
	 */
	bool removeDigit( const int inDigitIndex );
	
	/**
	 * @see Clock
	 * @return The clock number of the message.
	 */
	unsigned long getClock();
	
	/**
	 * @see Clock
	 * @param inClock The clock number of the message.
	 */
	void setClock( const unsigned long inClock );
	
	/**
	 * @see NumberReceivedMessage
	 * @return The number of the received message.
	 */
	unsigned long getNumberReceivedMessage();
	
	/**
	 * @see NumberReceivedMessage
	 * @param inNumberReceivedMessage The number of the received message.
	 */
	void setNumberReceivedMessage( const unsigned long inNumberReceivedMessage );
	
	
	/**
	 * This operator stores this message into the given stream.
	 *
	 * @param inStream the stream where this message should be stored to
	 * @param inMessageGloveStates the object to store
	 * @return the stream in which this message was stored
	 */
	friend std::ostream & operator<<( std::ostream & inStream,
			cMessageGloveStates const & inMessageGloveStates ) {
		
		inMessageGloveStates.store( inStream );
		
		return inStream;
	}


	/**
	 * This operator stores this message into the given stream.
	 *
	 * @param inStream the stream where this message should be stored to
	 * @param outMessageGloveStates the object into which to restore the readed values
	 * @return the stream from which the message was restored
	 */
	friend std::istream & operator>>( std::istream & inStream,
			cMessageGloveStates & outMessageGloveStates ) {
		
		outMessageGloveStates.read( inStream );
		
		return inStream;
	}
	
protected:
	
	/**
	 * Vector with the single diget values for the data glove state.
	 */
	std::vector< nMessageGloveStates::cMessageGloveDigit > VecDataGloveDigits;
	
	
	/**
	 * The clock number of the message.
	 */
	unsigned long Clock;
	
	/**
	 * The number of the received message.
	 */
	unsigned long NumberReceivedMessage;
	
#ifdef USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES
private:
	
	/**
	 * Use the boost serialization mechanism.
	 * So give the class access access.
	 */
	friend class boost::serialization::access;

	/**
	 * Method for serialization of the data.
	 *
	 * @see boost::serialization
	 * @param inArchiv the archive for the data
	 * @param inVersion the version of the stored data (if data should be read)
	 */
	template <typename Archive>
		void serialize( Archive & inArchiv, const unsigned int inVersion) {
	
		inArchiv & Clock & NumberReceivedMessage & VecDataGloveDigits;
	}
#endif  // USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES
	
};//end class cMessageGloveStates


};//end namespace nMapper
};//end namespace nDataGlove


#ifdef USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES
	BOOST_CLASS_VERSION( nDataGlove::nMapper::cMessageGloveStates, 0 )
#endif  // USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES

#endif //___N_DATA_GLOVE__N_MAPPER___C_MESSAGE_GLOVE_STATES_H__

