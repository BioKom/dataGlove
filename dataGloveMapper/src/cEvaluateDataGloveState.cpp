
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

//just simulate the call to the functions
//TODO weg? #define SIMULATE_CALL_FUNCTION

///prints information about the created border states
//#define DEBUG_BORDER_STATES


#include "cEvaluateDataGloveState.h"

#include <string>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <fstream>
#include <stdlib.h>




/*TODO use if codecvt
#include <locale>
#include <codecvt>
#include <cstdlib>
*/
//#include <boost/locale.hpp>




#include "cBorderDataGloveState.h"
#include "cDataGloveState.h"

#include "cCallSimulation.h"
#include "cCallPrepareChangeModus.h"
#include "cCallChangeModus.h"
#include "cCallPrepareKeybordFunction.h"
#include "cCallKeybordFunction.h"
#include "cCallPrepareMouseFunction.h"
#include "cCallMouseFunction.h"


#ifdef FEATURE_READ_DATA_TEXT_WITH_REGEX
	#include <regex>
#endif //FEATURE_READ_DATA_TEXT_WITH_REGEX


#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
	#include "cMapWString.h"
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
	#include "cMapString.h"
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE



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


namespace nDataGlove{
namespace nModelDataGloveDGTechVHand{
namespace nEvaluateDataGloveState{


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
	
	char c = 0;
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
 * This function reads a part from the stream till the next semikolon ';'
 * or new line.
 *
 * @param streamDataGloveStates the stream from which to read the string
 * @return a pair with:
 * 		first: the readed string without the semikolon ';'
 * 		second: true if a new line as read, else false
 */
pair< wstring, bool > readEntry( wistream & streamDataGloveStates ) {
	
	wchar_t c = 0;
	wstring strReadedString;
	while ( streamDataGloveStates.good() ) {
		streamDataGloveStates.get( c );
		if ( c == ';' ) {
			break;
		}
		if ( c == '\n' ) {
			//new line read
			return pair< wstring, bool >( strReadedString, true );
		}
		strReadedString.push_back( c );
	};
	
	return pair< wstring, bool >( strReadedString, false );
}


/**
 * Checks if the given string has a ending number.
 * (Ending spaces and semikolons will be skiped.)
 *
 * @param inString the string in which to search for a ending number
 * @return true if the given string ends with a number, else false
 */
bool hasEndingNumber( const string & inString ) {
	
	if ( inString.empty() ) {
		return false;
	}
	
	/* * read from back
	   * skip ending spaces and semikolons*/
	char itrChar;
	size_t iActualChar = inString.length() - 1;
	while ( true ) {
		
		itrChar = inString[ iActualChar ];
		
		if ( isdigit( itrChar ) ) {
			//ending number found
			return true;
		}
		if ( ( itrChar != ' ' ) && ( itrChar != ';' ) && ( itrChar != '\n' ) ) {
			//ending number can't exists
			return false;
		}
		
		if ( iActualChar == 0 ) {
			return false;
		}
		--iActualChar;
	}
	return false;
}


/**
 * Checks if the given string has a ending number.
 * (Ending spaces and semikolons will be skiped.)
 *
 * @param inString the string in which to search for a ending number
 * @return true if the given string ends with a number, else false
 */
bool hasEndingNumber( const wstring & inString ) {
	
	if ( inString.empty() ) {
		return false;
	}
	
	/* * read from back
	   * skip ending spaces and semikolons*/
	wchar_t itrChar;
	size_t iActualChar = inString.length() - 1;
	while ( true ) {
		
		itrChar = inString[ iActualChar ];
		
		if ( isdigit( itrChar ) ) {
			//ending number found
			return true;
		}
		if ( ( itrChar != ' ' ) && ( itrChar != ';' ) && ( itrChar != '\n' ) ) {
			//ending number can't exists
			return false;
		}
		
		if ( iActualChar == 0 ) {
			return false;
		}
		--iActualChar;
	}
	return false;
}


/**
 * Converts the given string to a long number.
 *
 * @param inString the string from which to read the number
 * @return the readed number
 */
long stringToLong( const wstring & inString ) {
	
	return wcstol( inString.c_str(), NULL, 10 );
}


/**
 * Converts the given string to a long number.
 *
 * @param inString the string from which to read the number
 * @return the readed number
 */
long stringToLong( const string & inString ) {
	
	return strtol( inString.c_str(), NULL, 10 );
}


/**
 * Reads from the given string a ending number.
 * (Ending spaces and semikolons will be skiped.)
 *
 * @param inString the string from which to read the ending number
 * @return the readed ending number
 */
long readEndingNumber( const string & inString ) {
	
	if ( inString.empty() ) {
		return false;
	}
	
	/* * read from back
	   * skip ending spaces and semikolons
	   * read all number letters
	   * convert readed number letters to char*/
	size_t iActualChar = inString.length() - 1;
	char itrChar;
	string szReadedNumber;
	while ( true ) {
		
		itrChar = inString[ iActualChar ];
		
		if ( isdigit( itrChar ) ) {
			//digit of ending number found
			szReadedNumber.insert( 0, 1, itrChar );
			
		} else if ( itrChar == '-' ) {
			//sign of ending number found
			if ( ! szReadedNumber.empty() ) {
				//add sign to ending number
				szReadedNumber.insert( 0, 1, itrChar );
			}
			break;  //done
		} else if ( ( itrChar != ' ' ) && ( itrChar != ';' ) && ( itrChar != '\n' ) ) {
			//ending number readed
			break;
		}
		if ( iActualChar == 0 ) {
			//done
			break;
		}
		--iActualChar;
	}
	return stringToLong( szReadedNumber );
}


/**
 * Reads from the given string a ending number.
 * (Ending spaces and semikolons will be skiped.)
 *
 * @param inString the string from which to read the ending number
 * @return the readed ending number
 */
long readEndingNumber( const wstring & inString ) {
	
	if ( inString.empty() ) {
		return false;
	}
	
	wchar_t itrChar;
	/* * read from back
	   * skip ending spaces and semikolons
	   * read all number letters
	   * convert readed number letters to char*/
	size_t iActualChar = inString.length() - 1;
	wstring szReadedNumber;
	while ( true ) {
		
		itrChar = inString[ iActualChar ];
		
		if ( isdigit( itrChar ) ) {
			//digit of ending number found
			szReadedNumber.insert( 0, 1, itrChar );
			
		} else if ( itrChar == '-' ) {
			//sign of ending number found
			if ( ! szReadedNumber.empty() ) {
				//add sign to ending number
				szReadedNumber.insert( 0, 1, itrChar );
			}
			break;  //done
		} else if ( ( itrChar != ' ' ) && ( itrChar != ';' ) && ( itrChar != '\n' ) ) {
			//ending number readed
			break;
		}
		if ( iActualChar == 0 ) {
			//done
			break;
		}
		--iActualChar;
	}
	return stringToLong( szReadedNumber );
}

};  //end namespace nEvaluateDataGloveState
};  //end namespace nModelDataGloveDGTechVHand
};  //end namespace nDataGlove


using namespace nDataGlove::nModelDataGloveDGTechVHand::nEvaluateDataGloveState;



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
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
bool cEvaluateDataGloveState::loadDataGloveStates(
		wistream & streamDataGloveStates ) {
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
bool cEvaluateDataGloveState::loadDataGloveStates(
		istream & streamDataGloveStates ) {
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
	
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
		
		/**
		 * compares this table head with the given
		 * (Neede to order this object for map.)
		 *
		 * @param inOtherTableHead other table head to compare this with
		 * @return true if the this table head is lower than the given
		 */
		bool operator<( const tTableHead & inOtherTableHead ) const {
			
			if ( bSamplingType != inOtherTableHead.bSamplingType ) {
				return bSamplingType;
			}  //bSamplingType == inOtherTableHead.bSamplingType
			if ( bSamplingType ) {
				if ( typeSamplingData.first != inOtherTableHead.typeSamplingData.first ) {
					return ( typeSamplingData.first < inOtherTableHead.typeSamplingData.first );
				}
				if ( typeSamplingData.second != inOtherTableHead.typeSamplingData.second ) {
					return ( typeSamplingData.second < inOtherTableHead.typeSamplingData.second );
				}
			} else {  //not bSamplingType
				if ( tableHead != inOtherTableHead.tableHead ) {
					return ( tableHead < inOtherTableHead.tableHead );
				}
			}  //else all relevant data equal -> this is not lower
			return false;
		};
		
	};  //end tTableHead
	
	
	//list with the column types
	list< tTableHead > liTableColumnType;
	
	//create mapping from string / regular expression to column type
	
	tTableHead typeOfColum;
	typeOfColum.bSamplingType = false;
	typeOfColum.tableHead = tTypeTableHeadExtra::EMPTY;
	
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
#ifdef FEATURE_READ_DATA_TEXT_WITH_REGEX
		cMapWString< tTableHead > mapTableColumnTypeExpr( typeOfColum,
				regex_constants::icase | regex_constants::ECMAScript );
#else  //FEATURE_READ_DATA_TEXT_WITH_REGEX
		cMapWString< tTableHead > mapTableColumnTypeExpr( typeOfColum );
#endif  //FEATURE_READ_DATA_TEXT_WITH_REGEX
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
#ifdef FEATURE_READ_DATA_TEXT_WITH_REGEX
		cMapString< tTableHead > mapTableColumnTypeExpr( typeOfColum,
				regex_constants::icase | regex_constants::ECMAScript );
#else  //FEATURE_READ_DATA_TEXT_WITH_REGEX
		cMapString< tTableHead > mapTableColumnTypeExpr( typeOfColum );
#endif  //FEATURE_READ_DATA_TEXT_WITH_REGEX
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE

	
#ifdef FEATURE_READ_DATA_TEXT_WITH_REGEX
	typeOfColum.bSamplingType = false;
	
	typeOfColum.tableHead = tTypeTableHeadExtra::MODUS;
	mapTableColumnTypeExpr.setMapPair( ".*MOD.*", typeOfColum );
	typeOfColum.tableHead = tTypeTableHeadExtra::FUNCTION;
	mapTableColumnTypeExpr.setMapPair( ".*fun.*", typeOfColum );
	typeOfColum.tableHead = tTypeTableHeadExtra::PARAMETER;
	mapTableColumnTypeExpr.setMapPair( ".*par.*", typeOfColum );
	typeOfColum.tableHead = tTypeTableHeadExtra::REPEAT_DELAY;
	mapTableColumnTypeExpr.setMapPair( ".*repeat.*", typeOfColum );
	mapTableColumnTypeExpr.setMapPair( ".*wied.*", typeOfColum );
	typeOfColum.tableHead = tTypeTableHeadExtra::CALLS;
	mapTableColumnTypeExpr.setMapPair( ".*calls.*", typeOfColum );
	mapTableColumnTypeExpr.setMapPair( ".*auf.*", typeOfColum );
	
	typeOfColum.bSamplingType = true;
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::FINGER_1;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*FIN.*1 .*MIN.*", typeOfColum );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*FIN.*1 .*MAX.*", typeOfColum );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.setMapPair( ".*FIN.*1 .*TARG.*", typeOfColum );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::FINGER_2;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*FIN.*2 .*MIN.*", typeOfColum );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*FIN.*2 .*MAX.*", typeOfColum );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.setMapPair( ".*FIN.*2 .*TAR.*", typeOfColum );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::FINGER_3;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*FIN.*3 .*MIN.*", typeOfColum );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*FIN.*3 .*MAX.*", typeOfColum );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.setMapPair( ".*FIN.*3 .*TAR.*", typeOfColum );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::FINGER_4;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*FIN.*4 .*MIN.*", typeOfColum );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*FIN.*4 .*MAX.*", typeOfColum );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.setMapPair( ".*FIN.*4 .*TAR.*", typeOfColum );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::FINGER_5;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*FIN.*5 .*MIN.*", typeOfColum );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*FIN.*5 .*MAX.*", typeOfColum );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.setMapPair( ".*FIN.*5 .*TAR.*", typeOfColum );
	
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::QUATERNION_1;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*QUA.*1 .*MIN.*", typeOfColum );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*QUA.*1 .*MAX.*", typeOfColum );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.setMapPair( ".*QUA.*1 .*TAR.*", typeOfColum );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::QUATERNION_2;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*QUA.*2 .*MIN.*", typeOfColum );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*QUA.*2 .*MAX.*", typeOfColum );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.setMapPair( ".*QUA.*2 .*TAR.*", typeOfColum );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::QUATERNION_3;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*QUA.*3 .*MIN.*", typeOfColum );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*QUA.*3 .*MAX.*", typeOfColum );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.setMapPair( ".*QUA.*3 .*TAR.*", typeOfColum );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::QUATERNION_4;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*QUA.*4 .*MIN.*", typeOfColum );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*QUA.*4 .*MAX.*", typeOfColum );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.setMapPair( ".*QUA.*4 .*TAR.*", typeOfColum );
	
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::GYROSCOPE_X;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*GYR.*X .*MIN.*", typeOfColum );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*GYR.*X .*MAX.*", typeOfColum );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.setMapPair( ".*GYR.*X .*TAR.*", typeOfColum );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::GYROSCOPE_Y;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*GYR.*Y .*MIN.*", typeOfColum );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*GYR.*Y .*MAX.*", typeOfColum );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.setMapPair( ".*GYR.*Y .*TAR.*", typeOfColum );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::GYROSCOPE_Z;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*GYR.*Z .*MIN.*", typeOfColum );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*GYR.*Z .*MAX.*", typeOfColum );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.setMapPair( ".*GYR.*Z .*TAR.*", typeOfColum );
	
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::MAGNETOMETER_X;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*MAG.*X .*MIN.*", typeOfColum );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*MAG.*X .*MAX.*", typeOfColum );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.setMapPair( ".*MAG.*X .*TAR.*", typeOfColum );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::MAGNETOMETER_Y;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*MAG.*Y .*MIN.*", typeOfColum );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*MAG.*Y .*MAX.*", typeOfColum );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.setMapPair( ".*MAG.*Y .*TAR.*", typeOfColum );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::MAGNETOMETER_Z;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*MAG.*Z .*MIN.*", typeOfColum );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*MAG.*Z .*MAX.*", typeOfColum );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.setMapPair( ".*MAG.*Z .*TAR.*", typeOfColum );
	
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::ACCELEROMETER_X;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*AC.*X .*MIN.*", typeOfColum );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*AC.*X .*MAX.*", typeOfColum );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.setMapPair( ".*AC.*X .*TAR.*", typeOfColum );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::ACCELEROMETER_Y;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*AC.*Y .*MIN.*", typeOfColum );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*AC.*Y .*MAX.*", typeOfColum );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.setMapPair( ".*AC.*Y .*TAR.*", typeOfColum );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::ACCELEROMETER_Z;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*AC.*Z .*MIN.*", typeOfColum );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.setMapPair( ".*AC.*Z .*MAX.*", typeOfColum );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.setMapPair( ".*AC.*Z .*TAR.*", typeOfColum );
	
#else  //FEATURE_READ_DATA_TEXT_WITH_REGEX
	typeOfColum.bSamplingType = false;
	
	typeOfColum.tableHead = tTypeTableHeadExtra::MODUS;
	mapTableColumnTypeExpr.setMapPair( "MODUS", typeOfColum );
	typeOfColum.tableHead = tTypeTableHeadExtra::FUNCTION;
	mapTableColumnTypeExpr.setMapPair( "function", typeOfColum );
	typeOfColum.tableHead = tTypeTableHeadExtra::PARAMETER;
	mapTableColumnTypeExpr.setMapPair( "parmeter", typeOfColum );
	typeOfColum.tableHead = tTypeTableHeadExtra::REPEAT_DELAY;
	mapTableColumnTypeExpr.setMapPair( "repeat delay", typeOfColum );
	typeOfColum.tableHead = tTypeTableHeadExtra::CALLS;
	mapTableColumnTypeExpr.setMapPair( "calls", typeOfColum );
	
	typeOfColum.bSamplingType = true;
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::FINGER_1;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.setMapPair( "FINGER_1 MIN", typeOfColum );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.setMapPair( "FINGER_1 MAX", typeOfColum );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.setMapPair( "FINGER_1 TARGET", typeOfColum );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::FINGER_2;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.setMapPair( "FINGER_2 MIN", typeOfColum );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.setMapPair( "FINGER_2 MAX", typeOfColum );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.setMapPair( "FINGER_2 TARGET", typeOfColum );
	
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::FINGER_3;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.setMapPair( "FINGER_3 MIN", typeOfColum );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.setMapPair( "FINGER_3 MAX", typeOfColum );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.setMapPair( "FINGER_3 TARGET", typeOfColum );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::FINGER_4;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.setMapPair( "FINGER_4 MIN", typeOfColum );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.setMapPair( "FINGER_4 MAX", typeOfColum );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.setMapPair( "FINGER_4 TARGET", typeOfColum );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::FINGER_5;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.setMapPair( "FINGER_5 MIN", typeOfColum );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.setMapPair( "FINGER_5 MAX", typeOfColum );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.setMapPair( "FINGER_5 TARGET", typeOfColum );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::QUATERNION_1;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.setMapPair( "QUATERNION_1 MIN", typeOfColum );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.setMapPair( "QUATERNION_1 MAX", typeOfColum );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.setMapPair( "QUATERNION_1 TARGET", typeOfColum );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::QUATERNION_2;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.setMapPair( "QUATERNION_2 MIN", typeOfColum );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.setMapPair( "QUATERNION_2 MAX", typeOfColum );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.setMapPair( "QUATERNION_2 TARGET", typeOfColum );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::QUATERNION_3;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.setMapPair( "QUATERNION_3 MIN", typeOfColum );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.setMapPair( "QUATERNION_3 MAX", typeOfColum );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.setMapPair( "QUATERNION_3 TARGET", typeOfColum );
	
	typeOfColum.typeSamplingData.first =
		cMessageSamplingDataFromDataGlove::QUATERNION_4;
	typeOfColum.typeSamplingData.second = MINIMUM;
	mapTableColumnTypeExpr.setMapPair( "QUATERNION_4 MIN", typeOfColum );
	typeOfColum.typeSamplingData.second = MAXIMUM;
	mapTableColumnTypeExpr.setMapPair( "QUATERNION_4 MAX", typeOfColum );
	typeOfColum.typeSamplingData.second = TARGET;
	mapTableColumnTypeExpr.setMapPair( "QUATERNION_4 TARGET", typeOfColum );
#endif  //FEATURE_READ_DATA_TEXT_WITH_REGEX
	
	//TODO mapTableColumnTypeExpr.optimize();
	
	
	//the given extremas (borders) for the different intervals
	map< tTableHead, long > mapGivenExtremas;
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
	pair< wstring, bool > readedEntry;
	wstring readedString;
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
	pair< string, bool > readedEntry;
	string readedString;
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
	while ( streamDataGloveStates.good() ) {
		
		readedEntry = readEntry( streamDataGloveStates );
		
		readedString = readedEntry.first;
		
		//search for matching string
		typeOfColum = mapTableColumnTypeExpr( readedString );
		//read/check for number after column type
		if ( hasEndingNumber( readedString ) ) {
			
			mapGivenExtremas[ typeOfColum ] = readEndingNumber( readedString );
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
	//evaluate corrections
	mapCorrections.clear();
	for ( map< tTableHead, long >::const_iterator itrExtrema = mapGivenExtremas.begin();
			itrExtrema != mapGivenExtremas.end(); ++itrExtrema ) {
		
		if ( itrExtrema->first.typeSamplingData.second == MINIMUM ) {
			//the extrema is for the minimum -> set lower border
			mapCorrections[ itrExtrema->first.typeSamplingData.first ].
				setLowerBorder( itrExtrema->second );
		} else if ( itrExtrema->first.typeSamplingData.second == MAXIMUM ) {
			//the extrema is for the maximum -> set upper border
			mapCorrections[ itrExtrema->first.typeSamplingData.first ].
				setUpperBorder( itrExtrema->second );
		}  //else do nothing
	}
	
	//read remaining lines
	list< tTableHead >::const_iterator itrColumnType = liTableColumnType.begin();
	cDataGloveState * pActualDataGloveState = new cDataGloveState();
	
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
	wstring szFunction;
	wstring szFunctionParameter;
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
	string szFunction = "";
	string szFunctionParameter = "";
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
	
	
	bool bFunctionParameterRead = false;
	cInterval * pInterval;
	
	map< cMessageSamplingDataFromDataGlove::tTypeSamplingValue,
			cIntervalCorrection >::const_iterator
		itrFoundCorrection;
	
	while ( streamDataGloveStates.good() ) {
		
		readedEntry = readEntry( streamDataGloveStates );
		
		readedString = trim_copy( readedEntry.first );
		
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
						
						itrFoundCorrection = mapCorrections.find(
							itrColumnType->typeSamplingData.first );
						if ( itrFoundCorrection == mapCorrections.end() ) {
							//correction not found
							pInterval = new cInterval();
						} else {  //correction found -> use it
							pInterval = new cInterval( &(itrFoundCorrection->second) );
						}
						
						pActualDataGloveState->setInterval(
							itrColumnType->typeSamplingData.first, pInterval );
					}
					
					//set the new interval value
					switch ( itrColumnType->typeSamplingData.second ) {
						case MINIMUM : {
							pInterval->setMinimum( stringToLong( readedString ) );
							
						}; break;
						case MAXIMUM : {
							pInterval->setMaximum( stringToLong( readedString ) );
							
						}; break;
						case TARGET : {
							pInterval->setTarget( stringToLong( readedString ) );
						}; break;
					}; //end
					
				} else {  //not a sampling type
					switch ( itrColumnType->tableHead ) {
						case EMPTY : {
							//nothing to do
						}; break;
						case MODUS : {
							pActualDataGloveState->setModus(
								stringToLong( readedString ) );
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
								stringToLong( readedString ) );
						}; break;
						case CALLS : {
							pActualDataGloveState->setCalls( stringToLong( readedString ) );
						}; break;
					};  //switch itrColumnType->tableHead
					if ( ( ! szFunction.empty() ) && bFunctionParameterRead ) {
						//set the call function
/*TODO weg debugging
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
if ( ( szFunctionParameter.compare( L"3" ) == 0 ) ||
		( szFunctionParameter.compare( L"Y" ) == 0 ) ||
		( szFunctionParameter.compare( L"Z" ) == 0 )
) {
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
if ( ( szFunctionParameter.compare( "3" ) == 0 ) ||
		( szFunctionParameter.compare( "Y" ) == 0 ) ||
		( szFunctionParameter.compare( "Z" ) == 0 )
) {
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
	szFunctionParameter = szFunctionParameter;
}*/
						
						
						
						iCallFunction * pCallFunction =
							getCallFunction( szFunction, szFunctionParameter );
						if ( pCallFunction ) {
							pActualDataGloveState->setCallFunction( pCallFunction, false );
						} else {  //try to create the call function
							pCallFunction =
								createCallFunction( szFunction, szFunctionParameter );
							if ( pCallFunction ) {
								pActualDataGloveState->setCallFunction( pCallFunction, true );
							}
						}
						szFunction.clear();
						szFunctionParameter.clear();
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
			pActualDataGloveState->orderIntervals();
			if ( pActualDataGloveState->isValid() ) {
				setAllStates.insert( pActualDataGloveState );
			} else {  //not a valid state -> delete it
				delete pActualDataGloveState;
			}
			//create new state
			pActualDataGloveState = new cDataGloveState();
			szFunction.clear();
			szFunctionParameter.clear();
		}
	}
	pActualDataGloveState->orderIntervals();
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
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
bool cEvaluateDataGloveState::loadDataGloveStates(
		const wstring & szPathDataGloveStates ) {
	
	const string stringPathDataGloveStates(
		szPathDataGloveStates.begin(), szPathDataGloveStates.end() );
//TODO
	wifstream streamDataGloveStates( stringPathDataGloveStates.c_str() );
	
/*TODO use if codecvt
	const std::locale empty_locale = std::locale::empty();
	typedef std::codecvt_utf8<wchar_t> converter_type;
	const converter_type * pConverter = new converter_type;
	const std::locale utf8_locale = std::locale( empty_locale, pConverter );
	std::wifstream streamDataGloveStates( stringPathDataGloveStates.c_str() );
	streamDataGloveStates.imbue( utf8_locale );
*/

/*TODO use boost.locale
	std::locale fromLoc = boost::locale::generator().generate("en_US.UTF-8");

	streamDataGloveStates.imbue( fromLoc );
*/

/*TODO weg
	std::wifstream streamDataGloveStates(
		stringPathDataGloveStates.c_str() );
	std::locale loc;
	std::locale loc2(
		loc,
		new std::codecvt_utf8<wchar_t> )
	streamDataGloveStates.imbue( loc2 );
	*/
/*TODO weg
	std::locale utf8_locale( std::locale(), new gel::stdx::utf8cvt<true> );
	wifstream streamDataGloveStates;
	streamDataGloveStates.imbue( utf8_locale );
	streamDataGloveStates.open( stringPathDataGloveStates.c_str() );
*/
	
	
	return loadDataGloveStates( streamDataGloveStates );
}
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE


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
	
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
//TODO
	wifstream streamDataGloveStates( szPathDataGloveStates.c_str() );
	
	
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
	ifstream streamDataGloveStates( szPathDataGloveStates.c_str() );
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
	
	return loadDataGloveStates( streamDataGloveStates );
}


#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE

/**
 * Adds a function to call.
 *
 * @see mapToUseCallFunctions
 * @param szFunctionParameterName the parameter name of the function,
 * 	for which to add the call function, like given in the data glove
 * 	state stream
 * 	@see loadDataGloveStates()
 * @param pCallFunction a pointer to the function to call
 * @return true if the function could be added, else false (e.g.
 * 	a function with the same name exists allready)
 */
bool cEvaluateDataGloveState::addToUseCallFunction(
		const wstring & szFunctionParameterName,
		iCallFunction * pCallFunction ) {
	
	if ( pCallFunction == NULL ) {
		//no call function given
		return false;
	}  //else call function given
	
	const pair< map< wstring, iCallFunction * >::iterator, bool > paInserted =
		mapToUseCallFunctions.insert( pair< wstring, iCallFunction * >(
			szFunctionParameterName, pCallFunction) );
	
	return paInserted.second;
}


/**
 * Adds a function to call.
 *
 * @see mapToUseCallFunctions
 * @param szFunctionParameterName the parameter name of the function,
 * 	for which to add the call function, like given in the data glove
 * 	state stream
 * 	@see loadDataGloveStates()
 * @param pCallFunction a pointer to the function to call
 * @return true if the function could be added, else false (e.g.
 * 	a function with the same name exists allready)
 */
bool cEvaluateDataGloveState::addToUseCallFunction(
		const string & szFunctionParameterName,
		iCallFunction * pCallFunction ) {
	
	if ( pCallFunction == NULL ) {
		//no call function given
		return false;
	}  //else call function given
	
	const pair< map< wstring, iCallFunction * >::iterator, bool > paInserted =
		mapToUseCallFunctions.insert( pair< wstring, iCallFunction * >(
			wstring( szFunctionParameterName.begin(), szFunctionParameterName.end() ),
			pCallFunction) );
	
	return paInserted.second;
}

#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE

/**
 * Adds a function to call.
 *
 * @see mapToUseCallFunctions
 * @param szFunctionParameterName the parameter name of the function,
 * 	for which to add the call function, like given in the data glove
 * 	state stream
 * 	@see loadDataGloveStates()
 * @param pCallFunction a pointer to the function to call
 * @return true if the function could be added, else false (e.g.
 * 	a function with the same name exists allready)
 */
bool cEvaluateDataGloveState::addToUseCallFunction(
		const string & szFunctionParameterName,
		iCallFunction * pCallFunction ) {
	
	if ( pCallFunction == NULL ) {
		//no call function given
		return false;
	}  //else call function given
	
	const pair< map< string, iCallFunction * >::iterator, bool > paInserted =
		mapToUseCallFunctions.insert( pair< string, iCallFunction * >(
			szFunctionParameterName, pCallFunction) );
	
	return paInserted.second;
}

#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE


/**
 * @return a map with the a functions to call
 * 	Elements:
 * 	key: he parameter name of the function, for which to
 * 		add the call function, like given in the data glove state stream
 * 		@see loadDataGloveStates()
 * 	value: a pointer to the to use function
 * 	@see mapToUseCallFunctions
 */
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
map< wstring, iCallFunction * > cEvaluateDataGloveState::getToUseCallFunction() {
	
	return mapToUseCallFunctions;
}
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
map< string, iCallFunction * > cEvaluateDataGloveState::getToUseCallFunction() {
	
	return mapToUseCallFunctions;
}
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE


/**
 * Removes the function to call.
 *
 * @see mapToUseCallFunctions
 * @param szFunctionParameterName the parameter name of the function,
 * 	for which to remove the call function, like given in the data glove
 * 	state stream
 * 	@see loadDataGloveStates()
 * @return true if the function could be removed, else false
 */
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
bool cEvaluateDataGloveState::removeToUseCallFunction(
		const wstring & szFunctionParameterName ) {
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
bool cEvaluateDataGloveState::removeToUseCallFunction(
		const string & szFunctionParameterName ) {
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
	
	const int numberElementsRemoved =
		mapToUseCallFunctions.erase( szFunctionParameterName );
	
	return ( 0 < numberElementsRemoved );
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
 * @param pBorderDataGlove the border for which to evaluate a next lower border
 * @param bForLower for which side of the border to evaluate the next lower
 * 	border
 * @return a pointer to the the created border (please care that deleted),
 * 	or NULL, if non could be created
 */
cBorderDataGloveState * cEvaluateDataGloveState::createGoodBorder(
		cBorderDataGloveState * pBorderDataGlove,
		const bool bForLower ) const {
		
	//evaluate the states for which to evaluate the border
	set< cDataGloveState * > & setDataGloveStates = bForLower ?
		pBorderDataGlove->getLowerStates() :
		pBorderDataGlove->getHigherStates();

	return createGoodBorder( setDataGloveStates, pBorderDataGlove, bForLower );
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
 * @param pBorderDataGlove the border for which to evaluate a next lower border
 * @param bForLower for which side of the border to evaluate the next lower
 * 	border
 * @return a pointer to the the created border (please care that deleted),
 * 	or NULL, if non could be created
 */
cBorderDataGloveState * cEvaluateDataGloveState::createGoodBorder(
		set< cDataGloveState * > & setDataGloveStates,
		cBorderDataGloveState * pBorderDataGlove,
		const bool bForLower ) const {
		
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
				
			//search for possible border values in the states
			cBorderDataGloveState * pNewBorder = NULL;
			for ( ; itrSamplingInterval != mapActualIntervalMap.end();
					++itrSamplingInterval ) {
				if ( itrSamplingInterval->second != NULL ) {
					//interval found
					if ( pBorderDataGlove != NULL ) {
						//use just valid values
						if ( pBorderDataGlove->isValueInsideSide(
								itrSamplingInterval->first,
								itrSamplingInterval->second->getMinimum(),
								bForLower ) ) {
							//minimum interval value is valid value -> use it
							pNewBorder = new cBorderDataGloveState(
								itrSamplingInterval->first,
								itrSamplingInterval->second->getMinimum() );
							break;  //valid border (value) found -> done
						}
						if ( pBorderDataGlove->isValueInsideSide(
								itrSamplingInterval->first,
								itrSamplingInterval->second->getMaximum() + 1,
								bForLower ) ) {
							//maximum interval value is valid value -> use it
							pNewBorder = new cBorderDataGloveState(
								itrSamplingInterval->first,
								itrSamplingInterval->second->getMaximum() + 1 );
							break;  //valid border (value) found -> done
						}
						
					} else {  //no parent border to check (create the first border)
						pNewBorder = new cBorderDataGloveState(
								itrSamplingInterval->first,
								itrSamplingInterval->second->getMinimum() );
						break;  //valid border (value) found -> done
					}
				}
			}
			
			if ( pNewBorder != NULL ) {
				pNewBorder->filterStates( setDataGloveStates );
				
				//did a interval corection for the border type exists
				map< cMessageSamplingDataFromDataGlove::tTypeSamplingValue,
						cIntervalCorrection >::const_iterator
					itrFoundCorrection = mapCorrections.find(
						pNewBorder->getTypeSamplingValue() );
				if ( itrFoundCorrection != mapCorrections.end() ) {
					//correction found -> use it
					pNewBorder->setCorrection( &(itrFoundCorrection->second) );
				}
			}
			
			return pNewBorder;
		}  // setDataGloveStates.size() == 0
		//not enough states -> no border to create
		return NULL;
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
	set< long > setPossibleBorderValues;
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
		setPossibleBorderValues.clear();
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
			if ( pBorderDataGlove != NULL ) {
				//use just valid values (values not excluded or used by other borders)
				if ( pBorderDataGlove->isValueInsideSide(
						*itrSamplingType, pInterval->getMinimum(), bForLower ) ) {
					//if the border has the interval minimum value the state is higher
					//than the border
					setPossibleBorderValues.insert( pInterval->getMinimum() );
				}
				if ( pBorderDataGlove->isValueInsideSide(
						*itrSamplingType, pInterval->getMaximum() + 1, bForLower ) ) {
					//the border has the has the interval maximum plus 1 value the
					//state is lower than the border
					setPossibleBorderValues.insert(
						pInterval->getMaximum() + 1 );
				}
			} else {  //no parent border to check (create the first border)
				//if the border has the interval minimum value the state is higher
				//than the border
				setPossibleBorderValues.insert( pInterval->getMinimum() );
				//the border has the has the interval maximum plus 1 value the
				//state is lower than the border
				setPossibleBorderValues.insert(
					pInterval->getMaximum() + 1 );
			}
		
			liStatesWithInterval.push_back( *itrState );
			ulMaxNumberOfCalls += (*itrState)->getCalls();
		}  //end search for possible border values
		
		if ( ( ! setPossibleBorderValues.empty() ) &&
				( paBestBorderValue.first == typeDataGloveSamplingData::UNKNOWN ) ) {
			//possible values found and no valid border found jet
			//use possible values as border values
			paBestBorderValue.first  = (*itrSamplingType);
			paBestBorderValue.second = (*(setPossibleBorderValues.begin()));
		}
		
		
		if ( ( ulMaxNumberOfCalls <= ulBestCallsSumEachSide ) &&
				( ulMaxNumberOfCalls <= ulBestCallsLower ) &&
				( ulMaxNumberOfCalls <= ulBestCallsHigher ) ) {
			//can't improve border with this sampling type
			continue;  //skip sampling type
		}
		
		//Search for value, which has maximum number of calls sum and most
		//similar sum of calls on both sides, for states just one one side of
		//the border.
		bool bHasLowerInterval  = false;
		bool bHasHigherInterval = false;
		bool bValueSeperatesIntervalsNotFound = true;
		bool bPartIntervalLower  = false;
		bool bPartIntervalHigher = false;
		for ( set< long >::const_iterator
				itrPossibleValue = setPossibleBorderValues.begin();
				itrPossibleValue != setPossibleBorderValues.end();
				++itrPossibleValue ) {
			ulPossibleCallsLower  = ulMaxNumberOfCalls;
			ulPossibleCallsHigher = ulMaxNumberOfCalls;
			//dos the border has intervals lower and higher?
			bHasLowerInterval  = false;
			bHasHigherInterval = false;
			bPartIntervalLower  = false;
			bPartIntervalHigher = false;
			
			for ( list< cDataGloveState * >::const_iterator
					itrState = liStatesWithInterval.begin();
					itrState != liStatesWithInterval.end(); ++itrState ) {
				
				pInterval = (*itrState)->getIntervalForType( *itrSamplingType );
				//check if state interval lower or/and higher border value
				bIsLower  = ( pInterval->getMaximum() < (*itrPossibleValue) );
				bIsHigher = ( (*itrPossibleValue) <= pInterval->getMinimum() );
				//is a part of the interval lower or higher
				bPartIntervalLower  |= ( pInterval->getMinimum() < (*itrPossibleValue) );
				bPartIntervalHigher |= ( (*itrPossibleValue) <= pInterval->getMaximum() );
				
				ulCalls = (*itrState)->getCalls();
				if ( bIsHigher ) {
					//not on lower border or on both sides
					ulPossibleCallsLower -= ulCalls;
					bHasHigherInterval = true;
				}
				if ( bIsLower ) {
					//not on higher border or on both sides
					ulPossibleCallsHigher -= ulCalls;
					bHasLowerInterval = true;
				}
				if ( ( ulPossibleCallsLower <= ulBestCallsLower ) &&
						( ulPossibleCallsHigher <= ulBestCallsHigher ) ) {
					//can't improve border with this sampling type and value
					break;  //skip sampling type and value
				}
			}  //end for all states with a interval for the sampling type
			
			if ( //just use borders with lower and higher intervals
					bHasHigherInterval && bHasLowerInterval &&
				//a value that seperates two intervalls is allways better than one that dos not
					( bValueSeperatesIntervalsNotFound ||
				//evalue how good / fit the border is
					( 0 < ( ulPossibleCallsLower + ulPossibleCallsHigher -
						ulBestCallsSumEachSide ) +
					//the factor 256 is choosen, because it is much better to
					//classify states than just having the border work on them
					256 * ( abs( ulPossibleCallsLower - ulPossibleCallsHigher ) -
						abs( ulBestCallsLower - ulBestCallsHigher ) ) ) ) ) {
				//better than actual best border -> new best border
				paBestBorderValue.first  = (*itrSamplingType);
				paBestBorderValue.second = (*itrPossibleValue);
				ulBestCallsSumEachSide = ulPossibleCallsLower + ulPossibleCallsHigher;
				bValueSeperatesIntervalsNotFound = false;
			} else if ( bValueSeperatesIntervalsNotFound &&
					( ( bHasHigherInterval && bPartIntervalLower ) ||
						( bHasLowerInterval && bPartIntervalHigher ) ) &&
				//evalue how good / fit the border is
					( 0 < ( ulPossibleCallsLower + ulPossibleCallsHigher -
						ulBestCallsSumEachSide ) +
					//the factor 256 is choosen, because it is much better to
					//classify states than just having the border work on them
					256 * ( abs( ulPossibleCallsLower - ulPossibleCallsHigher ) -
						abs( ulBestCallsLower - ulBestCallsHigher ) ) ) ) {
				
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


#ifdef DEBUG_BORDER_STATES

/**
 * Prints indention.
 *
 * @param uiIndention the actual indention
 */
void printIndention( const unsigned int uiIndention = 0 ) {
	
	for ( unsigned int uiActualIndention = 0;
			uiActualIndention < uiIndention; ++uiActualIndention  ) {
		cout<<"	";
	}
}

/**
 * Prints the given state.
 *
 * @param pStateDataGloveState the state to print
 * @param uiIndention the actual indention
 */
void printState( const cDataGloveState * pStateDataGloveState,
		const unsigned int uiIndention = 0 ) {
	
	printIndention( uiIndention );
	if ( pStateDataGloveState == NULL ) {
		//no border given
		cout<<"NULL state"<<endl;
		return;
	}
	//print call function
	iCallFunction * pCallFunction = pStateDataGloveState->getCallFunction();
	if ( pCallFunction ) {
		cout<<"call function: \""<<pCallFunction->getName()<<"\";";
	} else {
		cout<<"no call function; ";
	}
	//print intervalls
	cout<<" Intervals:";
	typedef cMessageSamplingDataFromDataGlove::tTypeSamplingValue
		typeDataGloveSamplingData;
	const map< typeDataGloveSamplingData, cInterval * > &
		mapIntervals = pStateDataGloveState->getMapForInterval();
	
	for ( map< typeDataGloveSamplingData, cInterval * >::const_iterator
			itrInterval = mapIntervals.begin();
			itrInterval != mapIntervals.end(); ++itrInterval ) {
		
		cout<<" "<<itrInterval->first<<":["<<
			itrInterval->second->getMinimum()<<".."<<
			itrInterval->second->getTarget()<<".."<<
			itrInterval->second->getMaximum()<<"];";
	}
	cout<<endl;
}


/**
 * Prints the tree of the given borders.
 *
 * @param pBorderDataGloveState the start border of the borders to print
 * @param uiIndention the actual indention
 */
void printBorderTree( const cBorderDataGloveState * pBorderDataGloveState,
		const unsigned int uiIndention = 0 ) {
	
	printIndention( uiIndention );
	if ( pBorderDataGloveState == NULL ) {
		//no border given
		cout<<"NULL"<<endl;
		return;
	}
	cout<<"type "<<pBorderDataGloveState->getTypeSamplingValue()<<
		" value "<<pBorderDataGloveState->getBorderValue()<<
		" : "<<pBorderDataGloveState->getLowerStates().size()<<" low "<<
		" | "<<pBorderDataGloveState->getHigherStates().size()<<" high"<<endl;
		
	const unsigned int uiNextIndention = uiIndention + 1;
	//print lower states
	printIndention( uiNextIndention );
	cout<<"Lower States:"<<endl;
	const std::set< cDataGloveState * > & setLowerStates =
		pBorderDataGloveState->getLowerStates();
	for ( std::set< cDataGloveState * >::const_iterator
			itrState = setLowerStates.begin();
			itrState != setLowerStates.end(); ++itrState ) {
		
		printState( (*itrState), uiNextIndention + 1 );
	}
	//print higher states
	printIndention( uiNextIndention );
	cout<<"Higher States:"<<endl;
	const std::set< cDataGloveState * > & setHigherStates =
		pBorderDataGloveState->getHigherStates();
	for ( std::set< cDataGloveState * >::const_iterator
			itrState = setHigherStates.begin();
			itrState != setHigherStates.end(); ++itrState ) {
		
		printState( (*itrState), uiNextIndention + 1 );
	}
	
	//print the next borders
	cout<<endl;
	printIndention( uiNextIndention );
	cout<<"Lower border:"<<endl;
	if ( ( pBorderDataGloveState->getLowerBorder() == NULL ) &&
			( pBorderDataGloveState->getLowerStates().size() != 1 ) ) {
		
		printIndention( uiNextIndention + 1 );
		cerr<<"WARNING: no lower border but "<<
			pBorderDataGloveState->getLowerStates().size()<<" states lower"<<endl;
	}
	printBorderTree( pBorderDataGloveState->getLowerBorder(), uiNextIndention );
	
	printIndention( uiNextIndention );
	cout<<"Higher border:"<<endl;
	if ( ( pBorderDataGloveState->getHigherBorder() == NULL ) &&
			( pBorderDataGloveState->getHigherStates().size() != 1 ) ) {
		
		printIndention( uiNextIndention + 1 );
		cerr<<"WARNING: no higher border but "<<
			pBorderDataGloveState->getHigherStates().size()<<" states higher"<<endl;
	}
	printBorderTree( pBorderDataGloveState->getHigherBorder(), uiNextIndention );
}

#endif  //DEBUG_BORDER_STATES


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
				pCreatedBorder = createGoodBorder(
					pActualBorder, true );
				
				if ( pCreatedBorder != NULL ) {
					setAllBorders.insert( pCreatedBorder );
					pActualBorder->setLowerBorder( pCreatedBorder );
					stackOpenBorders.push( pCreatedBorder );
				}
			}
			if ( 1 < pActualBorder->getHigherStates().size() ) {
				//if the higher side contains more than 1 states
				//-> create new border for the contained states
				pCreatedBorder = createGoodBorder(
					pActualBorder, false );
				
				if ( pCreatedBorder != NULL ) {
					setAllBorders.insert( pCreatedBorder );
					pActualBorder->setHigherBorder( pCreatedBorder );
					stackOpenBorders.push( pCreatedBorder );
				}
			}
		}  //while there are open borders to check
	}  //end for all modi
#ifdef DEBUG_BORDER_STATES
	cout<<"Created "<< mapStartBordersForModus.size() <<" Modi"<<endl;
	cout<<"==============="<<endl;
	for ( map< int, cBorderDataGloveState * >::iterator
			itrStartBordersForModus = mapStartBordersForModus.begin();
			itrStartBordersForModus != mapStartBordersForModus.end();
			++itrStartBordersForModus ) {
		
		cout<<"Modus "<< itrStartBordersForModus->first <<" :"<<endl;
		printBorderTree( itrStartBordersForModus->second, 1 );
	}
#endif  //DEBUG_BORDER_STATES
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
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
iCallFunction * cEvaluateDataGloveState::createCallFunction(
		const wstring & szFunction,
		const wstring & szFunctionParameter ) {
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
iCallFunction * cEvaluateDataGloveState::createCallFunction( const string & szFunction,
		const string & szFunctionParameter ) {
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
/*TODO weg?
#ifdef SIMULATE_CALL_FUNCTION
	if ( szFunction == "changeModusPrepare" ) {
		return new cCallPrepareChangeModus( stringToLong( szFunctionParameter.c_str() ) );
	} else if ( szFunction == "changeModus" ) {
		return new cCallChangeModus( stringToLong( szFunctionParameter.c_str() ),
			this );
	}
	
	
	return new cCallSimulation( szFunction, szFunctionParameter );
#else //SIMULATE_CALL_FUNCTION
*/
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
	if ( szFunction == L"changeModusPrepare" ) {
		return new cCallPrepareChangeModus( stringToLong( szFunctionParameter ) );
	} else if ( szFunction == L"changeModus" ) {
		return new cCallChangeModus( stringToLong( szFunctionParameter ),
			this );
	} else if ( szFunction == L"prepareKey" ) {
		return new cCallPrepareKeybordFunction( szFunctionParameter );
	} else if ( szFunction == L"key" ) {
		return new cCallKeybordFunction( szFunctionParameter, true );
	} else if ( szFunction == L"prepareMouse" ) {
		return new cCallPrepareMouseFunction( szFunctionParameter );
	} else if ( szFunction == L"mouse" ) {
		return new cCallMouseFunction( szFunctionParameter );
	}
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
	if ( szFunction == "changeModusPrepare" ) {
		return new cCallPrepareChangeModus( stringToLong( szFunctionParameter.c_str() ) );
	} else if ( szFunction == "changeModus" ) {
		return new cCallChangeModus( stringToLong( szFunctionParameter.c_str() ),
			this );
	} else if ( szFunction == "prepareKey" ) {
		return new cCallPrepareKeybordFunction( szFunctionParameter.c_str() );
	} else if ( szFunction == "key" ) {
		return new cCallKeybordFunction( szFunctionParameter.c_str(), true );
	} else if ( szFunction == "prepareMouse" ) {
		return new cCallPrepareMouseFunction( szFunctionParameter );
	} else if ( szFunction == "mouse" ) {
		return new cCallMouseFunction( szFunctionParameter );
	}
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
	//TODO
	
	
	return NULL;
//TODO weg? #endif //SIMULATE_CALL_FUNCTION
}


/**
 * Returns the call function with the given parameters.
 *
 * @see loadDataGloveStates()
 * @see mapToUseCallFunctions
 * @see iCallFunction
 * @see addToUseCallFunction()
 * @see getToUseCallFunction()
 * @see removeToUseCallFunction()
 * @param szFunction the name of the function for which to return the
 * 	call function
 * @param szFunctionParameter the parameter for the call function to return
 * @return a pointer to the call function object (do not delete), or
 * 	NULL if non could be created
 */
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
iCallFunction * cEvaluateDataGloveState::getCallFunction( const std::wstring & szFunction,
		const std::wstring & szFunctionParameter ) {
	
	map< wstring, iCallFunction * >::iterator itrToUseCallFunctions =
		mapToUseCallFunctions.find( szFunction );
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
iCallFunction * cEvaluateDataGloveState::getCallFunction( const std::string & szFunction,
		const std::string & szFunctionParameter ) {
	
	map< string, iCallFunction * >::iterator itrToUseCallFunctions =
		mapToUseCallFunctions.find( szFunction );
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR_IN_EVALUATE_DATA_GLOVE_STATE
	if ( ( itrToUseCallFunctions != mapToUseCallFunctions.end() ) &&
			( itrToUseCallFunctions->second != NULL ) ) {
		//to use call function found
		
		if ( ! szFunctionParameter.empty() ) {
			itrToUseCallFunctions->second->setParameter( szFunctionParameter );
		}
		//return pointer to function to call
		return itrToUseCallFunctions->second;
	}
	return NULL;
}








