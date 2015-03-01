
//TODO check

/**
 * @file cCallPrepareMouseFunction
 * file name: cCallPrepareMouseFunction.h
 * @author Betti Oesterholz
 * @date 28.02.2015
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies the class for a preparing a mouse function.
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
 * This file specifies the class for a preparing a mouse function.
 * Mouse function can be klicks (e.g. LEFT_KLICK) or directions (e.g. HORIZONTAL).
 * The class cCallMouseFunction will just work, if this operation was
 * activated for the correct modus.
 *
 * @pattern functor
 * @see cCallMouseFunction
 * @see iCallFunction
 * @see cDataGloveState
 * @see cEvaluateDataGloveState
 */
/*
History:
28.02.2015  Oesterholz  created
*/


#ifndef ___N_DATA_GLOVE__N_MAPPER__C_CALL_PREPARE_MOUSE_FUNCTION__
#define ___N_DATA_GLOVE__N_MAPPER__C_CALL_PREPARE_MOUSE_FUNCTION__


#include "version.h"

#include <string>

#include "cCallMouseFunction.h"

#include "eMouseFunction.h"


namespace nDataGlove{
namespace nMapper{


class cCallPrepareMouseFunction : public cCallMouseFunction{
public:

	/**
	 * constructor
	 *
	 * @param inSzParameters A string with parameters for the call to the mouse
	 * 	@see analyseAndSetParameters()
	 */
	cCallPrepareMouseFunction( const std::string inSzParameters );
	
	/**
	 * constructor
	 *
	 * @param MouseFunction The mouse function, which will be prepared
	 * 	with this operator.
	 * 	@see mouseFunction
	 */
	cCallPrepareMouseFunction( const eMouseFunction inMouseFunction );

	/**
	 * constructor for preparing mouse movements
	 *
	 * @param MouseFunction The mouse function, which will be prepared
	 * 	with this operator. (should be a mouse movement.)
	 * 	@see mouseFunction
	 * @param inAmount The amount for the mouse movement.
	 * 	@see mouseFunction
	 */
	cCallPrepareMouseFunction( const eMouseFunction inMouseFunction,
		const int inAmount );
	
	
	/**
	 * destructor
	 */
	virtual ~cCallPrepareMouseFunction();
	
	
	/**
	 * @return the name of this class "cCallPrepareMouseFunction"
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
	 * Unprepares this keybord function.
	 * (After the call, no keybord function will be prepared.)
	 */
	static void unprepare();
	

	/**
	 * @return The actual prepared mouse function, which
	 * 	cCallMouseFunction should use.
	 * 	If it is "CHAR" cCharPrepared contains the to output char.
	 * 	@see pMouseFunctionPrepared
	 */
	static cCallPrepareMouseFunction * getPreparedFunction();
	
	/**
	 * @return true if the given kyboard function is prepared, else false
	 */
	bool IsPrepared( cCallMouseFunction * pCallMouseFunction ) const;
	
	
protected:
	
//members
	
	/**
	 * The actual prepared mouse function, which cCallMouseFunction
	 * should use.
	 * @see getPreparedFunction()
	 */
	static cCallPrepareMouseFunction * pMouseFunctionPrepared;


};//end class cCallPrepareMouseFunction


};//end namespace nMapper
};//end namespace nDataGlove

#endif //___N_DATA_GLOVE__N_MAPPER__C_CALL_PREPARE_MOUSE_FUNCTION__





