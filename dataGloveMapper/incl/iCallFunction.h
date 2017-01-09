
//TODO check

/**
 * @file iCallFunction
 * file name: iCallFunction.h
 * @author Betti Oesterholz
 * @date 06.08.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies the interface for a function, which should be called.
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
 * This file specifies the interface for a function, which should be called.
 * It is a functor.
 *
 * @pattern functor
 * @pattern interface
 * @see cDataGloveState
 * @see cEvalueDataGloveState
 */
/*
History:
06.08.2014  Oesterholz  created
*/


#ifndef ___N_DATA_GLOVE__N_MAPPER__I_CALL_FUNCTION__
#define ___N_DATA_GLOVE__N_MAPPER__I_CALL_FUNCTION__


#include "version.h"

#include <string>
#include <chrono>


namespace nDataGlove{
namespace nMapper{


class iCallFunction{
public:

	iCallFunction();
	
	virtual ~iCallFunction();
	
	/**
	 * @return the name of this class "iCallFunction"
	 */
	virtual std::string getName() const;
	
	/**
	 * This functor operator is called the execute the function implemented
	 * by this object.
	 *
	 * @return true if the call to the function was sucessfull, else false
	 */
	virtual bool operator()();
	
	/**
	 * This method ends the operation.
	 * (Call if the state changes.)
	 */
	virtual void end();
	
	
	/**
	 * This method returns the parameter for the operation.
	 *
	 * @see Parameter
	 * @return all parameters for this functor operation as a string
	 */
	virtual std::string getParameter() const;
	
	
	/**
	 * This method sets the parameter for the operation.
	 * The parameter are readed from the given string.
	 *
	 * @see Parameter
	 * @param szFunctionParameter all parameters for this functor operation
	 * 	as a string
	 * @return true if the parameter could be set, else false
	 */
	virtual bool setParameter( const std::string & szFunctionParameter );
	
	
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	/**
	 * This method sets the parameter for the operation.
	 * The parameter are readed from the given string.
	 *
	 * @see Parameter
	 * @param szFunctionParameter all parameters for this functor operation
	 * 	as a string
	 * @return true if the parameter could be set, else false
	 */
	virtual bool setParameter( const std::wstring & szFunctionParameter );
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	
	/**
	 * This method sets the repeat delays for the operation.
	 * The repeat delays are readed from the given string.
	 *
	 * @param szFunctionRepeatDelay all repeat delays for this functor operation
	 * 	as a string
	 * @return true if the repeat delays could be set, else false
	 */
	virtual bool setRepeatDelay( const std::string & szFunctionRepeatDelay );
	
	
	
	/**
	 * This method repeats this function if it should be repeated.
	 *
	 * @see arrayRepeatDelay
	 * @return the number of times this function was repeated by the call
	 */
	virtual unsigned int checkAndRepeatCall();
	
protected:
	
	/**
	 * This functor initialise the repeat delay, if existing.
	 *
	 * @return true if the repeat delay is initialised, else false
	 */
	bool initRepeatDelay();
	
	/**
	 * Array with the delay times in milli seconds.
	 * After the call function is called, wait arrayRepeatDelay[0] milli seconds
	 * to call it again, then arrayRepeatDelay[ 1 ] milli seconds ... .
	 * @see countDelays
	 * @see indexNextDelay
	 */
	std::chrono::milliseconds * arrayRepeatDelay;
	
	/**
	 * Number of elements in arrayRepeatDelay;
	 * @see arrayRepeatDelay
	 */
	int countRepeatDelays;
	
	/**
	 * The index of the next delay time in arrayRepeatDelay.
	 * if -1 -> no next delay
	 * @see arrayRepeatDelay
	 */
	int indexNextDelay;
	
	/**
	 * If the last element in arrayRepeatDelay should be repeated infinitely.
	 * @see arrayRepeatDelay
	 */
	bool bRepeatLastDelay;
	
	/**
	 * The next time this function should be called.
	 * @see arrayRepeatDelay
	 */
	std::chrono::system_clock::time_point timeNextCall;
	
	/**
	 * The parameters for this functor operation as a string.
	 */
	std::string Parameter;
	
	
	
};//end class iCallFunction


};//end namespace nMapper
};//end namespace nDataGlove

#endif //___N_DATA_GLOVE__N_MAPPER__I_CALL_FUNCTION__





