
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
 * This file implements the class for a preparing a mouse function.
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
 * This file implements the class for a preparing a mouse function.
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


#include "cCallPrepareMouseFunction.h"

#if ( defined DEBUG_CALL_FUNCTION ) || ( defined DEBUG_PREPARED_KEYBOARD_FUNCTION )
	#include <iostream>
#endif  //( defined DEBUG_CALL_FUNCTION ) || ( defined DEBUG_PREPARED_KEYBOARD_FUNCTION )


#ifdef DEBUG_CALL_FUNCTION
	#ifndef DEBUG_PREPARED_KEYBOARD_FUNCTION
		#define DEBUG_PREPARED_KEYBOARD_FUNCTION
	#endif  //DEBUG_PREPARED_KEYBOARD_FUNCTION
#endif  //DEBUG_CALL_FUNCTION



using namespace nDataGlove::nMapper;
using namespace std;

cCallPrepareMouseFunction *
	cCallPrepareMouseFunction::pMouseFunctionPrepared = NULL;


/**
 * constructor
 *
 * @param inSzParameters A string with parameters for the call to the mouse
 * 	@see analyseAndSetParameters()
 */
cCallPrepareMouseFunction::cCallPrepareMouseFunction( const std::string inSzParameters ) :
		cCallMouseFunction( inSzParameters, true ) {
	//nothing to do
}


/**
 * constructor
 *
 * @param MouseFunction The mouse function, which will be prepared
 * 	with this operator.
 * 	@see mouseFunction
 */
cCallPrepareMouseFunction::cCallPrepareMouseFunction( const eMouseFunction inMouseFunction ) :
		cCallMouseFunction( inMouseFunction, true ) {
	//nothing to do
}


/**
 * constructor for preparing mouse movements
 *
 * @param MouseFunction The mouse function, which will be prepared
 * 	with this operator. (should be a mouse movement.)
 * 	@see mouseFunction
 * @param inAmount The amount for the mouse movement.
 * 	@see mouseFunction
 */
cCallPrepareMouseFunction::cCallPrepareMouseFunction( const eMouseFunction inMouseFunction,
	const int inAmount ) :
		cCallMouseFunction( inMouseFunction, inAmount, true ) {
	//nothing to do
}



/**
 * destructor
 */
cCallPrepareMouseFunction::~cCallPrepareMouseFunction() {
	//nothing to do
}


/**
 * @return the name of this class "cCallPrepareMouseFunction"
 */
std::string cCallPrepareMouseFunction::getName() const {
	
	return std::string("cCallPrepareMouseFunction");
}


/**
 * This functor operator is called the execute the function implemented
 * by this object.
 *
 * @return true if the call to the function was sucessfull, else false
 */
bool cCallPrepareMouseFunction::operator()() {
	
	//call parent operator
	iCallFunction::operator()();
	//prepare this mouse function
	pMouseFunctionPrepared = this;
#ifdef DEBUG_PREPARED_KEYBOARD_FUNCTION
	cout<<"preparing mouse function: "<<mouseFunction<<
		" ("<<mouseFunction<<"'; "<<iAmount<<")"<<endl;
#endif  //DEBUG_PREPARED_KEYBOARD_FUNCTION
	return true;
}


/**
 * This method ends the operation.
 * (Call if the state changes.)
 */
void cCallPrepareMouseFunction::end() {
	
#ifdef DEBUG_CALL_FUNCTION
	cout<<"end preparing mouse function: "<<mouseFunction<<
		" ("<<mouseFunction<<"'; "<<iAmount<<")"<<endl;
#endif  //DEBUG_CALL_FUNCTION
}


/**
 * Unprepares this keybord function.
 * (After the call, no keybord function will be prepared.)
 */
void cCallPrepareMouseFunction::unprepare() {
	
#ifdef DEBUG_CALL_FUNCTION
	cout<<"unpreparing mouse function: "<<mouseFunction<<
		" ("<<mouseFunction<<"'; "<<iAmount<<")"<<endl;
	} else {
		cout<<"NULL"<<endl;
	}
#endif  //DEBUG_CALL_FUNCTION
	pMouseFunctionPrepared = NULL;
}


/**
 * @return The actual prepared mouse function, which
 * 	cCallMouseFunction should use.
 * 	If it is "CHAR" cCharPrepared contains the to output char.
 * 	@see pMouseFunctionPrepared
 */
cCallPrepareMouseFunction * cCallPrepareMouseFunction::getPreparedFunction() {
	
	return pMouseFunctionPrepared;
}


/**
 * @return true if the given kyboard function is prepared, else false
 */
bool cCallPrepareMouseFunction::IsPrepared(
		cCallMouseFunction * pCallMouseFunction ) const {
	
	return ( pCallMouseFunction->getMouseFunction() == mouseFunction ) &&
			( pCallMouseFunction->getAmount() == iAmount ) &&
			( pCallMouseFunction->getAmount2() == iAmount2 );
}




























