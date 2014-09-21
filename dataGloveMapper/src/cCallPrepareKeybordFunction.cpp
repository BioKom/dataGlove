
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
 * This file implements the class for a preparing a keyboard function.
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
 * This file implements the class for a preparing a keyboard function.
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


#include "cCallPrepareKeybordFunction.h"

#ifdef DEBUG_CALL_FUNCTION
	#include <iostream>
#endif  //DEBUG_CALL_FUNCTION



using namespace nDataGlove::nMapper;
using namespace std;

cCallPrepareKeybordFunction *
	cCallPrepareKeybordFunction::pKeybordFunctionPrepared = NULL;


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
cCallPrepareKeybordFunction::cCallPrepareKeybordFunction(
		const std::string inSzParameters, const bool bInPrepareNeeded ) :
		cCallKeybordFunction( inSzParameters, bInPrepareNeeded ) {
	//nothing to do
}


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
cCallPrepareKeybordFunction::cCallPrepareKeybordFunction( const int inKeyCode1,
	const bool bInPrepareNeeded ) :
		cCallKeybordFunction( inKeyCode1, bInPrepareNeeded ) {
	//nothing to do
}


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
cCallPrepareKeybordFunction::cCallPrepareKeybordFunction( const int inKeyCode1, const int inKeyCode2,
	const bool bInPrepareNeeded ) :
		cCallKeybordFunction( inKeyCode1, inKeyCode2, bInPrepareNeeded ) {
	//nothing to do
}


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
cCallPrepareKeybordFunction::cCallPrepareKeybordFunction(
	const std::list< unsigned int > inLiKeys,
	const bool bInPrepareNeeded ) :
		cCallKeybordFunction( inLiKeys, bInPrepareNeeded ) {
	//nothing to do
}



/**
 * constructor
 *
 * @param KeybordFunction The keyboard function, which will be prepared
 * 	with this operator.
 * 	@see keybordFunction
 */
cCallPrepareKeybordFunction::cCallPrepareKeybordFunction(
		const eKeybordFunction inKeybordFunction ) :
		cCallKeybordFunction( inKeybordFunction ) {
	//nothing to do
}


/**
 * constructor
 *
 * @param cChar The char/key, which will be prepared with this operator.
 * 	@see cChar
 * 	@see keybordFunction
 */
cCallPrepareKeybordFunction::cCallPrepareKeybordFunction(
		const wchar_t cInChar ) : cCallKeybordFunction( cInChar ) {
	//nothing to do
}


/**
 * destructor
 */
cCallPrepareKeybordFunction::~cCallPrepareKeybordFunction() {
	//nothing to do
}


/**
 * @return the name of this class "cCallPrepareKeybordFunction"
 */
std::string cCallPrepareKeybordFunction::getName() const {
	
	return std::string("cCallPrepareKeybordFunction");
}


/**
 * This functor operator is called the execute the function implemented
 * by this object.
 *
 * @return true if the call to the function was sucessfull, else false
 */
bool cCallPrepareKeybordFunction::operator()() {
	
	pKeybordFunctionPrepared = this;
#ifdef DEBUG_CALL_FUNCTION
	cout<<"preparing keyboard function: "<<keybordFunction<<" ("<<cChar<<")"<<endl;
	return true;
#endif  //DEBUG_CALL_FUNCTION
}


/**
 * This method ends the operation.
 * (Call if the state changes.)
 */
void cCallPrepareKeybordFunction::end() {
	
	if ( pKeybordFunctionPrepared == this ) {
		
		pKeybordFunctionPrepared = NULL;
	}
#ifdef DEBUG_CALL_FUNCTION
	cout<<"unpreparing keyboard function: "<<getKeybordFunction()<<" ("<<getChar()<<")"<<endl;
#endif  //DEBUG_CALL_FUNCTION
}



/**
 * @return The actual prepared keyboard function, which
 * 	cCallKeybordFunction should use.
 * 	If it is "CHAR" cCharPrepared contains the to output char.
 * 	@see pKeybordFunctionPrepared
 */
cCallPrepareKeybordFunction * cCallPrepareKeybordFunction::getPreparedFunction() {
	
	return pKeybordFunctionPrepared;
}


/*TODO weg
 * template <typename Type>
	equalList( const list< Type > & liFirstList )
*/


/**
 * @return true if the given kyboard function is prepared, else false
 */
bool cCallPrepareKeybordFunction::IsPrepared(
		cCallKeybordFunction * pCallKeybordFunction ) const {
	
	return ( ( pCallKeybordFunction->getKeybordFunction() != keybordFunction ) ||
		( ( keybordFunction == CHAR ) &&
			( pCallKeybordFunction->getChar() != cChar ) ) ||
		( ( keybordFunction == INPUT_KEY_CODE ) &&
			( pCallKeybordFunction->getKeyCodes() != getKeyCodes() ) ) ||
		( ( keybordFunction == INPUT_KEY_CODE_1 ) &&
			( pCallKeybordFunction->getKeyCode1() != getKeyCode1() ) ) ||
		( ( keybordFunction == INPUT_KEY_CODE_2 ) &&
			( pCallKeybordFunction->getKeyCode1() != getKeyCode1() ) &&
			( pCallKeybordFunction->getKeyCode2() != getKeyCode2() ) ) );
}




























