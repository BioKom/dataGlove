
//TODO check

/**
 * @file cDataGloveState
 * file name: cDataGloveState.h
 * @author Betti Oesterholz
 * @date 05.08.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies the class for a data glove state.
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
 * This file specifies the class for a data glove state.
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


#ifndef ___N_DATA_GLOVE__N_MAPPER__C_DATA_GLOVE_STATE__
#define ___N_DATA_GLOVE__N_MAPPER__C_DATA_GLOVE_STATE__


#include "version.h"

#include <string>
#include <map>

#include "cMessageSamplingDataFromDataGlove.h"

#include "iCallFunction.h"
#include "cInterval.h"



namespace nDataGlove{
namespace nMapper{


//forward declarations

class cDataGloveState{
public:

	
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
	explicit cDataGloveState( const int iInModus = 0,
		iCallFunction * pInCallFunction = NULL,
		const unsigned int iInRepeatAllMilliSecondsAll = 0 );
	
	
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
	cDataGloveState( const int iInModus,
			std::map< nDataGlove::nModelDataGloveDGTechVHand::
				cMessageSamplingDataFromDataGlove::tTypeSamplingValue,
				cInterval * > mapInStateArea,
		iCallFunction * pInCallFunction = NULL,
		const unsigned int iInRepeatAllMilliSecondsAll = 0 );
	
	/**
	 * destructor
	 */
	~cDataGloveState();
	
	/**
	 * @return the name of this class "cDataGloveState"
	 */
	std::string getName() const;
	
	
	
protected:
	
//members
	
	/**
	 * The modus, for which this state is.
	 * @see getModus()
	 * @see setModus()
	 */
	int iModus;
	
	/**
	 * The areas of data glove sampling values, for this state.
	 * The intervalls are owned by this object and will be deleted by it.
	 * @see isIn()
	 * @see getIntervalForType()
	 * @see getMapForInterval()
	 * @see setInterval()
	 * @see removeInterval()
	 */
	std::map< nDataGlove::nModelDataGloveDGTechVHand::
		cMessageSamplingDataFromDataGlove::tTypeSamplingValue,
		cInterval * > mapStateArea;
	
	/**
	 * The function, which should be called in this state.
	 * The call function object is owned by this object, and will be deleted
	 * by it.
	 * @see getCallFunction()
	 * @see setCallFunction()
	 */
	iCallFunction * pCallFunction;
	
	/**
	 * All this milli seconds the call function is called.
	 * If 0 the call function will be repeated once.
	 * @see pCallFunction
	 * @see pRepeaterThread
	 */
	unsigned int iRepeatAllMilliSeconds;
	
//TODO extern:
	/**
	 * A pointer to the thread wich repeats the call to the call function.
	 * @see iRepeatAllMilliSeconds
	 * @see pCallFunction
	 */
	//cThreadRepeater * pRepeaterThread;
	
	/**
	 * The number of calls to this state.
	 * @see getCalls()
	 * @see setCalls()
	 * @see incCalls()
	 */
	unsigned long ulCalls;
	
	
public:
	
	/**
	 * Checks if the given value is in the interval.
	 *
	 * @param lInValue the value, which should be checked
	 * @return true if the given value lInValue is in this interval, else false
	 * 	( lMinimum <= lInValue <= lMaximum )
	 */
	inline bool isIn( const nDataGlove::nModelDataGloveDGTechVHand::
			cMessageSamplingDataFromDataGlove * pSamplingMessage ) const {
		
		if ( pSamplingMessage == NULL ) {
			//no sampling message -> sampling values not in this state
			return false;
		}
		for ( std::map< nDataGlove::nModelDataGloveDGTechVHand::
				cMessageSamplingDataFromDataGlove::tTypeSamplingValue,
				cInterval * >::const_iterator
				itrValueArea = mapStateArea.begin();
				itrValueArea != mapStateArea.end(); ++itrValueArea ) {
			
			if ( ! itrValueArea->second->isIn(
						pSamplingMessage->getValue( itrValueArea->first ) ) ) {
				//sampling values not in this state
				return false;
			}
		}
		return true;
	}
	
	
	/**
	 * @return The modus, for which this state is.
	 * 	@see iModus
	 */
	inline int getModus() const {
		return iModus;
	}
	
	/**
	 * @param iNewModus The modus, for which this state is.
	 * 	@see iModus
	 */
	void setModus( const int iNewModus );
	
	
	/**
	 * @return the area of data glove sampling values, for this state.
	 * @see mapStateArea
	 */
	inline std::map< nDataGlove::nModelDataGloveDGTechVHand::
			cMessageSamplingDataFromDataGlove::tTypeSamplingValue,
			cInterval * > & getMapForInterval() {
		
		return mapStateArea;
	}
	
	/**
	 * @return the area of data glove sampling values, for this state.
	 * 	@see mapStateArea
	 */
	inline const std::map< nDataGlove::nModelDataGloveDGTechVHand::
			cMessageSamplingDataFromDataGlove::tTypeSamplingValue,
			cInterval * > & getMapForInterval() const {
		
		return mapStateArea;
	}
	
	/**
	 * Returns the interval for the given sampling value type.
	 *
	 * @see mapStateArea
	 * @param typeSamplingValue the type of the sampling value, for which an
	 * 	interval should be returned
	 * @return the interval for the given sampling value type, or NULL if
	 * 	non exists
	 */
	cInterval * getIntervalForType( const nDataGlove::nModelDataGloveDGTechVHand::
			cMessageSamplingDataFromDataGlove::tTypeSamplingValue typeSamplingValue ) {
		
		std::map< nDataGlove::nModelDataGloveDGTechVHand::
			cMessageSamplingDataFromDataGlove::tTypeSamplingValue,
			cInterval * >::iterator itrFoundInterval =
				mapStateArea.find( typeSamplingValue );
		if ( itrFoundInterval == mapStateArea.end() ) {
			//no interval for the sampling value
			return NULL;
		}
		return itrFoundInterval->second;
	}
	
	
	/**
	 * Returns the interval for the given sampling value type.
	 *
	 * @see mapStateArea
	 * @param typeSamplingValue the type of the sampling value, for which an
	 * 	interval should be returned
	 * @return the interval for the given sampling value type, or NULL if
	 * 	non exists
	 */
	const cInterval * getIntervalForType( const nDataGlove::nModelDataGloveDGTechVHand::
			cMessageSamplingDataFromDataGlove::tTypeSamplingValue typeSamplingValue ) const {
		
		std::map< nDataGlove::nModelDataGloveDGTechVHand::
			cMessageSamplingDataFromDataGlove::tTypeSamplingValue,
			cInterval * >::const_iterator itrFoundInterval =
				mapStateArea.find( typeSamplingValue );
		if ( itrFoundInterval == mapStateArea.end() ) {
			//no interval for the sampling value
			return NULL;
		}
		return itrFoundInterval->second;
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
	void setInterval( const nDataGlove::nModelDataGloveDGTechVHand::
			cMessageSamplingDataFromDataGlove::tTypeSamplingValue typeSamplingValue,
			cInterval * pInterval );
	
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
	bool removeInterval( const nDataGlove::nModelDataGloveDGTechVHand::
			cMessageSamplingDataFromDataGlove::tTypeSamplingValue typeSamplingValue );
	
	
	/**
	 * @return The function, which should be called in this state.
	 * 	@see pCallFunction
	 */
	inline iCallFunction * getCallFunction() {
		
		return pCallFunction;
	}
	
	
	/**
	 * @return The function, which should be called in this state.
	 * 	@see pCallFunction
	 */
	inline iCallFunction * getCallFunction() const {
		
		return pCallFunction;
	}
	
	
	/**
	 * Sets the function, which should be called in this state.
	 * @see pCallFunction
	 *
	 * @param pInCallFunction the function, which should be called in this state
	 */
	inline void setCallFunction( iCallFunction * pInCallFunction ) {
		
		if ( pCallFunction ) {
			//delete old call function
			delete pCallFunction;
		}
		pCallFunction = pInCallFunction;
	}
	
	
	/**
	 * @return All this milli seconds the call function is called.
	 * 	If 0 the call function will be repeated once.
	 * @see iRepeatAllMilliSeconds
	 * @see setRepeatAllMilliSeconds()
	 * @see pCallFunction
	 * @see callFunction()
	 * @see pRepeaterThread
	 */
	inline unsigned int getRepeatAllMilliSeconds() const {
		
		return iRepeatAllMilliSeconds;
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
	void setRepeatAllMilliSeconds(
			const unsigned int uiNewRepeatAllMilliSeconds );
	
	
	/**
	 * @return true if this a a valid data glove state, else false
	 */
	bool isValid() const;

	
	/**
	 * @return The number of calls to this state.
	 * @see ulCalls
	 * @see setCalls()
	 * @see incCalls()
	 */
	inline unsigned long getCalls() const {
		
		return ulCalls;
	}
	
	
	/**
	 * @param ulNewCalls The number of calls to this state.
	 * @see ulCalls
	 * @see getCalls()
	 * @see incCalls()
	 */
	inline void setCalls( const unsigned long ulNewCalls ) {
		
		ulCalls = ulNewCalls;
	}
	
	
	/**
	 * Increases the number of calls to this state with one.
	 * @see ulCalls
	 * @see getCalls()
	 * @see setCalls()
	 */
	inline void incCalls() {
		
		ulCalls++;
	}

	
};//end class cDataGloveState


};//end namespace nMapper
};//end namespace nDataGlove

#endif //___N_DATA_GLOVE__N_MAPPER__C_DATA_GLOVE_STATE__





