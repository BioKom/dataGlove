
//TODO check

/**
 * @file cCallPrepareKeybordFunction
 * file name: cCallPrepareKeybordFunction.h
 * @author Betti Oesterholz
 * @date 28.08.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies the class for a preparing a keyboard function.
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
 * This file specifies the class for a preparing a keyboard function.
 * Keyboard functions can be pressed chars (e.g. 'a', 'B', '#') or other
 * keys (e.g. Strg, F1).
 * The class cCallKeybordFunction will just work, if this operation was
 * activated for the correct modus.
 *
 * @pattern functor
 * @see cCallKeybordFunction
 * @see iCallFunction
 * @see cDataGloveState
 * @see cEvaluateDataGloveState
 */
/*
History:
28.08.2014  Oesterholz  created
*/


#ifndef ___N_DATA_GLOVE__N_MAPPER__C_CALL_PREPARE_KEYBOARD_FUNCTION__
#define ___N_DATA_GLOVE__N_MAPPER__C_CALL_PREPARE_KEYBOARD_FUNCTION__


#include "version.h"

#include <string>

#include "cCallKeybordFunction.h"

#include "eKeybordFunction.h"


namespace nDataGlove{
namespace nMapper{


class cCallPrepareKeybordFunction : public cCallKeybordFunction{
public:

#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	/**
	 * constructor
	 *
	 * @param inSzParameters A string with parameters for the call to a key code
	 * 	@see analyseAndSetParameters()
	 */
	cCallPrepareKeybordFunction( const std::wstring inSzParameters );
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	
	/**
	 * constructor
	 *
	 * @param inSzParameters A string with parameters for the call to a key code
	 * 	@see analyseAndSetParameters()
	 */
	cCallPrepareKeybordFunction( const std::string inSzParameters );
	
	/**
	 * constructor
	 *
	 * @param inKeyCode1 (First) Key code to call (like defined in <linux/input.h>)
	 * 	@see keyCode1
	 */
	cCallPrepareKeybordFunction( const int inKeyCode1 );

	/**
	 * constructor
	 *
	 * @param inKeyCode1 First key code to call (like defined in <linux/input.h>)
	 * 	@see keyCode1
	 * @param inKeyCode1 second key code to call (like defined in <linux/input.h>)
	 * 	@see keyCode2
	 */
	cCallPrepareKeybordFunction( const int inKeyCode1, const int inKeyCode2 );
	
	/**
	 * constructor
	 *
	 * @param inLiKeys A list with key codes to call (like defined in <linux/input.h>)
	 * 	@see keyCode1
	 */
	cCallPrepareKeybordFunction( const std::list< unsigned int > inLiKeys );

	/**
	 * constructor
	 *
	 * @param KeybordFunction The keyboard function, which will be prepared
	 * 	with this operator.
	 * 	@see keybordFunction
	 */
	cCallPrepareKeybordFunction( const eKeybordFunction inKeybordFunction );
	
	/**
	 * constructor
	 *
	 * @param cChar The char/key, which will be prepared with this operator.
	 * 	@see cChar
	 * 	@see keybordFunction
	 */
	cCallPrepareKeybordFunction( const wchar_t cInChar );
	
	/**
	 * destructor
	 */
	virtual ~cCallPrepareKeybordFunction();
	
	
	/**
	 * @return the name of this class "cCallPrepareKeybordFunction"
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
	 * @return The actual prepared keyboard function, which
	 * 	cCallKeybordFunction should use.
	 * 	If it is "CHAR" cCharPrepared contains the to output char.
	 * 	@see pKeybordFunctionPrepared
	 */
	static cCallPrepareKeybordFunction * getPreparedFunction();
	
	/**
	 * @return true if the given kyboard function is prepared, else false
	 */
	bool IsPrepared( cCallKeybordFunction * pCallKeybordFunction ) const;
	
	
protected:
	
//members
	
	/**
	 * The actual prepared keyboard function, which cCallKeybordFunction
	 * should use.
	 * @see getPreparedFunction()
	 */
	static cCallPrepareKeybordFunction * pKeybordFunctionPrepared;


};//end class cCallPrepareKeybordFunction


};//end namespace nMapper
};//end namespace nDataGlove

#endif //___N_DATA_GLOVE__N_MAPPER__C_CALL_PREPARE_KEYBOARD_FUNCTION__





