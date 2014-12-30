/**
 * file name: version.h
 * @author Betti Oesterholz
 * @date 17.08.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file contains switches for the different data glove versions.
 *
 *
 * Copyright (C) @c GPL3 2014 Betti Oesterholz
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
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
 * This file contains switches for the different data glove versions.
 * This includes descriptions of declarations for versioned
 * programming switches.
 *
 */
/*
History:
17.08.2014  Oesterholz  created
*/


//debugging information; define DEBUG to activate them
//#define DEBUG
#include "debugOut.inc"


/**************************************
* Version Information
**************************************/

//versions information for the data glove program system
#define DATA_GLOVE_VERSION          2
#define DATA_GLOVE_VERSION_NAME     V0.0.2


/*****************************************
* C++ switches
******************************************/

#if __cplusplus > 199711
//the C++ 2011 standard is available
#define CPP_2011
#endif


/*****************************************
* versioned programming debugging switches
******************************************/

/**
 * Prints messages for the call functions.
 * @see iCallFunction
 */
//#define DEBUG_CALL_FUNCTION


/**
 * Prints messages for every change of modus.
 * @see cCallChangeModus
 */
#define DEBUG_CHANGE_MODUS

/**
 * Prints messages for every prepared keyboard function.
 * @see cCallPrepareKeybordFunction
 */
#define DEBUG_PREPARED_KEYBOARD_FUNCTION



/**************************************
* versioned programming switches
**************************************/


/**
 * With this feature the method @see cEvaluateDataGloveState::loadDataGloveStates()
 * will use regext. The method will search the given string for
 * patterns with the C++ 2011 regex functionalities.
 *
 * @see cEvaluateDataGloveState::loadDataGloveStates()
 * created: 17.08.2014 Betti Oesterholz
 * status:  implemented
 *//*
History:
*/
#ifdef CPP_2011
#define FEATURE_READ_DATA_TEXT_WITH_REGEX
#endif //CPP_2011
























