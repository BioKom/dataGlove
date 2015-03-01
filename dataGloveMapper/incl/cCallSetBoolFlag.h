
//TODO check

/**
 * @file cCallSetBoolFlag
 * file name: cCallSetBoolFlag.h
 * @author Betti Oesterholz
 * @date 03.01.2015
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies the class for setting a flag to true.
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
 * This file specifies the class for setting a flag to true.
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


#ifndef ___N_DATA_GLOVE__N_MAPPER__C_CALL_SET_BOOL_FLAG__
#define ___N_DATA_GLOVE__N_MAPPER__C_CALL_SET_BOOL_FLAG__


#include "version.h"

#include <string>

#include "iCallFunction.h"


namespace nDataGlove{
namespace nMapper{


class cCallSetBoolFlag : public iCallFunction{
public:

	/**
	 * constructor
	 */
	cCallSetBoolFlag();
	
	/**
	 * destructor
	 */
	virtual ~cCallSetBoolFlag();
	
	/**
	 * @return the name of this class "cCallSetBoolFlag"
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
	 * @return The flag, which will be set to true by this operator.
	 * 	@see bFlag
	 */
	bool getFlag() const;

	
	
protected:
	
//members
	/**
	 * The flag, which will be set to true by this operator.
	 * @see getFlag()
	 */
	bool bFlag;
	
};//end class cCallSetBoolFlag


};//end namespace nMapper
};//end namespace nDataGlove

#endif //___N_DATA_GLOVE__N_MAPPER__C_CALL_SET_BOOL_FLAG__





