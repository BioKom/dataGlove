
//TODO check

/**
 * @file cCallMouseFunction
 * file name: cCallMouseFunction.h
 * @author Betti Oesterholz
 * @date 04.01.2015
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies the class for calling a mouse function.
 *
 *
 * Copyright (C) @c GPL3 2015 Betti Oesterholz
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
 * This file specifies the class for calling a mouse function.
 * Mouse function can be klicks (e.g. LEFT_KLICK) or directions (e.g. HORIZONTAL).
 *
 * @pattern functor
 * @see cCallPrepareMouseFunction
 * @see iCallFunction
 * @see cDataGloveState
 * @see cEvaluateDataGloveState
 */
/*
History:
04.01.2015  Oesterholz  created
*/


#ifndef ___N_DATA_GLOVE__N_MAPPER__C_CALL_MOUSE_FUNCTION__
#define ___N_DATA_GLOVE__N_MAPPER__C_CALL_MOUSE_FUNCTION__


#include "version.h"

#include <string>
#include <list>

#include "iCallFunction.h"
#include "eMouseFunction.h"


namespace nDataGlove{
namespace nMapper{

namespace nCallMouseFunction{
	class cMapperMouseFunction;
};//end namespace nCallMouseFunction


class cCallMouseFunction : public iCallFunction{
public:

	/**
	 * constructor
	 * (Also evaluates if preparing the mouse function is needed.)
	 *
	 * @see bPrepareNeeded
	 * @param inSzParameters A string with parameters for the call to the mouse
	 * 	@see analyseAndSetParameters()
	 */
	cCallMouseFunction( const std::string inSzParameters );
	
	/**
	 * constructor
	 *
	 * @param inSzParameters A string with parameters for the call to the mouse
	 * 	@see analyseAndSetParameters()
	 * @param bInPrepareNeeded if true preparing the mouse function is
	 * 	needed, else not
	 * 	@see bPrepareNeeded
	 * 	@see cCallPrepareMouseFunction
	 */
	cCallMouseFunction( const std::string inSzParameters,
		const bool bInPrepareNeeded );
	
	/**
	 * constructor
	 *
	 * @param MouseFunction The mouse function for this this operator.
	 * 	@see mouseFunction
	 * @param bInPrepareNeeded if true preparing the mouse function is
	 * 	needed, else not
	 * 	@see bPrepareNeeded
	 * 	@see cCallPrepareMouseFunction
	 */
	cCallMouseFunction( const eMouseFunction inMouseFunction,
		const bool bInPrepareNeeded = true );

	/**
	 * constructor for mouse movements
	 *
	 * @param MouseFunction The mouse function for this operator. 
	 * 	(should be a mouse movement.)
	 * 	@see mouseFunction
	 * @param inAmount The amount for the mouse movement.
	 * 	@see mouseFunction
	 * @param bInPrepareNeeded if true preparing the mouse function is
	 * 	needed, else not
	 * 	@see bPrepareNeeded
	 * 	@see cCallPrepareMouseFunction
	 */
	cCallMouseFunction( const eMouseFunction inMouseFunction,
		const int inAmount,
		const bool bInPrepareNeeded = true );
	
	
	/**
	 * destructor
	 */
	virtual ~cCallMouseFunction();
	
	/**
	 * @return the name of this class "cCallMouseFunction"
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
	 * This method sets the parameter for the operation.
	 * The parameter are readed from the given string.
	 *
	 * @param szFunctionParameter all parameters for this functor operation
	 * 	as a string
	 * @return true if the parameter could be set, else false
	 */
	virtual bool setParameter( const std::string & szFunctionParameter );
	
	/**
	 * @return If true preparing the modus change is needed, else not.
	 * 	@see bPrepareNeeded
	 */
	bool getPrepareNeeded() const;
	
	/**
	 * @return The mouse function, which will be prepared
	 * 	with this operator.
	 * 	If it is a directions (e.g. "MOUSE_HORIZONTAL") iAmount contains the
	 * 	amount of the move in the direction.
	 * 	@see mouseFunction
	 */
	eMouseFunction getMouseFunction() const;
	
	/**
	 * @return The amount for the mouse movement, if this object is for a
	 * 	mouse movement, else 1.
	 * 	@see iAmount
	 * @see mouseFunction
	 */
	int getAmount() const;
	
	/**
	 * @return The second amount for the mouse movement, if this object is for a
	 * 	mouse movement in two directions / dimensions, else 0.
	 * 	(E.g. MOUSE_MOVE_HORIZONTAL_VERTICAL for move iAmount horizontal and
	 * 	iAmount2 vertical.)
	 * 	@see iAmount2
	 * @see mouseFunction
	 */
	int getAmount2() const;
	
protected:
	
	/**
	 * This function returns the mouse function for the given string.
	 *
	 * @param inSzParameters the string for which to return the mouse function
	 * @return the mouse function for the given string, or NON if non could
	 * 	be evaluated
	 */
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	static eMouseFunction getMouseFunction(
		const std::wstring & inSzParameters );
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	static eMouseFunction getMouseFunction(
		const std::string & inSzParameters );
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	
	
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	/**
	 * This method analyses and setst the given parameters.
	 * It wil set:
	 * 	@see mouseFunction
	 * 	@see keyCode1
	 * 	@see keyCode2
	 * 	@see liKeys
	 *
	 * @param inSzParameters the string with the parameters for the mouse function
	 * @param inEvaluateIfPrepareNeeded if true evaluates if preparing the
	 * 	mouse function is needed or not
	 * 	(buttons need preparing moving not)
	 * 	@see bPrepareNeeded
	 * @return true if the parameter could be set, else false
	 */
	bool analyseAndSetParameters( const std::string & inSzParameters,
		const bool inEvaluateIfPrepareNeeded = false );
	
	/**
	 * This method analyses and setst the given parameters.
	 * It wil set:
	 * 	@see mouseFunction
	 * 	@see keyCode1
	 * 	@see keyCode2
	 * 	@see liKeys
	 *
	 * @param inSzParameters the string with the parameters for the mouse function
	 * @param inEvaluateIfPrepareNeeded if true evaluates if preparing the
	 * 	mouse function is needed or not
	 * 	(buttons need preparing moving not)
	 * 	@see bPrepareNeeded
	 * @return true if the parameter could be set, else false
	 */
	bool analyseAndSetParameters( const std::wstring & inSzParameters,
		const bool inEvaluateIfPrepareNeeded = false );
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	/**
	 * This method analyses and setst the given parameters.
	 * It wil set:
	 * 	@see mouseFunction
	 * 	@see keyCode1
	 * 	@see keyCode2
	 * 	@see liKeys
	 *
	 * @param inSzParameters the string with the parameters for the mouse function
	 * @param inEvaluateIfPrepareNeeded if true evaluates if preparing the
	 * 	mouse function is needed or not
	 * 	(buttons need preparing moving not)
	 * 	@see bPrepareNeeded
	 * @return true if the parameter could be set, else false
	 */
	bool analyseAndSetParameters( const std::string & inSzParameters,
		const bool inEvaluateIfPrepareNeeded = false );
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	
//members
	
	/**
	 * The mouse function, which will be used with this operator.
	 * If it is a directions (e.g. "MOUSE_HORIZONTAL") iAmount contains the
	 * amount of the move in the direction.
	 * @see getMouseFunction()
	 * @see iAmount
	 */
	eMouseFunction mouseFunction;
	
	/**
	 * The amount for the mouse movement, if this object is for a mouse
	 * movement, else 1.
	 * @see getAmount()
	 * @see mouseFunction
	 */
	int iAmount;
	
	/**
	 * The second amount for the mouse movement, if this object is for a
	 * mouse movement in two directions / dimensions, else 0.
	 * (E.g. MOUSE_MOVE_HORIZONTAL_VERTICAL for move iAmount horizontal and
	 * iAmount2 vertical.)
	 * @see getAmount2()
	 * @see mouseFunction
	 */
	int iAmount2;
	
	
	/**
	 * If true preparing the modus change is needed, else not.
	 * @see getPrepareNeeded()
	 * @see cCallPrepareMouseFunction
	 */
	bool bPrepareNeeded;
	
	/**
	 * Mapper for the strings to mouse functions.
	 */
	static nCallMouseFunction::cMapperMouseFunction mapperMouseFunction;
	
};//end class cCallMouseFunction


};//end namespace nMapper
};//end namespace nDataGlove

#endif //___N_DATA_GLOVE__N_MAPPER__C_CALL_MOUSE_FUNCTION__





