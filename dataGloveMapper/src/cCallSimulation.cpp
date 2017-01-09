
//TODO check

/**
 * @file cCallSimulation
 * file name: cCallSimulation.cpp
 * @author Betti Oesterholz
 * @date 06.08.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file implements the class for a simulation of a call to a function.
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
 * This file implements the class for a simulation of a call to a function.
 * It will write a message every time it is called.
 * It is a functor.
 *
 * @pattern functor
 * @see iCallFunction
 * @see cDataGloveState
 * @see cEvalueDataGloveState
 */
/*
History:
06.08.2014  Oesterholz  created
*/


#include "cCallSimulation.h"


#include <iostream>



using namespace nDataGlove::nMapper;
using namespace std;



/**
 * constructor
 *
 * @param szInCalledFunction The name of the function to call.
 * 	@see szCalledFunction
 * @param szInCallParameter The parameter for the call of the function.
 * 	@see szCallParameter
 */
cCallSimulation::cCallSimulation( const std::string & szInCalledFunction,
		const std::string & szInCallParameter ) :
			szCalledFunction( szInCalledFunction ),
			szCallParameter( szInCallParameter ) {
	
	Parameter = szInCalledFunction + " " + szInCallParameter;
}


/**
 * destructor
 */
cCallSimulation::~cCallSimulation() {
	//nothing to do
}


/**
 * @return the name of this class "cCallSimulation"
 */
std::string cCallSimulation::getName() const {
	return std::string("cCallSimulation");
}


/**
 * This functor operator is called the execute the function implemented
 * by this object.
 *
 * @return true if the call to the function was sucessfull, else false
 */
bool cCallSimulation::operator()() {
	
	cout<<"simulate calling: "<<szCalledFunction<<"("<<szCallParameter<<")"<<endl;
	//call parent operator
	iCallFunction::operator()();
	return true;
}


/**
 * This method ends the operation.
 * (Call if the state changes.)
 */
void cCallSimulation::end() {
	
	cout<<"calling end: "<<szCalledFunction<<"("<<szCallParameter<<")"<<endl;
}


/**
 * @return The name of the function to call.
 * 	@see szCalledFunction
 */
std::string cCallSimulation::getCalledFunction() const {
	
	return szCalledFunction;
}


/**
 * @return The parameter for the call of the function.
 * 	@see szCallParameter
 */
std::string cCallSimulation::getCallParameter() const {
	
	return szCallParameter;
}






