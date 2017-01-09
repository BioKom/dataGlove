
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


#ifndef ___N_DATA_GLOVE__N_MAPPER___N_MESSAGE_GLOVE_STATES_H__C_MESSAGE_GLOVE_DIGIT__
#define ___N_DATA_GLOVE__N_MAPPER___N_MESSAGE_GLOVE_STATES_H__C_MESSAGE_GLOVE_DIGIT__


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

#include "nMessageGloveStates_cDigitState.h"

#define MAX_LOAD_NUMBER_VECTOR_STATES 8


namespace nDataGlove{
namespace nMapper{


namespace nMessageGloveStates{


/**
 * The class to store state data for one digit of the data glove.
 */
class cMessageGloveDigit{
public:
	
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
	cMessageGloveDigit( const int inDigitTyp = 0, const std::string inDigitName = "",
		const int inValue = 0 );
	
	
	/**
	 * The copy constructor for data glove state digit message.
	 *
	 * @param gloveDigit the to copy data glove state digit message
	 *
	 */
	cMessageGloveDigit( const cMessageGloveDigit & gloveDigit );
	
	/**
	 * destructor
	 */
	~cMessageGloveDigit();
	
	/**
	 * This method clones this data glove state digit message.
	 */
	cMessageGloveDigit * clone();

	/**
	 * Checks if this cMessageGloveDigit is equal to the given cMessageGloveDigit
	 * @param inDigitState the to compare cMessageGloveDigit
	 * @return true if this cMessageGloveDigit is equal to the given cMessageGloveDigit
	 */
	bool equal( const cMessageGloveDigit & inDigitState ) const;
	
	/**
	 * Checks if this cMessageGloveDigit is equal to the given cMessageGloveDigit
	 * @param inDigitState the to compare cMessageGloveDigit
	 * @return true if this cMessageGloveDigit is equal to the given cMessageGloveDigit
	 */
	inline bool operator==( const cMessageGloveDigit & inDigitState ) const {
		
		return equal( inDigitState );
	}
	
	/**
	 * @return the name of this class "cMessageGloveDigit"
	 */
	std::string getName() const;
	
	/**
	 * This method stores this message into the given stream.
	 *
	 * @param stream the stream where this message should be stored to
	 * @return true if this message is stored, else false
	 */
	bool store( std::ostream & inStream ) const;
	
#ifndef USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES
	/**
	 * This method stores this message into the given stream.
	 *
	 * @param stream the stream where this message should be stored to
	 * @return true if this message is stored, else false
	 */
	bool store( cByteOStream & inStream ) const;
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
	static cMessageGloveDigit * restore( std::istream & inStream, int * outStatus=NULL );
	
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
	int getDigitTyp() const;
	
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
	std::string getDigitTypName() const;
	
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
	void setDigitTyp( const int inDigitTyp );

	/**
	 * @see DigitName
	 * @return Name of this digit.
	 * 	e.g. tumb, finger 1
	 */
	std::string getDigitName() const;
	
	
	/**
	 * @see DigitName
	 * @param inDigitName Name of this digit.
	 * 	e.g. tumb, finger 1
	 */
	void setDigitName( const std::string inDigitName );
	
	/**
	 * @see Value
	 * @return Actual data glove value for this digit.
	 */
	long getValue() const;
	
	/**
	 * @see Value
	 * @param inValue data glove value for this digit.
	 */
	void setValue( const long inValue );
	
	/**
	 * @see Correction
	 * @return The value with which the data glove value is corrected.
	 */
	long getCorrection() const;
	
	/**
	 * @see Correction
	 * @param inCorrection The value with which the data glove value is corrected.
	 */
	void setCorrection( const long inCorrection );
	
	/**
	 * @see VecNeighbourStates
	 * @return the number of Dataglove neighbour states for the digit
	 */
	unsigned int getNeighbourStateCount() const;
	
	/**
	 * @see VecNeighbourStates
	 * @return the vector with the data glove neighbour states for the digit
	 */
	std::vector< cDigitState > & getDataGloveNeighbourStates();
	
	/**
	 * @see VecNeighbourStates
	 * @return the const vector with the data glove neighbour states for the digit
	 */
	const std::vector< cDigitState > & getDataGloveNeighbourStatesConst() const;
	
	/**
	 * Sets the vector with the data glove neighbour states for the digit.
	 *
	 * @see VecNeighbourStates
	 * @param inVecNeighbourStates the const vector with the data glove
	 * 	neighbour states to copy
	 */
	void setDataGloveNeighbourStates(
		const std::vector< cDigitState > & inVecNeighbourStates );

	/**
	 * @see VecNeighbourStates
	 * @param inNeighbourStateIndex the index of the data glove neighbour
	 * 	state for the digit to return
	 * @return the dataglove neighbour state for the digit with the index
	 * 	inNeighbourStateIndex (count starts with 0)
	 */
	cDigitState * getNeighbourState( const int inNeighbourStateIndex );
	
	/**
	 * @see VecNeighbourStates
	 * @param inNeighbourStateIndex the index of the data glove neighbour
	 * 	state for the digit to return
	 * @return the dataglove neighbour state for the digit with the index
	 * 	inNeighbourStateIndex (count starts with 0)
	 */
	const cDigitState * getNeighbourState( const int inNeighbourStateIndex ) const;
	
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
	void setNeighbourState( const int inNeighbourStateIndex, cDigitState & inMessageGloveNeighbourState );
	
	/**
	 * Adds the the data glove neighbour state for the digit to the end of
	 * the data glove neighbour state vector for the digit.
	 *
	 * @see VecNeighbourStates
	 * @param inMessageGloveNeighbourState the data glove neighbour state to
	 * 	add for the digit
	 */
	void addNeighbourState( cDigitState & inMessageGloveNeighbourState );
	
	/**
	 * Removes the the data glove neighbour state for the digit with the
	 * index inNeighbourStateIndex'th.
	 *
	 * @see VecNeighbourStates
	 * @param inNeighbourStateIndex the index of the data glove neighbour
	 * 	state to remove from the digit
	 * @return true if a data glove neighbour state was removed, else false
	 */
	bool removeNeighbourState( const int inNeighbourStateIndex );
	
	
	/**
	 * @see IndexActiveNeighbour
	 * @see VecNeighbourStates
	 * @return Index of the neighbour state, which is activ.
	 * 	if -1 no state is active.
	 */
	int getIndexActiveNeighbour() const;
	
	/**
	 * @see IndexActiveNeighbour
	 * @see VecNeighbourStates
	 * @param inIndexActiveNeighbour Index of the neighbour state, which is activ.
	 * 	if -1 no state is active.
	 */
	void setIndexActiveNeighbour( const int inIndexActiveNeighbour );
	
	
	/**
	 * This operator stores this message into the given stream.
	 *
	 * @param inStream the stream where this message should be stored to
	 * @param inMessageGloveDigit the object to store
	 * @return the stream in which this message was stored
	 */
	friend std::ostream & operator<<( std::ostream & inStream,
			cMessageGloveDigit const & inMessageGloveDigit ) {
		
		inMessageGloveDigit.store( inStream );
		
		return inStream;
	}


	/**
	 * This operator stores this message into the given stream.
	 *
	 * @param inStream the stream where this message should be stored to
	 * @param outMessageGloveDigit the object into which to restore the readed values
	 * @return the stream from which the message was restored
	 */
	friend std::istream & operator>>( std::istream & inStream,
			cMessageGloveDigit & outMessageGloveDigit ) {
		
		outMessageGloveDigit.read( inStream );
		
		return inStream;
	}	
	
protected:
	
	/**
	 * Typ of this digit.
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
	int DigitTyp;
	
	/**
	 * Name of this digit.
	 * e.g. tumb, finger 1
	 */
	std::string DigitName;
	
	/**
	 * Actual data glove value for this digit.
	 */
	long Value;
	
	/**
	 * The value with which the data glove value is corrected.
	 */
	long Correction;
	
	/**
	 * Vector with the neighbour states for this digit.
	 * @see IndexActiveNeighbour
	 */
	std::vector< cDigitState > VecNeighbourStates;
	
	/**
	 * Index of the neighbour state, which is activ.
	 * If -1 no state is active.
	 * @see VecNeighbourStates
	 */
	int IndexActiveNeighbour;
	
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
		void serialize( Archive & inArchiv, const unsigned int inVersion){
	
		inArchiv & DigitTyp & DigitName & Value &
			VecNeighbourStates & IndexActiveNeighbour;
	}
#endif  // USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES
	
};//end class cMessageGloveDigit


};//end namespace nMessageGloveStates


};//end namespace nMapper
};//end namespace nDataGlove


#ifdef USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES
	BOOST_CLASS_VERSION( nDataGlove::nMapper::nMessageGloveStates::cMessageGloveDigit, 0 )
#endif  // USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES

#endif //___N_DATA_GLOVE__N_MAPPER___N_MESSAGE_GLOVE_STATES_H__C_MESSAGE_GLOVE_DIGIT__

