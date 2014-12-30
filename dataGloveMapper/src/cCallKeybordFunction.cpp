
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
 * This file implements the class for calling a keyboard function.
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
 * This file implements the class for calling a keyboard function.
 * Keyboard functions can be pressed chars (e.g. 'a', 'B', '#') or other
 * keys (e.g. Strg, F1).
 *
 * @pattern functor
 * @see cCallPrepareKeybordFunction
 * @see iCallFunction
 * @see cDataGloveState
 * @see cEvaluateDataGloveState
 */
/*
History:
29.08.2014  Oesterholz  created
*/


#include "cCallKeybordFunction.h"

#ifdef DEBUG_CALL_FUNCTION
	#include <iostream>
#endif  //DEBUG_CALL_FUNCTION


#include "cEvaluateDataGloveState.h"
#include "cCallPrepareKeybordFunction.h"
#include "cUInputDevice.h"

#ifdef FEATURE_READ_DATA_TEXT_WITH_REGEX
	#include <regex>
#endif //FEATURE_READ_DATA_TEXT_WITH_REGEX



using namespace nDataGlove::nMapper;
using namespace std;




/**
 * constructor
 *
 * @param inSzParameters A string with parameters for the call to a key code
 * @param bInPrepareNeeded if true preparing the keyboard function is
 * 	needed, else not
 * 	@see bPrepareNeeded
 * 	@see cCallPrepareKeybordFunction
 */
cCallKeybordFunction::cCallKeybordFunction( const std::string inSzParameters,
		const bool bInPrepareNeeded ) :
		keybordFunction( NON ),
		keybordFunctionForOperator( NON ),
		keyCode1( 0 ), keyCode2( 0 ), cChar( 0x0 ),
		bPrepareNeeded( bInPrepareNeeded ) {
	
	analyseAndSetParameters( inSzParameters );
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
cCallKeybordFunction::cCallKeybordFunction( const int inKeyCode1,
		const bool bInPrepareNeeded ) :
		keybordFunction( INPUT_KEY_CODE_1 ),
		keybordFunctionForOperator( INPUT_KEY_CODE_1 ),
		keyCode1( inKeyCode1 ),
		keyCode2( 0 ), cChar( 0x0 ), bPrepareNeeded( bInPrepareNeeded ) {
	
	liKeys.push_back( keyCode1 );
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
cCallKeybordFunction::cCallKeybordFunction( const int inKeyCode1,
		const int inKeyCode2,
		const bool bInPrepareNeeded ) :
		keybordFunction( INPUT_KEY_CODE_2 ),
		keybordFunctionForOperator( INPUT_KEY_CODE_2 ),
		keyCode1( inKeyCode1 ),
		keyCode2( inKeyCode2 ), cChar( 0x0 ), bPrepareNeeded( bInPrepareNeeded ) {
	
	liKeys.push_back( keyCode1 );
	liKeys.push_back( keyCode2 );
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
cCallKeybordFunction::cCallKeybordFunction( const std::list< unsigned int > inLiKeys,
	const bool bInPrepareNeeded ) :
		keybordFunction( INPUT_KEY_CODE ),
		keybordFunctionForOperator( INPUT_KEY_CODE ),
		keyCode1( 0 ), keyCode2( 0 ), liKeys( inLiKeys ),
		cChar( 0x0 ), bPrepareNeeded( bInPrepareNeeded ) {
	
	std::list< unsigned int >::const_iterator itrKey = liKeys.begin();
	if ( itrKey != liKeys.end() ) {
		//set first key code
		keyCode1 = (*itrKey);
		itrKey++;
		if ( itrKey != liKeys.end() ) {
			//set second key code
			keyCode2 = (*itrKey);
		}
	}
}


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
cCallKeybordFunction::cCallKeybordFunction( const eKeybordFunction inKeybordFunction,
	const bool bInPrepareNeeded ) :
		keybordFunction( inKeybordFunction ), keyCode1( 0 ), keyCode2( 0 ),
		cChar( 0x0 ), bPrepareNeeded( bInPrepareNeeded ) {
	
	keyboardFunctionToOperatorFunction( inKeybordFunction );
	
}


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
cCallKeybordFunction::cCallKeybordFunction( const char cInChar,
	const bool bInPrepareNeeded ) :
		keybordFunction( CHAR ), keyCode1( 0 ), keyCode2( 0 ),
		cChar( cInChar ), bPrepareNeeded( bInPrepareNeeded ) {
	
	charToOperatorFunction( cInChar );
}



/**
 * destructor
 */
cCallKeybordFunction::~cCallKeybordFunction() {
	//nothing to do
}


/**
 * @return the name of this class "cCallKeybordFunction"
 */
std::string cCallKeybordFunction::getName() const {
	return std::string("cCallKeybordFunction");
}


/**
 * This functor operator is called the execute the function implemented
 * by this object.
 *
 * @return true if the call to the function was sucessfull, else false
 */
bool cCallKeybordFunction::operator()() {
	
	if ( bPrepareNeeded &&
			( ( cCallPrepareKeybordFunction::getPreparedFunction() == NULL ) ||
			( ! cCallPrepareKeybordFunction::getPreparedFunction()->
				IsPrepared( this ) ) ) ) {
		//keyboard function was not prepared, but should be
		//call parent operator (unprepare)
#ifdef DEBUG_CALL_FUNCTION
		cout<<"keyboard function was not prepared: "<<keybordFunction<<
			" ("<<keybordFunctionForOperator<<"; '"<<cChar<<"'; "<<
			keyCode1<<" "<<keyCode2<<")"<<endl;
#endif  //DEBUG_CALL_FUNCTION
		iCallFunction::operator()();
		return false;
	}
	//call keybord function
	
	//open / create uinput device
	cUInputDevice * pUInputDevice = cUInputDevice::getInstance();
	
	if ( ! pUInputDevice->isDeviceOpen() ) {
		return false;  //Error: Device could not be opened
	}
	
	//press keys
	switch ( keybordFunctionForOperator ) {
		case INPUT_KEY_CODE : {
			pUInputDevice->keysDown( liKeys );
		}; break;
		case INPUT_KEY_CODE_1 : {
			pUInputDevice->keyDown( keyCode1 );
		}; break;
		case INPUT_KEY_CODE_2 : {
			pUInputDevice->key2Down( keyCode1, keyCode2 );
		}; break;
		default: break;  //do nothing
	};  //end switch keybordFunctionForOperator
	
#ifdef DEBUG_CALL_FUNCTION
	cout<<"calling keyboard function: "<<keybordFunction<<
		" ("<<keybordFunctionForOperator<<"; '"<<cChar<<"'; "<<
		keyCode1<<" "<<keyCode2<<")"<<endl;
#endif  //DEBUG_CALL_FUNCTION
	return true;
}


/**
 * This method ends the operation.
 * (Call if the state changes.)
 */
void cCallKeybordFunction::end() {
	
	//open / create uinput device
	cUInputDevice * pUInputDevice = cUInputDevice::getInstance();
	
	if ( ! pUInputDevice->isDeviceOpen() ) {
		return;  //Error: Device could not be opened
	}
	//unpress keys
	switch ( keybordFunctionForOperator ) {
		case INPUT_KEY_CODE : {
			pUInputDevice->keysUpReverse( liKeys );
		}; break;
		case INPUT_KEY_CODE_1 : {
			pUInputDevice->keyUp( keyCode1 );
		}; break;
		case INPUT_KEY_CODE_2 : {
			pUInputDevice->key2UpReverse( keyCode1, keyCode2 );
		}; break;
		default: break;  //do nothing
	};  //end switch keybordFunctionForOperator
#ifdef DEBUG_CALL_FUNCTION
	cout<<"end calling keyboard function: "<<keybordFunction<<
		" ("<<keybordFunctionForOperator<<"; '"<<cChar<<"'; "<<
		keyCode1<<" "<<keyCode2<<")"<<endl;
#endif  //DEBUG_CALL_FUNCTION
}


/**
 * @return The keyboard function, which will be prepared
 * 	with this operator.
 * 	If it is "CHAR" cChar contains the to output char.
 * 	@see keybordFunction
 */
eKeybordFunction cCallKeybordFunction::getKeybordFunction() const {
	
	return keybordFunction;
}


/**
 * @return First key code like defined in <linux/input.h> or 0 if non is used.
 * 	Just valid if keybordFunction is "INPUT_KEY_CODE_1" or "INPUT_KEY_CODE_2".
 * 	@see keyCode1
 * @see keybordFunction
 * @see eKeybordFunction::INPUT_KEY_CODE_1
 * @see eKeybordFunction::INPUT_KEY_CODE_2
 */
int cCallKeybordFunction::getKeyCode1() const {
	
	return keyCode1;
}


/**
 * @return Second key code like defined in <linux/input.h> or 0 if non is used.
 * 	Just valid if keybordFunction is "INPUT_KEY_CODE_2".
 * 	@see keyCode2
 * @see keybordFunction
 * @see eKeybordFunction::INPUT_KEY_CODE_2
 */
int cCallKeybordFunction::getKeyCode2() const {
	
	return keyCode2;
}


/**
 * @return A list of key codes like defined in <linux/input.h> or empty if non is used.
 * 	Just valid if keybordFunction is "INPUT_KEY_CODE".
 * 	@see liKeys
 * @see keybordFunction
 * @see eKeybordFunction::INPUT_KEY_CODE
 */
std::list< unsigned int > cCallKeybordFunction::getKeyCodes() const {
	
	return liKeys;
}



/**
 * @return If true preparing the modus change is needed, else not.
 * 	@see bPrepareNeeded
 */
bool cCallKeybordFunction::getPrepareNeeded() const {
	
	return bPrepareNeeded;
}



/**
 * @return The char/key, which will be prepared with this operator.
 * 	Just valid if keybordFunction is "CHAR".
 * 	@see cChar
 * 	@see keybordFunction
 */
wchar_t cCallKeybordFunction::getChar() const {
	
	return cChar;
}


/**
 * This function converts the given char to a key code to call with
 * the operator.
 *
 * @see keybordFunctionForOperator
 * @param cInChar the char to convert to a operator call
 */
void cCallKeybordFunction::charToOperatorFunction( const wchar_t cInChar ) {
	
	switch ( cInChar ) {
		case '1': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 2;
		}; break;
		case '2': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 3;
		}; break;
		case '3': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 4;
		}; break;
		case '4': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 5;
		}; break;
		case '5': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 6;
		}; break;
		case '6': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 7;
		}; break;
		case '7': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 8;
		}; break;
		case '8': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 9;
		}; break;
		case '9': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 10;
		}; break;
		case '0': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 11;
		}; break;
		case L'ß': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 12;
		}; break;
		case L'´': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 13;
		}; break;
		case '	': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 15;
		}; break;
		case 'q': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 16;
		}; break;
		case 'w': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 17;
		}; break;
		case 'e': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 18;
		}; break;
		case 'r': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 19;
		}; break;
		case 't': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 20;
		}; break;
		case 'z': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 21;
		}; break;
		case 'u': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 22;
		}; break;
		case 'i': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 23;
		}; break;
		case 'o': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 24;
		}; break;
		case 'p': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 25;
		}; break;
		case L'ü': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 26;
		}; break;
		case '+': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 27;
		}; break;
		case 'a': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 30;
		}; break;
		case 's': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 31;
		}; break;
		case 'd': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 32;
		}; break;
		case 'f': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 33;
		}; break;
		case 'g': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 34;
		}; break;
		case 'h': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 35;
		}; break;
		case 'j': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 36;
		}; break;
		case 'k': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 37;
		}; break;
		case 'l': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 38;
		}; break;
		case L'ö': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 39;
		}; break;
		case L'ä': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 40;
		}; break;
		case '^': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 41;
		}; break;
		case '#': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 43;
		}; break;
		case 'y': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 44;
		}; break;
		case 'x': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 45;
		}; break;
		case 'c': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 46;
		}; break;
		case 'v': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 47;
		}; break;
		case 'b': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 48;
		}; break;
		case 'n': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 49;
		}; break;
		case 'm': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 50;
		}; break;
		case ',': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 51;
		}; break;
		case '.': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 52;
		}; break;
		case '-': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 53;
		}; break;
		case '*': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 55;
		}; break;
		case ' ': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 57;
		}; break;
		case '<': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 86;
		}; break;
		case '/': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 98;
		}; break;
		case '=': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 117;
		}; break;
		case L'±': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 118;
		}; break;
		case '!': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 2;
		}; break;
		case '"': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 3;
		}; break;
		case L'§': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 4;
		}; break;
		case '$': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 5;
		}; break;
		case '%': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 6;
		}; break;
		case '&': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 7;
		}; break;
		case '(': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 9;
		}; break;
		case ')': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 10;
		}; break;
		case '?': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 12;
		}; break;
		case '`': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 13;
		}; break;
		case 'Q': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 16;
		}; break;
		case 'W': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 17;
		}; break;
		case 'E': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 18;
		}; break;
		case 'R': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 19;
		}; break;
		case 'T': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 20;
		}; break;
		case 'Z': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 21;
		}; break;
		case 'U': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 22;
		}; break;
		case 'I': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 23;
		}; break;
		case 'O': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 24;
		}; break;
		case 'P': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 25;
		}; break;
		case L'Ü': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 26;
		}; break;
		case 'A': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 30;
		}; break;
		case 'S': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 31;
		}; break;
		case 'D': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 32;
		}; break;
		case 'F': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 33;
		}; break;
		case 'G': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 34;
		}; break;
		case 'H': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 35;
		}; break;
		case 'J': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 36;
		}; break;
		case 'K': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 37;
		}; break;
		case 'L': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 38;
		}; break;
		case L'Ö': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 39;
		}; break;
		case L'Ä': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 40;
		}; break;
		case L'°': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 41;
		}; break;
		case '\'': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 43;
		}; break;
		case 'Y': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 44;
		}; break;
		case 'X': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 45;
		}; break;
		case 'C': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 46;
		}; break;
		case 'V': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 47;
		}; break;
		case 'B': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 48;
		}; break;
		case 'N': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 49;
		}; break;
		case 'M': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 50;
		}; break;
		case ';': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 51;
		}; break;
		case ':': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 52;
		}; break;
		case '_': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 53;
		}; break;
		case '>': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 86;
		}; break;
		case '{': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_RIGHTALT;
			keyCode2 = 8;
		}; break;
		case '[': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_RIGHTALT;
			keyCode2 = 9;
		}; break;
		case ']': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_RIGHTALT;
			keyCode2 = 10;
		}; break;
		case '}': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_RIGHTALT;
			keyCode2 = 11;
		}; break;
		case '\\': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_RIGHTALT;
			keyCode2 = 12;
		}; break;
		case '|': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_RIGHTALT;
			keyCode2 = 86;
		}; break;
	};  //end switch cInChar
	//fill liKeys
	if ( keybordFunctionForOperator == INPUT_KEY_CODE_1 ) {
		//just one key code exists
		liKeys.push_back( keyCode1 );
	} else if ( keybordFunctionForOperator == INPUT_KEY_CODE_2 ) {
		//two key codes exists
		liKeys.push_back( keyCode1 );
		liKeys.push_back( keyCode2 );
	}
}



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
void cCallKeybordFunction::keyboardFunctionToOperatorFunction(
		const eKeybordFunction inKeybordFunction ) {
	
	keybordFunction = inKeybordFunction;
	//empty old values
	keyCode1 = 0;
	keyCode2 = 0;
	liKeys.clear();
	
	switch ( inKeybordFunction ) {
		case F1: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_F1;
		}; break;
		case F2: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_F2;
		}; break;
		case F3: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_F3;
		}; break;
		case F4: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_F4;
		}; break;
		case F5: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_F5;
		}; break;
		case F6: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_F6;
		}; break;
		case F7: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_F7;
		}; break;
		case F8: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_F8;
		}; break;
		case F9: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_F9;
		}; break;
		case F10: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_F10;
		}; break;
		case F11: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_F11;
		}; break;
		case F12: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_F12;
		}; break;
		case TAB: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_TAB;
		}; break;
		case BACK_TAB: {
			//SHIFT + TAB
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = KEY_TAB;
		}; break;
		case ESC: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_ESC;
		}; break;
		case SPACE: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_SPACE;
		}; break;
		case ENTER: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_ENTER;
		}; break;
		case UPPER_CASE: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_CAPSLOCK;
		}; break;
		case CHAPS_LOCK: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_CAPSLOCK;
		}; break;
		case UP: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_UP;
		}; break;
		case DOWN: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_DOWN;
		}; break;
		case LEFT: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_LEFT;
		}; break;
		case RIGHT: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_RIGHT;
		}; break;
		case PAGE_UP: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_PAGEUP;
		}; break;
		case PAGE_DOWN: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_PAGEDOWN;
		}; break;
		case POS1: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_HOME;
		}; break;
		case END: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_END;
		}; break;
		case DELETE: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_BACKSPACE;
		}; break;
		case REMOVE: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_DELETE;
		}; break;
		case INSERT: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_INSERT;
		}; break;
		case ALT: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_RIGHTALT;
		}; break;
		case CTRL: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_LEFTCTRL;
		}; break;
		case PRINT: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_PRINT;
		}; break;
		case MENUE: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_MENU;
		}; break;
		case SCROLL: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_SCROLLLOCK;
		}; break;
		case BREAK: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_BREAK;
			break;
		};
		
		case UNDO: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_UNDO;
		}; break;
		case UNUNDO: {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = KEY_UNDO;
		}; break;
		
		case END_APPLICATION: {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTCTRL;
			keyCode2 = 16;
		}; break;
		
		case COPY: {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTCTRL;
			keyCode2 = 46;
		}; break;
		case PAST: {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTCTRL;
			keyCode2 = 47;
		}; break;
		
		case NEXT_WORD: {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTCTRL;
			keyCode2 = KEY_RIGHT;
		}; break;
		case PREVIOS_WORD: {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTCTRL;
			keyCode2 = KEY_LEFT;
		}; break;
		case MARK_NEXT_WORD: {
			keybordFunctionForOperator = INPUT_KEY_CODE;
			liKeys.push_back( KEY_LEFTCTRL );
			liKeys.push_back( KEY_LEFTSHIFT );
			liKeys.push_back( KEY_RIGHT );
		}; break;
		
		
		case QUOTE: {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 3;
		}; break;
		case SINGLE_QUOTE: {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 43;
		}; break;
		case SEMICOLON: {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 51;
		}; break;
		
		default: break;  //unknown keybord function
	};  //end switch inKeybordFunction
	//fill liKeys
	if ( keybordFunctionForOperator == INPUT_KEY_CODE_1 ) {
		//just one key code exists
		liKeys.push_back( keyCode1 );
	} else if ( keybordFunctionForOperator == INPUT_KEY_CODE_2 ) {
		//two key codes exists
		liKeys.push_back( keyCode1 );
		liKeys.push_back( keyCode2 );
	}
}


/**
 * This function returns the keyboard function for the given string.
 *
 * @param inSzParameters the string for which to return the keyboard function
 * @return the keyboard function for the given string, or NON if non could
 * 	be evaluated
 */
eKeybordFunction cCallKeybordFunction::getKeyboardFunction(
		const string & inSzParameters ) {
	
	//create mapping from regular expressions (string) to keyboard function
	map< string, eKeybordFunction > mapKeybordFunctionExpr;
	
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"F1", F1 ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"F2", F2 ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"F3", F3 ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"F4", F4 ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"F5", F5 ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"F6", F6 ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"F7", F7 ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"F8", F8 ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"F9", F9 ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"F10", F10 ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"F11", F11 ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"F12", F12 ) );
	
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"SPACE", SPACE ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"TAB", TAB ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"TABULATOR", TAB ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"BACK_TAB", BACK_TAB ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"BACK_TABULATOR", BACK_TAB ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"ESC", ESC ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"ESCAPE", ESC ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"ENTER", ENTER ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"UPPER_CASE", UPPER_CASE ) );
	
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"UP", UP ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"DOWN", DOWN ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"LEFT", LEFT ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"RIGHT", RIGHT ) );
	
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"PAGE_UP", PAGE_UP ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"PAGE UP", PAGE_UP ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"PAGE_DOWN", PAGE_DOWN ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"PAGE DOWN", PAGE_DOWN ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"POS1", POS1 ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"POSITION1", POS1 ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"POS 1", POS1 ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"POSITION 1", POS1 ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"POS_1", POS1 ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"POSITION_1", POS1 ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"HOME", POS1 ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"END", END ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"ENDE", END ) );
	
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"DELETE", DELETE ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"DEL", DELETE ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"REMOVE", REMOVE ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"ENTF", REMOVE ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"INSERT", INSERT ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"INS", INSERT ) );
	
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"ALT", ALT ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"CTRL", CTRL ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"STRG", CTRL ) );
	
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"PRINT", PRINT ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"MENUE", MENUE ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"SCROLL", SCROLL ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"ROLLEN", SCROLL ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"BREAK", BREAK ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"PAUSE", BREAK ) );
	
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"UNDO", UNDO ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"UNUNDO", UNUNDO ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"REDO", UNUNDO ) );
	
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"END_APPLICATION", END_APPLICATION ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"COPY", COPY ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"PAST", PAST ) );
	
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"NEXT_WORD", NEXT_WORD ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"PREVIOS_WORD", PREVIOS_WORD ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"MARK_NEXT_WORD", MARK_NEXT_WORD ) );
	
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"'\"'", QUOTE ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"'''", SINGLE_QUOTE ) );
	mapKeybordFunctionExpr.insert( pair< string, eKeybordFunction >(
		"';'", SEMICOLON ) );
	
#ifdef FEATURE_READ_DATA_TEXT_WITH_REGEX
	for ( map< string, eKeybordFunction >::const_iterator
			itrMapKeybordFunction = mapKeybordFunctionExpr.begin();
			itrMapKeybordFunction != mapKeybordFunctionExpr.end();
			++itrMapKeybordFunction ) {
		if ( regex_match( inSzParameters, regex( itrMapKeybordFunction->first,
				regex_constants::icase | regex_constants::ECMAScript ) ) ) {
			//matching string found -> return its keyboard function
			return itrMapKeybordFunction->second;
		}
	}
	
#else  //FEATURE_READ_DATA_TEXT_WITH_REGEX
	
	for ( map< string, eKeybordFunction >::const_iterator
			itrMapKeybordFunction = mapKeybordFunctionExpr.begin();
			itrMapKeybordFunction != mapKeybordFunctionExpr.end();
			++itrMapKeybordFunction ) {

		if ( itrMapKeybordFunction->first.compare( inSzParameters ) == 0 ) {
			//matching string found -> return its keyboard function
			return itrMapKeybordFunction->second;
		}
	}
#endif  //FEATURE_READ_DATA_TEXT_WITH_REGEX
	return NON;
}


/**
 * This function returns the key code like defined in <linux/input.h> for
 * the given string.
 *
 * @param inSzKeyCode the string for which to return the key code
 * @return the key code for the given string, or 0 if non could be evaluated
 */
unsigned int cCallKeybordFunction::readKeyCode(
		const std::string & inSzKeyCode ) {
	
	//create mapping from regular expressions (string) to keyboard function
	map< string, unsigned int > mapKeybordFunctionExpr;
	
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_ESC", KEY_ESC ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_1", KEY_1 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_2", KEY_2 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_3", KEY_3 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_4", KEY_4 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_5", KEY_5 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_6", KEY_6 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_7", KEY_7 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_8", KEY_8 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_9", KEY_9 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_0", KEY_0 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_MINUS", KEY_MINUS ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_EQUAL", KEY_EQUAL ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_BACKSPACE", KEY_BACKSPACE ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_TAB", KEY_TAB ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_Q", KEY_Q ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_W", KEY_W ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_E", KEY_E ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_R", KEY_R ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_T", KEY_T ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_Y", KEY_Y ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_U", KEY_U ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_I", KEY_I ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_O", KEY_O ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_P", KEY_P ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_LEFTBRACE", KEY_LEFTBRACE ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_RIGHTBRACE", KEY_RIGHTBRACE ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_ENTER", KEY_ENTER ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_LEFTCTRL", KEY_LEFTCTRL ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_A", KEY_A ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_S", KEY_S ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_D", KEY_D ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_F", KEY_F ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_G", KEY_G ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_H", KEY_H ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_J", KEY_J ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_K", KEY_K ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_L", KEY_L ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_SEMICOLON", KEY_SEMICOLON ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_APOSTROPHE", KEY_APOSTROPHE ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_GRAVE", KEY_GRAVE ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_LEFTSHIFT", KEY_LEFTSHIFT ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_SHIFT", KEY_LEFTSHIFT ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_BACKSLASH", KEY_BACKSLASH ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_Z", KEY_Z ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_X", KEY_X ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_C", KEY_C ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_V", KEY_V ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_B", KEY_B ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_N", KEY_N ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_M", KEY_M ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_COMMA", KEY_COMMA ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_DOT", KEY_DOT ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_SLASH", KEY_SLASH ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_RIGHTSHIFT", KEY_RIGHTSHIFT ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_KPASTERISK", KEY_KPASTERISK ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_LEFTALT", KEY_LEFTALT ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_SPACE", KEY_SPACE ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_CAPSLOCK", KEY_CAPSLOCK ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_F1", KEY_F1 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_F2", KEY_F2 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_F3", KEY_F3 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_F4", KEY_F4 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_F5", KEY_F5 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_F6", KEY_F6 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_F7", KEY_F7 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_F8", KEY_F8 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_F9", KEY_F9 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_F10", KEY_F10 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_NUMLOCK", KEY_NUMLOCK ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_SCROLLLOCK", KEY_SCROLLLOCK ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_KP7", KEY_KP7 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_KP8", KEY_KP8 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_KP9", KEY_KP9 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_KPMINUS", KEY_KPMINUS ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_KP4", KEY_KP4 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_KP5", KEY_KP5 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_KP6", KEY_KP6 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_KPPLUS", KEY_KPPLUS ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_KP1", KEY_KP1 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_KP2", KEY_KP2 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_KP3", KEY_KP3 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_KP0", KEY_KP0 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_KPDOT", KEY_KPDOT ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_ZENKAKUHANKAKU", KEY_ZENKAKUHANKAKU ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_102ND", KEY_102ND ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_F11", KEY_F11 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_F12", KEY_F12 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_RO", KEY_RO ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_KATAKANA", KEY_KATAKANA ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_HIRAGANA", KEY_HIRAGANA ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_HENKAN", KEY_HENKAN ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_KATAKANAHIRAGANA", KEY_KATAKANAHIRAGANA ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_MUHENKAN", KEY_MUHENKAN ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_KPJPCOMMA", KEY_KPJPCOMMA ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_KPENTER", KEY_KPENTER ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_RIGHTCTRL", KEY_RIGHTCTRL ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_KPSLASH", KEY_KPSLASH ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_SYSRQ", KEY_SYSRQ ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_RIGHTALT", KEY_RIGHTALT ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_LINEFEED", KEY_LINEFEED ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_HOME", KEY_HOME ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_UP", KEY_UP ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_PAGEUP", KEY_PAGEUP ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_LEFT", KEY_LEFT ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_RIGHT", KEY_RIGHT ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_END", KEY_END ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_DOWN", KEY_DOWN ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_PAGEDOWN", KEY_PAGEDOWN ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_INSERT", KEY_INSERT ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_MACRO", KEY_MACRO ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_MUTE", KEY_MUTE ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_VOLUMEDOWN", KEY_VOLUMEDOWN ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_VOLUMEUP", KEY_VOLUMEUP ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_POWER", KEY_POWER ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_KPEQUAL", KEY_KPEQUAL ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_KPPLUSMINUS", KEY_KPPLUSMINUS ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_PAUSE", KEY_PAUSE ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_SCALE", KEY_SCALE ) );
	
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_KPCOMMA", KEY_KPCOMMA ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_HANGEUL", KEY_HANGEUL ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_HANGUEL", KEY_HANGUEL ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_HANJA", KEY_HANJA ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_YEN", KEY_YEN ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_LEFTMETA", KEY_LEFTMETA ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_RIGHTMETA", KEY_RIGHTMETA ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_COMPOSE", KEY_COMPOSE ) );
	
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_STOP", KEY_STOP ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_AGAIN", KEY_AGAIN ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_PROPS", KEY_PROPS ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_UNDO", KEY_UNDO ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_FRONT", KEY_FRONT ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_COPY", KEY_COPY ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_OPEN", KEY_OPEN ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_PASTE", KEY_PASTE ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_FIND", KEY_FIND ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_CUT", KEY_CUT ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_HELP", KEY_HELP ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_MENU", KEY_MENU ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_CALC", KEY_CALC ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_SETUP", KEY_SETUP ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_SLEEP", KEY_SLEEP ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_WAKEUP", KEY_WAKEUP ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_FILE", KEY_FILE ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_SENDFILE", KEY_SENDFILE ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_DELETEFILE", KEY_DELETEFILE ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_XFER", KEY_XFER ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_PROG1", KEY_PROG1 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_PROG2", KEY_PROG2 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_WWW", KEY_WWW ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_MSDOS", KEY_MSDOS ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_COFFEE", KEY_COFFEE ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_SCREENLOCK", KEY_SCREENLOCK ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_DIRECTION", KEY_DIRECTION ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_CYCLEWINDOWS", KEY_CYCLEWINDOWS ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_MAIL", KEY_MAIL ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_BOOKMARKS", KEY_BOOKMARKS ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_COMPUTER", KEY_COMPUTER ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_BACK", KEY_BACK ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_FORWARD", KEY_FORWARD ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_CLOSECD", KEY_CLOSECD ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_EJECTCD", KEY_EJECTCD ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_EJECTCLOSECD", KEY_EJECTCLOSECD ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_NEXTSONG", KEY_NEXTSONG ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_PLAYPAUSE", KEY_PLAYPAUSE ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_PREVIOUSSONG", KEY_PREVIOUSSONG ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_STOPCD", KEY_STOPCD ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_RECORD", KEY_RECORD ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_REWIND", KEY_REWIND ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_PHONE", KEY_PHONE ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_ISO", KEY_ISO ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_CONFIG", KEY_CONFIG ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_HOMEPAGE", KEY_HOMEPAGE ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_REFRESH", KEY_REFRESH ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_EXIT", KEY_EXIT ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_MOVE", KEY_MOVE ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_EDIT", KEY_EDIT ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_SCROLLUP", KEY_SCROLLUP ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_SCROLLDOWN", KEY_SCROLLDOWN ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_KPLEFTPAREN", KEY_KPLEFTPAREN ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_KPRIGHTPAREN", KEY_KPRIGHTPAREN ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_NEW", KEY_NEW ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_REDO", KEY_REDO ) );
	
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_F13", KEY_F13 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_F14", KEY_F14 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_F15", KEY_F15 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_F16", KEY_F16 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_F17", KEY_F17 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_F18", KEY_F18 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_F19", KEY_F19 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_F20", KEY_F20 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_F21", KEY_F21 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_F22", KEY_F22 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_F23", KEY_F23 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_F24", KEY_F24 ) );
	
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_PLAYCD", KEY_PLAYCD ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_PAUSECD", KEY_PAUSECD ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_PROG3", KEY_PROG3 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_PROG4", KEY_PROG4 ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_DASHBOARD", KEY_DASHBOARD ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_SUSPEND", KEY_SUSPEND ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_CLOSE", KEY_CLOSE ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_PLAY", KEY_PLAY ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_FASTFORWARD", KEY_FASTFORWARD ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_BASSBOOST", KEY_BASSBOOST ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_PRINT", KEY_PRINT ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_HP", KEY_HP ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_CAMERA", KEY_CAMERA ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_SOUND", KEY_SOUND ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_QUESTION", KEY_QUESTION ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_EMAIL", KEY_EMAIL ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_CHAT", KEY_CHAT ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_SEARCH", KEY_SEARCH ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_CONNECT", KEY_CONNECT ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_FINANCE", KEY_FINANCE ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_SPORT", KEY_SPORT ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_SHOP", KEY_SHOP ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_ALTERASE", KEY_ALTERASE ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_CANCEL", KEY_CANCEL ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_BRIGHTNESSDOWN", KEY_BRIGHTNESSDOWN ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_BRIGHTNESSUP", KEY_BRIGHTNESSUP ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_MEDIA", KEY_MEDIA ) );
	
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_SWITCHVIDEOMODE", KEY_SWITCHVIDEOMODE ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_KBDILLUMTOGGLE", KEY_KBDILLUMTOGGLE ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_KBDILLUMDOWN", KEY_KBDILLUMDOWN ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_KBDILLUMUP", KEY_KBDILLUMUP ) );

	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_SEND", KEY_SEND ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_REPLY", KEY_REPLY ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_FORWARDMAIL", KEY_FORWARDMAIL ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_SAVE", KEY_SAVE ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_DOCUMENTS", KEY_DOCUMENTS ) );
	
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_BATTERY", KEY_BATTERY ) );
	
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_BLUETOOTH", KEY_BLUETOOTH ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_WLAN", KEY_WLAN ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_UWB", KEY_UWB ) );
	
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_UNKNOWN", KEY_UNKNOWN ) );
	
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_VIDEO_NEXT", KEY_VIDEO_NEXT ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_VIDEO_PREV", KEY_VIDEO_PREV ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_BRIGHTNESS_CYCLE", KEY_BRIGHTNESS_CYCLE ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_BRIGHTNESS_ZERO", KEY_BRIGHTNESS_ZERO ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_DISPLAY_OFF", KEY_DISPLAY_OFF ) );

	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_WIMAX", KEY_WIMAX ) );
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_RFKILL", KEY_RFKILL ) );
	
	mapKeybordFunctionExpr.insert( pair< string, unsigned int >(
		"KEY_MICMUTE", KEY_MICMUTE ) );
	
#ifdef FEATURE_READ_DATA_TEXT_WITH_REGEX
	for ( map< string, unsigned int >::const_iterator
			itrMapKeybordFunction = mapKeybordFunctionExpr.begin();
			itrMapKeybordFunction != mapKeybordFunctionExpr.end();
			++itrMapKeybordFunction ) {
		if ( regex_match( inSzKeyCode, regex( itrMapKeybordFunction->first,
				regex_constants::icase | regex_constants::ECMAScript ) ) ) {
			//matching string found -> return its key code
			return itrMapKeybordFunction->second;
		}
	}
	
#else  //FEATURE_READ_DATA_TEXT_WITH_REGEX
	
	for ( map< string, unsigned int >::const_iterator
			itrMapKeybordFunction = mapKeybordFunctionExpr.begin();
			itrMapKeybordFunction != mapKeybordFunctionExpr.end();
			++itrMapKeybordFunction ) {

		if ( itrMapKeybordFunction->first.compare( inSzKeyCode ) == 0 ) {
			//matching string found -> return its key code
			break itrMapKeybordFunction->second;
		}
	}
#endif  //FEATURE_READ_DATA_TEXT_WITH_REGEX
	
	return atoi( inSzKeyCode.c_str() );
}



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
void cCallKeybordFunction::analyseAndSetParameters( const string & inSzParameters ) {
	
	if ( inSzParameters == "" ) {
		//no parameters -> no keyboard function
		return;
	}
	if ( inSzParameters.size() == 1 ) {
		//char given
		cChar = inSzParameters[ 0 ];
		keybordFunction = CHAR;
		
		charToOperatorFunction( cChar );
		return;
	}  //else more than one char in string
	
	//try to get a keyboard function
	const eKeybordFunction keybordFunctionTmp =
		getKeyboardFunction( inSzParameters );
	if ( keybordFunctionTmp != NON ) {
		//set read keyboard function
		keyboardFunctionToOperatorFunction( keybordFunctionTmp );
	} else {  //read the keys into liKeys
		string szKeyCode;
		unsigned int iReadedKey = 0;
		unsigned int iIndexNextParameter = 0;
		unsigned int iIndexActualParameter = 0;
		while ( iIndexNextParameter < inSzParameters.size() ) {
			//while more parameter to read
			
//TODO weg
if ( inSzParameters == "YEN" ) {
	
	iIndexNextParameter = iIndexNextParameter;
}

			do {
				iIndexNextParameter = inSzParameters.find ( ',', iIndexNextParameter );
				
				if ( ( iIndexNextParameter == string::npos ) ||
						( inSzParameters.size() < iIndexNextParameter ) ) {
					//next comma ',' not found -> read till end
					iIndexNextParameter = inSzParameters.size();
					break;
				}
			} while ( ( iIndexNextParameter == 0 ) ||
					//or ',' is masked
					( inSzParameters.at( iIndexNextParameter - 1 ) == '\\' ) );
			//szKeyCode contains the part betwean two commas ','
			szKeyCode = inSzParameters.substr( iIndexActualParameter,
				iIndexNextParameter - iIndexActualParameter );
			
			iIndexActualParameter = iIndexNextParameter;
			//discard comma ','
			iIndexNextParameter++;
			//read the next key code
			iReadedKey = readKeyCode( szKeyCode );
			if ( iReadedKey != 0 ) {
				liKeys.push_back( iReadedKey );
			}  //else unknown key
		}  //end while more parameter to read
		
		//fill keyCode1 and keyCode2
		std::list< unsigned int >::const_iterator itrKey = liKeys.begin();
		if ( itrKey != liKeys.end() ) {
			//set first key code
			keyCode1 = (*itrKey);
			itrKey++;
			if ( itrKey != liKeys.end() ) {
				//set second key code
				keyCode2 = (*itrKey);
			}
		}
		
		if ( liKeys.size() == 1 ) {
			//exactly one key code
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
		} else if ( liKeys.size() == 2 ) {
			//exactly two key codes
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
		}
	}
}






