
//TODO check

/**
 * @file iCallFunction
 * file name: iCallFunction.h
 * @author Betti Oesterholz
 * @date 06.08.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies the interface for a function, which should be called.
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
 * This file specifies the interface for a function, which should be called.
 * It is a functor.
 *
 * @pattern functor
 * @pattern interface
 * @see cDataGloveState
 * @see cEvalueDataGloveState
 */
/*
History:
06.08.2014  Oesterholz  created
*/


#ifndef ___N_DATA_GLOVE__N_MAPPER__I_CALL_FUNCTION__
#define ___N_DATA_GLOVE__N_MAPPER__I_CALL_FUNCTION__


#include "version.h"

#include <string>


namespace nDataGlove{
namespace nMapper{


class iCallFunction{
public:

	virtual ~iCallFunction() {
		//nothing to do
	}
	
	/**
	 * @return the name of this class "iCallFunction"
	 */
	virtual std::string getName() const = 0;
	
	/**
	 * This functor operator is called the execute the function implemented
	 * by this object.
	 *
	 * @return true if the call to the function was sucessfull, else false
	 */
	virtual bool operator()() = 0;
	
	/**
	 * This method ends the operation.
	 * (Call if the state changes.)
	 */
	virtual void end() {
		//default -> do nothing
	}
	
	
};//end class iCallFunction


};//end namespace nMapper
};//end namespace nDataGlove

#endif //___N_DATA_GLOVE__N_MAPPER__I_CALL_FUNCTION__





