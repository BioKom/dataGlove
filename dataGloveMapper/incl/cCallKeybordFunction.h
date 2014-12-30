
//TODO check

/**
 * @file cCallKeybordFunction
 * file name: cCallKeybordFunction.h
 * @author Betti Oesterholz
 * @date 29.08.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies the class for calling a keyboard function.
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
 * This file specifies the class for calling a keyboard function.
 * Keyboard functions can be pressed chars (e.g. 'a', 'B', '#') or other
 * keys (e.g. Strg, F1).
 *
 * @pattern functor
 * @see cCallKeybordFunction
 * @see iCallFunction
 * @see cDataGloveState
 * @see cEvaluateDataGloveState
 */
/*
History:
29.08.2014  Oesterholz  created
*/


#ifndef ___N_DATA_GLOVE__N_MAPPER__C_CALL_KEYBOARD_FUNCTION__
#define ___N_DATA_GLOVE__N_MAPPER__C_CALL_KEYBOARD_FUNCTION__


#include "version.h"

#include <string>
#include <list>

#include "iCallFunction.h"
#include "eKeybordFunction.h"


namespace nDataGlove{
namespace nMapper{


class cCallKeybordFunction : public iCallFunction{
public:

	/**
	 * constructor
	 *
	 * @param inSzParameters A string with parameters for the call to a key code
	 * 	@see analyseAndSetParameters()
	 * @param bInPrepareNeeded if true preparing the keyboard function is
	 * 	needed, else not
	 * 	@see bPrepareNeeded
	 * 	@see cCallPrepareKeybordFunction
	 */
	cCallKeybordFunction( const std::string inSzParameters,
		const bool bInPrepareNeeded = true );
	
	/**
	 * constructor
	 *
	 * @param inKeyCode1 (First) Key code to call (like defined in <linux/input.h>)
	 * 	@see keyCode1
	 * @param bInPrepareNeeded if true preparing the keyboard function is
	 * 	needed, else not
	 * 	@see bPrepareNeeded
	 * 	@see cCallPrepareKeybordFunction
	 */
	cCallKeybordFunction( const int inKeyCode1,
		const bool bInPrepareNeeded = true );

	/**
	 * constructor
	 *
	 * @param inKeyCode1 First key code to call (like defined in <linux/input.h>)
	 * 	@see keyCode1
	 * @param inKeyCode1 second key code to call (like defined in <linux/input.h>)
	 * 	@see keyCode2
	 * @param bInPrepareNeeded if true preparing the keyboard function is
	 * 	needed, else not
	 * 	@see bPrepareNeeded
	 * 	@see cCallPrepareKeybordFunction
	 */
	cCallKeybordFunction( const int inKeyCode1, const int inKeyCode2,
		const bool bInPrepareNeeded = true );
	
	/**
	 * constructor
	 *
	 * @param inLiKeys A list with key codes to call (like defined in <linux/input.h>)
	 * 	@see keyCode1
	 * @param bInPrepareNeeded if true preparing the keyboard function is
	 * 	needed, else not
	 * 	@see bPrepareNeeded
	 * 	@see cCallPrepareKeybordFunction
	 */
	cCallKeybordFunction( const std::list< unsigned int > inLiKeys,
		const bool bInPrepareNeeded = true );
	
	/**
	 * constructor
	 *
	 * @param KeybordFunction The keyboard function, which will be prepared
	 * 	with this operator.
	 * 	@see keybordFunction
	 * @param bInPrepareNeeded if true preparing the keyboard function is
	 * 	needed, else not
	 * 	@see bPrepareNeeded
	 * 	@see cCallPrepareKeybordFunction
	 */
	cCallKeybordFunction( const eKeybordFunction inKeybordFunction,
		const bool bInPrepareNeeded = true );
	
	/**
	 * constructor
	 *
	 * @param cChar The char/key, which will be prepared with this operator.
	 * 	@see cChar
	 * 	@see keybordFunction
	 * @param bInPrepareNeeded if true preparing the keyboard function is
	 * 	needed, else not
	 * 	@see bPrepareNeeded
	 * 	@see cCallPrepareKeybordFunction
	 */
	cCallKeybordFunction( const char cInChar,
		const bool bInPrepareNeeded = true );
	
	
	/**
	 * destructor
	 */
	virtual ~cCallKeybordFunction();
	
	/**
	 * @return the name of this class "cCallKeybordFunction"
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
	 * @return The keyboard function, which will be prepared
	 * 	with this operator.
	 * 	If it is "CHAR" cChar contains the to output char.
	 * 	@see keybordFunction
	 */
	eKeybordFunction getKeybordFunction() const;
	
	/**
	 * @return If true preparing the modus change is needed, else not.
	 * 	@see bPrepareNeeded
	 */
	bool getPrepareNeeded() const;

	
	/**
	 * @return First key code like defined in <linux/input.h> or 0 if non is used.
	 * 	Just valid if keybordFunction is "INPUT_KEY_CODE_1" or "INPUT_KEY_CODE_2".
	 * 	@see keyCode1
	 * @see keybordFunction
	 * @see eKeybordFunction::INPUT_KEY_CODE_1
	 * @see eKeybordFunction::INPUT_KEY_CODE_2
	 */
	int getKeyCode1() const;
	
	/**
	 * @return Second key code like defined in <linux/input.h> or 0 if non is used.
	 * 	Just valid if keybordFunction is "INPUT_KEY_CODE_2".
	 * 	@see keyCode2
	 * @see keybordFunction
	 * @see eKeybordFunction::INPUT_KEY_CODE_2
	 */
	int getKeyCode2() const;
	
	/**
	 * @return A list of key codes like defined in <linux/input.h> or empty if non is used.
	 * 	Just valid if keybordFunction is "INPUT_KEY_CODE".
	 * 	@see liKeys
	 * @see keybordFunction
	 * @see eKeybordFunction::INPUT_KEY_CODE
	 */
	std::list< unsigned int > getKeyCodes() const;
	
	/**
	 * @return The char/key, which will be prepared with this operator.
	 * 	Just valid if keybordFunction is "CHAR".
	 * 	@see cChar
	 * 	@see keybordFunction
	 */
	wchar_t getChar() const;
	
	
protected:
	
	/**
	 * This function converts the given char to a key code to call with
	 * the operator.
	 *
	 * @see keybordFunctionForOperator
	 * @param cInChar the char to convert to a operator call
	 */
	void charToOperatorFunction( const wchar_t cInChar );
	
	/**
	 * This function converts the given keyboard function to the members to
	 * call with the operator.
	 * Sets the value for:
	 * 	@see keybordFunction
	 * 	@see keybordFunctionForOperator
	 *
	 * Also sets if needed:
	 * 	@see keyCode1
	 * 	@see keyCode2
	 * 	@see liKeys
	 *
	 * @param inKeybordFunction the keyboard function to set
	 * 	@see keybordFunction
	 */
	void keyboardFunctionToOperatorFunction(
			const eKeybordFunction inKeybordFunction );
	
	/**
	 * This function returns the keyboard function for the given string.
	 *
	 * @param inSzParameters the string for which to return the keyboard function
	 * @return the keyboard function for the given string, or NON if non could
	 * 	be evaluated
	 */
	static eKeybordFunction getKeyboardFunction(
		const std::string & inSzParameters );
	
	/**
	 * This function returns the key code like defined in <linux/input.h> for
	 * the given string.
	 *
	 * @param inSzKeyCode the string for which to return the key code
	 * @return the key code for the given string, or 0 if non could be evaluated
	 */
	static unsigned int readKeyCode( const std::string & inSzKeyCode );
	
	
	/**
	 * This method analyses and setst the given parameters.
	 * It wil set:
	 * 	@see keybordFunctionForOperator
	 * 	@see keybordFunction
	 * 	@see keyCode1
	 * 	@see keyCode2
	 * 	@see liKeys
	 * 	@see cChar
	 *
	 * @param inSzParameters the string with the parameters for the keyboard function
	 */
	void analyseAndSetParameters( const std::string & inSzParameters );
	
//members
	/**
	 * The keyboard function, which will be prepared with this operator.
	 * If it is "CHAR" cChar contains the to output char.
	 * @see getKeybordFunction()
	 * @see cChar
	 */
	eKeybordFunction keybordFunction;

	/**
	 * The keyboard function, which will be prepared with this operator.
	 * If it is "CHAR" cChar contains the to output char.
	 * This function wil be used in the operator method.
	 * @see getKeybordFunction()
	 * @see cChar
	 */
	eKeybordFunction keybordFunctionForOperator;
	
	/**
	 * First key code like defined in <linux/input.h> or 0 if non is used.
	 * Just valid if keybordFunction is "INPUT_KEY_CODE_1" or "INPUT_KEY_CODE_2".
	 * @see keybordFunction
	 * @see eKeybordFunction::INPUT_KEY_CODE_1
	 * @see eKeybordFunction::INPUT_KEY_CODE_2
	 */
	unsigned int keyCode1;
	
	/**
	 * Second key code like defined in <linux/input.h> or 0 if non is used.
	 * Just valid if keybordFunction is "INPUT_KEY_CODE_2".
	 * @see keybordFunction
	 * @see eKeybordFunction::INPUT_KEY_CODE_2
	 */
	unsigned int keyCode2;
	
	/**
	 * A list of key codes like defined in <linux/input.h> or empty if non is used.
	 * Just valid if keybordFunction is "INPUT_KEY_CODE".
	 * @see keybordFunction
	 * @see eKeybordFunction::INPUT_KEY_CODE
	 */
	std::list< unsigned int > liKeys;
	
	/**
	 * The char/key, which will be prepared with this operator.
	 * Just valid if keybordFunction is "CHAR".
	 * @see getChar()
	 * @see keybordFunction
	 * @see eKeybordFunction::CHAR
	 */
	wchar_t cChar;
	
	
	/**
	 * If true preparing the modus change is needed, else not.
	 * @see getPrepareNeeded()
	 * @see cCallPrepareKeybordFunction
	 */
	const bool bPrepareNeeded;
	
};//end class cCallKeybordFunction


};//end namespace nMapper
};//end namespace nDataGlove

#endif //___N_DATA_GLOVE__N_MAPPER__C_CALL_KEYBOARD_FUNCTION__





