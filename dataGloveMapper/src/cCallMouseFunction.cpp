
//TODO check

/**
 * @file cCallMouseFunction
 * file name: cCallMouseFunction.cpp
 * @author Betti Oesterholz
 * @date 04.01.2015
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file implements the class for calling a mouse function.
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
 * This file implements the class for calling a mouse function.
 * Mouse function can be klicks (e.g. LEFT_KLICK) or directions (e.g. MOUSE_HORIZONTAL).
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

#include "cCallMouseFunction.h"



#ifdef DEBUG_CALL_MOUSE_FUNCTION
	#include <iostream>
#endif  //DEBUG_CALL_MOUSE_FUNCTION

#include "cCallPrepareMouseFunction.h"
#include "cEvaluateDataGloveState.h"
#include "cUInputDevice.h"


#ifdef FEATURE_READ_DATA_TEXT_WITH_REGEX
	#include <regex>
#endif //FEATURE_READ_DATA_TEXT_WITH_REGEX

#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	#include "cMapWString.h"
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	#include "cMapString.h"
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR


using namespace nDataGlove::nMapper;
using namespace std;



namespace nDataGlove{
namespace nMapper{
namespace nCallMouseFunction{

	class cMapperMouseFunction : public
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
		cMapWString<eMouseFunction>
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
		cMapString<eMouseFunction>
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	{
	public:
		
		cMapperMouseFunction() :
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
#ifdef FEATURE_READ_DATA_TEXT_WITH_REGEX
			cMapWString< eMouseFunction >( MOUSE_NON,
				regex_constants::icase | regex_constants::ECMAScript )
#else  //FEATURE_READ_DATA_TEXT_WITH_REGEX
			cMapWString< eMouseFunction >( MOUSE_NON )
#endif  //FEATURE_READ_DATA_TEXT_WITH_REGEX
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
#ifdef FEATURE_READ_DATA_TEXT_WITH_REGEX
			cMapString< eMouseFunction >( MOUSE_NON,
				regex_constants::icase | regex_constants::ECMAScript )
#else  //FEATURE_READ_DATA_TEXT_WITH_REGEX
			cMapString< eMouseFunction >( MOUSE_NON )
#endif  //FEATURE_READ_DATA_TEXT_WITH_REGEX
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
		{
			setMapPair( "MOUSE_MOVE.*", MOUSE_MOVE_HORIZONTAL_VERTICAL );
			setMapPair( "MOVE.*", MOUSE_MOVE_HORIZONTAL_VERTICAL );
			setMapPair( "MOUSE_HORIZONTAL", MOUSE_HORIZONTAL );
			setMapPair( "HOR.*", MOUSE_HORIZONTAL );
			setMapPair( "MOUSE_VERTICAL", MOUSE_VERTICAL );
			setMapPair( "VERT.*", MOUSE_VERTICAL );
			
			setMapPair( "MOUSE_WHEEL", MOUSE_WHEEL );
			
			setMapPair( "MOUSE_BTN_LEFT_KLICK", MOUSE_BTN_LEFT_KLICK );
			setMapPair( "MOUSE_BTN_LEFT_DOUBLE_KLICK", MOUSE_BTN_LEFT_DOUBLE_KLICK );
			setMapPair( "MOUSE_BTN_RIGHT_KLICK", MOUSE_BTN_RIGHT_KLICK );
			setMapPair( "MOUSE_BTN_RIGHT_DOUBLE_KLICK", MOUSE_BTN_RIGHT_DOUBLE_KLICK );
			setMapPair( "MOUSE_BTN_MIDDLE_KLICK", MOUSE_BTN_MIDDLE_KLICK );
			setMapPair( "MOUSE_BTN_MIDDLE_DOUBLE_KLICK", MOUSE_BTN_MIDDLE_DOUBLE_KLICK );
			
			setMapPair( "MOUSE_BTN_SIDE_KLICK", MOUSE_BTN_SIDE_KLICK );
			setMapPair( "MOUSE_BTN_SIDE_DOUBLE_KLICK", MOUSE_BTN_SIDE_DOUBLE_KLICK );
			setMapPair( "MOUSE_BTN_EXTRA_KLICK", MOUSE_BTN_EXTRA_KLICK );
			setMapPair( "MOUSE_BTN_EXTRA_DOUBLE_KLICK", MOUSE_BTN_EXTRA_DOUBLE_KLICK );
			setMapPair( "MOUSE_BTN_FORWARD_KLICK", MOUSE_BTN_FORWARD_KLICK );
			setMapPair( "MOUSE_BTN_FORWARD_DOUBLE_KLICK", MOUSE_BTN_FORWARD_DOUBLE_KLICK );
			setMapPair( "MOUSE_BTN_BACK_KLICK", MOUSE_BTN_BACK_KLICK );
			setMapPair( "MOUSE_BTN_BACK_DOUBLE_KLICK", MOUSE_BTN_BACK_DOUBLE_KLICK );
			setMapPair( "MOUSE_BTN_TASK_KLICK", MOUSE_BTN_TASK_KLICK );
			setMapPair( "MOUSE_BTN_TASK_DOUBLE_KLICK", MOUSE_BTN_TASK_DOUBLE_KLICK );
			
			setMapPair( "MOUSE_BTN_LEFT_DOWN", MOUSE_BTN_LEFT_DOWN );
			setMapPair( "MOUSE_BTN_LEFT_UP", MOUSE_BTN_LEFT_UP );
			setMapPair( "MOUSE_BTN_RIGHT_DOWN", MOUSE_BTN_RIGHT_DOWN );
			setMapPair( "MOUSE_BTN_RIGHT_UP", MOUSE_BTN_RIGHT_UP );
			setMapPair( "MOUSE_BTN_MIDDLE_DOWN", MOUSE_BTN_MIDDLE_DOWN );
			setMapPair( "MOUSE_BTN_MIDDLE_UP", MOUSE_BTN_MIDDLE_UP );
			
			setMapPair( "MOUSE_BTN_SIDE_DOWN", MOUSE_BTN_SIDE_DOWN );
			setMapPair( "MOUSE_BTN_SIDE_UP", MOUSE_BTN_SIDE_UP );
			setMapPair( "MOUSE_BTN_EXTRA_DOWN", MOUSE_BTN_EXTRA_DOWN );
			setMapPair( "MOUSE_BTN_EXTRA_UP", MOUSE_BTN_EXTRA_UP );
			setMapPair( "MOUSE_BTN_FORWARD_DOWN", MOUSE_BTN_FORWARD_DOWN );
			setMapPair( "MOUSE_BTN_FORWARD_UP", MOUSE_BTN_FORWARD_UP );
			setMapPair( "MOUSE_BTN_BACK_DOWN", MOUSE_BTN_BACK_DOWN );
			setMapPair( "MOUSE_BTN_BACK_UP", MOUSE_BTN_BACK_UP );
			setMapPair( "MOUSE_BTN_TASK_DOWN", MOUSE_BTN_TASK_DOWN );
			setMapPair( "MOUSE_BTN_TASK_UP", MOUSE_BTN_TASK_UP );
			
			
//TODO create const object for it ( template<return_type>FindType ? )
			//search with tree (split search prases iterativ into groups)
			optimize();
		}
		
		
	};  //class cMapperMouseFunction
	
	
	/**
	 * Converts the given string to a long number.
	 *
	 * @param inString the string from which to read the number
	 * @return the readed number
	 */
	long stringToLong( const wstring & inString ) {
		
		return wcstol( inString.c_str(), NULL, 10 );
	}


	/**
	 * Converts the given string to a long number.
	 *
	 * @param inString the string from which to read the number
	 * @return the readed number
	 */
	long stringToLong( const string & inString ) {
		
		return strtol( inString.c_str(), NULL, 10 );
	}

	
};//end namespace nCallMouseFunction


using namespace nDataGlove::nMapper::nCallMouseFunction;

	cMapperMouseFunction cCallMouseFunction::mapperMouseFunction;

};//end namespace nMapper
};//end namespace nDataGlove



/**
 * constructor
 * (Also evaluates if if true preparing the mouse function is needed.)
 *
 * @see bPrepareNeeded
 * @param inSzParameters A string with parameters for the call to the mouse
 * 	@see analyseAndSetParameters()
 */
cCallMouseFunction::cCallMouseFunction( const std::string inSzParameters ) :
		mouseFunction( MOUSE_NON ),
		iAmount( 1 ), iAmount2( 0 ),
		bPrepareNeeded( false ) {
	
	analyseAndSetParameters( inSzParameters, true );
}


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
cCallMouseFunction::cCallMouseFunction( const std::string inSzParameters,
	const bool bInPrepareNeeded ) :
		mouseFunction( MOUSE_NON ),
		iAmount( 1 ), iAmount2( 0 ),
		bPrepareNeeded( bInPrepareNeeded ) {
	
	analyseAndSetParameters( inSzParameters );
}


/**
 * constructor
 *
 * @param MouseFunction The mouse function for this this operator.
 * 	@see mouseFunction
 * 	@see mouseFunction
 * @param bInPrepareNeeded if true preparing the mouse function is
 * 	needed, else not
 * 	@see bPrepareNeeded
 * 	@see cCallPrepareMouseFunction
 */
cCallMouseFunction::cCallMouseFunction( const eMouseFunction inMouseFunction,
	const bool bInPrepareNeeded ) :
		mouseFunction( inMouseFunction ),
		iAmount( 1 ), iAmount2( 0 ),
		bPrepareNeeded( bInPrepareNeeded ) {
	
	//nothing to do
}


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
cCallMouseFunction::cCallMouseFunction( const eMouseFunction inMouseFunction,
	const int inAmount,
	const bool bInPrepareNeeded ) :
		mouseFunction( inMouseFunction ),
		iAmount( inAmount ), iAmount2( 0 ),
		bPrepareNeeded( bInPrepareNeeded ) {
	
	//nothing to do
}


/**
 * destructor
 */
cCallMouseFunction::~cCallMouseFunction() {
	//nothing to do
}


/**
 * @return the name of this class "cCallMouseFunction"
 */
std::string cCallMouseFunction::getName() const {
	
	return "cCallMouseFunction";
}


/**
 * This functor operator is called the execute the function implemented
 * by this object.
 *
 * @return true if the call to the function was sucessfull, else false
 */
bool cCallMouseFunction::operator()() {
	
	if ( bPrepareNeeded &&
			( ( cCallPrepareMouseFunction::getPreparedFunction() == NULL ) ||
			( ! cCallPrepareMouseFunction::getPreparedFunction()->
				IsPrepared( this ) ) ) ) {
		//mouse function was not prepared, but should be
		//call parent operator (unprepare)
#ifdef DEBUG_CALL_MOUSE_FUNCTION
		cout<<"mouse function was not prepared: "<<mouseFunction<<
			" ("<<mouseFunction<<"'; "<<iAmount<<")"<<endl;
#endif  //DEBUG_CALL_MOUSE_FUNCTION
		iCallFunction::operator()();
		return false;
	}
	initRepeatDelay();
	//call mouse function
	
	//open / create uinput device
	cUInputDevice * pUInputDevice = cUInputDevice::getInstance();
	
	if ( ! pUInputDevice->isDeviceOpen() ) {
		return false;  //Error: Device could not be opened
	}
	
	//call mouse function
	switch ( mouseFunction ) {
		case MOUSE_HORIZONTAL : {
			pUInputDevice->mouseMoveHorizontal( iAmount );
		}; break;
		case MOUSE_VERTICAL : {
			pUInputDevice->mouseMoveVertical( iAmount );
		}; break;
		
		case MOUSE_MOVE_HORIZONTAL_VERTICAL : {
			pUInputDevice->mouseMove( iAmount, iAmount2 );
		}; break;
		
		case MOUSE_WHEEL : {
			pUInputDevice->mouseScroll( iAmount );
		}; break;
		
		case MOUSE_BTN_LEFT_KLICK : {
			pUInputDevice->keyDown( BTN_LEFT );
		}; break;
		case MOUSE_BTN_LEFT_DOUBLE_KLICK : {
			pUInputDevice->mouseLeftClick();
			pUInputDevice->mouseLeftClick();
		}; break;
		
		case MOUSE_BTN_RIGHT_KLICK : {
			pUInputDevice->keyDown( BTN_RIGHT );
		}; break;
		case MOUSE_BTN_RIGHT_DOUBLE_KLICK : {
			pUInputDevice->mouseRightClick();
			pUInputDevice->mouseRightClick();
		}; break;
		
		case MOUSE_BTN_MIDDLE_KLICK : {
			pUInputDevice->keyDown( BTN_MIDDLE );
		}; break;
		case MOUSE_BTN_MIDDLE_DOUBLE_KLICK : {
			pUInputDevice->mouseMiddleClick();
			pUInputDevice->mouseMiddleClick();
		}; break;
		
		case MOUSE_BTN_SIDE_KLICK : {
			pUInputDevice->keyDown( BTN_SIDE );
		}; break;
		case MOUSE_BTN_SIDE_DOUBLE_KLICK : {
			pUInputDevice->mouseSideClick();
			pUInputDevice->mouseSideClick();
		}; break;
		
		case MOUSE_BTN_EXTRA_KLICK : {
			pUInputDevice->keyDown( BTN_EXTRA );
		}; break;
		case MOUSE_BTN_EXTRA_DOUBLE_KLICK : {
			pUInputDevice->mouseExtraClick();
			pUInputDevice->mouseExtraClick();
		}; break;
		
		case MOUSE_BTN_FORWARD_KLICK : {
			pUInputDevice->keyDown( BTN_FORWARD );
		}; break;
		case MOUSE_BTN_FORWARD_DOUBLE_KLICK : {
			pUInputDevice->mouseForwardClick();
			pUInputDevice->mouseForwardClick();
		}; break;
		
		case MOUSE_BTN_BACK_KLICK : {
			pUInputDevice->keyDown( BTN_BACK );
			pUInputDevice->mouseBackwardClick();
		}; break;
		case MOUSE_BTN_BACK_DOUBLE_KLICK : {
			pUInputDevice->mouseBackwardClick();
			pUInputDevice->mouseBackwardClick();
		}; break;
		
		case MOUSE_BTN_TASK_KLICK : {
			pUInputDevice->keyDown( BTN_TASK );
		}; break;
		case MOUSE_BTN_TASK_DOUBLE_KLICK : {
			pUInputDevice->mouseTaskClick();
			pUInputDevice->mouseTaskClick();
		}; break;
		
		
		case MOUSE_BTN_LEFT_DOWN : {
			pUInputDevice->keyDown( BTN_LEFT );
		}; break;
		case MOUSE_BTN_RIGHT_DOWN : {
			pUInputDevice->keyDown( BTN_RIGHT );
		}; break;
		case MOUSE_BTN_MIDDLE_DOWN : {
			pUInputDevice->keyDown( BTN_MIDDLE );
		}; break;
		case MOUSE_BTN_SIDE_DOWN : {
			pUInputDevice->keyDown( BTN_SIDE );
		}; break;
		case MOUSE_BTN_EXTRA_DOWN : {
			pUInputDevice->keyDown( BTN_EXTRA );
		}; break;
		case MOUSE_BTN_FORWARD_DOWN : {
			pUInputDevice->keyDown( BTN_FORWARD );
		}; break;
		case MOUSE_BTN_BACK_DOWN : {
			pUInputDevice->keyDown( BTN_BACK );
		}; break;
		case MOUSE_BTN_TASK_DOWN : {
			pUInputDevice->keyDown( BTN_TASK );
		}; break;
		
		case MOUSE_BTN_LEFT_UP : {
			pUInputDevice->keyUp( BTN_LEFT );
		}; break;
		case MOUSE_BTN_RIGHT_UP : {
			pUInputDevice->keyUp( BTN_RIGHT );
		}; break;
		case MOUSE_BTN_MIDDLE_UP : {
			pUInputDevice->keyUp( BTN_MIDDLE );
		}; break;
		case MOUSE_BTN_SIDE_UP : {
			pUInputDevice->keyUp( BTN_SIDE );
		}; break;
		case MOUSE_BTN_EXTRA_UP : {
			pUInputDevice->keyUp( BTN_EXTRA );
		}; break;
		case MOUSE_BTN_FORWARD_UP : {
			pUInputDevice->keyUp( BTN_FORWARD );
		}; break;
		case MOUSE_BTN_BACK_UP : {
			pUInputDevice->keyUp( BTN_BACK );
		}; break;
		case MOUSE_BTN_TASK_UP : {
			pUInputDevice->keyUp( BTN_TASK );
		}; break;
		
		default: break;  //do nothing
	};  //end switch mouseFunction
	
#ifdef DEBUG_CALL_MOUSE_FUNCTION
	cout<<"calling mouse function: "<<mouseFunction<<
		" ("<<mouseFunction<<"'; "<<iAmount<<"; "<<iAmount2<<")"<<endl;
#endif  //DEBUG_CALL_MOUSE_FUNCTION
	return true;
}


/**
 * This method ends the operation.
 * (Call if the state changes.)
 */
void cCallMouseFunction::end() {
	
	//open / create uinput device
	cUInputDevice * pUInputDevice = cUInputDevice::getInstance();
	
	if ( ! pUInputDevice->isDeviceOpen() ) {
		return;  //Error: Device could not be opened
	}
	
	//call mouse function
	switch ( mouseFunction ) {
		
		case MOUSE_BTN_LEFT_KLICK : {
			pUInputDevice->keyUp( BTN_LEFT );
		}; break;
		
		case MOUSE_BTN_RIGHT_KLICK : {
			pUInputDevice->keyUp( BTN_RIGHT );
		}; break;
		
		case MOUSE_BTN_MIDDLE_KLICK : {
			pUInputDevice->keyUp( BTN_MIDDLE );
		}; break;
		
		case MOUSE_BTN_SIDE_KLICK : {
			pUInputDevice->keyUp( BTN_SIDE );
		}; break;
		
		case MOUSE_BTN_EXTRA_KLICK : {
			pUInputDevice->keyUp( BTN_EXTRA );
		}; break;
		
		case MOUSE_BTN_FORWARD_KLICK : {
			pUInputDevice->keyUp( BTN_FORWARD );
		}; break;
		
		case MOUSE_BTN_BACK_KLICK : {
			pUInputDevice->keyUp( BTN_BACK );
			pUInputDevice->mouseBackwardClick();
		}; break;
		
		case MOUSE_BTN_TASK_KLICK : {
			pUInputDevice->keyUp( BTN_TASK );
		}; break;
		
		default: break;  //do nothing
	};  //end switch mouseFunction
	
#ifdef DEBUG_CALL_MOUSE_FUNCTION
	cout<<"end calling mouse function: "<<mouseFunction<<
		" ("<<mouseFunction<<"'; "<<iAmount<<"; "<<iAmount2<<")"<<endl;
#endif  //DEBUG_CALL_MOUSE_FUNCTION
}


/**
 * This method sets the parameter for the operation.
 * The parameter are readed from the given string.
 *
 * @param szFunctionParameter all parameters for this functor operation
 * 	as a string
 * @return true if the parameter could be set, else false
 */
bool cCallMouseFunction::setParameter( const std::string & szFunctionParameter ) {
	
	Parameter = szFunctionParameter;
	return analyseAndSetParameters( wstring(
		szFunctionParameter.begin(), szFunctionParameter.end() ) );
}


/**
 * @return If true preparing the modus change is needed, else not.
 * 	@see bPrepareNeeded
 */
bool cCallMouseFunction::getPrepareNeeded() const {
	
	return bPrepareNeeded;
}


/**
 * @return The mouse function, which will be prepared
 * 	with this operator.
 * 	If it is a directions (e.g. "MOUSE_HORIZONTAL") iAmount contains the
 * 	amount of the move in the direction.
 * 	@see mouseFunction
 */
eMouseFunction cCallMouseFunction::getMouseFunction() const {
	
	return mouseFunction;
}


/**
 * @return The amount for the mouse movement, if this object is for a
 * 	mouse movement, else 0.
 * 	@see iAmount
 * @see mouseFunction
 */
int cCallMouseFunction::getAmount() const {
	
	return iAmount;
}


/**
 * @return The second amount for the mouse movement, if this object is for a
 * 	mouse movement in two directions / dimensions, else 0.
 * 	(E.g. MOUSE_MOVE_HORIZONTAL_VERTICAL for move iAmount horizontal and
 * 	iAmount2 vertical.)
 * 	@see iAmount2
 * @see mouseFunction
 */
int cCallMouseFunction::getAmount2() const {
	
	return iAmount2;
}


/**
 * This function returns the mouse function for the given string.
 *
 * @param inSzParameters the string for which to return the mouse function
 * @return the mouse function for the given string, or MOUSE_NON if non could
 * 	be evaluated
 */
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
eMouseFunction cCallMouseFunction::getMouseFunction(
	const std::wstring & inSzParameters ) {
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
eMouseFunction cCallMouseFunction::getMouseFunction(
	const std::string & inSzParameters ) {
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	
	if ( inSzParameters.empty() ) {
		//no mouse function to evaluate
		return MOUSE_NON;
	}

	//map from regular expressions (string) to mouse function
	return mapperMouseFunction( inSzParameters );
}





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
bool cCallMouseFunction::analyseAndSetParameters( const std::string & inSzParameters,
		const bool inEvaluateIfPrepareNeeded ) {
	
	return analyseAndSetParameters(
		wstring( inSzParameters.begin(), inSzParameters.end() ) );
}


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
bool cCallMouseFunction::analyseAndSetParameters( const std::wstring & inSzParameters,
		const bool inEvaluateIfPrepareNeeded ) {
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
bool cCallMouseFunction::analyseAndSetParameters( const std::string & inSzParameters,
		const bool inEvaluateIfPrepareNeeded ) {
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	
	if ( inSzParameters.empty() ) {
		//no parameters -> no mouse function
		return false;
	}

	//try to get a mouse function
	const eMouseFunction mouseFunctionTmp =
		getMouseFunction( inSzParameters );
	if ( mouseFunctionTmp != MOUSE_NON ) {
		//set read mouse function
		mouseFunction = mouseFunctionTmp;
		
		//if MOUSE_HORIZONTAL, MOUSE_VERTICAL or MOUSE_WHEEL read iAmount
		//(e.g "MOUSE_HORIZONTAL 3", "VERT_-24", "MOUSE_WHEEL:4"
		if (  ( mouseFunction == MOUSE_HORIZONTAL ) or
				( mouseFunction == MOUSE_VERTICAL ) or
				( mouseFunction == MOUSE_WHEEL ) or
				( mouseFunction == MOUSE_MOVE_HORIZONTAL_VERTICAL ) ) {
			//search for first ' ', ',', '_' or ':' and read number after it
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
			unsigned int iIndexForNumber =
				inSzParameters.find_first_of( L" ,_:" );
			if ( ( iIndexForNumber != string::npos ) &&
					( iIndexForNumber < inSzParameters.size() ) ) {
				
				iIndexForNumber = inSzParameters.find_first_of(
					L"-+0123456789.", iIndexForNumber );
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
			iIndexForNumber =
				inSzParameters.find_first_of( " ,_:", iIndexForNumber );
			if ( ( iIndexForNumber != string::npos ) &&
					( iIndexForNumber < inSzParameters.size() ) ) {
				
				iIndexForNumber = inSzParameters.find_first_of(
					"-+0123456789.", iIndexForNumber );
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
				
				if ( ( iIndexForNumber != string::npos ) &&
						( iIndexForNumber < inSzParameters.size() ) ) {
					
					iAmount = stringToLong( inSzParameters.substr( iIndexForNumber ) );
					
					if ( mouseFunction == MOUSE_MOVE_HORIZONTAL_VERTICAL ) {
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
						iIndexForNumber =
							inSzParameters.find_first_of( L" ,_:", iIndexForNumber );
						if ( ( iIndexForNumber != string::npos ) &&
								( iIndexForNumber < inSzParameters.size() ) ) {
							
							iIndexForNumber = inSzParameters.find_first_of(
								L"-+0123456789.", iIndexForNumber );
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
						iIndexForNumber =
							inSzParameters.find_first_of( " ,_:", iIndexForNumber );
						if ( ( iIndexForNumber != string::npos ) &&
								( iIndexForNumber < inSzParameters.size() ) ) {
							
							iIndexForNumber = inSzParameters.find_first_of(
								"-+0123456789.", iIndexForNumber );
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
							
							if ( ( iIndexForNumber != string::npos ) &&
									( iIndexForNumber < inSzParameters.size() ) ) {
								
								iAmount2 = stringToLong( inSzParameters.substr( iIndexForNumber ) );
							}
						}
					}  //else mouseFunction != MOUSE_MOVE_HORIZONTAL_VERTICAL
				}
			}
		}
		
		if ( inEvaluateIfPrepareNeeded ) {
			//evaluate if preparing the mouse function is needed or not
			//preparing need if not a mouse movement
			bPrepareNeeded = ( mouseFunction != MOUSE_HORIZONTAL ) &&
				( mouseFunction != MOUSE_VERTICAL ) &&
				( mouseFunction != MOUSE_MOVE_HORIZONTAL_VERTICAL ) &&
				( mouseFunction != MOUSE_WHEEL );
		}
		
		return true;
	}// else read the keys into liKeys
	
	return false;
}






