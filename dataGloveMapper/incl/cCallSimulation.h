
//TODO check

/**
 * @file cCallSimulation
 * file name: cCallSimulation.h
 * @author Betti Oesterholz
 * @date 06.08.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies the class for a simulation of a call to a function.
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
 * This file specifies the class for a simulation of a call to a function.
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


#ifndef ___N_DATA_GLOVE__N_MAPPER__C_CALL_SIMULATION__
#define ___N_DATA_GLOVE__N_MAPPER__C_CALL_SIMULATION__


#include "version.h"

#include <string>

#include "iCallFunction.h"


namespace nDataGlove{
namespace nMapper{


class cCallSimulation : public iCallFunction{
public:

	/**
	 * constructor
	 *
	 * @param szInCalledFunction The name of the function to call.
	 * 	@see szCalledFunction
	 * @param szInCallParameter The parameter for the call of the function.
	 * 	@see szCallParameter
	 */
	cCallSimulation( const std::string & szInCalledFunction,
		const std::string & szInCallParameter );
	
	/**
	 * destructor
	 */
	virtual ~cCallSimulation();
	
	/**
	 * @return the name of this class "cCallSimulation"
	 */
	virtual std::string getName() const;
	
	/**
	 * This functor operator is called the execute the function implemented
	 * by this object.
	 *
	 * @return true if the call to the function was sucessfull, else false
	 */
	virtual bool operator()();
	
	/**
	 * This method ends the operation.
	 * (Call if the state changes.)
	 */
	virtual void end();
	
	/**
	 * @return The name of the function to call.
	 * 	@see szCalledFunction
	 */
	std::string getCalledFunction() const;
	
	/**
	 * @return The parameter for the call of the function.
	 * 	@see szCallParameter
	 */
	std::string getCallParameter() const;
	
	
protected:
	
//members
	/**
	 * The name of the function to call.
	 * @see szCallParameter
	 */
	std::string szCalledFunction;
	
	/**
	 * The parameter for the call of the function.
	 * @see szCalledFunction
	 */
	std::string szCallParameter;
	
	
};//end class cCallSimulation


};//end namespace nMapper
};//end namespace nDataGlove

#endif //___N_DATA_GLOVE__N_MAPPER__C_CALL_SIMULATION__





