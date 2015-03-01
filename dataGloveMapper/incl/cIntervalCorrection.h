
//TODO check

/**
 * @file cIntervalCorrection
 * file name: cIntervalCorrection.h
 * @author Betti Oesterholz
 * @date 11.01.2015
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies the class for a correction interval of long values.
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
 * This file specifies the class for a correction interval of long values.
 * For some values (e.g. Quaterion) the message value to hand position
 * mapping changes (e.g. no tilt is one time 0 and the other 2000 ).
 * The interval corrections are for these cases.
 * The correction objects can have a lower and a upper border.
 * If a value is lower / higher than the lower / upper border of the
 * correction object the correction value will be adapted, so that the
 * value is the lower / upper border.
 * Every given value will be corrected with the correction value.
 *
 * @see cInterval
 * @see cEvaluateDataGloveState
 */
/*
History:
11.01.2015  Oesterholz  created
*/


#ifndef ___N_DATA_GLOVE__N_MAPPER__C_INTERVAL_CORRECTION__
#define ___N_DATA_GLOVE__N_MAPPER__C_INTERVAL_CORRECTION__


#include "version.h"

#include <string>


namespace nDataGlove{
namespace nMapper{


class cIntervalCorrection {
public:
	
	/**
	 * The standard constructor for the interval correction.
	 */
	cIntervalCorrection();
	
	/**
	 * @return the name of this class "cIntervalCorrection"
	 */
	std::string getName() const;
	
	
	/**
	 * Sets the lower border for the correction.
	 * No value should exceed it.
	 * If a value is lower than the lower border of the
	 * correction object the correction value will be adapted, so that the
	 * value is the lower border.
	 *
	 * @see lLowerBorder
	 * @see bHasLowerBorder
	 * @see correct()
	 * @param inLowerBorder the lower border for the correction
	 * 	@see lLowerBorder
	 */
	void setLowerBorder( const long inLowerBorder );
	
	/**
	 * Sets the upper border for the correction.
	 * No value should exceed it.
	 * If a value is higher than the upper border of the
	 * correction object the correction value will be adapted, so that the
	 * value is the upper border.
	 *
	 * @see lUpperBorder
	 * @see bHasUpperBorder
	 * @see correct()
	 * @param lNewMinimum the upper border for the correction
	 * 	@see lUpperBorder
	 */
	void setUpperBorder( const long inUpperBorder );
	
	
protected:
	
	
	/**
	 * This interval correction has a lower border.
	 * If a value is lower than the lower border of the correction object
	 * the correction value will be adapted, so that the value is the lower
	 * border.
	 *
	 * @see cIntervalCorrection::isIn()
	 * @see correct()
	 * @see lCorrectionValue
	 * @see lLowerBorder
	 * @see bHasUpperBorder
	 * @see lUpperBorder
	 */
	bool bHasLowerBorder;
	
	/**
	 * The lower border.
	 * If a value is lower than the lower border of the correction object
	 * the correction value will be adapted, so that the value is the lower
	 * border.
	 *
	 * @see cIntervalCorrection::isIn()
	 * @see correct()
	 * @see lCorrectionValue
	 * @see bHasLowerBorder
	 * @see bHasUpperBorder
	 * @see lUpperBorder
	 */
	long lLowerBorder;
	
	/**
	 * This interval correction has a upper border.
	 * If a value is higher than the upper border of the correction object
	 * the correction value will be adapted, so that the value is the upper
	 * border.
	 *
	 * @see cIntervalCorrection::isIn()
	 * @see correct()
	 * @see lCorrectionValue
	 * @see lUpperBorder
	 * @see lLowerBorder
	 * @see bHasLowerBorder
	 */
	bool bHasUpperBorder;
	
	/**
	 * The upper border.
	 * If a value is higher than the upper border of the correction object
	 * the correction value will be adapted, so that the  value is the upper
	 * border.
	 *
	 * @see cIntervalCorrection::isIn()
	 * @see correct()
	 * @see lCorrectionValue
	 * @see bHasUpperBorder
	 * @see bHasLowerBorder
	 * @see lLowerBorder
	 */
	long lUpperBorder;
	
	/**
	 * The correction value.
	 * Every given value will be corrected with the correction value.
	 *
	 * @see cIntervalCorrection::isIn()
	 * @see correct()
	 * @see bHasLowerBorder
	 * @see lLowerBorder
	 * @see bHasUpperBorder
	 * @see lUpperBorder
	 */
	mutable long lCorrectionValue;
	
	
public:  //inline functions
	
	/**
	 * Correct the given value.
	 * For some values (e.g. Quaterion) the message value to hand position
	 * mapping changes (e.g. no tilt is one time 0 and the other 2000 ).
	 * The this object can have a lower and a upper border.
	 * If a value is lower / higher than the lower / upper border of the
	 * correction object the correction value will be adapted, so that the
	 * value is the lower / upper border.
	 * Every given value will be corrected with the correction value.
	 *
	 * @param lValue the value to correct
	 * @return the corrected value
	 */
	inline long correct( const long lValue ) const {
		
		if ( bHasLowerBorder && ( lValue < lLowerBorder ) ) {
			
			lCorrectionValue = lLowerBorder - lValue;
		}
		if ( bHasUpperBorder && ( lUpperBorder < lValue ) ) {
			
			lCorrectionValue = lUpperBorder - lValue;
		}
		
		return lValue + lCorrectionValue;
	}
	
	
};  //end class cIntervalCorrection



};//end namespace nMapper
};//end namespace nDataGlove

#endif //___N_DATA_GLOVE__N_MAPPER__C_INTERVAL_CORRECTION__





