
//TODO check

/**
 * @file cInterval
 * file name: cInterval.cpp
 * @author Betti Oesterholz
 * @date 05.08.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file implements the class for a interval of long values.
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
 * This file implements the class for a interval of long values.
 *
 *
 */
/*
History:
05.08.2014  Oesterholz  created
*/



#include "cInterval.h"


using namespace nDataGlove::nMapper;

/**
 * The standard constructor for the DGTech VHand data glove handler.
 */
cInterval::cInterval() :
		lMinimum( 0 ), lMaximum( 0 ), lTarget( 0 ) {
	//nothing to do
}


/**
 * The standard constructor for the DGTech VHand data glove handler.
 *
 * @param lInMin the minimal value, which the interval contains
 * 	@see lMinimum
 * @param lInMax the maximal value, which the interval contains
 * 	@see lMaximum
 */
cInterval::cInterval( const long lInMin, const long lInMax ) :
		lMinimum( lInMin ), lMaximum( lInMax ),
		lTarget( ( lInMax - lInMin ) / 2 ) {
	
	if ( lMaximum < lMinimum ) {
		const long lTmpValue = lMaximum;
		lMaximum = lMinimum;
		lMinimum = lTmpValue;
		lTarget = ( lMaximum - lMinimum ) / 2;
	}
}


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
cInterval::cInterval( const long lInMin, const long lInMax, const long lInTarget ) :
		lMinimum( lInMin ), lMaximum( lInMax ), lTarget( lInTarget ) {
	
	if ( lMaximum < lMinimum ) {
		const long lTmpValue = lMaximum;
		lMaximum = lMinimum;
		lMinimum = lTmpValue;
		lTarget = ( lMaximum - lMinimum ) / 2;
	}
}


/**
 * @return the name of this class "cInterval"
 */
std::string cInterval::getName() const {
	
	return std::string("cInterval");
}


/**
 * @param lNewMinimum The minimal value, which the interval contains.
 * 	@see lMinimum
 */
void cInterval::setMinimum( const long lNewMinimum ) {
	
	lMinimum = lNewMinimum;
	if ( lMaximum < lMinimum ) {
		const long lTmpValue = lMaximum;
		lMaximum = lMinimum;
		lMinimum = lTmpValue;
	}
	if ( ( lTarget < lMinimum ) || ( lMaximum < lTarget ) ) {
		//target outside intervall bonds -> evalue new target
		lTarget = ( lMaximum - lMinimum ) / 2;
	}
}


/**
 * @param lNewMaximum The maximal value, which the interval contains.
 * 	@see lMaximum
 */
void cInterval::setMaximum( const long lNewMaximum ) {
	
	lMaximum = lNewMaximum;
	if ( lMaximum < lMinimum ) {
		const long lTmpValue = lMaximum;
		lMaximum = lMinimum;
		lMinimum = lTmpValue;
	}
	if ( ( lTarget < lMinimum ) || ( lMaximum < lTarget ) ) {
		//target outside intervall bonds -> evalue new target
		lTarget = ( lMaximum - lMinimum ) / 2;
	}
}


/**
 * @param lNewTarget The target value for the intervals.
 * 	@see lTarget
 */
void cInterval::setTarget( const long lNewTarget ) {
	
	lTarget = lNewTarget;
}





