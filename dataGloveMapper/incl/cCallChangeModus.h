
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
 * This file specifies the class for changing of the modus.
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
 * This file specifies the class for changing of the modus.
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


#ifndef ___N_DATA_GLOVE__N_MAPPER__C_CALL_CHANGE_MODUS__
#define ___N_DATA_GLOVE__N_MAPPER__C_CALL_CHANGE_MODUS__


#include "version.h"

#include <string>

#include "iCallFunction.h"


namespace nDataGlove{
namespace nMapper{

//forward declarations
class cEvaluateDataGloveState;

class cCallChangeModus : public iCallFunction{
public:

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
	cCallChangeModus( const int iInModus,
		cEvaluateDataGloveState * pInEvaluateDataGloveState,
		const bool bInPrepareNeeded = true );
	
	/**
	 * destructor
	 */
	virtual ~cCallChangeModus();
	
	/**
	 * @return the name of this class "cCallChangeModus"
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
	 * This method sets the parameter for the operation.
	 * The parameter are readed from the given string.
	 *
	 * @param szModus the number for the modus as a string
	 * @return true if the parameter could be set, else false
	 */
	virtual bool setParameter( const std::string & szModus );
	
	/**
	 * @return The modus, which will be prepared with this operator.
	 * 	@see iModus
	 */
	int getModus() const;
	
	/**
	 * @return A pointer to the object to evaluate the data glove state and
	 * 	change the modus.
	 * 	@see pEvaluateDataGloveState
	 */
	cEvaluateDataGloveState * getEvaluateDataGloveState() const;
	
	/**
	 * @return If true preparing the modus change is needed, else not.
	 * 	@see bPrepareNeeded
	 */
	bool getPrepareNeeded() const;

	
	
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
	 * A pointer to the object to evaluate the data glove state and change
	 * the modus.
	 * @see getEvaluateDataGloveState()
	 * @see cEvaluateDataGloveState::setActualModus()
	 */
	cEvaluateDataGloveState * pEvaluateDataGloveState;
	
	/**
	 * If true preparing the modus change is needed, else not.
	 * @see getPrepareNeeded()
	 * @see cCallPrepareChangeModus
	 */
	const bool bPrepareNeeded;
	
};//end class cCallChangeModus


};//end namespace nMapper
};//end namespace nDataGlove

#endif //___N_DATA_GLOVE__N_MAPPER__C_CALL_CHANGE_MODUS__





