
//TODO check

/**
 * @file cDataGloveState
 * file name: cDataGloveState.cpp
 * @author Betti Oesterholz
 * @date 05.08.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file implements the class for a data glove state.
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
 * This file implements the class for a data glove state.
 * A data glove state is a number of intervals for data glove sampling
 * values and a action, which is executed, if the data glove sampling values
 * are in the intervalls.
 * The state is also for a modus, in which the application is.
 *
 *
 */
/*
History:
05.08.2014  Oesterholz  created
*/



#include "cDataGloveState.h"

#include "iCallFunction.h"
#include "cInterval.h"



using namespace std;
using namespace nDataGlove::nMapper;



/**
 * The standard constructor for the DGTech VHand data glove handler.
 *
 * @param iInModus The modus, for which this state is.
 * 	@see iModus
 * @param pInCallFunction The function, which should be called in this state
 * 	Note: The given call function object will be owned by this object,
 * 		and will be deleted by it. (Don't delete it!)
 * 	@see pCallFunction
 * @param iInRepeatAllMilliSecondsAll this milli seconds the call function is called.
 * 	If 0 the call function will be repeated once.
 * 	@see iRepeatAllMilliSecondsAll
 */
cDataGloveState::cDataGloveState( const int iInModus,
		iCallFunction * pInCallFunction,
		const unsigned int iInRepeatAllMilliSecondsAll ) : iModus( iInModus ),
		pCallFunction( pInCallFunction ),
		iRepeatAllMilliSeconds( iInRepeatAllMilliSecondsAll ), ulCalls( 0 ) {
	//nothing to do
}


/**
 * The standard constructor for the DGTech VHand data glove handler.
 *
 * @param iInModus The modus, for which this state is.
 * 	@see iModus
 * @param mapInStateArea The areas of data glove sampling values, for
 * 	this state.
 * 	Note: The given intervalls will be owned by this object and will
 * 		be deleted by it. (Don't delete them!)
 * 	@see mapStateArea
 * @param pInCallFunction The function, which should be called in this state
 * 	Note: The given call function object will be owned by this object,
 * 	and will be deleted by it. (Don't delete it!)
 * 	@see pCallFunction
 * @param iInRepeatAllMilliSecondsAll this milli seconds the call
 * 	function is called.
 * 	If 0 the call function will be repeated once.
 * 	@see iRepeatAllMilliSecondsAll
 */
cDataGloveState::cDataGloveState( const int iInModus,
			std::map< nDataGlove::nModelDataGloveDGTechVHand::
				cMessageSamplingDataFromDataGlove::tTypeSamplingValue,
				cInterval * > mapInStateArea,
		iCallFunction * pInCallFunction,
		const unsigned int iInRepeatAllMilliSecondsAll ) : iModus( iInModus ),
		mapStateArea( mapInStateArea ),
		pCallFunction( pInCallFunction ),
		iRepeatAllMilliSeconds( iInRepeatAllMilliSecondsAll ), ulCalls( 0 ) {
	//nothing to do
}


/**
 * destructor
 */
cDataGloveState::~cDataGloveState() {
	
	for ( std::map< nDataGlove::nModelDataGloveDGTechVHand::
			cMessageSamplingDataFromDataGlove::tTypeSamplingValue,
			cInterval * >::const_iterator
			itrValueArea = mapStateArea.begin();
			itrValueArea != mapStateArea.end(); ++itrValueArea ) {
		
		delete itrValueArea->second;
	}
	if ( pCallFunction ) {
		delete pCallFunction;
	}
}


/**
 * @return the name of this class "cDataGloveState"
 */
std::string cDataGloveState::getName() const {
	
	return std::string( "cDataGloveState" );
}


/**
 * This method will set the given interval, as the interval for the
 * given sampling type for this state. (Yust sampling messages, with
 * sampling values of the given type in the given interval, will be in
 * this state.)
 *
 * @see mapStateArea
 * @see isIn()
 * @see removeInterval()
 * @see getIntervalForType()
 * @see getMapForInterval()
 * @param typeSamplingValue the type of the sampling value, for which
 * 	the interval is
 * @param pInterval a pointer to the interval for the given sampling
 * 	type value
 * 	Note: This object will take ownership of the interval and will
 * 		delete it. (Don't delete it.)
 */
void cDataGloveState::setInterval( const nDataGlove::nModelDataGloveDGTechVHand::
		cMessageSamplingDataFromDataGlove::tTypeSamplingValue typeSamplingValue,
		cInterval * pInterval ) {
	
	std::map< nDataGlove::nModelDataGloveDGTechVHand::
		cMessageSamplingDataFromDataGlove::tTypeSamplingValue,
		cInterval * >::iterator itrFoundInterval =
			mapStateArea.find( typeSamplingValue );
	if ( itrFoundInterval == mapStateArea.end() ) {
		//no interval for the sampling value
		mapStateArea.insert( pair< nDataGlove::nModelDataGloveDGTechVHand::
			cMessageSamplingDataFromDataGlove::tTypeSamplingValue,
			cInterval * >( typeSamplingValue, pInterval ) );
		
	} else {//interval for the sampling value exists allready
		//delete old interval
		delete itrFoundInterval->second;
		//set new interval
		itrFoundInterval->second = pInterval;
	}
}


/**
 * This method will remove the interval, as the interval for the
 * given sampling type for this state.
 *
 * @see mapStateArea
 * @see isIn()
 * @see setInterval()
 * @see getIntervalForType()
 * @see getMapForInterval()
 * @param typeSamplingValue the type of the sampling value, for which
 * 	interval should be removed
 * @return true if a interval was removed, else false
 */
bool cDataGloveState::removeInterval( const nDataGlove::nModelDataGloveDGTechVHand::
		cMessageSamplingDataFromDataGlove::tTypeSamplingValue typeSamplingValue ) {
	
	std::map< nDataGlove::nModelDataGloveDGTechVHand::
		cMessageSamplingDataFromDataGlove::tTypeSamplingValue,
		cInterval * >::iterator itrFoundInterval =
			mapStateArea.find( typeSamplingValue );
	if ( itrFoundInterval == mapStateArea.end() ) {
		//no such value type
		return false;
	}//else interval for sampling value found -> erase it
	delete itrFoundInterval->second;
	mapStateArea.erase( itrFoundInterval );
	return true;
}


/**
 * @param iNewModus The modus, for which this state is.
 * 	@see iModus
 */
void cDataGloveState::setModus( const int iNewModus ) {
	
	iModus = iNewModus;
}


/**
 * @param uiNewRepeatAllMilliSeconds All this milli seconds the call
 * 	function is called.
 * 	If 0 the call function will be repeated once.
 * @see iRepeatAllMilliSeconds
 * @see getRepeatAllMilliSeconds()
 * @see pCallFunction
 * @see callFunction()
 * @see pRepeaterThread
 */
void cDataGloveState::setRepeatAllMilliSeconds(
		const unsigned int uiNewRepeatAllMilliSeconds ) {
	
	iRepeatAllMilliSeconds = uiNewRepeatAllMilliSeconds;
}


/**
 * Orders all interval values.
 * @see cInterval:order()
 */
void cDataGloveState::orderIntervals() {
	
	for ( std::map< nDataGlove::nModelDataGloveDGTechVHand::
			cMessageSamplingDataFromDataGlove::tTypeSamplingValue,
			cInterval * >::const_iterator
			itrValueArea = mapStateArea.begin();
			itrValueArea != mapStateArea.end(); ++itrValueArea ) {
		
		itrValueArea->second->order();
	}
}


/**
 * @return true if this a a valid data glove state, else false
 */
bool cDataGloveState::isValid() const {
	
	return ( pCallFunction != NULL ) && ( ! mapStateArea.empty() );
}






