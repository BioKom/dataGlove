
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
 * This file specifies the class for a preparing the change of the modus.
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
 * This file specifies the class for a preparing the change of the modus.
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


#ifndef ___N_DATA_GLOVE__N_MAPPER__C_CALL_PREPARE_CHANGE_MODUS__
#define ___N_DATA_GLOVE__N_MAPPER__C_CALL_PREPARE_CHANGE_MODUS__


#include "version.h"

#include <string>

#include "iCallFunction.h"


namespace nDataGlove{
namespace nMapper{


class cCallPrepareChangeModus : public iCallFunction{
public:

	/**
	 * constructor
	 *
	 * @param iInModus The modus, which will be prepared with this operator.
	 * 	@see iModus
	 */
	cCallPrepareChangeModus( const int iInModus );
	
	/**
	 * destructor
	 */
	virtual ~cCallPrepareChangeModus();
	
	/**
	 * @return the name of this class "cCallPrepareChangeModus"
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
	 * @return The modus, which will be prepared with this operator.
	 * 	@see iModus
	 */
	int getModus() const;
	
	/**
	 * @return The actual prepared modus in which cCallChangeModus should change.
	 * 	@see iPreparedModus
	 */
	static int getPreparedModus();
	
	/**
	 * @return If true this operator was called and not ended.
	 * 	@see bIsActiv
	 */
	static bool isActiv();
	
protected:
	
//members
	/**
	 * The modus, which will be prepared with this operator.
	 * @see getModus()
	 * @see iPreparedModus
	 * @see bIsActiv
	 */
	int iModus;
	
	/**
	 * The actual prepared modus in which cCallChangeModus should change.
	 * @see getModus()
	 * @see bIsActiv
	 */
	static int iPreparedModus;

	/**
	 * If true this operator was called and not ended.
	 * ( operator()() called but not end() )
	 * @see operator()();
	 * @see end()
	 * @see iModus
	 */
	static bool bIsActiv;
	
};//end class cCallPrepareChangeModus


};//end namespace nMapper
};//end namespace nDataGlove

#endif //___N_DATA_GLOVE__N_MAPPER__C_CALL_PREPARE_CHANGE_MODUS__





