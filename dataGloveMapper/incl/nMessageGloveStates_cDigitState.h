
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
 * This file specifies a class for a message of the actual data glove digit
 * state.
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


#ifndef ___N_DATA_GLOVE__N_MAPPER___N_MESSAGE_GLOVE_STATES__C_DIGIT_STATE_H__
#define ___N_DATA_GLOVE__N_MAPPER___N_MESSAGE_GLOVE_STATES__C_DIGIT_STATE_H__


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



namespace nDataGlove{
namespace nMapper{

namespace nMessageGloveStates{


/**
 * The class to store one state data for one digit.
 */
class cDigitState{
public:
	
	/**
	 * The constructor for data glove digit state message.
	 *
	 * @param inStateTyp The typ of the state.
	 * @param inStateName The name of the state.
	 * @param inParameter Parameter of the state.
	 * @param inTargetValue Target value of the state digit.
	 * @param inMinValue Minimum value of the state digit.
	 * @param inMaxValue Maximum value of the state digit.
	 *
	 */
	cDigitState( int inStateTyp = 0, const std::string inStateName = "",
		const std::string inParameter = "",
		const long inTargetValue = 0, const long inMinValue = 0, const long inMaxValue = 0 );
	
	/**
	 * The copy constructor for data glove digit state message.
	 *
	 * @param digitState the to copy digit state
	 *
	 */
	cDigitState( const cDigitState & digitState );
	
	/**
	 * destructor
	 */
	~cDigitState();
	
	/**
	 * This method clones this data glove digit state message.
	 *
	 */
	cDigitState * clone();
	
	/**
	 * @return the name of this class "cDigitState"
	 */
	std::string getName() const;
	
	/**
	 * Checks if this cDigitState is equal to the given cDigitState
	 * @param inDigitState the to compare cDigitState
	 * @return true if this cDigitState is equal to the given cDigitState
	 */
	bool equal( const cDigitState & inDigitState ) const;
	
	/**
	 * Checks if this cDigitState is equal to the given cDigitState
	 * @param inDigitState the to compare cDigitState
	 * @return true if this cDigitState is equal to the given cDigitState
	 */
	inline bool operator==( const cDigitState & inDigitState ) const {
		
		return equal( inDigitState );
	}
	
	
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
	static cDigitState * restore( std::istream & inStream, int * outStatus=NULL );
	
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
	 * This method prints this message into the given stream.
	 * The output is in a human readable form.
	 *
	 * @param stream the stream where this message should be printed to
	 * @return true if this message is printed, else false
	 */
	void print( std::ostream & inStream ) const;
	
	/**
	 * @see StateTyp
	 * @return The typ of the state.
	 */
	int getStateTyp() const;
	
	/**
	 * Sets the typ of the state.
	 *
	 * @see StateTyp
	 * @param inStateTyp The typ of the state.
	 */
	void setStateTyp( const int inStateTyp );
	
	/**
	 * @see StateTyp
	 * @return The name of the typ of the state.
	 */
	std::string getStateTypName() const;
	
	/**
	 * @see StateName
	 * @return The name of the state.
	 */
	std::string getStateName() const;
	
	/**
	 * Sets the name of the state.
	 *
	 * @see StateName
	 * @param inStateName The name of the state.
	 */
	void setStateName( const std::string inStateName );

	/**
	 * @see Parameter
	 * @return Parameter of the state.
	 */
	std::string getParameter() const;
	
	/**
	 * Sets the Parameter of the state.
	 *
	 * @see Parameter
	 * @param inParameter Parameter of the state.
	 */
	void setParameter( const std::string inParameter );
	
	/**
	 * @see TargetValue
	 * @return Target value of the state digit.
	 */
	long getTargetValue() const;
	
	/**
	 * Sets the Target value of the state digit.
	 *
	 * @see TargetValue
	 * @param inTargetValue Target value of the state digit.
	 */
	void setTargetValue( const long inTargetValue );
	
	/**
	 * @see MinValue
	 * @return Minimum value of the state digit.
	 */
	long getMinValue() const;
	
	/**
	 * Sets the Minimum value of the state digit.
	 *
	 * @see MinValue
	 * @param inMinValue Minimum value of the state digit.
	 */
	void setMinValue( const long inMinValue );
	
	/**
	 * @see MaxValue
	 * @return Maximum value of the state digit.
	 */
	long getMaxValue() const;
	
	/**
	 * Sets the Maximum value of the state digit.
	 *
	 * @see MaxValue
	 * @param inMaxValue Maximum value of the state digit.
	 */
	void setMaxValue( const long inMaxValue );
	
	/**
	 * This operator stores this message into the given stream.
	 *
	 * @param inStream the stream where this message should be stored to
	 * @param inDigitState the object to store
	 * @return the stream in which this message was stored
	 */
	friend std::ostream & operator<<( std::ostream & inStream,
			cDigitState const & inDigitState ) {
		
		inDigitState.store( inStream );
		
		return inStream;
	}


	/**
	 * This operator stores this message into the given stream.
	 *
	 * @param inStream the stream where this message should be stored to
	 * @param outDigitState the object into which to restore the readed values
	 * @return the stream from which the message was restored
	 */
	friend std::istream & operator>>( std::istream & inStream,
			cDigitState & outDigitState ) {
		
		outDigitState.read( inStream );
		
		return inStream;
	}
	
	
protected:
	
	/**
	 * The typ of the state.
	 * @see getStateTyp()
	 * @see setStateTyp()
	 * @see StateTyp
	 * @see StateName
	 * @see Parameter
	 *
	 * Types of states:
	 * 	0: undefined
	 * 	1: modus
	 * 	2: keyboard
	 * 	3: mouse
	 * 	101: prepare modus
	 * 	102: prepare keyboard
	 * 	103: prepare mouse
	 * 	1000: user defined (see name and parameter)
	 */
	int StateTyp;
	
	/**
	 * The name of the state.
	 * @see getStateName()
	 * @see setStateName()
	 * @see Parameter
	 */
	std::string StateName;
	
	/**
	 * Parameter of the state.
	 * @see getParameter()
	 * @see setParameter()
	 * @see StateTyp
	 * @see StateName
	 */
	std::string Parameter;
	
	
	/**
	 * Target value of the state digit.
	 * @see getTargetValue()
	 * @see setTargetValue()
	 */
	long TargetValue;
	
	/**
	 * Minimum value of the state digit.
	 * @see getMinValue()
	 * @see setMinValue()
	 */
	long MinValue;
	
	/**
	 * Maximum value of the state digit.
	 * @see getMaxValue()
	 * @see setMaxValue()
	 */
	long MaxValue;

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
		
		inArchiv & StateTyp & StateName & Parameter &
			TargetValue & MinValue & MaxValue;
	}

#endif //USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES

};//end cDigitState


};//end namespace nMessageGloveStates

};//end namespace nMapper
};//end namespace nDataGlove


#ifdef USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES
	BOOST_CLASS_VERSION( nDataGlove::nMapper::nMessageGloveStates::cDigitState, 0 )
#endif //USE_BOOST_SERIALIZIATION_FOR_GLOVE_STATES

#endif //___N_DATA_GLOVE__N_MAPPER___N_MESSAGE_GLOVE_STATES__C_DIGIT_STATE_H__

