
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
 * This file implements the class for a evaluate a data glove state.
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
 * This file implements the class for a evaluate a data glove state.
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

//for debugging
#define DEBUG_CALL_FUNCTION
///just simulate the call to the functions
#define SIMULATE_CALL_FUNCTION


#include "cEvaluateDataGloveState.h"

#include <string>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <fstream>

#include "cBorderDataGloveState.h"
#include "cDataGloveState.h"

#include "cCallSimulation.h"
#include "cCallPrepareChangeModus.h"
#include "cCallChangeModus.h"

#ifdef FEATURE_READ_DATA_TEXT_WITH_REGEX
	#include <regex>
#endif //FEATURE_READ_DATA_TEXT_WITH_REGEX



using namespace nDataGlove::nMapper;
using namespace nDataGlove::nModelDataGloveDGTechVHand;
using namespace std;



/**
 * constructor
 */
cEvaluateDataGloveState::cEvaluateDataGloveState() :
		iActualModus( 0 ), pActualModusStartBorder( NULL ) {
	//nothing to do
}


/**
 * destructor
 */
cEvaluateDataGloveState::~cEvaluateDataGloveState() {
	//delete all states
	for ( std::set< cDataGloveState * >::iterator
			itrState = setAllStates.begin();
			itrState != setAllStates.end(); ++itrState ) {
		
		delete (*itrState);
	}
	//delete all borders
	for ( std::set< cBorderDataGloveState * >::iterator
			itrBorder = setAllBorders.begin();
			itrBorder != setAllBorders.end(); ++itrBorder ) {
		
		delete (*itrBorder);
	}
}


/**
 * @return the name of this class "cEvaluateDataGloveState"
 */
std::string cEvaluateDataGloveState::getName() const {
	
	return std::string("cEvaluateDataGloveState");
}


/**
 * This function reads a part from the stream till the next semikolon ';'
 * or new line.
 *
 * @param streamDataGloveStates the stream from which to read the string
 * @return a pair with:
 * 		first: the readed string without the semikolon ';'
 * 		second: true if a new line as read, else false
 */
pair< string, bool > readEntry( istream & streamDataGloveStates ) {
	
	char c;
	string strReadedString = "";
	while ( streamDataGloveStates.good() ) {
		streamDataGloveStates.get( c );
		if ( c == ';' ) {
			break;
		}
		if ( c == '\n' ) {
			//new line read
			return pair< string, bool >( strReadedString, true );
		}
		strReadedString.push_back( c );
	};
	
	return pair< string, bool >( strReadedString, false );
}



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
bool cEvaluateDataGloveState::loadDataGloveStates(
		istream & streamDataGloveStates ) {
	
	//read the table head
	//types not for sampling data
	enum tTypeTableHeadExtra{
		EMPTY,  //empty column
		MODUS,
		FUNCTION,
		PARAMETER,
		REPEAT_DELAY,
		CALLS
	};  //end tTypeTableHeadExtra
	//the kind of sampling data column
	enum tTypeSamplingDataKind {
		MINIMUM,
		MAXIMUM,
		TARGET
	};  //end tTypeSamplingDataKind
	//type of the column
	struct tTableHead {
		///standard constructor
		tTableHead() : bSamplingType( false ), tableHead( EMPTY ) {
			//nothing to do
		};
		///copy constructor
		tTableHead( const tTableHead & inTableHead ) :
				bSamplingType( inTableHead.bSamplingType ) {
				
			if ( bSamplingType ) {
				typeSamplingData = inTableHead.typeSamplingData;
			} else {
				tableHead = inTableHead.tableHead;
			}
		};
		///assignment operator
		tTableHead & operator=( const tTableHead & inTableHead ) {
			
			bSamplingType = inTableHead.bSamplingType;
			if ( bSamplingType ) {
				typeSamplingData = inTableHead.typeSamplingData;
			} else {
				tableHead = inTableHead.tableHead;
			}
			return *this;
		};
		
		bool bSamplingType;
		union {
			//if bSamplingType == false
			tTypeTableHeadExtra tableHead;
			//if bSamplingType == true
			pair< cMessageSamplingDataFromDataGlove::tTypeSamplingValue,
				tTypeSamplingDataKind > typeSamplingData;
		};
	};
	//list with the column types
	list< tTableHead > liTableColumnType;
	
	//create mapping from string / regular expression to column type
	map< string, tTableHead > mapTableColumnTypeExpr;
	
	tTableHead typeOfColum;
#ifdef FEATURE_READ_DATA_TEXT_WITH_REGEX
	typeOfColum.bSamplingType = false;
	
	typeOfColum.tableHead = tTypeTableHeadExtra::MODUS;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*MOD.*", typeOfColum ) );
	typeOfColum.tableHead = tTypeTableHeadExtra::FUNCTION;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*fun.*", typeOfColum ) );
	typeOfColum.tableHead = tTypeTableHeadExtra::PARAMETER;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*par.*", typeOfColum ) );
	typeOfColum.tableHead = tTypeTableHeadExtra::REPEAT_DELAY;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*repeat.*", typeOfColum ) );
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*wied.*", typeOfColum ) );
	typeOfColum.tableHead = tTypeTableHeadExtra::CALLS;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*calls.*", typeOfColum ) );
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*auf.*", typeOfColum ) );
	
	typeOfColum.bSamplingType = true;
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::FINGER_1;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*FIN.*1 .*MIN.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*FIN.*1 .*MAX.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*FIN.*1 .*TARG.*", typeOfColum ) );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::FINGER_2;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*FIN.*2 .*MIN.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*FIN.*2 .*MAX.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*FIN.*2 .*TAR.*", typeOfColum ) );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::FINGER_3;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*FIN.*3 .*MIN.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*FIN.*3 .*MAX.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*FIN.*3 .*TAR.*", typeOfColum ) );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::FINGER_4;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*FIN.*4 .*MIN.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*FIN.*4 .*MAX.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*FIN.*4 .*TAR.*", typeOfColum ) );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::FINGER_5;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*FIN.*5 .*MIN.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*FIN.*5 .*MAX.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*FIN.*5 .*TAR.*", typeOfColum ) );
	
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::QUATERNION_1;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*QUA.*1 .*MIN.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*QUA.*1 .*MAX.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*QUA.*1 .*TAR.*", typeOfColum ) );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::QUATERNION_2;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*QUA.*2 .*MIN.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*QUA.*2 .*MAX.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*QUA.*2 .*TAR.*", typeOfColum ) );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::QUATERNION_3;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*QUA.*3 .*MIN.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*QUA.*3 .*MAX.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*QUA.*3 .*TAR.*", typeOfColum ) );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::QUATERNION_4;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*QUA.*4 .*MIN.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*QUA.*4 .*MAX.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*QUA.*4 .*TAR.*", typeOfColum ) );
	
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::GYROSCOPE_X;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*GYR.*X .*MIN.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*GYR.*X .*MAX.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*GYR.*X .*TAR.*", typeOfColum ) );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::GYROSCOPE_Y;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*GYR.*Y .*MIN.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*GYR.*Y .*MAX.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*GYR.*Y .*TAR.*", typeOfColum ) );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::GYROSCOPE_Z;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*GYR.*Z .*MIN.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*GYR.*Z .*MAX.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*GYR.*Z .*TAR.*", typeOfColum ) );
	
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::MAGNETOMETER_X;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*MAG.*X .*MIN.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*MAG.*X .*MAX.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*MAG.*X .*TAR.*", typeOfColum ) );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::MAGNETOMETER_Y;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*MAG.*Y .*MIN.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*MAG.*Y .*MAX.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*MAG.*Y .*TAR.*", typeOfColum ) );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::MAGNETOMETER_Z;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*MAG.*Z .*MIN.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*MAG.*Z .*MAX.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*MAG.*Z .*TAR.*", typeOfColum ) );
	
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::ACCELEROMETER_X;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*AC.*X .*MIN.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*AC.*X .*MAX.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*AC.*X .*TAR.*", typeOfColum ) );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::ACCELEROMETER_Y;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*AC.*Y .*MIN.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*AC.*Y .*MAX.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*AC.*Y .*TAR.*", typeOfColum ) );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::ACCELEROMETER_Z;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*AC.*Z .*MIN.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*AC.*Z .*MAX.*", typeOfColum ) );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		".*AC.*Z .*TAR.*", typeOfColum ) );
	
#else  //FEATURE_READ_DATA_TEXT_WITH_REGEX
	typeOfColum.bSamplingType = false;
	
	typeOfColum.tableHead = tTypeTableHeadExtra::MODUS;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"MODUS", typeOfColum ) );
	typeOfColum.tableHead = tTypeTableHeadExtra::FUNCTION;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"function", typeOfColum ) );
	typeOfColum.tableHead = tTypeTableHeadExtra::PARAMETER;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"parmeter", typeOfColum ) );
	typeOfColum.tableHead = tTypeTableHeadExtra::REPEAT_DELAY;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"repeat delay", typeOfColum ) );
	typeOfColum.tableHead = tTypeTableHeadExtra::CALLS;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"calls", typeOfColum ) );
	
	typeOfColum.bSamplingType = true;
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::FINGER_1;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"FINGER_1 MIN", typeOfColum ) );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"FINGER_1 MAX", typeOfColum ) );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"FINGER_1 TARGET", typeOfColum ) );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::FINGER_2;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"FINGER_2 MIN", typeOfColum ) );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"FINGER_2 MAX", typeOfColum ) );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"FINGER_2 TARGET", typeOfColum ) );
	
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::FINGER_3;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"FINGER_3 MIN", typeOfColum ) );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"FINGER_3 MAX", typeOfColum ) );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"FINGER_3 TARGET", typeOfColum ) );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::FINGER_4;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"FINGER_4 MIN", typeOfColum ) );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"FINGER_4 MAX", typeOfColum ) );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"FINGER_4 TARGET", typeOfColum ) );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::FINGER_5;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"FINGER_5 MIN", typeOfColum ) );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"FINGER_5 MAX", typeOfColum ) );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"FINGER_5 TARGET", typeOfColum ) );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::QUATERNION_1;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"QUATERNION_1 MIN", typeOfColum ) );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"QUATERNION_1 MAX", typeOfColum ) );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"QUATERNION_1 TARGET", typeOfColum ) );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::QUATERNION_2;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"QUATERNION_2 MIN", typeOfColum ) );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"QUATERNION_2 MAX", typeOfColum ) );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"QUATERNION_2 TARGET", typeOfColum ) );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::QUATERNION_3;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"QUATERNION_3 MIN", typeOfColum ) );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"QUATERNION_3 MAX", typeOfColum ) );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"QUATERNION_3 TARGET", typeOfColum ) );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::QUATERNION_4;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"QUATERNION_4 MIN", typeOfColum ) );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"QUATERNION_4 MAX", typeOfColum ) );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.insert( pair< string, tTableHead >(
		"QUATERNION_4 TARGET", typeOfColum ) );
#endif  //FEATURE_READ_DATA_TEXT_WITH_REGEX
	
	pair< string, bool > readedEntry;
	while ( streamDataGloveStates.good() ) {
		
		readedEntry = readEntry( streamDataGloveStates );
		
		const string readedString = readedEntry.first;
		
		typeOfColum.bSamplingType = false;
		typeOfColum.tableHead = tTypeTableHeadExtra::EMPTY;
		for ( map< string, tTableHead >::const_iterator
				itrMapColumnType = mapTableColumnTypeExpr.begin();
				itrMapColumnType != mapTableColumnTypeExpr.end();
				++itrMapColumnType ) {
			
#ifdef FEATURE_READ_DATA_TEXT_WITH_REGEX
			if ( regex_match( readedString, regex( itrMapColumnType->first,
					regex_constants::icase | regex_constants::ECMAScript ) ) ) {
				//matchin string found
				typeOfColum = itrMapColumnType->second;
				break;
			}
#else  //FEATURE_READ_DATA_TEXT_WITH_REGEX
			if ( itrMapColumnType->first.compare( readedString ) == 0 ) {
				//matchin string found
				typeOfColum = itrMapColumnType->second;
				break;
			}
#endif  //FEATURE_READ_DATA_TEXT_WITH_REGEX
		}
		liTableColumnType.push_back( typeOfColum );
		
		if ( readedEntry.second && ( ! liTableColumnType.empty() ) ) {
			//new line read -> no more headlines
			bool bAllColumnEmpty = true;
			for ( list< tTableHead >::const_iterator
					itrColumnType = liTableColumnType.begin();
					itrColumnType != liTableColumnType.end(); ++itrColumnType ) {
				
				if ( itrColumnType->bSamplingType ||
						( itrColumnType->tableHead != EMPTY ) ) {
					//this line is not empty
					bAllColumnEmpty = false;
					break;
				}
			}
			if ( ! bAllColumnEmpty ) {
				//table head read -> read data
				break;
			}  //else try to read line with column types
			liTableColumnType.clear();
		}
	}
	if ( liTableColumnType.empty() ) {
		//no data to read
		return false;
	}
	//read remaining lines
	list< tTableHead >::const_iterator itrColumnType = liTableColumnType.begin();
	cDataGloveState * pActualDataGloveState = new cDataGloveState();
	string szFunction = "";
	string szFunctionParameter = "";
	bool bFunctionParameterRead = false;
	cInterval * pInterval;
	while ( streamDataGloveStates.good() ) {
		
		readedEntry = readEntry( streamDataGloveStates );
		
		const string readedString = readedEntry.first;
		
		//fill / adapt state
		if ( itrColumnType != liTableColumnType.end() ) {
			if ( ( ! readedString.empty() ) ||
					( itrColumnType->tableHead == PARAMETER ) ) {
				//just parameters for the function can be empty
				
				if ( itrColumnType->bSamplingType ) {
					//adapt / set the intervall for the readed value
					pInterval = pActualDataGloveState->getIntervalForType(
							itrColumnType->typeSamplingData.first );
					
					if ( pInterval == NULL ) {
						pInterval = new cInterval();
						pActualDataGloveState->setInterval(
							itrColumnType->typeSamplingData.first, pInterval );
					}
					//set the new interval value
					switch ( itrColumnType->typeSamplingData.second ) {
						case MINIMUM : {
							pInterval->setMinimum( atol( readedString.c_str() ) );
						}; break;
						case MAXIMUM : {
							pInterval->setMaximum( atol( readedString.c_str() ) );
						}; break;
						case TARGET : {
							pInterval->setTarget( atol( readedString.c_str() ) );
						}; break;
					}; //end
					
				} else {  //not a sampling type
					switch ( itrColumnType->tableHead ) {
						case EMPTY : {
							//nothing to do
						}; break;
						case MODUS : {
							pActualDataGloveState->setModus(
								atoi( readedString.c_str() ) );
						}; break;
						case FUNCTION : {
							szFunction = readedString;
						}; break;
						case PARAMETER : {
							szFunctionParameter = readedString;
							bFunctionParameterRead = true;
						}; break;
						case REPEAT_DELAY : {
							pActualDataGloveState->setRepeatAllMilliSeconds(
								atol( readedString.c_str() ) );
						}; break;
						case CALLS : {
							pActualDataGloveState->setCalls( atol( readedString.c_str() ) );
						}; break;
					};  //switch itrColumnType->tableHead
					if ( ( ! szFunction.empty() ) && bFunctionParameterRead ) {
						//set the call function
						pActualDataGloveState->setCallFunction(
							getCallFunction( szFunction, szFunctionParameter ) );
						szFunction = "";
						szFunctionParameter = "";
						bFunctionParameterRead = false;
					}
				}
			}
			//next column
			itrColumnType++;
		}
		if ( readedEntry.second ) {
			//new line read -> start columns with first column
			itrColumnType = liTableColumnType.begin();
			if ( pActualDataGloveState->isValid() ) {
				setAllStates.insert( pActualDataGloveState );
			} else {  //not a valid state -> delete it
				delete pActualDataGloveState;
			}
			//create new state
			pActualDataGloveState = new cDataGloveState();
			szFunction = "";
			szFunctionParameter = "";
		}
	}
	if ( pActualDataGloveState->isValid() ) {
		setAllStates.insert( pActualDataGloveState );
	} else {  //not a valid state -> delete it
		delete pActualDataGloveState;
	}
	
	return createDataGloveStateBorderTrees();
}


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
bool cEvaluateDataGloveState::loadDataGloveStates(
		const string & szPathDataGloveStates ) {
	
	ifstream streamDataGloveStates( szPathDataGloveStates.c_str() );
	
	return loadDataGloveStates( streamDataGloveStates );
}


/**
 * Compare function for sorting a list of state pointers.
 * Compares two states for bigger number of calls.
 *
 * @see cDataGloveState::getCalls()
 * @param firstState a pointer to the first data glove state to compare
 * @param secondState a pointer to the second data glove state to compare
 * @return true if the first state firstState has more calls than the
 * 	second, else false
 */
bool compareBiggestCalls(const  cDataGloveState * firstState,
	const  cDataGloveState * secondState ) {
	
	return ( secondState->getCalls() < firstState->getCalls() );
}


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
cBorderDataGloveState * cEvaluateDataGloveState::createGoodBorder(
		set< cDataGloveState * > & setDataGloveStates ) {
	
	typedef cMessageSamplingDataFromDataGlove::tTypeSamplingValue
		typeDataGloveSamplingData;
	if ( setDataGloveStates.size() < 2 ) {
		
		if ( setDataGloveStates.size() == 1 ) {
			//create borde with/for one state
			set< cDataGloveState * >::const_iterator
				itrState = setDataGloveStates.begin();
			
			const std::map< typeDataGloveSamplingData, cInterval * > &
				mapActualIntervalMap = (*itrState)->getMapForInterval();
			
			if ( mapActualIntervalMap.empty() ) {
				//no valid sampling type intervals
				return NULL;
			}
			
			std::map< typeDataGloveSamplingData, cInterval * >::const_iterator
				itrSamplingInterval = mapActualIntervalMap.begin();
				
			for ( ; itrSamplingInterval != mapActualIntervalMap.end();
					++itrSamplingInterval ) {
				if ( itrSamplingInterval->second != NULL ) {
					//interval found
					break;
				}
			}
			if ( ( itrSamplingInterval == mapActualIntervalMap.end() ) ||
					( itrSamplingInterval->second == NULL ) ) {
				//Error: state has no interval -> can't use it for border
				return NULL;
			}
			
			cBorderDataGloveState * pNewBorder =
				new cBorderDataGloveState( itrSamplingInterval->first,
					itrSamplingInterval->second->getMinimum() );
			
			pNewBorder->filterStates( setDataGloveStates );
			
			return pNewBorder;
		}  // setDataGloveStates.size() == 0
		//not enough states -> no border to create
	}
	//evalue all interval (sampling data) types
	//a list with all states sorted for the number of calls (biggest first)
	list< cDataGloveState * > liCallsSortedList;
	set< typeDataGloveSamplingData > setPossibleSamplingTypes;
	for ( set< cDataGloveState * >::const_iterator
			itrState = setDataGloveStates.begin();
			itrState != setDataGloveStates.end(); ++itrState ) {
		
		const std::map< typeDataGloveSamplingData, cInterval * > &
			mapActualIntervalMap = (*itrState)->getMapForInterval();
		
		for ( std::map< typeDataGloveSamplingData, cInterval * >::const_iterator
				itrSamplingInterval = mapActualIntervalMap.begin();
				itrSamplingInterval != mapActualIntervalMap.end();
				++itrSamplingInterval ) {
			setPossibleSamplingTypes.insert( itrSamplingInterval->first );
		}
		liCallsSortedList.push_back( *itrState );
	}
	
	//sort states for number of calls (biggest first)
	liCallsSortedList.sort( compareBiggestCalls );
	
	unsigned long ulPossibleCallsLower;
	unsigned long ulPossibleCallsHigher;
	//values of the best border to seperate the states
	unsigned long ulBestCallsSumEachSide = 0;
	unsigned long ulBestCallsLower  = 0;
	unsigned long ulBestCallsHigher = 0;
	pair< typeDataGloveSamplingData, long > paBestBorderValue(
		typeDataGloveSamplingData::UNKNOWN, 0 );
	
	//for each sampling type
	cInterval * pInterval;
	list< long > liPossibleBorderValues;
	unsigned long ulMaxNumberOfCalls = 0;
	unsigned long ulCalls;
	bool bIsLower, bIsHigher;
	
	list< cDataGloveState * > liStatesWithInterval;
	for ( set< typeDataGloveSamplingData >::const_iterator
			itrSamplingType = setPossibleSamplingTypes.begin();
			itrSamplingType != setPossibleSamplingTypes.end();
			++itrSamplingType ) {
		//get all possible border values (possible values are the minimum and
		//maximum values of the intervals for the sampling type)
		liPossibleBorderValues.clear();
		liStatesWithInterval.clear();
		ulMaxNumberOfCalls = 0;
		for ( list< cDataGloveState * >::const_iterator
				itrState = liCallsSortedList.begin();
				itrState != liCallsSortedList.end(); ++itrState ) {
			
			pInterval = (*itrState)->getIntervalForType( *itrSamplingType );
			if ( pInterval == NULL ) {
				//state has no such interval -> can't use it for border
				continue;
			}
			//the passible value should seperate the state
			//if the border has the interval minimum value the state is higher
			//than the border
			liPossibleBorderValues.push_back(
				pInterval->getMinimum() );
			//the border has the has the interval maximum plus 1 value the
			//state is lower than the border
			liPossibleBorderValues.push_back(
				pInterval->getMaximum() + 1 );
		
			liStatesWithInterval.push_back( *itrState );
			ulMaxNumberOfCalls += (*itrState)->getCalls();
		}  //end search for possible border values
		if ( ( ulMaxNumberOfCalls <= ulBestCallsSumEachSide ) &&
				( ulMaxNumberOfCalls <= ulBestCallsLower ) &&
				( ulMaxNumberOfCalls <= ulBestCallsHigher ) ) {
			//can't improve border with this sampling type
			continue;  //skip sampling type
		}
		
		//Search for value, which has maximum number of calls sum and most
		//similar sum of calls on both sides, for states just one one side of
		//the border.
		for ( list< long >::const_iterator
				itrPossibleValue = liPossibleBorderValues.begin();
				itrPossibleValue != liPossibleBorderValues.end();
				++itrPossibleValue ) {
			ulPossibleCallsLower  = ulMaxNumberOfCalls;
			ulPossibleCallsHigher = ulMaxNumberOfCalls;
			for ( list< cDataGloveState * >::const_iterator
					itrState = liStatesWithInterval.begin();
					itrState != liStatesWithInterval.end(); ++itrState ) {
				
				pInterval = (*itrState)->getIntervalForType( *itrSamplingType );
				//check if state interval lower or/and higher border value
				bIsLower  = ( pInterval->getMinimum() < (*itrPossibleValue) );
				bIsHigher = ( (*itrPossibleValue) <= pInterval->getMaximum() );
				
				ulCalls = (*itrState)->getCalls();
				if ( bIsHigher ) {
					//not on lower border or on both sides
					ulPossibleCallsLower -= ulCalls;
				}
				if ( bIsLower ) {
					//not on higher border or on both sides
					ulPossibleCallsHigher -= ulCalls;
				}
				if ( ( ulPossibleCallsLower <= ulBestCallsLower ) &&
						( ulPossibleCallsHigher <= ulBestCallsHigher ) ) {
					//can't improve border with this sampling type and value
					break;  //skip sampling type and value
				}
			}  //end for all states with a interval for the sampling type
			
			if ( 0 < ( ulPossibleCallsLower + ulPossibleCallsHigher -
						ulBestCallsSumEachSide ) +
					//the factor 256 is choosen, because it is much better to
					//classify states than just having the border work on them
					256 * ( abs( ulPossibleCallsLower - ulPossibleCallsHigher ) -
						abs( ulBestCallsLower - ulBestCallsHigher ) ) ) {
				//better than actual best border -> new best border
				paBestBorderValue.first  = (*itrSamplingType);
				paBestBorderValue.second = (*itrPossibleValue);
				ulBestCallsSumEachSide = ulPossibleCallsLower + ulPossibleCallsHigher;
			}
		};  //end for all possible values
	};  //end for each sampling type
	
	//Create the border with the best maximum number of calls sum and most
	//similar sum of calls on both sides, for states just one one side of
	//the border, of all sampling types.
	if ( paBestBorderValue.first == typeDataGloveSamplingData::UNKNOWN ) {
		//no valid border found
		return NULL;
	}
	cBorderDataGloveState * pNewBorder = new cBorderDataGloveState(
		paBestBorderValue.first, paBestBorderValue.second );
	
	pNewBorder->filterStates( setDataGloveStates );
	if ( pNewBorder->getLowerStates() == pNewBorder->getHigherStates() ) {
		//border did not seperate the states -> no valid border
		delete pNewBorder;
		pNewBorder = NULL;
	}
	
	return pNewBorder;
}



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
bool cEvaluateDataGloveState::createDataGloveStateBorderTrees() {
	
	//delete all old borders
	for ( std::set< cBorderDataGloveState * >::iterator
			itrBorder = setAllBorders.begin();
			itrBorder != setAllBorders.end(); ++itrBorder ) {
		
		delete (*itrBorder);
	}
	setAllBorders.clear();
	
	if ( setAllStates.empty() ) {
		//no data glove states -> no borders
		return true;
	}
	//sort into lists for Modus
	map< int, set< cDataGloveState * > > mapStatesForModi;
	
	for ( std::set< cDataGloveState * >::iterator
			itrState = setAllStates.begin();
			itrState != setAllStates.end(); ++itrState ) {
		
		mapStatesForModi[ (*itrState)->getModus() ].insert( *itrState );
		//no state should have 0 calls
		if ( (*itrState)->getCalls() < 1 ) {
			(*itrState)->setCalls( 1 );
		}
	}
	//stack with the borders to check, if subborders for them should be created
	stack< cBorderDataGloveState * > stackOpenBorders;
	cBorderDataGloveState * pCreatedBorder;
	cBorderDataGloveState * pActualBorder;
	
	//for each modi create the borders
	for ( map< int, set< cDataGloveState * > >::iterator
			itrModiStates = mapStatesForModi.begin();
			itrModiStates != mapStatesForModi.end(); ++itrModiStates ) {
		
		pCreatedBorder = createGoodBorder( itrModiStates->second );
		
		if ( pCreatedBorder != NULL ) {
			setAllBorders.insert( pCreatedBorder );
			mapStartBordersForModus[ itrModiStates->first ] = pCreatedBorder;
			stackOpenBorders.push( pCreatedBorder );
		}
		
		while ( ! stackOpenBorders.empty() ) {
			//create subborders for open borders
			pActualBorder = stackOpenBorders.top();
			stackOpenBorders.pop();
			
			if ( 1 < pActualBorder->getLowerStates().size() ) {
				//if the lower side contains more than 1 states
				//-> create new border for the contained states
				pCreatedBorder = createGoodBorder( pActualBorder->getLowerStates() );
				
				if ( pCreatedBorder != NULL ) {
					setAllBorders.insert( pCreatedBorder );
					mapStartBordersForModus[ itrModiStates->first ] = pCreatedBorder;
					stackOpenBorders.push( pCreatedBorder );
				}
			}
			if ( 1 < pActualBorder->getHigherStates().size() ) {
				//if the higher side contains more than 1 states
				//-> create new border for the contained states
				pCreatedBorder = createGoodBorder( pActualBorder->getHigherStates() );
				
				if ( pCreatedBorder != NULL ) {
					setAllBorders.insert( pCreatedBorder );
					mapStartBordersForModus[ itrModiStates->first ] = pCreatedBorder;
					stackOpenBorders.push( pCreatedBorder );
				}
			}
		}  //while there are open borders to check
	}  //end for all modi
	//set the border for the actual modi
	std::map< int, cBorderDataGloveState * >::iterator
		itrNewStartBorder = mapStartBordersForModus.find( iActualModus );
	if ( itrNewStartBorder == mapStartBordersForModus.end() ) {
		return false;  //no such modus
	}  //else set new modus and start border
	pActualModusStartBorder = itrNewStartBorder->second;
	
	return true;
}


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
iCallFunction * cEvaluateDataGloveState::getCallFunction( const string & szFunction,
		const string & szFunctionParameter ) {
	
#ifdef SIMULATE_CALL_FUNCTION
	if ( szFunction == "changeModusPrepare" ) {
		return new cCallPrepareChangeModus( atoi( szFunctionParameter.c_str() ) );
	} else if ( szFunction == "changeModus" ) {
		return new cCallChangeModus( atoi( szFunctionParameter.c_str() ),
			this );
	}
	
	
	return new cCallSimulation( szFunction, szFunctionParameter );
#else //SIMULATE_CALL_FUNCTION
	if ( szFunction == "changeModusPrepare" ) {
		return new cCallPrepareChangeModus( atoi( szFunctionParameter.c_str() ) );
	} else if ( szFunction == "changeModus" ) {
		return new cCallChangeModus( atoi( szFunctionParameter.c_str() ),
			this );
	}
	//TODO
	
	
	return NULL;
#endif //SIMULATE_CALL_FUNCTION
}








