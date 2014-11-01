
//TODO check

/**
 * @file cBorderDataGloveState
 * file name: cBorderDataGloveState.cpp
 * @author Betti Oesterholz
 * @date 05.08.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file implements the class for a barder betwaen data glove states.
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
 * This file implements the class for a barder betwaen data glove states.
 * All intervals with values lower the border value are lower than the border.
 * All intervals with values higher or equal the border value are higher
 * than the border.
 * The border is a integer value for a data glove sampling value type,
 * which seperates two sets of data glove states.
 * If a data glove message is given, a border tree should be used, to
 * evaluate the data glove state for it.
 * It also contains a lower and higher border, so a end state can be evalued.
 * The common usage should be to create a border tree, in which each
 * border node as as the lower and higher each one other border or one
 * data glove state. If there is a border (as lower or higher) given, it
 * shoulb be checked. If there is exactly one data glove state given,
 * the search for the data glove stae is done.
 *
 * @see cDataGloveState
 */
/*
History:
05.08.2014  Oesterholz  created
*/



#include "cBorderDataGloveState.h"


#include "cDataGloveState.h"


using namespace std;
using namespace nDataGlove::nMapper;




/**
 * The standard constructor for the DGTech VHand data glove handler.
 *
 * @param inTypeSamplingValue the type of the data glove sampling value,
 * 	for which the border is
 * 	@see typeSamplingValue
 * @param lInBorderValue the value of the border;
 * 	All data glove sampling values below inTypeSamplingValue are
 * 	in the lower set of data glove states, all values above or equal
 * 	are in the higher set of data glove states.
 * 	@see lBorderValue
 */
cBorderDataGloveState::cBorderDataGloveState( const nDataGlove::nModelDataGloveDGTechVHand::
		cMessageSamplingDataFromDataGlove::tTypeSamplingValue inTypeSamplingValue,
		const long lInBorderValue ) : pParent( NULL ),
		typeSamplingValue( inTypeSamplingValue ), lBorderValue( lInBorderValue ),
		pLowerBorder( NULL ), pHigherBorder( NULL ) {
	//nothing to do
}


/**
 * @return the name of this class "cBorderDataGloveState"
 */
std::string cBorderDataGloveState::getName() const {
	
	return std::string("cBorderDataGloveState");
}


/**
 * This method sorts the given data glove states into the sets for the
 * lower and higher (or equal) states of ths border.
 * The old sets will be meptied before the sorting.
 * @see lowerStates
 * @see higherStates
 * It also calls filterStates() of the lower and higher borders, if
 * they exists.
 * @see pLowerBorder
 * @see pHigherBorder
 *
 * @see lowerStates
 * @see higherStates
 * @see pLowerBorder
 * @see pHigherBorder
 * @param setAllDataGloveStates a set with all data glove states to sort
 * 	into the sets of the lower and higher (or equal) states of this
 * 	and all (directly or indirectly) contained borders
 */
void cBorderDataGloveState::filterStates(
		std::set< cDataGloveState* > setAllDataGloveStates ) {
	
	cInterval * pActualInterval;
	for ( std::set< cDataGloveState* >::iterator
			itrState = setAllDataGloveStates.begin();
			itrState != setAllDataGloveStates.end(); ++itrState ) {
		
		
		pActualInterval = (*itrState)->getIntervalForType( typeSamplingValue );
		if ( pActualInterval == NULL ) {
			//no interval -> this border can't seperate state
			//-> add to both lower and higher
			lowerStates.insert( *itrState );
			higherStates.insert( *itrState );
			continue;
		}
		
		if ( pActualInterval->getMinimum() < lBorderValue ) {
			//interval for type is lower border -> state is lower
			lowerStates.insert( *itrState );
		}
		if ( lBorderValue <= pActualInterval->getMaximum() ) {
			//interval for type is higher or equal border -> state is higher or equa
			higherStates.insert( *itrState );
		}
		//Note: a state / interval can be lower and higher (or equal) this
		// 	border, if:
		// 		getMinimum() < lBorderValue <= getMaximum()
		// 	(the border value is in the interval)
	}
	
	if ( pLowerBorder != NULL ) {
		pLowerBorder->filterStates( lowerStates );
	}
	if ( pHigherBorder != NULL ) {
		pHigherBorder->filterStates( higherStates );
	}
}


/**
 * This method evaluates the set of states (lowerStates or higherStates)
 * for the massge.
 *
 * @see lowerStates
 * @see higherStates
 * @param pMessageSamplingDataFromDataGlove the sampling message, for
 * 	which to evaluate the next states
 * @return the next states (lowerStates or higherStates) for the
 * 	massage, if existing, or NULL if no border for the sampling
 * 	message exists
 */
const set< cDataGloveState * > & cBorderDataGloveState::evalueNextBorderStates( const
		nDataGlove::nModelDataGloveDGTechVHand::cMessageSamplingDataFromDataGlove *
			pMessageSamplingDataFromDataGlove ) {
	
	if ( pMessageSamplingDataFromDataGlove->getValue( typeSamplingValue )
			< lBorderValue ) {
		//the message state is lower then this border
		return lowerStates;
	}  // else the message state is higher or equal then this border
	return higherStates;
}


/**
 * @return The type of the data glove sampling value, for which the border is.
 * 	@see typeSamplingValue
 * @see lBorderValue
 * @see lowerStates
 * @see higherStates
 * @see pLowerBorder
 * @see pHigherBorder
 */
nDataGlove::nModelDataGloveDGTechVHand::cMessageSamplingDataFromDataGlove::
		tTypeSamplingValue cBorderDataGloveState::getTypeSamplingValue() const {
	
	return typeSamplingValue;
}


/**
 * @return The parent border of this border, which contains this border.
 * 	@see pParent
 */
cBorderDataGloveState * cBorderDataGloveState::getParent() {
	
	return pParent;
}


/**
 * @return The parent border of this border, which contains this border.
 * 	@see pParent
 */
const cBorderDataGloveState * cBorderDataGloveState::getParent() const {
	
	return pParent;
}


/**
 * @return The value of the border.
 * 	All data glove sampling values below inTypeSamplingValue are
 * 	in the lower set of data glove states, all values above or equal
 * 	are in the higher set of data glove states.
 * 	@see lBorderValue
 * @see typeSamplingValue
 * @see lowerStates
 * @see higherStates
 * @see pLowerBorder
 * @see pHigherBorder
 */
long cBorderDataGloveState::getBorderValue() const {
	
	return lBorderValue;
}


/**
 * @return The next to check border, if the given value is lower than the
 * 	lBorderValue.
 * 	@see pLowerBorder
 * @see typeSamplingValue
 * @see lBorderValue
 * @see lowerStates
 */
cBorderDataGloveState * cBorderDataGloveState::getLowerBorder() {
	
	return pLowerBorder;
}


/**
 * @return The next to check border, if the given value is lower than the
 * 	lBorderValue.
 * 	@see pLowerBorder
 * @see typeSamplingValue
 * @see lBorderValue
 * @see lowerStates
 */
const cBorderDataGloveState * cBorderDataGloveState::getLowerBorder() const {
	
	return pLowerBorder;
}


/**
 * Sets the lower border.
 *
 * @see typeSamplingValue
 * @see lBorderValue
 * @see lowerStates
 * @param pNewLowerBorder The next to check border, if the given value
 * 	is lower than the lBorderValue.
 * 	@see pLowerBorder
 */
void cBorderDataGloveState::setLowerBorder(
		cBorderDataGloveState * pNewLowerBorder ) {
	
	if ( pLowerBorder != NULL ) {
		pLowerBorder->pParent = NULL;
	}
	pLowerBorder = pNewLowerBorder;
	
	if ( pLowerBorder != NULL ) {
		pLowerBorder->pParent = this;
	}
}


/**
 * @return The set with the states, which are lower than the border.
 * 	@see lowerStates
 * @see filterStates()
 * @see typeSamplingValue
 * @see lBorderValue
 * @see pLowerBorder
 */
set< cDataGloveState * > & cBorderDataGloveState::getLowerStates() {
	
	return lowerStates;
}


/**
 * @return The set with the states, which are lower than the border.
 * 	@see lowerStates
 * @see filterStates()
 * @see typeSamplingValue
 * @see lBorderValue
 * @see pLowerBorder
 */
const set< cDataGloveState * > & cBorderDataGloveState::getLowerStates() const {
	
	return lowerStates;
}


/**
 * Sets the lower states for the border.
 *
 * @see filterStates()
 * @see typeSamplingValue
 * @see lBorderValue
 * @see pLowerBorder
 * @param setInLowerStates The set with the states, which are lower than the border.
 * 	@see lowerStates
 */
void cBorderDataGloveState::setLowerStates(
		const set< cDataGloveState * > & setInLowerStates ) {
	
	lowerStates = setInLowerStates;
}


/**
 * @return The next to check border, if the given value is higher or
 * 	equal than the lBorderValue.
 * 	@see pHigherBorder
 * @see typeSamplingValue
 * @see lBorderValue
 * @see higherStates
 */
cBorderDataGloveState * cBorderDataGloveState::getHigherBorder() {
	
	return pHigherBorder;
}


/**
 * @return The next to check border, if the given value is higher or
 * 	equal than the lBorderValue.
 * 	@see pHigherBorder
 * @see typeSamplingValue
 * @see lBorderValue
 * @see higherStates
 */
const cBorderDataGloveState * cBorderDataGloveState::getHigherBorder() const {
	
	return pHigherBorder;
}


/**
 * Sets the higher border.
 *
 * @see typeSamplingValue
 * @see lBorderValue
 * @see higherStates
 * @param pNewHigherBorder The next to check border, if the given value
 * 	is higher or equal than the lBorderValue.
 * 	@see pHigherBorder
 */
void cBorderDataGloveState::setHigherBorder(
		cBorderDataGloveState * pNewHigherBorder ) {
	
	if ( pHigherBorder != NULL ) {
		pHigherBorder->pParent = NULL;
	}
	pHigherBorder = pNewHigherBorder;
	
	if ( pHigherBorder != NULL ) {
		pHigherBorder->pParent = this;
	}
}


/**
 * @return The set with the states, which are higher or equal than the
 * 	border.
 * 	@see higherStates
 * @see filterStates()
 * @see typeSamplingValue
 * @see lBorderValue
 * @see pHigherBorder
 */
set< cDataGloveState * > & cBorderDataGloveState::getHigherStates() {
	
	return higherStates;
}


/**
 * @return The set with the states, which are higher or equal than the
 * 	border.
 * 	@see higherStates
 * @see filterStates()
 * @see typeSamplingValue
 * @see lBorderValue
 * @see pHigherBorder
 */
const set< cDataGloveState * > & cBorderDataGloveState::getHigherStates() const {
	
	return higherStates;
}


/**
 * Sets the higher or equal states for the border.
 *
 * @see filterStates()
 * @see typeSamplingValue
 * @see lBorderValue
 * @see pHigherBorder
 * @param setInHigherStates The set with the states, which are higher or
 * 	equal than the border.
 * 	@see higherStates
 */
void cBorderDataGloveState::setHigherStates(
		const set< cDataGloveState * > & setInHigherStates ) {
	
	higherStates = setInHigherStates;
}


/**
 * This method evalues if the given value of the given type is inside
 * the give border side.
 * A value can not be inside, if it is a border value @see lBorderValue .
 * Parents will also be checked.
 *
 * @see lBorderValue
 * @see pParent
 * @see filterStates()
 * @param typeValue the type of the value
 * @param lValue the value, for which should be checked, on which side it is
 * @param bIsLower the side on which the value should be
 * @return true if the value of the given type is inside the given border
 * 	side else false
 */
bool cBorderDataGloveState::isValueInsideSide( const nDataGlove::nModelDataGloveDGTechVHand::
		cMessageSamplingDataFromDataGlove::tTypeSamplingValue typeValue,
		const long lValue, const bool bIsLower ) const {
	
	if ( typeSamplingValue == typeValue ) {
		//value for this border type -> check the given value
		if ( bIsLower ) {
			if ( lBorderValue <= lValue ) {
				//the given value is not lower as this border
				return false;
			}
		} else {  // is higher?
			if ( lValue <= lBorderValue ) {
				//the given value is not higher as this border
				return false;
			}
		}
	}  //(else value not sorted by this border)
	if ( pParent != NULL ) {
		//check value for parent
		return pParent->isValueInsideSide( typeValue, lValue,
			( pParent->getLowerBorder() == this ) );
	}  //else value is inside given side of this border
	return true;
}















