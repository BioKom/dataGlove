
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



#include "iCallFunction.h"

#include "cCallPrepareKeybordFunction.h"
#include "cCallPrepareChangeModus.h"


using namespace nDataGlove::nMapper;



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
}


/**
 * This method ends the operation.
 * (Call if the state changes.)
 */
void iCallFunction::end() {
	//default -> do nothing
}




