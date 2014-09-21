
//TODO check

/**
 * @file cEvaluateDataGloveState
 * file name: cEvaluateDataGloveState.h
 * @author Betti Oesterholz
 * @date 10.08.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies the class for a evaluate a data glove state.
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
 * This file specifies the class for a evaluate a data glove state.
 * It will find a data glove state for a given data glove sampling message
 * ( cMessageSamplingDataFromDataGlove ) and a given modus.
 * The search should be fast.
 * It will generate a tree of data glove state borders, so it can fast find
 * the correct states.
 * The data glove states for the sampling data, will be read from a file.
 *
 * @see cMessageSamplingDataFromDataGlove
 * @see cDataGloveState
 * @see iCallFunction
 * @see cBorderDataGloveState
 * @see cEvaluateDataGloveState::evalueDataGloveState()
 */
/*
History:
10.08.2014  Oesterholz  created
*/

#ifndef ___N_DATA_GLOVE__N_MAPPER__C_EVALUATE_DATA_GLOVE_STATE__
#define ___N_DATA_GLOVE__N_MAPPER__C_EVALUATE_DATA_GLOVE_STATE__


#include "version.h"

#include <string>
#include <set>
#include <map>
#include <iostream>

#include "cBorderDataGloveState.h"


namespace nDataGlove{

namespace nModelDataGloveDGTechVHand{

//forward declarations
class cMessageSamplingDataFromDataGlove;

};  //end namespace nModelDataGloveDGTechVHand

namespace nMapper{


//forward declarations
class cDataGloveState;


class cEvaluateDataGloveState{
public:

	/**
	 * constructor
	 */
	cEvaluateDataGloveState();
	
	/**
	 * destructor
	 */
	~cEvaluateDataGloveState();
	
	/**
	 * @return the name of this class "cEvaluateDataGloveState"
	 */
	std::string getName() const;
	
	
	//also creats border tree
	/**
	 * This method reads the data glove states from the given stream into
	 * this object.
	 * The column's should be seperated by a semikolon ';' . Each rows
	 * should be written in a seperate line.
	 * The first line of the stream should be the headlines for the rows.
	 * Possible headlines are:
	 * 	* "MODUS": the modus of the state
	 * 		@see cDataGloveState::iModus
	 * 	* "function": the function which to call in the state
	 * 		@see iCallFunction
	 * 		@see cDataGloveState::pCallFunction
	 * 	* "parmeter": the parameter for the call to the function
	 * 		@see iCallFunction
	 * 		@see cDataGloveState::pCallFunction
	 * 	* "repeat delay":  All this milli seconds the call function is called.
	 * 		If 0 the call function will be repeated once.
	 * 		@see cDataGloveState::iRepeatAllMilliSeconds
	 * 	* The sampling data types tTypeSamplingValue followed by "MIN",
	 * 	  "MAX" or "TARGET". The tTypeSamplingValue can be for example
	 * 	  "FINGER_1", "QUATERNION_3" or "ACCELEROMETER_X" .
	 * 	  The given values in the row, will indicate the interval for
	 * 	  data glove values of the type. If no value is given, no interval
	 * 	  for the state for this type of data glove sampling value will be
	 * 	  created.
	 * 	** tTypeSamplingValue + " MIN": The minimum value the sampling
	 * 	   value in the state should have.
	 * 	** tTypeSamplingValue + " MAX": The maximum value the sampling
	 * 	   value in the state should have.
	 * 	** tTypeSamplingValue + " TARGET": The target value for the
	 * 	   sampling value in the state.
	 * 	* "calls": How often the state function was called.
	 * 
	 * Example: "
	 * 	MODUS;function;parmeter;repeat delay;;FINGER_1 MIN;FINGER_1 MAX;FINGER_1 TARGET;;FINGER_2 MIN;FINGER_2 MAX;FINGER_2 TARGET;;FINGER_3 MIN;FINGER_3 MAX;FINGER_3 TARGET;;FINGER_4 MIN;FINGER_4 MAX;FINGER_4 TARGET;;FINGER_5 MIN;FINGER_5 MAX;FINGER_5 TARGET;;QUATERNION_1 MIN;QUATERNION_1 MAX;QUATERNION_1 TARGET;;QUATERNION_2 MIN;QUATERNION_2 MAX;QUATERNION_2 TARGET;;QUATERNION_3 MIN;QUATERNION_3 MAX;QUATERNION_3 TARGET;;QUATERNION_4 MIN;QUATERNION_4 MAX;QUATERNION_4 TARGET;calls
	 * 	1;prepareKey;e;0;;50;250;200;;250;600;450;;600;800;700;;750;900;850;;700;800;750;;;;;;10000;20000;15000;;;;;;;;;
	 * 	"
	 *
	 * @param streamDataGloveStates the stream from which to read the data
	 * 	glove states
	 * @return true if the data glove states where read, else false
	 */
	bool loadDataGloveStates( std::istream & streamDataGloveStates );
	
	/**
	 * This method reads the data glove states from the given stream into
	 * this object.
	 * The column's should be seperated by a semikolon ';' . Each rows
	 * should be written in a seperate line.
	 * The first line of the stream should be the headlines for the rows.
	 * Possible headlines are:
	 * 	* "MODUS": the modus of the state
	 * 		@see cDataGloveState::iModus
	 * 	* "function": the function which to call in the state
	 * 		@see iCallFunction
	 * 		@see cDataGloveState::pCallFunction
	 * 	* "parmeter": the parameter for the call to the function
	 * 		@see iCallFunction
	 * 		@see cDataGloveState::pCallFunction
	 * 	* "repeat delay":  All this milli seconds the call function is called.
	 * 		If 0 the call function will be repeated once.
	 * 		@see cDataGloveState::iRepeatAllMilliSeconds
	 * 	* The sampling data types tTypeSamplingValue followed by "MIN",
	 * 	  "MAX" or "TARGET". The tTypeSamplingValue can be for example
	 * 	  "FINGER_1", "QUATERNION_3" or "ACCELEROMETER_X" .
	 * 	  The given values in the row, will indicate the interval for
	 * 	  data glove values of the type. If no value is given, no interval
	 * 	  for the state for this type of data glove sampling value will be
	 * 	  created.
	 * 	** tTypeSamplingValue + " MIN": The minimum value the sampling
	 * 	   value in the state should have.
	 * 	** tTypeSamplingValue + " MAX": The maximum value the sampling
	 * 	   value in the state should have.
	 * 	** tTypeSamplingValue + " TARGET": The target value for the
	 * 	   sampling value in the state.
	 * 	* "calls": How often the state function was called.
	 * 
	 * Example: "
	 * 	MODUS;function;parmeter;repeat delay;;FINGER_1 MIN;FINGER_1 MAX;FINGER_1 TARGET;;FINGER_2 MIN;FINGER_2 MAX;FINGER_2 TARGET;;FINGER_3 MIN;FINGER_3 MAX;FINGER_3 TARGET;;FINGER_4 MIN;FINGER_4 MAX;FINGER_4 TARGET;;FINGER_5 MIN;FINGER_5 MAX;FINGER_5 TARGET;;QUATERNION_1 MIN;QUATERNION_1 MAX;QUATERNION_1 TARGET;;QUATERNION_2 MIN;QUATERNION_2 MAX;QUATERNION_2 TARGET;;QUATERNION_3 MIN;QUATERNION_3 MAX;QUATERNION_3 TARGET;;QUATERNION_4 MIN;QUATERNION_4 MAX;QUATERNION_4 TARGET;calls
	 * 	1;prepareKey;e;0;;50;250;200;;250;600;450;;600;800;700;;750;900;850;;700;800;750;;;;;;10000;20000;15000;;;;;;;;;
	 * 	"
	 *
	 * @param streamDataGloveStates the stream from which to read the data
	 * 	glove states
	 * @return true if the data glove states where read, else false
	 */
	bool loadDataGloveStates( const std::string & szPathDataGloveStates );
	
/*TODO later
	bool storeDataGloveStates( std::ostream & streamDataGloveStates );
	
	
	
	bool storeDataGloveStates( const std::string & szPathDataGloveStates );
	
*/
	
	
protected:
	
	/**
	 * This function creates a good border for the given list of data glove
	 * states.
	 * It is a helper function of createDataGloveStateBorderTrees() .
	 * Note: No states should have 0 calls.
	 *
	 * @see createDataGloveStateBorderTrees()
	 * @param setDataGloveStates the data glove states, for which a good border
	 * 	should be created
	 * @return a pointer to the the created border (please care that deleted),
	 * 	or NULL, if non could be created
	 */
	cBorderDataGloveState * createGoodBorder(
			std::set< cDataGloveState * > & setDataGloveStates );
	
	/**
	 * This method creats for each modus a tree of borders to fast find a
	 * data glove state.
	 * It uses the setAllStates to create data glove state borders and fill:
	 * 	@see mapStartBordersForModus
	 * 	@see setAllBorders
	 * It will also set the actual modus iActualModus to the lowest modus
	 * for the data glove states.
	 *
	 * @see setAllStates
	 * @see mapStartBordersForModus
	 * @see setAllBorders
	 * @return true if the data glove search trees could be generated, else false.
	 */
	bool createDataGloveStateBorderTrees();
	
	/**
	 * Returns the call function with the given parameters.
	 * Creates the call function.
	 *
	 * @see loadDataGloveStates()
	 * @see iCallFunction
	 * @param szFunction the name of the function for which to return the
	 * 	call function
	 * @param szFunctionParameter the parameter for the call function to return
	 * @return a pointer to the call function object (pleas delete), or
	 * 	NULL if non could be created
	 */
	iCallFunction * getCallFunction( const std::string & szFunction,
		const std::string & szFunctionParameter );
	
//members
	
	/**
	 * The actual modus of the data glove mapper.
	 * @see getActualModus()
	 * @see setActualModus()
	 * @see mapStartBordersForModus
	 * @see pActualModusStartBorder
	 */
	int iActualModus;
	
	/**
	 * A pointer to the actual modus start border, to start the search for
	 * the sampling data data glove state.
	 * @see mapStartBordersForModus
	 * @see iActualModus
	 */
	cBorderDataGloveState * pActualModusStartBorder;
	
	/**
	 * A map for the modi and their start borders, to start the search for
	 * the sampling data data glove state.
	 * Elements:
	 * 	key: the number of the modus
	 * 	value: a pointer to the start border for the key modus
	 * @see pActualModusStartBorder
	 * @see iActualModus
	 */
	std::map< int, cBorderDataGloveState * > mapStartBordersForModus;
	
	/**
	 * A set with all data glove states.
	 */
	std::set< cDataGloveState * > setAllStates;
	
	/**
	 * A set with all borders, to search for the data glove states.
	 */
	std::set< cBorderDataGloveState * > setAllBorders;
	
public:
	
	/**
	 * This function evaluates the state for the given data glove message,
	 * in the actual modus.
	 * It will search for the state in the actual modus, for the given
	 * sampling message.
	 * load the data glove states ( loadDataGloveStates() ) before using
	 * this function.
	 *
	 * @see iActualModus
	 * @see pActualModusStartBorder
	 * @see loadDataGloveStates()
	 * @param pMessageSamplingDataFromDataGlove the data glove sampling
	 * 	message, for which to evaluate the data glove state
	 * @return the data glove state for the given sampling message, or
	 * 	NULL if non could be found
	 */
	inline cDataGloveState * evalueDataGloveState( const nModelDataGloveDGTechVHand::
		cMessageSamplingDataFromDataGlove * pMessageSamplingDataFromDataGlove ) {
		
		if ( pActualModusStartBorder == NULL ) {
			//no start border for modus -> no state for the modus and message
			return NULL;
		}  //else use the start border to evaluate the data glove state
		return pActualModusStartBorder->evalueBorderState(
			pMessageSamplingDataFromDataGlove );
	}
	
	/**
	 * @see setActualModus()
	 * @return the actual data glove mapping modus
	 * 	@see iActualModus
	 */
	inline int getActualModus() const {
		
		return iActualModus;
	}
	
	
	/**
	 * This method sets the actual data glove mapping modus.
	 *
	 * @see iActualModus
	 * @see pActualModusStartBorder
	 * @see getActualModus()
	 * @param iNewModus the to set data glove mapping modus
	 * @return true if the new modus is set, else false
	 */
	inline bool setActualModus( const int iNewModus ) {
		
		if ( iNewModus == iActualModus ) {
			
			return true;
		}
		std::map< int, cBorderDataGloveState * >::iterator
			itrNewStartBorder = mapStartBordersForModus.find( iNewModus );
		if ( itrNewStartBorder == mapStartBordersForModus.end() ) {
			return false;  //no such modus
		}  //else set new modus and start border
		pActualModusStartBorder = itrNewStartBorder->second;
		iActualModus = iNewModus;
		return true;
	}
	
	
};//end class cEvaluateDataGloveState


};//end namespace nMapper
};//end namespace nDataGlove

#endif //___N_DATA_GLOVE__N_MAPPER__C_EVALUATE_DATA_GLOVE_STATE__





