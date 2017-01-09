
//TODO check

/**
 * @file iCallFunction
 * file name: iCallFunction.h
 * @author Betti Oesterholz
 * @date 18.10.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file implements the interface for a function, which should be called.
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
 * This file implements the interface for a function, which should be called.
 * It is a functor.
 *
 * @pattern functor
 * @pattern interface
 * @see cDataGloveState
 * @see cEvalueDataGloveState
 */
/*
History:
18.10.2014  Oesterholz  created
*/

//#define DEBUG


#include "iCallFunction.h"

#include "cCallPrepareKeybordFunction.h"
#include "cCallPrepareChangeModus.h"

#include <list>
#include <string>


using namespace nDataGlove::nMapper;

iCallFunction::iCallFunction() : arrayRepeatDelay( NULL ), countRepeatDelays( 0 ),
		indexNextDelay( -1 ), bRepeatLastDelay( false ), Parameter("") {
	
}


iCallFunction::~iCallFunction() {
	//nothing to do
}


/**
 * @return the name of this class "iCallFunction"
 */
std::string iCallFunction::getName() const {
	
	return std::string("iCallFunction");
}


/**
 * This functor operator is called the execute the function implemented
 * by this object.
 *
 * @return true if the call to the function was sucessfull, else false
 */
bool iCallFunction::operator()() {
	
	//call unprepare
	cCallPrepareKeybordFunction::unprepare();
	cCallPrepareChangeModus::unprepare();
	
	initRepeatDelay();
	
	return true;
}

/**
 * This method returns the parameter for the operation.
 *
 * @see Parameter
 * @return all parameters for this functor operation as a string
 */
std::string iCallFunction::getParameter() const {
	
	return Parameter;
}


/**
 * This functor initialise the repeat delay, if existing.
 *
 * @return true if the repeat delay is initialised, else false
 */
bool iCallFunction::initRepeatDelay() {
	
	DEBUG_OUT_L4(<<"iCallFunction("<<this<<")::initRepeatDelay() start"<<endl<<flush);
	if ( countRepeatDelays < 1 ) {
		//no repeat delay
		DEBUG_OUT_L4(<<"iCallFunction("<<this<<")::initRepeatDelay() end; no repeat delay"<<endl<<flush);
		return false;
	}
	if ( 0 <= indexNextDelay ) {
		//repeat delay allready initialised
		DEBUG_OUT_L4(<<"iCallFunction("<<this<<")::initRepeatDelay() end; repeat delay allready initialised"<<endl<<flush);
		return true;
	}
	//set the next time this function should be repeated
	timeNextCall = std::chrono::system_clock::now() +
		arrayRepeatDelay[ 0 ];
	
	if ( 1 < countRepeatDelays ) {
		indexNextDelay = 1;
	} else {
		if ( bRepeatLastDelay ) {
			//repeat first entry infinitely
			indexNextDelay = 0;
		}
	}
		DEBUG_OUT_L4(<<"iCallFunction("<<this<<")::initRepeatDelay() end; repeat delay initialised"<<endl<<flush);
	//repeat delay initialised
	return true;
}


/**
 * This method ends the operation.
 * (Call if the state changes.)
 */
void iCallFunction::end() {
	//default -> do nothing
	indexNextDelay = -1;
}


/**
 * This method sets the parameter for the operation.
 * The parameter are readed from the given string.
 *
 * @param szFunctionParameter all parameters for this functor operation
 * 	as a string
 * @return true if the parameter could be set, else false
 */
bool iCallFunction::setParameter( const std::string & szFunctionParameter ) {
	//can't set parameter
	return false;
}


#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR

/**
 * This method sets the parameter for the operation.
 * The parameter are readed from the given string.
 *
 * @param szFunctionParameter all parameters for this functor operation
 * 	as a string
 * @return true if the parameter could be set, else false
 */
bool iCallFunction::setParameter( const std::wstring & szFunctionParameter ) {
	//can't set parameter
	return false;
}

#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR



/**
 * This method sets the repeat delays for the operation.
 * The repeat delays are readed from the given string.
 *
 * @param szFunctionRepeatDelay all repeat delays for this functor operation
 * 	as a string
 * @return true if the repeat delays could be set, else false
 */
bool iCallFunction::setRepeatDelay( const std::string & szFunctionRepeatDelay ) {
	
	DEBUG_OUT_L2(<<"iCallFunction("<<this<<")::setRepeatDelay( szFunctionRepeatDelay=\""<<szFunctionRepeatDelay<<"\" ) started"<<endl<<flush);
	/* repeat delays are millisecond numbers seperated by ','
	 * * if the string ends with ',' last delay is not repeated, else it is
	 *   repeated infinitely.
	 */
	if ( arrayRepeatDelay != NULL ) {
		delete [] arrayRepeatDelay;
	}
	arrayRepeatDelay = NULL;
	countRepeatDelays = 0;
	indexNextDelay = -1;
	bRepeatLastDelay = false;
	
	if ( szFunctionRepeatDelay.empty() ) {
		DEBUG_OUT_L2(<<"iCallFunction("<<this<<")::setRepeatDelay( "<<szFunctionRepeatDelay<<" ) end; no repeat delay"<<endl<<flush);
		return true;
	}
	if ( ( szFunctionRepeatDelay == "key" ) ||
			( szFunctionRepeatDelay == "KEY" ) ) {
		arrayRepeatDelay = new std::chrono::milliseconds[ 2 ];
		arrayRepeatDelay[ 0 ] = std::chrono::milliseconds( 1000 );
		arrayRepeatDelay[ 1 ] = std::chrono::milliseconds( 100 );
		countRepeatDelays = 2;
		bRepeatLastDelay = true;
		
		DEBUG_OUT_L2(<<"iCallFunction("<<this<<")::setRepeatDelay( "<<szFunctionRepeatDelay<<" ) end; \"key\" delay"<<endl<<flush);
		return true;
	}
	
	std::list< int > liReadedRepeatDelays;
	//read the repeat delay values
	unsigned int iIndexForNumber = 0;
	unsigned long uiMilliSecondsToWait;
	while ( ( iIndexForNumber != std::string::npos ) &&
			( iIndexForNumber < szFunctionRepeatDelay.size() ) ) {
		iIndexForNumber = szFunctionRepeatDelay.find_first_of(
			"+0123456789", iIndexForNumber );
		if ( ( iIndexForNumber == std::string::npos ) ||
				( szFunctionRepeatDelay.size() <= iIndexForNumber ) ) {
			break;  //end reached
		}
		//read number for delay in milli seconds
		uiMilliSecondsToWait = strtol(
			szFunctionRepeatDelay.substr( iIndexForNumber ).c_str(), NULL, 10 );
		if ( 0 < uiMilliSecondsToWait ) {
			liReadedRepeatDelays.push_back( uiMilliSecondsToWait );
		}  //else ignore values below 1 milli second
		//read next seperator
		iIndexForNumber =
			szFunctionRepeatDelay.find_first_of( " ,_:", iIndexForNumber );
		if ( ( iIndexForNumber == std::string::npos ) ||
				( szFunctionRepeatDelay.size() <= iIndexForNumber ) ) {
			break;  //end reached
		}
	}
	
	//store readed values into arrayRepeatDelay
	countRepeatDelays = liReadedRepeatDelays.size();
	if ( 0 < countRepeatDelays ) {
		//repeat delays exists
		iIndexForNumber = szFunctionRepeatDelay.find_first_of(
			",_:", iIndexForNumber );
		if ( ( iIndexForNumber == std::string::npos ) ||
				( szFunctionRepeatDelay.size() <= iIndexForNumber ) ) {
			//string ends with seperator -> repeat infinitely
			bRepeatLastDelay = true;
			DEBUG_OUT_L2(<<"iCallFunction("<<this<<")::setRepeatDelay( "<<szFunctionRepeatDelay<<" ) repeat last delay infinitely"<<endl<<flush);
		}
		
		DEBUG_OUT_L2(<<"iCallFunction("<<this<<")::setRepeatDelay( "<<szFunctionRepeatDelay<<" ) repeat delays: "<<countRepeatDelays<<", which are: "<<flush);
		arrayRepeatDelay = new std::chrono::milliseconds[ countRepeatDelays ];
		unsigned int uiIndexRepeatDelay = 0;
		for ( std::list< int >::const_iterator
				itrReadedRepeatDelay = liReadedRepeatDelays.begin();
				itrReadedRepeatDelay != liReadedRepeatDelays.end();
				++itrReadedRepeatDelay ) {
			
			arrayRepeatDelay[ uiIndexRepeatDelay ] = std::chrono::milliseconds(
					(*itrReadedRepeatDelay) );
			DEBUG_OUT_L2(<<(*itrReadedRepeatDelay)<<","<<flush);
			++uiIndexRepeatDelay;
		}
		DEBUG_OUT_L2(<<endl<<flush);
	}  //else no repeat delays to set
	DEBUG_OUT_L2(<<"iCallFunction("<<this<<")::setRepeatDelay( "<<szFunctionRepeatDelay<<" ) end; number of repeat delays found: "<<countRepeatDelays<<endl<<flush);
	return true;
}


/**
 * This method repeats this function if it should be repeated.
 *
 * @see arrayRepeatDelay
 * @return the number of times this function was repeated by the call
 */
unsigned int iCallFunction::checkAndRepeatCall() {
	
	//DEBUG_OUT_L4(<<"iCallFunction("<<this<<")::checkAndRepeatCall() start (indexNextDelay="<<indexNextDelay<<", countRepeatDelays="<<countRepeatDelays<<")"<<endl<<flush);
	if ( 0 <= indexNextDelay ) {
		
		unsigned int uiNumberOfRepeats = 0;
		while ( timeNextCall <= std::chrono::system_clock::now() ) {
			//call this function again
			DEBUG_OUT_L2(<<"iCallFunction("<<this<<")::checkAndRepeatCall() call this function again"<<endl<<flush);
			operator()();
			++uiNumberOfRepeats;
			//evaluate next repeat time
			if ( indexNextDelay < countRepeatDelays ) {
				//set the next time this function should be repeated
				DEBUG_OUT_L2(<<"iCallFunction("<<this<<")::checkAndRepeatCall() set the next time this function should be repeated: "<<indexNextDelay<<endl<<flush);
				timeNextCall = timeNextCall +
					arrayRepeatDelay[ indexNextDelay ];
				
				if ( ( indexNextDelay + 1 ) < countRepeatDelays ) {
					++indexNextDelay;
				} else if ( ! bRepeatLastDelay ) {
					//repeat entry not infinitely
					DEBUG_OUT_L2(<<"iCallFunction("<<this<<")::checkAndRepeatCall() repeat entry not infinitely"<<endl<<flush);
					indexNextDelay = -1;
					break;  //done repeat again
				}
				DEBUG_OUT_L2(<<"iCallFunction("<<this<<")::checkAndRepeatCall() new index repeat delay: "<<indexNextDelay<<endl<<flush);
			}
		}  //end while time to repeat again
		//DEBUG_OUT_L4(<<"iCallFunction("<<this<<")::checkAndRepeatCall() end; number of repeats: "<<uiNumberOfRepeats<<endl<<flush);
		return uiNumberOfRepeats;
	}  //else nothing repeated
	//DEBUG_OUT_L4(<<"iCallFunction("<<this<<")::checkAndRepeatCall() end"<<endl<<flush);
	return 0;
}







