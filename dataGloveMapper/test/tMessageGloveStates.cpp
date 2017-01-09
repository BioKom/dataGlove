/**
 * @file tMessageGloveStates
 * file name: tMessageGloveStates.cpp
 * @author Betti Oesterholz
 * @date 03.09.2016
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This test is for the class cMessageGloveStates and subclases.
 *
 *
 * Copyright (C) @c GPL3 2016 Betti Oesterholz
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
 * This test is for the class cMessageGloveStates and subclases.
 * It should test the message classes for a data glove state.
 *
 * Tested Clases:
 * * cDigitState -> nMessageGloveStates_cDigitState.h
 * * cMessageGloveDigit -> nMessageGloveStates_cMessageGloveDigit.h
 * * cMessageGloveStates
 *
 */
/*
History:
03.09.2016  Oesterholz  created
*/

#include "version.h"

#include "cMessageGloveStates.h"
#include "nMessageGloveStates_cDigitState.h"
#include "nMessageGloveStates_cMessageGloveDigit.h"

#include <iostream>
#include <fstream>
#include <ostream>
#include <istream>
#include <ctime>
#include <map>

#include <unistd.h>


using namespace std;

using namespace nDataGlove::nMapper::nMessageGloveStates;
using namespace nDataGlove::nMapper;


int testDigitState( const unsigned long ulTestphase, const unsigned long ulIterations );
int testMessageGloveDigit( const unsigned long ulTestphase, const unsigned long ulIterations );
int testMessageGloveStates( const unsigned long ulTestphase, const unsigned long ulIterations );


string SzFileNamePrefix = "tMessageGloveStates_test";


int main(int argc, char* argv[]){

	unsigned long ulTestphase = 0;//actual phase of the test 
	int iReturn = 0;//return value of the test; the number of occured Errors
	
	
	srand( time( NULL ) );
	
	//* read input parameters
	unsigned int ulIterations = 256; //TODO 256;
	
	if ( argc >= 2 ){
		//one parameter given; get parameter max size
		ulIterations = atol( argv[1] );
		if ( ulIterations < 4 ){
			ulIterations = 4;
		}
	}
	
	cout<<endl<<"Running Test for cMessageGloveStates and subclases"<<endl;
	cout<<      "=================================================="<<endl;
	
	cout<<endl<<"Running Test for cDigitState"<<endl;
	cout<<      "============================"<<endl;
	
	++ulTestphase;
	iReturn += testDigitState( ulTestphase, ulIterations );
	
	cout<<endl<<"Running Test for cMessageGloveDigit"<<endl;
	cout<<      "==================================="<<endl;
	
	++ulTestphase;
	iReturn += testMessageGloveDigit( ulTestphase, ulIterations );
	
	cout<<endl<<"Running Test for cMessageGloveStates"<<endl;
	cout<<      "===================================="<<endl;
	
	++ulTestphase;
	iReturn += testMessageGloveStates( ulTestphase, ulIterations );
	
	
	
	if ( iReturn == 0 ){
	
		cout<<endl<<endl<<"Test successfull: no errors occoured"<<endl;
	}else{
		cerr<<endl<<endl<<"Test failed: "<<iReturn<<" errors occoured"<<endl;
	}

	return iReturn;
}


/**
 * Class for holding the member values of a cDigitState object.
 * @see cDigitState
 */
class cDigitStateValues {
public:
	
	/**
	 * The typ of the state.
	 */
	int StateTyp;
	
	/**
	 * The name of the state.
	 */
	std::string StateName;
	
	/**
	 * Parameter of the state.
	 */
	std::string Parameter;
	
	
	/**
	 * Target value of the state digit.
	 */
	long TargetValue;
	
	/**
	 * Minimum value of the state digit.
	 */
	long MinValue;
	
	/**
	 * Maximum value of the state digit.
	 */
	long MaxValue;
	
	
	/**
	 * Checks if the given given cDigitState has the same values as this
	 * object.
	 *
	 * @param inPDigitState the to check cDigitState
	 * @return the number of values not equal / the number of errors
	 */
	unsigned int checkDigitState( cDigitState * inPDigitState ) const {
		
		unsigned int uiErrors = 0;
		
		if ( inPDigitState == nullptr ) {
			
			cerr<<"Error: No cDigitState given (NULL)."<<endl;
			return 1;
		}
		
		if ( StateTyp != inPDigitState->getStateTyp() ) {
			
			cerr<<"Error: The state type is \""<<inPDigitState->getStateTyp()<<
				"\", but should be \""<<StateTyp<<"\"."<<endl;
			uiErrors++;
		}
		if ( StateName != inPDigitState->getStateName() ) {
			
			cerr<<"Error: The state name is \""<<inPDigitState->getStateName()<<
				"\", but should be \""<<StateName<<"\"."<<endl;
			uiErrors++;
		}
		if ( Parameter != inPDigitState->getParameter() ) {
			
			cerr<<"Error: The state parameters are \""<<inPDigitState->getParameter()<<
				"\", but should be \""<<Parameter<<"\"."<<endl;
			uiErrors++;
		}
		if ( TargetValue != inPDigitState->getTargetValue() ) {
			
			cerr<<"Error: The state target value is \""<<inPDigitState->getTargetValue()<<
				"\", but should be \""<<TargetValue<<"\"."<<endl;
			uiErrors++;
		}
		if ( MinValue != inPDigitState->getMinValue() ) {
			
			cerr<<"Error: The state minimum value is \""<<inPDigitState->getMinValue()<<
				"\", but should be \""<<MinValue<<"\"."<<endl;
			uiErrors++;
		}
		if ( MaxValue != inPDigitState->getMaxValue() ) {
			
			cerr<<"Error: The state maximum value is \""<<inPDigitState->getMaxValue()<<
				"\", but should be \""<<MaxValue<<"\"."<<endl;
			uiErrors++;
		}
		
		return uiErrors;
	}
	
	
};  //cDigitStateValues


/**
 * @return a random string
 */
string getRandomString() {
	
	switch ( rand() % 16 ) {
		case 0 : return "0";
		case 1 : return "next";
		case 2 : return "some text";
		case 3 : return "Österholz";
		case 4 : return "me";
		case 5 : return "Betti Österholz";
		case 6 : return "do you know more?";
		case 7 : return ";:_'*,.-#+^°";
		case 8 : return "test;semikolon;betwean; ;the;text;";
		case 9 : return "haha";
		case 10 : return "da";
		case 11 : return "yes";
		case 12 : return "no";
		case 13 : return "now";
		case 14 : return "we ";
		case 15 : return "know";
	};
	return "";
}


/**
 * Checks the serialisation (storing and loading) of the given cDigitState.
 *
 * @see cDigitState
 * @param inPDigitState the cDigitState to serialisize
 * @param correctDigitStateValues the object with the correct values of the
 * 	cDigitState members
 */
unsigned int checkSerialisationDigitState( cDigitState * inPDigitState,
		const cDigitStateValues & inCorrectDigitStateValues ) {
	
	if ( inPDigitState == nullptr ) {
		
		cerr<<"Error: No cDigitState given (NULL)."<<endl;
		return 1;
	}
	unsigned int uiErrors = 0;
	int iResultEqualCorrect = 0;
	{
		string szFileName( SzFileNamePrefix );
		szFileName += "_DS.dat";
		//store cDigitState
		ofstream * pOutFileData = new ofstream();
		pOutFileData->open( szFileName.c_str() );
		
		const bool bStoreResult = inPDigitState->store( *pOutFileData );
		pOutFileData->close();
		delete pOutFileData;
		if ( ! bStoreResult ) {
			cerr<<"ERROR: The store method returned false status. "<<
				"("<<szFileName.c_str()<<")"<<endl;
			uiErrors++;
		}
		
		//restore cDigitState
		ifstream * pIFileData = new ifstream();
		pIFileData->open(szFileName.c_str());
		
		int iOutStatus = 0;
		cDigitState * pRestoreData = cDigitState::restore( *pIFileData, &iOutStatus );
		pIFileData->close();
		delete pIFileData;
		
		if ( iOutStatus != 0 ) {
			cerr<<"ERROR: The restore method returned a error status not 0, it is "<<
				iOutStatus<<".("<<szFileName.c_str()<<")"<<endl;
			uiErrors++;
		}
		//compare with correct values
		if ( pRestoreData != NULL ) {
			iResultEqualCorrect += inCorrectDigitStateValues.checkDigitState( pRestoreData );
			uiErrors += iResultEqualCorrect;
			if ( iResultEqualCorrect != 0 ) {
				cerr<<"ERROR: While restoring cDigitState with restore().("<<szFileName.c_str()<<")"<<endl;
			}
			delete pRestoreData;
		}else{
			cerr<<"ERROR: No data loaded.("<<szFileName.c_str()<<")"<<endl;
			uiErrors++;
		}
	}
	{
		string szFileName( SzFileNamePrefix );
		szFileName += "_DS2.dat";
		//store cDigitState with "operator<<"
		ofstream * pOutFileData = new ofstream();
		pOutFileData->open( szFileName.c_str() );
		
		(*pOutFileData)<<(*inPDigitState);
		pOutFileData->close();
		delete pOutFileData;

		//restore cDigitState with operator "operator>>"
		ifstream * pIFileData2 = new ifstream( szFileName.c_str(),
			std::ifstream::in | std::ifstream::binary );

		cDigitState * pRestoreData = new cDigitState();
		(*pIFileData2)>>(*pRestoreData);
		pIFileData2->close();
		delete pIFileData2;
		
		//compare with correct values
		iResultEqualCorrect += inCorrectDigitStateValues.checkDigitState( pRestoreData );
		uiErrors += iResultEqualCorrect;
		if ( iResultEqualCorrect != 0 ) {
			cerr<<"ERROR: While restoring cDigitState with operator>>() (after store with \"operator<<\" ).("<<szFileName.c_str()<<")"<<endl;
		}
		delete pRestoreData;

		
		//restore cDigitState with "read()"
		pIFileData2 = new ifstream( szFileName.c_str(), std::ifstream::in | std::ifstream::binary );
		//pIFileData2->open(szFileName.c_str());
		
		pRestoreData = new cDigitState();
		
		pRestoreData->read( *pIFileData2 );
		pIFileData2->close();
		delete pIFileData2;
		
		//compare with correct values
		iResultEqualCorrect += inCorrectDigitStateValues.checkDigitState( pRestoreData );
		uiErrors += iResultEqualCorrect;
		if ( iResultEqualCorrect != 0 ) {
			cerr<<"ERROR: While restoring cDigitState with read().("<<szFileName.c_str()<<")"<<endl;
		}
		delete pRestoreData;
	}
	
	return uiErrors;
}



/**
 * This function tests cDigitState.
 *
 * @see cDigitState
 * @param ulTestphase the actual testphase
 * @param ulIterations the number of iterations to pervorm in the random test phases
 */
int testDigitState( const unsigned long ulTestphase, const unsigned long ulIterations ) {
	
	int iReturn = 0;
	if ( ulIterations == 0 ) {
		return 0;
	}
	
	cDigitStateValues correctDigitStateValues;
	
	for ( unsigned long ulIteration = 0; ulIteration < ulIterations; ++ulIteration ) {
		//create cDigitState with random values
		correctDigitStateValues.StateTyp  = rand() % ulIterations;
		correctDigitStateValues.StateName = getRandomString();
		correctDigitStateValues.Parameter = getRandomString();
		correctDigitStateValues.MinValue  = rand() % ulIterations - (ulIterations / 2);
		correctDigitStateValues.MaxValue  = correctDigitStateValues.MinValue +
			rand() % ulIterations;
		correctDigitStateValues.TargetValue = correctDigitStateValues.MinValue +
			rand() % ( correctDigitStateValues.MaxValue - correctDigitStateValues.MinValue + 1 );
		
		
		cDigitState * pDigitState = new cDigitState(
			correctDigitStateValues.StateTyp,
			correctDigitStateValues.StateName,
			correctDigitStateValues.Parameter,
			correctDigitStateValues.TargetValue,
			correctDigitStateValues.MinValue,
			correctDigitStateValues.MaxValue );
		
		//check values of the created cDigitState object
		iReturn += correctDigitStateValues.checkDigitState( pDigitState );
		
		//test store and restore
		iReturn += checkSerialisationDigitState( pDigitState, correctDigitStateValues );
		
		for ( unsigned long ulIteration2 = 0; ulIteration2 < ulIterations; ++ulIteration2 ) {
			//change random value of cDigitState random
			switch ( rand() % 6 ) {
				case 0 : {
					correctDigitStateValues.StateTyp  = rand() % ulIterations;
					pDigitState->setStateTyp( correctDigitStateValues.StateTyp );
				}; break;
				case 1 : {
					correctDigitStateValues.StateName = getRandomString();
					pDigitState->setStateName( correctDigitStateValues.StateName );
				}; break;
				case 2 : {
					correctDigitStateValues.Parameter = getRandomString();
					pDigitState->setParameter( correctDigitStateValues.Parameter );
				}; break;
				case 3 : {
					correctDigitStateValues.MinValue =
						correctDigitStateValues.TargetValue - rand() % ulIterations;
					pDigitState->setMinValue( correctDigitStateValues.MinValue );
				}; break;
				case 4 : {
					correctDigitStateValues.MaxValue  =
						correctDigitStateValues.TargetValue + rand() % ulIterations;
					pDigitState->setMaxValue( correctDigitStateValues.MaxValue );
				}; break;
				case 5 : {
					correctDigitStateValues.TargetValue =
						correctDigitStateValues.MinValue +
						rand() % ( correctDigitStateValues.MaxValue -
							correctDigitStateValues.MinValue + 1 );
					pDigitState->setTargetValue( correctDigitStateValues.TargetValue );
				}; break;
			};
			
			//test store and restore
			iReturn += correctDigitStateValues.checkDigitState( pDigitState );
			
			//check values of the changed cDigitState object
			iReturn += checkSerialisationDigitState( pDigitState, correctDigitStateValues );
		}
		delete pDigitState;
	}
	
	return iReturn;
}



/**
 * Class for holding the member values of a cMessageGloveDigit object.
 * @see cMessageGloveDigit
 */
class cMessageGloveDigitValues {
public:
	
	/**
	 * The typ of the digit.
	 */
	int DigitTyp;
	
	/**
	 * The name of the digit.
	 */
	std::string DigitName;
	
	/**
	 * Actual value of the digit.
	 */
	int Value;
	
	/**
	 * The value with which the data glove value is corrected
	 */
	int Correction;

	/**
	 * Vector with the neighbour states values for this digit.
	 */
	std::vector< cDigitStateValues > VecNeighbourStates;
	
	/**
	 * Index of the neighbour state, which is activ.
	 * If -1 no state is active.
	 */
	int IndexActiveNeighbour;
	
	
	
	/**
	 * Checks if the given given cMessageGloveDigit has the same values as
	 * this object.
	 *
	 * @param inPMessageGloveDigit the to check cMessageGloveDigit
	 * @return the number of values not equal / the number of errors
	 */
	unsigned int check( cMessageGloveDigit * inPMessageGloveDigit ) const {
		
		unsigned int uiErrors = 0;
		
		if ( inPMessageGloveDigit == nullptr ) {
			
			cerr<<"Error: No cMessageGloveDigit given (NULL)."<<endl;
			return 1;
		}
		
		if ( DigitTyp != inPMessageGloveDigit->getDigitTyp() ) {
			
			cerr<<"Error: The digit type is \""<<inPMessageGloveDigit->getDigitTyp()<<
				"\", but should be \""<<DigitTyp<<"\"."<<endl;
			uiErrors++;
		}
		if ( DigitName != inPMessageGloveDigit->getDigitName() ) {
			
			cerr<<"Error: The digit name is \""<<inPMessageGloveDigit->getDigitName()<<
				"\", but should be \""<<DigitName<<"\"."<<endl;
			uiErrors++;
		}
		if ( Value != inPMessageGloveDigit->getValue() ) {
			
			cerr<<"Error: The digit value is \""<<inPMessageGloveDigit->getValue()<<
				"\", but should be \""<<Value<<"\"."<<endl;
			uiErrors++;
		}
		if ( Correction != inPMessageGloveDigit->getCorrection() ) {
			
			cerr<<"Error: The digit correction value is \""<<inPMessageGloveDigit->getCorrection()<<
				"\", but should be \""<<Correction<<"\"."<<endl;
			uiErrors++;
		}
		
		if ( IndexActiveNeighbour != inPMessageGloveDigit->getIndexActiveNeighbour() ) {
			
			cerr<<"Error: The digit activ neighbour is \""<<
					inPMessageGloveDigit->getIndexActiveNeighbour()<<
				"\", but should be \""<<IndexActiveNeighbour<<"\"."<<endl;
			uiErrors++;
		}
		
		if ( VecNeighbourStates.size() != inPMessageGloveDigit->getNeighbourStateCount() ) {
			
			cerr<<"Error: The digit has \""<<
					inPMessageGloveDigit->getNeighbourStateCount()<<
				"\" neighbours, but should have \""<<VecNeighbourStates.size()<<"\"."<<endl;
			uiErrors++;
			return uiErrors;
		}
		unsigned int iuErrorsInNeighbourState = 0;
		for ( unsigned int indexNeighbourState = 0;
				indexNeighbourState < VecNeighbourStates.size();
				++indexNeighbourState ) {
			
			iuErrorsInNeighbourState = VecNeighbourStates[ indexNeighbourState ].
				checkDigitState( inPMessageGloveDigit->getNeighbourState( indexNeighbourState ) );
			
			if ( iuErrorsInNeighbourState != 0 ) {
				cerr<<"Error: "<<iuErrorsInNeighbourState<<" errors in "<<
					indexNeighbourState<<"'th neighbour state."<<endl;
				uiErrors += iuErrorsInNeighbourState;;
			}
		}
		
		return uiErrors;
	}
	
	
};  //cMessageGloveDigitValues



/**
 * Checks the serialisation (storing and loading) of the given cMessageGloveDigit.
 *
 * @see cMessageGloveDigit
 * @param inPMessageGloveDigit the cMessageGloveDigit to serialisize
 * @param correctMessageGloveDigitValues the object with the correct values of the
 * 	cMessageGloveDigit members
 */
unsigned int checkSerialisationMessageGloveDigit( cMessageGloveDigit * inPMessageGloveDigit,
		const cMessageGloveDigitValues & inCorrectMessageGloveDigitValues ) {
	
	if ( inPMessageGloveDigit == nullptr ) {
		
		cerr<<"Error: No cMessageGloveDigit given (NULL)."<<endl;
		return 1;
	}
	unsigned int uiErrors = 0;
	int iResultEqualCorrect = 0;
	{
		string szFileName( SzFileNamePrefix );
		szFileName += "_GD.dat";
		//store cMessageGloveDigit
		ofstream * pOutFileData = new ofstream();
		pOutFileData->open( szFileName.c_str() );
		
		const bool bStoreResult = inPMessageGloveDigit->store( *pOutFileData );
		pOutFileData->close();
		delete pOutFileData;
		
		if ( ! bStoreResult ) {
			cerr<<"ERROR: The store method returned false status. "<<
				"("<<szFileName.c_str()<<")"<<endl;
			uiErrors++;
		}
		
		//restore cMessageGloveDigit
		ifstream * pIFileData = new ifstream();
		pIFileData->open( szFileName.c_str() );
		
		int iOutStatus = 0;
		cMessageGloveDigit * pRestoreData = cMessageGloveDigit::restore( *pIFileData, &iOutStatus );
		pIFileData->close();
		delete pIFileData;
		
		if ( iOutStatus != 0 ) {
			cerr<<"ERROR: The restore method returned a error status not 0, it is "<<
				iOutStatus<<".("<<szFileName<<")"<<endl;
			uiErrors++;
		}
		//compare with correct values
		if ( pRestoreData != NULL ) {
			iResultEqualCorrect += inCorrectMessageGloveDigitValues.check( pRestoreData );
			uiErrors += iResultEqualCorrect;
			if ( iResultEqualCorrect != 0 ) {
				cerr<<"ERROR: While restoring cMessageGloveDigit with restore().("<<szFileName<<")"<<endl;
			}
			delete pRestoreData;
		}else{
			cerr<<"ERROR: No data loaded.("<<szFileName<<")"<<endl;
			uiErrors++;
		}
	}
	{
		//store cMessageGloveDigit with "operator<<"
		string szFileName( SzFileNamePrefix );
		szFileName += "_GD2.dat";
		
		ofstream * pOutFileData = new ofstream();
		pOutFileData->open( szFileName.c_str() );
		
		(*pOutFileData)<<(*inPMessageGloveDigit);
		pOutFileData->close();
		delete pOutFileData;

		//restore cMessageGloveDigit with operator "operator>>"
		ifstream * pIFileData2 = new ifstream( szFileName.c_str(),
			std::ifstream::in | std::ifstream::binary );

		cMessageGloveDigit * pRestoreData = new cMessageGloveDigit();
		(*pIFileData2)>>(*pRestoreData);
		pIFileData2->close();
		delete pIFileData2;
		
		//compare with correct values
		iResultEqualCorrect += inCorrectMessageGloveDigitValues.check( pRestoreData );
		uiErrors += iResultEqualCorrect;
		if ( iResultEqualCorrect != 0 ) {
			cerr<<"ERROR: While restoring cMessageGloveDigit with operator>>() (after store with \"operator<<\" ).("<<szFileName<<")"<<endl;
		}
		delete pRestoreData;

		
		//restore cMessageGloveDigit with "read()"
		pIFileData2 = new ifstream( szFileName.c_str(), std::ifstream::in | std::ifstream::binary );
		//pIFileData2->open(szFileName.c_str());
		
		pRestoreData = new cMessageGloveDigit();
		
		pRestoreData->read( *pIFileData2 );
		pIFileData2->close();
		delete pIFileData2;
		
		//compare with correct values
		iResultEqualCorrect += inCorrectMessageGloveDigitValues.check( pRestoreData );
		uiErrors += iResultEqualCorrect;
		if ( iResultEqualCorrect != 0 ) {
			cerr<<"ERROR: While restoring cMessageGloveDigit with read().("<<szFileName<<")"<<endl;
		}
		delete pRestoreData;
	}
	
	return uiErrors;
}



/**
 * This function generates a new cDigitState.
 *
 * @see cDigitState
 * @param outGeneratedDigitStateValues the object for the values of the generated cDigitState
 * @param inNumberSize a number arroud which the maximum generated number should be
 * @return the generated cDigitState
 */
cDigitState * generateDigitState( cDigitStateValues & outGeneratedDigitStateValues,
		const unsigned long inNumberSize = 1024 ) {
	
	outGeneratedDigitStateValues.StateTyp  = rand() % inNumberSize;
	outGeneratedDigitStateValues.StateName = getRandomString();
	outGeneratedDigitStateValues.Parameter = getRandomString();
	outGeneratedDigitStateValues.MinValue  = rand() % inNumberSize - (inNumberSize / 2);
	outGeneratedDigitStateValues.MaxValue  = outGeneratedDigitStateValues.MinValue +
		rand() % inNumberSize;
	outGeneratedDigitStateValues.TargetValue = outGeneratedDigitStateValues.MinValue +
		rand() % ( outGeneratedDigitStateValues.MaxValue - outGeneratedDigitStateValues.MinValue + 1 );
	
	
	return new cDigitState(
		outGeneratedDigitStateValues.StateTyp,
		outGeneratedDigitStateValues.StateName,
		outGeneratedDigitStateValues.Parameter,
		outGeneratedDigitStateValues.TargetValue,
		outGeneratedDigitStateValues.MinValue,
		outGeneratedDigitStateValues.MaxValue );
}


/**
 * This function tests cMessageGloveDigit.
 *
 * @see cMessageGloveDigit
 * @param ulTestphase the actual testphase
 * @param ulIterations the number of iterations to pervorm in the random test phases
 */
int testMessageGloveDigit( const unsigned long ulTestphase, const unsigned long ulIterations ) {
	
	int iReturn = 0;
	int iErrors = 0;
	if ( ulIterations == 0 ) {
		return 0;
	}
	
	cMessageGloveDigitValues correctGloveDigitValues;
	string szOperationsTillNow;
	
	for ( unsigned long ulIteration = 0; ulIteration < ulIterations; ++ulIteration ) {
		
		//create cMessageGloveDigit with random values
		correctGloveDigitValues.DigitTyp  = rand() % ulIterations;
		correctGloveDigitValues.DigitName = getRandomString();
		correctGloveDigitValues.Value  = rand() % ulIterations - (ulIterations / 2);
		correctGloveDigitValues.Correction = 0;
		correctGloveDigitValues.IndexActiveNeighbour = -1;
		correctGloveDigitValues.VecNeighbourStates.clear();
		
		
		cMessageGloveDigit * pGloveDigit = new cMessageGloveDigit(
			correctGloveDigitValues.DigitTyp,
			correctGloveDigitValues.DigitName,
			correctGloveDigitValues.Value );
		szOperationsTillNow = "pGloveDigit = new cMessageGloveDigit(" +
			to_string( correctGloveDigitValues.DigitTyp ) + ",\"" +
			correctGloveDigitValues.DigitName + "\"," +
			to_string( correctGloveDigitValues.Value ) + ")";
		
		//check values of the created cMessageGloveDigit object
		iReturn += correctGloveDigitValues.check( pGloveDigit );
		
		//test store and restore
		iReturn += checkSerialisationMessageGloveDigit( pGloveDigit, correctGloveDigitValues );
		
		for ( unsigned long ulIteration2 = 0; ulIteration2 < ulIterations; ++ulIteration2 ) {
			
			//change random value of cMessageGloveDigit random
			switch ( rand() % 7 ) {
				case 0 : {
					correctGloveDigitValues.DigitTyp  = rand() % ulIterations;
					pGloveDigit->setDigitTyp( correctGloveDigitValues.DigitTyp );
					szOperationsTillNow += "\npGloveDigit->setDigitTyp( " +
						to_string( correctGloveDigitValues.DigitTyp ) + " );";
				}; break;
				case 1 : {
					correctGloveDigitValues.DigitName = getRandomString();
					pGloveDigit->setDigitName( correctGloveDigitValues.DigitName );
					szOperationsTillNow += "\npGloveDigit->setDigitName( \"" +
						correctGloveDigitValues.DigitName + "\" );";
				}; break;
				case 2 : {
					correctGloveDigitValues.Value =rand() % ulIterations;
					pGloveDigit->setValue( correctGloveDigitValues.Value );
					szOperationsTillNow += "\npGloveDigit->setValue( " +
						to_string( correctGloveDigitValues.Value ) + " );";
				}; break;
				case 3 : {
					correctGloveDigitValues.Correction =rand() % ulIterations;
					pGloveDigit->setCorrection( correctGloveDigitValues.Correction );
					szOperationsTillNow += "\npGloveDigit->setCorrection( " +
						to_string( correctGloveDigitValues.Correction ) + " );";
				}; break;
				case 4 : {
					if ( 0 < correctGloveDigitValues.VecNeighbourStates.size() ) {
						correctGloveDigitValues.IndexActiveNeighbour =
							rand() % correctGloveDigitValues.VecNeighbourStates.size();
					} else {
						correctGloveDigitValues.IndexActiveNeighbour = -1;
					}
					pGloveDigit->setIndexActiveNeighbour( correctGloveDigitValues.IndexActiveNeighbour );
					szOperationsTillNow += "\npGloveDigit->setIndexActiveNeighbour( " +
						to_string( correctGloveDigitValues.IndexActiveNeighbour ) + " );";
				}; break;
				
				case 5 : {
					//create cMessageGloveDigit: generateGloveDigit() -> addNeighbourState()
					if ( MAX_LOAD_NUMBER_VECTOR_STATES <=
							correctGloveDigitValues.VecNeighbourStates.size() ) {
						continue;
					}
					cDigitStateValues generatedDigitStateValues;
					cDigitState * pNewDigitState = generateDigitState(
						generatedDigitStateValues, ulIterations );
					
					correctGloveDigitValues.VecNeighbourStates.push_back(
						generatedDigitStateValues );
					pGloveDigit->addNeighbourState( *pNewDigitState );
					
					szOperationsTillNow += "\npGloveDigit->addNeighbourState( pNewDigitState );";
					
					delete pNewDigitState;
				}; break;
				case 6 : {  //delete cMessageGloveDigit -> removeNeighbourState()
					int indexToRemove = 0;
					if ( ( rand() % 16 ) &&
							( 0 < correctGloveDigitValues.VecNeighbourStates.size() ) ) {
						
						indexToRemove = ( rand() % correctGloveDigitValues.VecNeighbourStates.size() );
					} else {  //include impossible indexes
						if ( rand() % 2 ) {
							indexToRemove = rand() % 1000000;
						} else {
							indexToRemove = 0 - rand() % 1024;
						}
					}
					
					pGloveDigit->removeNeighbourState( indexToRemove );
					szOperationsTillNow += "\npGloveDigit->removeNeighbourState( " +
						to_string( indexToRemove ) + " );";
					
					if ( ( 0 < correctGloveDigitValues.VecNeighbourStates.size() ) &&
							( 0 <= indexToRemove ) &&
							( indexToRemove < ((int)(correctGloveDigitValues.VecNeighbourStates.size())) ) ){
						std::vector< cDigitStateValues >::iterator
							itrRemove = correctGloveDigitValues.VecNeighbourStates.begin();
						while ( 0 < indexToRemove ) {
							--indexToRemove;
							++itrRemove;
						}
						correctGloveDigitValues.VecNeighbourStates.erase( itrRemove );
					}
					
				}; break;
			};
			
			//test store and restore
			iErrors = correctGloveDigitValues.check( pGloveDigit );
			if ( 0 < iErrors ) {
				iReturn += iErrors;
				cerr<<"Operations till now:"<<endl<<szOperationsTillNow<<endl;
			}
			
			//check values of the changed cMessageGloveDigit object
			iErrors = checkSerialisationMessageGloveDigit( pGloveDigit, correctGloveDigitValues );
			if ( 0 < iErrors ) {
				iReturn += iErrors;
				cerr<<"Operations till now:"<<endl<<szOperationsTillNow<<endl;
			}
		}
		delete pGloveDigit;
	}
	
	return iReturn;
}


//cMessageGloveStates

/**
 * Class for holding the member values of a cMessageGloveStates object.
 * @see cMessageGloveStates
 */
class cMessageGloveStatesValues {
public:
	
	/**
	 * The typ of the digit.
	 */
	unsigned long Clock;
	
	/**
	 * The name of the digit.
	 */
	unsigned long NumberReceivedMessage;
	
	/**
	 * Vector with the neighbour states values for this digit.
	 */
	std::vector< cMessageGloveDigitValues > VecDataGloveDigits;
	
	
	/**
	 * Checks if the given given cMessageGloveStates has the same values as
	 * this object.
	 *
	 * @param inPMessageGloveStates the to check cMessageGloveStates
	 * @return the number of values not equal / the number of errors
	 */
	unsigned int check( cMessageGloveStates * inPMessageGloveStates ) const {
		
		unsigned int uiErrors = 0;
		
		if ( inPMessageGloveStates == nullptr ) {
			
			cerr<<"Error: No cMessageGloveStates given (NULL)."<<endl;
			return 1;
		}
		
		if ( Clock != inPMessageGloveStates->getClock() ) {
			
			cerr<<"Error: The clock is \""<<inPMessageGloveStates->getClock()<<
				"\", but should be \""<<Clock<<"\"."<<endl;
			uiErrors++;
		}
		if ( NumberReceivedMessage != inPMessageGloveStates->getNumberReceivedMessage() ) {
			
			cerr<<"Error: The number of received message is \""<<
					inPMessageGloveStates->getNumberReceivedMessage()<<
				"\", but should be \""<<NumberReceivedMessage<<"\"."<<endl;
			uiErrors++;
		}
		
		if ( VecDataGloveDigits.size() != inPMessageGloveStates->getDigitCount() ) {
			
			cerr<<"Error: There are \""<<
					inPMessageGloveStates->getDigitCount()<<
				"\" digit, but it should be \""<<VecDataGloveDigits.size()<<"\"."<<endl;
			uiErrors++;
			return uiErrors;
		}
		unsigned int iuErrorsInDigits = 0;
		for ( unsigned int indexDigit = 0;
				indexDigit < VecDataGloveDigits.size();
				++indexDigit ) {
			
			iuErrorsInDigits = VecDataGloveDigits[ indexDigit ].
				check( inPMessageGloveStates->getDigit( indexDigit ) );
			
			if ( iuErrorsInDigits != 0 ) {
				cerr<<"Error: "<<iuErrorsInDigits<<" errors in "<<
					indexDigit<<"'th digit."<<endl;
				uiErrors += iuErrorsInDigits;;
			}
		}
		
		return uiErrors;
	}
	
	
};  //cMessageGloveStatesValues



/**
 * Checks the serialisation (storing and loading) of the given cMessageGloveStates.
 *
 * @see cMessageGloveStates
 * @param inPMessageGloveStates the cMessageGloveStates to serialisize
 * @param correctMessageGloveStatesValues the object with the correct values of the
 * 	cMessageGloveStates members
 */
unsigned int checkSerialisationMessageGloveStates( cMessageGloveStates * inPMessageGloveStates,
		const cMessageGloveStatesValues & inCorrectMessageGloveStatesValues ) {
	
	if ( inPMessageGloveStates == nullptr ) {
		
		cerr<<"Error: No cMessageGloveStates given (NULL)."<<endl;
		return 1;
	}
	unsigned int uiErrors = 0;
	int iResultEqualCorrect = 0;
	{
		string szFileName( SzFileNamePrefix );
		szFileName += "_GS.dat";
		//store cMessageGloveStates
		ofstream * pOutFileData = new ofstream();
		pOutFileData->open( szFileName.c_str() );
		
		const bool bStoreResult = inPMessageGloveStates->store( *pOutFileData );
		pOutFileData->close();
		delete pOutFileData;
		
		if ( ! bStoreResult ) {
			cerr<<"ERROR: The store method returned false status. "<<
				"("<<szFileName.c_str()<<")"<<endl;
			uiErrors++;
		}
		
		//restore cMessageGloveStates
		ifstream * pIFileData = new ifstream();
		pIFileData->open(szFileName.c_str());
		
		int iOutStatus = 0;
		cMessageGloveStates * pRestoreData = cMessageGloveStates::restore( *pIFileData, &iOutStatus );
		pIFileData->close();
		delete pIFileData;
		
		if ( iOutStatus != 0 ) {
			cerr<<"ERROR: The restore method returned a error status not 0, it is "<<
				iOutStatus<<".("<<szFileName.c_str()<<")"<<endl;
			uiErrors++;
		}
		//compare with correct values
		if ( pRestoreData != NULL ) {
			iResultEqualCorrect += inCorrectMessageGloveStatesValues.check( pRestoreData );
			uiErrors += iResultEqualCorrect;
			if ( iResultEqualCorrect != 0 ) {
				cerr<<"ERROR: While restoring cMessageGloveStates with restore().("<<szFileName.c_str()<<")"<<endl;
			}
			delete pRestoreData;
		}else{
			cerr<<"ERROR: No data loaded.("<<szFileName.c_str()<<")"<<endl;
			uiErrors++;
		}
	}
	{
		string szFileName( SzFileNamePrefix );
		szFileName += "_GS2.dat";
		//store cMessageGloveStates with "operator<<"
		ofstream * pOutFileData = new ofstream();
		pOutFileData->open( szFileName.c_str() );
		
		(*pOutFileData)<<(*inPMessageGloveStates);
		pOutFileData->close();
		delete pOutFileData;

		//restore cMessageGloveStates with operator "operator>>"
		ifstream * pIFileData2 = new ifstream( szFileName.c_str(),
			std::ifstream::in | std::ifstream::binary );

		cMessageGloveStates * pRestoreData = new cMessageGloveStates();
		(*pIFileData2)>>(*pRestoreData);
		pIFileData2->close();
		delete pIFileData2;
		
		//compare with correct values
		iResultEqualCorrect += inCorrectMessageGloveStatesValues.check( pRestoreData );
		uiErrors += iResultEqualCorrect;
		if ( iResultEqualCorrect != 0 ) {
			cerr<<"ERROR: While restoring cMessageGloveStates with operator>>() (after store with \"operator<<\" ).("<<szFileName.c_str()<<")"<<endl;
		}
		delete pRestoreData;

		
		//restore cMessageGloveStates with "read()"
		pIFileData2 = new ifstream( szFileName.c_str(), std::ifstream::in | std::ifstream::binary );
		//pIFileData2->open(szFileName.c_str());
		
		pRestoreData = new cMessageGloveStates();
		
		pRestoreData->read( *pIFileData2 );
		pIFileData2->close();
		delete pIFileData2;
		
		//compare with correct values
		iResultEqualCorrect += inCorrectMessageGloveStatesValues.check( pRestoreData );
		uiErrors += iResultEqualCorrect;
		if ( iResultEqualCorrect != 0 ) {
			cerr<<"ERROR: While restoring cMessageGloveStates with read().("<<szFileName.c_str()<<")"<<endl;
		}
		delete pRestoreData;
	}
	
	return uiErrors;
}


/**
 * This function generates a new cMessageGloveDigit.
 *
 * @see cMessageGloveDigit
 * @param outGeneratedMessageGloveDigitValues the object for the values of the generated cMessageGloveDigit
 * @param inNumberSize a number arroud which the maximum generated number should be
 * @return the generated cMessageGloveDigit
 */
cMessageGloveDigit * generateMessageGloveDigit(
		cMessageGloveDigitValues & outGeneratedMessageGloveDigitValues,
		const unsigned long inNumberSize = 1024  ) {
	
	//create cMessageGloveDigit with random values
	outGeneratedMessageGloveDigitValues.DigitTyp  = rand() % inNumberSize;
	outGeneratedMessageGloveDigitValues.DigitName = getRandomString();
	outGeneratedMessageGloveDigitValues.Value  = rand() % inNumberSize - (inNumberSize / 2);
	outGeneratedMessageGloveDigitValues.IndexActiveNeighbour = -1;
	outGeneratedMessageGloveDigitValues.VecNeighbourStates.clear();
	
	
	return new cMessageGloveDigit(
		outGeneratedMessageGloveDigitValues.DigitTyp,
		outGeneratedMessageGloveDigitValues.DigitName,
		outGeneratedMessageGloveDigitValues.Value );
}


/**
 * This function tests cMessageGloveStates.
 *
 * @see cMessageGloveStates
 * @param ulTestphase the actual testphase
 * @param ulIterations the number of iterations to pervorm in the random test phases
 */
int testMessageGloveStates( const unsigned long ulTestphase, const unsigned long ulIterations ) {
	
	int iReturn = 0;
	int iErrors = 0;
	if ( ulIterations == 0 ) {
		return 0;
	}
	
	cMessageGloveStatesValues correctGloveStatesValues;
	string szOperationsTillNow;
	
	for ( unsigned long ulIteration = 0; ulIteration < ulIterations; ++ulIteration ) {
		
		//create cMessageGloveStates with random values
		correctGloveStatesValues.Clock  = 0;
		correctGloveStatesValues.NumberReceivedMessage = 0;
		correctGloveStatesValues.VecDataGloveDigits.clear();
		
		
		cMessageGloveStates * pGloveStates = new cMessageGloveStates();
		szOperationsTillNow = "pGloveStates = new cMessageGloveStates()";
		
		//check values of the created cMessageGloveStates object
		iReturn += correctGloveStatesValues.check( pGloveStates );
		
		//test store and restore
		iReturn += checkSerialisationMessageGloveStates( pGloveStates, correctGloveStatesValues );
		
		for ( unsigned long ulIteration2 = 0; ulIteration2 < ulIterations; ++ulIteration2 ) {
			
			//change random value of cMessageGloveStates random
			switch ( rand() % 4 ) {
				case 0 : {
					correctGloveStatesValues.Clock  = rand() % ulIterations;
					pGloveStates->setClock( correctGloveStatesValues.Clock );
					szOperationsTillNow += "\npGloveStates->setClock( " +
						to_string( correctGloveStatesValues.Clock ) + " );";
				}; break;
				case 1 : {
					correctGloveStatesValues.NumberReceivedMessage  = rand() % ulIterations;
					pGloveStates->setNumberReceivedMessage(
						correctGloveStatesValues.NumberReceivedMessage );
					szOperationsTillNow += "\npGloveStates->setNumberReceivedMessage( " +
						to_string( correctGloveStatesValues.NumberReceivedMessage ) + " );";
				}; break;
				case 2 : {
					//create cMessageGloveStates: generateMessageGloveDigit() -> addDigit()
					if ( MAX_LOAD_NUMBER_GLOVE_DIGITS <=
							correctGloveStatesValues.VecDataGloveDigits.size() ) {
						continue;
					}
					cMessageGloveDigitValues generatedMessageGloveStatesValues;
					cMessageGloveDigit * pNewMessageGloveDigit = generateMessageGloveDigit(
						generatedMessageGloveStatesValues, ulIterations );
					
					correctGloveStatesValues.VecDataGloveDigits.push_back(
						generatedMessageGloveStatesValues );
					pGloveStates->addDigit( *pNewMessageGloveDigit );
					
					szOperationsTillNow += "\npGloveStates->addDigit( pNewMessageGloveDigit );";
					
					delete pNewMessageGloveDigit;
				}; break;
				case 3 : {  //delete cMessageGloveStates -> removeNeighbourState()
					int indexToRemove = 0;
					if ( ( rand() % 16 ) &&
							( 0 < correctGloveStatesValues.VecDataGloveDigits.size() ) ) {
						
						indexToRemove = ( rand() % correctGloveStatesValues.VecDataGloveDigits.size() );
					} else {  //include impossible indexes
						if ( rand() % 2 ) {
							indexToRemove = rand() % 1000000;
						} else {
							indexToRemove = 0 - rand() % 1024;
						}
					}
					
					pGloveStates->removeDigit( indexToRemove );
					szOperationsTillNow += "\npGloveStates->removeDigit( " +
						to_string( indexToRemove ) + " );";
					
					if ( ( 0 < correctGloveStatesValues.VecDataGloveDigits.size() ) &&
							( 0 <= indexToRemove ) &&
							( indexToRemove < ((int)(correctGloveStatesValues.VecDataGloveDigits.size())) ) ){
						std::vector< cMessageGloveDigitValues >::iterator
							itrRemove = correctGloveStatesValues.VecDataGloveDigits.begin();
						while ( 0 < indexToRemove ) {
							--indexToRemove;
							++itrRemove;
						}
						correctGloveStatesValues.VecDataGloveDigits.erase( itrRemove );
					}
					
				}; break;
			};
			
			//test store and restore
			iErrors = correctGloveStatesValues.check( pGloveStates );
			if ( 0 < iErrors ) {
				iReturn += iErrors;
				cerr<<"Operations till now:"<<endl<<szOperationsTillNow<<endl;
			}
			
			//check values of the changed cMessageGloveStates object
			iErrors = checkSerialisationMessageGloveStates( pGloveStates, correctGloveStatesValues );
			if ( 0 < iErrors ) {
				iReturn += iErrors;
				cerr<<"Operations till now:"<<endl<<szOperationsTillNow<<endl;
			}
		}
		delete pGloveStates;
	}
	
	return iReturn;
}


















