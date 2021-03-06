
//TODO check

/**
 * @file cCallChangeModus
 * file name: cCallChangeModus.h
 * @author Betti Oesterholz
 * @date 26.08.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file implements the class for changing of the modus.
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
 * This file implementsNeeded the class for changing of the modus.
 * It will call to cEvaluateDataGloveState::setActualModus() .
 *
 * @pattern functor
 * @see cEvaluateDataGloveState::setActualModus()
 * @see cCallPrepareChangeModus
 * @see iCallFunction
 * @see cDataGloveState
 * @see cEvaluateDataGloveState
 */
/*
History:
26.08.2014  Oesterholz  created
*/


#include "cCallChangeModus.h"

#if ( defined DEBUG_CALL_FUNCTION ) || ( defined DEBUG_CHANGE_MODUS )
	#include <iostream>
#endif  //( defined DEBUG_CALL_FUNCTION ) || ( defined DEBUG_CHANGE_MODUS )


#ifdef DEBUG_CALL_FUNCTION
	
	#ifndef DEBUG_CHANGE_MODUS
		#define DEBUG_CHANGE_MODUS
	#endif //DEBUG_CHANGE_MODUS
#endif  //DEBUG_CALL_FUNCTION


#include "cEvaluateDataGloveState.h"
#include "cCallPrepareChangeModus.h"



using namespace nDataGlove::nMapper;
using namespace std;


/**
 * constructor
 *
 * @param iInModus The modus, which will set with this operator.
 * 	@see iModus
 * @param pInEvaluateDataGloveState a pointer to the object to evaluate the
 * 	data glove state and change the modus
 * 	@see pEvaluateDataGloveState
 * 	@see cEvaluateDataGloveState::setActualModus()
 * @param bInPrepareNeeded if true preparing the modus change is needed,
 * 	else not
 * 	@see bPrepareNeeded
 * 	@see cCallPrepareChangeModus
 */
cCallChangeModus::cCallChangeModus( const int iInModus,
		cEvaluateDataGloveState * pInEvaluateDataGloveState,
		const bool bInPrepareNeeded ) :
		iModus( iInModus ), pEvaluateDataGloveState( pInEvaluateDataGloveState ),
		bPrepareNeeded( bInPrepareNeeded ) {
	//nothing to do
}


/**
 * destructor
 */
cCallChangeModus::~cCallChangeModus() {
	//nothing to do
}


/**
 * @return the name of this class "cCallChangeModus"
 */
std::string cCallChangeModus::getName() const {
	return std::string("cCallChangeModus");
}


/**
 * This functor operator is called the execute the function implemented
 * by this object.
 *
 * @return true if the call to the function was sucessfull, else false
 */
bool cCallChangeModus::operator()() {
	
	if ( pEvaluateDataGloveState == NULL ) {
		return false; //can't change modus
	}
	if ( bPrepareNeeded &&
			( ( ! cCallPrepareChangeModus::isActiv() ) ||
				cCallPrepareChangeModus::getPreparedModus() != iModus ) ) {
		//modus to set was not prepared, but should be
		//call parent operator (unprepare)
		iCallFunction::operator()();
#ifdef DEBUG_CALL_FUNCTION
		cout<<"modus to set was not prepared, but should be: "<<iModus<<endl;
#endif  //DEBUG_CALL_FUNCTION
		return false;
	}
	
#ifdef DEBUG_CHANGE_MODUS
	const bool bModusChanged =
		pEvaluateDataGloveState->setActualModus( iModus );
	if ( bModusChanged ) {
		cout<<"modus changed to: "<<iModus<<endl;
	} else {
		cout<<"could not chang modus to: "<<iModus<<endl;
	}
#else
	pEvaluateDataGloveState->setActualModus( iModus );
#endif  //DEBUG_CHANGE_MODUS
	return true;
}


/**
 * This method ends the operation.
 * (Call if the state changes.)
 */
void cCallChangeModus::end() {
	
#ifdef DEBUG_CALL_FUNCTION
	cout<<"end changing to modus: "<<iModus<<endl;
#endif  //DEBUG_CALL_FUNCTION
}


/**
 * @return The modus, which will be prepared with this operator.
 * 	@see iModus
 */
int cCallChangeModus::getModus() const {
	
	return iModus;
}



/**
 * This method sets the parameter for the operation.
 * The parameter are readed from the given string.
 *
 * @param szModus the number for the modus as a string
 * @return true if the parameter could be set, else false
 */
bool cCallChangeModus::setParameter( const string & szModus ) {
	//set modus  parameter
	Parameter = szModus;
	iModus = atoi( szModus.c_str() );
	return true;
}


/**
 * @return A pointer to the object to evaluate the data glove state and
 * 	change the modus.
 * 	@see pEvaluateDataGloveState
 */
cEvaluateDataGloveState * cCallChangeModus::getEvaluateDataGloveState() const {
	
	return pEvaluateDataGloveState;
}


/**
 * @return If true preparing the modus change is needed, else not.
 * 	@see bPrepareNeeded
 */
bool cCallChangeModus::getPrepareNeeded() const {
	
	return bPrepareNeeded;
}










