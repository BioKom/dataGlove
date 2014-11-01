
//TODO check

/**
 * @file cBorderDataGloveState
 * file name: cBorderDataGloveState.h
 * @author Betti Oesterholz
 * @date 05.08.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies the class for a barder betwaen data glove states.
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
 * This file specifies the class for a barder betwaen data glove states.
 * All intervalls with values lower the border value are lower than the border.
 * All intervalls with values higher or equal the border value are higher
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


#ifndef ___N_DATA_GLOVE__N_MAPPER__C_BORDER_DATA_GLOVE_STATE__
#define ___N_DATA_GLOVE__N_MAPPER__C_BORDER_DATA_GLOVE_STATE__


#include "version.h"

#include <string>
#include <set>

#include "cMessageSamplingDataFromDataGlove.h"
#include "cDataGloveState.h"


namespace nDataGlove{
namespace nMapper{


//forward declarations
class cDataGloveState;


class cBorderDataGloveState{
public:

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
	cBorderDataGloveState( const nDataGlove::nModelDataGloveDGTechVHand::
			cMessageSamplingDataFromDataGlove::tTypeSamplingValue inTypeSamplingValue,
			const long lInBorderValue );
	
	/**
	 * @return the name of this class "cBorderDataGloveState"
	 */
	std::string getName() const;
	
	
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
	void filterStates( std::set< cDataGloveState* > setAllDataGloveStates );
	
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
	const std::set< cDataGloveState * > & evalueNextBorderStates( const
		nDataGlove::nModelDataGloveDGTechVHand::cMessageSamplingDataFromDataGlove *
			pMessageSamplingDataFromDataGlove );
	
	
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
		tTypeSamplingValue getTypeSamplingValue() const;

	
	/**
	 * @return The parent border of this border, which contains this border.
	 * 	@see pParent
	 */
	cBorderDataGloveState * getParent();
	
	/**
	 * @return The parent border of this border, which contains this border.
	 * 	@see pParent
	 */
	const cBorderDataGloveState * getParent() const;

	
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
	long getBorderValue() const;
	
	
	/**
	 * @return The next to check border, if the given value is lower than the
	 * 	lBorderValue.
	 * 	@see pLowerBorder
	 * @see typeSamplingValue
	 * @see lBorderValue
	 * @see lowerStates
	 */
	cBorderDataGloveState * getLowerBorder();
	
	/**
	 * @return The next to check border, if the given value is lower than the
	 * 	lBorderValue.
	 * 	@see pLowerBorder
	 * @see typeSamplingValue
	 * @see lBorderValue
	 * @see lowerStates
	 */
	const cBorderDataGloveState * getLowerBorder() const;
	
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
	void setLowerBorder( cBorderDataGloveState * pNewLowerBorder );
	
	
	/**
	 * @return The set with the states, which are lower than the border.
	 * 	@see lowerStates
	 * @see filterStates()
	 * @see typeSamplingValue
	 * @see lBorderValue
	 * @see pLowerBorder
	 */
	std::set< cDataGloveState * > & getLowerStates();
	
	/**
	 * @return The set with the states, which are lower than the border.
	 * 	@see lowerStates
	 * @see filterStates()
	 * @see typeSamplingValue
	 * @see lBorderValue
	 * @see pLowerBorder
	 */
	const std::set< cDataGloveState * > & getLowerStates() const;
	
	/**
	 * Sets the lower states for the border.
	 *
	 * @see filterStates()
	 * @see typeSamplingValue
	 * @see lBorderValue
	 * @see pLowerBorder
	 * @param setInLowerStates The set with the states, which are lower than
	 * 	the border.
	 * 	@see lowerStates
	 */
	void setLowerStates( const std::set< cDataGloveState * > & setInLowerStates );
	
	
	/**
	 * @return The next to check border, if the given value is higher or
	 * 	equal than the lBorderValue.
	 * 	@see pHigherBorder
	 * @see typeSamplingValue
	 * @see lBorderValue
	 * @see higherStates
	 */
	cBorderDataGloveState * getHigherBorder();
	
	/**
	 * @return The next to check border, if the given value is higher or
	 * 	equal than the lBorderValue.
	 * 	@see pHigherBorder
	 * @see typeSamplingValue
	 * @see lBorderValue
	 * @see higherStates
	 */
	const cBorderDataGloveState * getHigherBorder() const;
	
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
	void setHigherBorder( cBorderDataGloveState * pNewHigherBorder );
	
	
	/**
	 * @return The set with the states, which are higher or equal than the
	 * 	border.
	 * 	@see higherStates
	 * @see filterStates()
	 * @see typeSamplingValue
	 * @see lBorderValue
	 * @see pHigherBorder
	 */
	std::set< cDataGloveState * > & getHigherStates();
	
	/**
	 * @return The set with the states, which are higher or equal than the
	 * 	border.
	 * 	@see higherStates
	 * @see filterStates()
	 * @see typeSamplingValue
	 * @see lBorderValue
	 * @see pHigherBorder
	 */
	const std::set< cDataGloveState * > & getHigherStates() const;
	
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
	void setHigherStates( const std::set< cDataGloveState * > & setInHigherStates );
	
	
	
protected:
	
//members
	
	/**
	 * The parent border of this border, which contains this border.
	 */
	cBorderDataGloveState * pParent;
	
	/**
	 * The type of the data glove sampling value, for which the border is.
	 * @see lBorderValue
	 * @see lowerStates
	 * @see higherStates
	 * @see pLowerBorder
	 * @see pHigherBorder
	 */
	nDataGlove::nModelDataGloveDGTechVHand::cMessageSamplingDataFromDataGlove::
		tTypeSamplingValue typeSamplingValue;

	
	/**
	 * The value of the border.
	 * All data glove sampling values below inTypeSamplingValue are
	 * in the lower set of data glove states, all values above or equal
	 * are in the higher set of data glove states.
	 * @see typeSamplingValue
	 * @see lowerStates
	 * @see higherStates
	 * @see pLowerBorder
	 * @see pHigherBorder
	 */
	long lBorderValue;
	
	
	/**
	 * The next to check border, if the given value is lower than the
	 * lBorderValue.
	 * (Note: The border is not owned by this object. -> It won't be deleted
	 * 	by this object.)
	 * @see typeSamplingValue
	 * @see lBorderValue
	 * @see lowerStates
	 */
	cBorderDataGloveState * pLowerBorder;
	
	/**
	 * The set with the states, which are lower than the border.
	 * (Note: The states are not owned by this object. -> They won't be deleted
	 * 	by this object.)
	 * @see typeSamplingValue
	 * @see lBorderValue
	 * @see pLowerBorder
	 * @see getLowerStates()
	 * @see setLowerStates()
	 */
	std::set< cDataGloveState * > lowerStates;
	
	
	/**
	 * The next to check border, if the given value is higher or equal than
	 * the lBorderValue.
	 * (Note: The border is not owned by this object. -> It won't be deleted
	 * 	by this object.)
	 * @see typeSamplingValue
	 * @see lBorderValue
	 * @see higherStates
	 */
	cBorderDataGloveState * pHigherBorder;
	
	/**
	 * The set with the states, which are lower than the border.
	 * (Note: The states are not owned by this object. -> They won't be deleted
	 * 	by this object.)
	 * @see typeSamplingValue
	 * @see lBorderValue
	 * @see lowerStates
	 * @see getHigherStates()
	 * @see setHigherStates()
	 */
	std::set< cDataGloveState * > higherStates;
	
public:  //inline
	
	/**
	 * This method evaluates the border state for the given data glove
	 * sampling message.
	 * It implments the search for a state of a data glove message
	 * It will evalue the contained /next  borders, till the to use lower or
	 * higher (or equal) side of a border contains no border.
	 *
	 * @see typeSamplingValue
	 * @see lBorderValue
	 * @see lowerStates
	 * @see higherStates
	 * @see pLowerBorder
	 * @see pHigherBorder
	 * @param pMessageSamplingDataFromDataGlove the sampling message, for
	 * 	which to evaluate the state
	 * @return the state for the massage, if existing, or NULL if no state
	 * 	for the sampling message exists
	 */
	inline cDataGloveState * evalueBorderState( const
			nDataGlove::nModelDataGloveDGTechVHand::cMessageSamplingDataFromDataGlove *
				pMessageSamplingDataFromDataGlove ) {
		
		if ( pMessageSamplingDataFromDataGlove->getValue( typeSamplingValue )
				< lBorderValue ) {
			//the message state is lower then this border
			if ( pLowerBorder != NULL ) {
				return pLowerBorder->evalueBorderState(
					pMessageSamplingDataFromDataGlove );
			}  //no next lower border -> evalute the state
			//if more than one state in set -> take first in wich isIn
			for ( std::set< cDataGloveState * >::iterator
					itrState = lowerStates.begin();
					itrState != lowerStates.end(); ++itrState ) {
				
				if ( (*itrState)->isIn( pMessageSamplingDataFromDataGlove ) ) {
					//state for message found -> return it
					return (*itrState);
				}
			}//else no state for message found -> return NULL
			return NULL;
		}// else the message state is higher or equal then this border
		//the message state is lower then this border
		if ( pHigherBorder != NULL ) {
			return pHigherBorder->evalueBorderState(
				pMessageSamplingDataFromDataGlove );
		}  //no next lower border -> evalute the state
		
		//if more than one state in set -> take first in wich isIn
		for ( std::set< cDataGloveState * >::iterator
				itrState = higherStates.begin();
				itrState != higherStates.end(); ++itrState ) {
			
			if ( (*itrState)->isIn( pMessageSamplingDataFromDataGlove ) ) {
				//state for message found -> return it
				return (*itrState);
			}
		}//else no state for message found -> return NULL
		return NULL;
	}
	
	
	/**
	 * This method evaluates the next border (pLowerBorder or pHigherBorder)
	 * to check for the massge.
	 *
	 * @see pLowerBorder
	 * @see pHigherBorder
	 * @param pMessageSamplingDataFromDataGlove the sampling message, for
	 * 	which to evaluate the next border
	 * @return the next border for the massage, if existing, or NULL if no
	 * 	border for the sampling message exists
	 */
	inline cBorderDataGloveState * evalueNextBorder( const
			nDataGlove::nModelDataGloveDGTechVHand::cMessageSamplingDataFromDataGlove *
				pMessageSamplingDataFromDataGlove ) {
		
		if ( pMessageSamplingDataFromDataGlove->getValue( typeSamplingValue )
				< lBorderValue ) {
			//the message state is lower then this border
			return pLowerBorder;
		}  // else the message state is higher or equal then this border
		return pHigherBorder;
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
	bool isValueInsideSide( const nDataGlove::nModelDataGloveDGTechVHand::
		cMessageSamplingDataFromDataGlove::tTypeSamplingValue typeValue,
			const long lValue, const bool bIsLower ) const;
	
};//end class cBorderDataGloveState


};//end namespace nMapper
};//end namespace nDataGlove

#endif //___N_DATA_GLOVE__N_MAPPER__C_BORDER_DATA_GLOVE_STATE__





