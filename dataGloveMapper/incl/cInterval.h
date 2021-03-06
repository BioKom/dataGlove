
//TODO check

/**
 * @file cInterval
 * file name: cInterval.h
 * @author Betti Oesterholz
 * @date 05.08.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies the class for a interval of long values.
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
 * This file specifies the class for a interval of long values.
 *
 *
 */
/*
History:
05.08.2014  Oesterholz  created
*/


#ifndef ___N_DATA_GLOVE__N_MAPPER__C_INTERVAL__
#define ___N_DATA_GLOVE__N_MAPPER__C_INTERVAL__


#include "version.h"

#include <string>

#include "cIntervalCorrection.h"


namespace nDataGlove{
namespace nMapper{


class cInterval{
public:

	/**
	 * The standard constructor for the interval.
	 *
	 * @param inPIntervalCorrection The pointer to the correction object for
	 * 	the intervals, or NULL if non exists.
	 * 	@see pIntervalCorrection
	 */
	cInterval( const cIntervalCorrection * inPIntervalCorrection = NULL );
	
	/**
	 * The constructor for the DGTech VHand data glove handler.
	 *
	 * @param lInMin the minimal value, which the interval contains
	 * 	@see lMinimum
	 * @param lInMax the maximal value, which the interval contains
	 * 	@see lMaximum
	 */
	cInterval( const long lInMin, const long lInMax );
	
	/**
	 * The constructor for the DGTech VHand data glove handler.
	 *
	 * @param lInMin the minimal value, which the interval contains
	 * 	@see lMinimum
	 * @param lInMax the maximal value, which the interval contains
	 * 	@see lMaximum
	 * @param lInTarget the target value for the interval
	 * 	@see lTarget
	 */
	cInterval( const long lInMin, const long lInMax, const long lInTarget );
	
	/**
	 * @return the name of this class "cInterval"
	 */
	std::string getName() const;
	
	/**
	 * @param lNewMinimum The minimal value, which the interval contains.
	 * 	@see lMinimum
	 */
	void setMinimum( const long lNewMinimum );
	
	/**
	 * @param lNewMaximum The maximal value, which the interval contains.
	 * 	@see lMaximum
	 */
	void setMaximum( const long lNewMaximum );
	
	/**
	 * @param lNewTarget The target value for the intervals.
	 * 	@see lTarget
	 */
	void setTarget( const long lNewTarget );
	
	/**
	 * Orders the maximum and minimum values
	 */
	void order();
	
protected:
	
//members
	
	/**
	 * The minimal value, which the interval contains.
	 */
	long lMinimum;
	
	/**
	 * The maximal value, which the interval contains.
	 */
	long lMaximum;
	
	/**
	 * The target value for the intervals.
	 * The target value is the value, which is the best value in the interval.
	 */
	long lTarget;
	
	
	/**
	 * The pointer to the correction object for the intervals, or NULL if
	 * non exists.
	 *
	 * For some values (e.g. Quaterion) the message value to hand position
	 * mapping changes (e.g. no tilt is one time 0 and the other 2000 ).
	 * The interval corrections are for theas cases.
	 * The correction objects can have a lower and a upper border.
	 * If a value is lower / higher than the lower / upper border of the
	 * correction the correction value will be adapted, so that the value is
	 * the lower / upper border.
	 * Every given value will be corrected with the corection value.
	 *
	 * @see pIntervalCorrectionUsed
	 */
	const cIntervalCorrection * pIntervalCorrection;
	
	/**
	 * True if pIntervalCorrection is set (not NULL), else false.
	 * @see pIntervalCorrection
	 */
	bool bIntervalCorrectionUsed;
	
	
public:  //inline
	
	/**
	 * @return The minimal value, which the interval contains.
	 * 	@see lMinimum
	 */
	inline long getMinimum() const {
		return lMinimum;
	}
	
	/**
	 * @return The maximal value, which the interval contains.
	 * 	@see lMaximum
	 */
	inline long getMaximum() const {
		return lMaximum;
	}
	
	/**
	 * @return The target value for the intervals.
	 * 	@see lTarget
	 */
	inline long getTarget() const {
		return lTarget;
	}
	
	/**
	 * Checks if the given value is in the interval.
	 *
	 * @param lInValue the value, which should be checked
	 * @return true if the given value lInValue is in this interval, else false
	 * 	( lMinimum <= lInValue <= lMaximum )
	 */
	inline bool isIn( const long lInValue ) const {
		
		if ( bIntervalCorrectionUsed ) {
			//use correction object
			const long lCorrectedValue = pIntervalCorrection->correct( lInValue );
			return ( lMinimum <= lCorrectedValue ) &&
				( lCorrectedValue <= lMaximum );
		}
		
		return ( lMinimum <= lInValue ) && ( lInValue <= lMaximum );
	}
	
	/**
	 * Evaluates the distance of the given value to the target value.
	 *
	 * @param lInValue the value, for which the distance should be evalued
	 * @return the distance to the target value
	 * 	( abs( lTarget - lInValue ) )
	 */
	inline long getDistanceToTarget( const long lInValue ) const {
		
		if ( bIntervalCorrectionUsed ) {
			//use correction object
			const long lCorrectedValue = pIntervalCorrection->correct( lInValue );
			return ( lTarget < lCorrectedValue ) ?
				( lCorrectedValue - lTarget ) : ( lTarget - lCorrectedValue );
		}
		return ( lTarget < lInValue ) ?
			( lInValue - lTarget ) : ( lTarget - lInValue );
	}
	
};//end class cInterval


};//end namespace nMapper
};//end namespace nDataGlove

#endif //___N_DATA_GLOVE__N_MAPPER__C_INTERVAL__





