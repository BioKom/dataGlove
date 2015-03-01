
//TODO check

/**
 * @file cCallSetBoolFlag
 * file name: cCallSetBoolFlag.cpp
 * @author Betti Oesterholz
 * @date 03.01.2015
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file implements the class for setting a flag to true.
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
 * This file implements the class for setting a flag to true.
 * If this function is called the functor flag will be set to true.
 *
 * @pattern functor
 * @see bFlag
 * @see getFlag()
 * @see iCallFunction
 * @see cDataGloveState
 * @see cEvaluateDataGloveState
 */
/*
History:
03.01.2015  Oesterholz  created
*/



#include "cCallSetBoolFlag.h"



using namespace std;
using namespace nDataGlove::nMapper;


/**
 * constructor
 */
cCallSetBoolFlag::cCallSetBoolFlag() : bFlag( false ) {
	//nothing to do
}


/**
 * destructor
 */
cCallSetBoolFlag::~cCallSetBoolFlag() {
	//nothing to do
}


/**
 * @return the name of this class "cCallSetBoolFlag"
 */
std::string cCallSetBoolFlag::getName() const {
	
	return "cCallSetBoolFlag";
}


/**
 * This functor operator is called the execute the function implemented
 * by this object.
 *
 * @return true if the call to the function was sucessfull, else false
 */
bool cCallSetBoolFlag::operator()() {
	
	bFlag = true;
	
	return true;
}


/**
 * This method ends the operation.
 * (Call if the state changes.)
 */
void cCallSetBoolFlag::end() {
	//nothing to do
}


/**
 * @return The flag, which will be set to true by this operator.
 * 	@see bFlag
 */
bool cCallSetBoolFlag::getFlag() const {
	
	return bFlag;
}





