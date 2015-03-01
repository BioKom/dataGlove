
//TODO check

/**
 * @file cIntervalCorrection
 * file name: cIntervalCorrection.cpp
 * @author Betti Oesterholz
 * @date 11.01.2015
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file implements the class for a correction interval of long values.
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
 * This file implements the class for a correction interval of long values.
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


#include "cIntervalCorrection.h"


using namespace nDataGlove::nMapper;


/**
 * The standard constructor for the interval correction.
 */
cIntervalCorrection::cIntervalCorrection() :
		bHasLowerBorder( false ), lLowerBorder( 0 ),
		bHasUpperBorder( false ), lUpperBorder( 0 ),
		lCorrectionValue( 0 ) {
	//nothing to do
}


/**
 * @return the name of this class "cIntervalCorrection"
 */
std::string cIntervalCorrection::getName() const {
	
	return std::string("cIntervalCorrection");
}


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
void cIntervalCorrection::setLowerBorder( const long inLowerBorder ) {
	
	bHasLowerBorder = true;
	lLowerBorder = inLowerBorder;
}


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
void cIntervalCorrection::setUpperBorder( const long inUpperBorder ) {
	
	bHasUpperBorder = true;
	lUpperBorder = inUpperBorder;
}







