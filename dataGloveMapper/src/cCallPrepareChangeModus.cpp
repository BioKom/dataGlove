
//TODO check

/**
 * @file cCallPrepareChangeModus
 * file name: cCallPrepareChangeModus.h
 * @author Betti Oesterholz
 * @date 24.08.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file implements the class for a preparing the change of the modus.
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
 * This file implements the class for a preparing the change of the modus.
 * It will prepare the call to cEvaluateDataGloveState::setActualModus() .
 * The class cCallChangeModus will just work, if this operation was
 * activated for the correct modus.
 *
 * @pattern functor
 * @see cEvaluateDataGloveState::setActualModus()
 * @see cCallChangeModus
 * @see iCallFunction
 * @see cDataGloveState
 * @see cEvaluateDataGloveState
 */
/*
History:
24.08.2014  Oesterholz  created
*/



#include "cCallPrepareChangeModus.h"


#if ( defined DEBUG_CALL_FUNCTION ) || ( defined DEBUG_PREPARED_MODUS_FUNCTION )
	#include <iostream>
#endif  //( defined DEBUG_CALL_FUNCTION ) || ( defined DEBUG_PREPARED_MODUS_FUNCTION )


#ifdef DEBUG_CALL_FUNCTION
	#ifndef DEBUG_PREPARED_MODUS_FUNCTION
		#define DEBUG_PREPARED_MODUS_FUNCTION
	#endif  //DEBUG_PREPARED_MODUS_FUNCTION
#endif  //DEBUG_CALL_FUNCTION




using namespace nDataGlove::nMapper;
using namespace std;

int cCallPrepareChangeModus::iPreparedModus = 0;
bool cCallPrepareChangeModus::bIsActiv = false;


/**
 * constructor
 *
 * @param iInModus The modus, which will be prepared with this operator.
 * 	@see iModus
 */
cCallPrepareChangeModus::cCallPrepareChangeModus( const int iInModus ) :
		iModus( iInModus ) {
	//nothing to do
}


/**
 * constructor
 *
 * @param szInModus The modus as a string, which will be prepared with
 * 	this operator.
 * 	@see iModus
 */
cCallPrepareChangeModus::cCallPrepareChangeModus( const string & szInModus ) {
	
	setParameter( szInModus );
}


/**
 * destructor
 */
cCallPrepareChangeModus::~cCallPrepareChangeModus() {
	//nothing to do
}


/**
 * @return the name of this class "cCallPrepareChangeModus"
 */
std::string cCallPrepareChangeModus::getName() const {
	return std::string("cCallPrepareChangeModus");
}


/**
 * This functor operator is called the execute the function implemented
 * by this object.
 *
 * @return true if the call to the function was sucessfull, else false
 */
bool cCallPrepareChangeModus::operator()() {
	
	//call parent operator
	iCallFunction::operator()();
	//prepare modus
	iPreparedModus = iModus;
	bIsActiv = true;
#ifdef DEBUG_PREPARED_MODUS_FUNCTION
	cout<<"preparing changing to modus: "<<iModus<<endl;
#endif  //DEBUG_PREPARED_MODUS_FUNCTION
	return true;
}


/**
 * This method ends the operation.
 * (Call if the state changes.)
 */
void cCallPrepareChangeModus::end() {
	
#ifdef DEBUG_CALL_FUNCTION
	cout<<"end preparing changing to modus: "<<iModus<<endl;
#endif  //DEBUG_CALL_FUNCTION
}


/**
 * Unprepares this change modus function.
 * (After the call, a change modus function will not be prepared.)
 */
void cCallPrepareChangeModus::unprepare() {
	
	bIsActiv = false;
	iPreparedModus = -1;
	
#ifdef DEBUG_CALL_FUNCTION
	cout<<"unpreparing changing to modus"<<endl;
#endif  //DEBUG_CALL_FUNCTION
}


/**
 * This method sets the parameter for the operation.
 * The parameter are readed from the given string.
 *
 * @param szModus the number for the modus as a string
 * @return true if the parameter could be set, else false
 */
bool cCallPrepareChangeModus::setParameter( const string & szModus ) {
	//set modus  parameter
	Parameter = szModus;
	iModus = atoi( szModus.c_str() );
	return true;
}


/**
 * @return The modus, which will be prepared with this operator.
 * 	@see iModus
 */
int cCallPrepareChangeModus::getModus() const {
	
	return iModus;
}


/**
 * @return The actual prepared modus in which cCallChangeModus should change.
 * 	@see iPreparedModus
 */
int cCallPrepareChangeModus::getPreparedModus() {
	
	return iPreparedModus;
}


/**
 * @return If true this operator was called and not ended.
 * 	@see bIsActiv
 */
bool cCallPrepareChangeModus::isActiv() {
	
	return bIsActiv;
}







