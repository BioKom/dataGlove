
//TODO check

/**
 * @file tMapString
 * file name: tMapString.cpp
 * @author Betti Oesterholz
 * @date 19.04.2015
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This test is for the class cMapString and cMapWString.
 *
 *
 * Copyright (C) @c GPL3 2015 Betti Oesterholz
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
 * This file contains the test for the class cMapString and cMapWString,
 * which are the classes for mapping strings into values of a type.
 *
 *
 */
/*
History:
19.04.2015  Oesterholz  created
*/

#include "version.h"

#include "cMapString.h"
#include "cMapWString.h"

#include <iostream>
#include <fstream>
#include <ostream>
#include <istream>
#include <ctime>
#include <map>

#include <unistd.h>


using namespace std;


int testMapping( const bool bWideString, const bool bOptimize );
int testMapping();


int main(int argc, char* argv[]){

//	unsigned long ulTestphase = 0;//actual phase of the test 
	int iReturn = 0;//return value of the test; the number of occured Errors
	
	cout<<endl<<"Running Test for cUInputDevice"<<endl;
	cout<<      "=============================="<<endl;
	
	iReturn += testMapping();
	
	
	if ( iReturn == 0 ){
	
		cout<<endl<<endl<<"Test successfull: no errors occoured"<<endl;
	}else{
		cerr<<endl<<endl<<"Test failed: "<<iReturn<<" errors occoured"<<endl;
	}

	return iReturn;
}



/**
 * This function tests the mapping strings into values of a type.
 *
 * @see cMapString
 * @see cMapWString
 */
int testMapping() {
	
	int iReturn = testMapping( false, false );
	iReturn += testMapping( false, true );
	iReturn += testMapping( true, false );
	iReturn += testMapping( true, true );
	
	return iReturn;
}



/**
 * This function tests the mapping strings into values of a type.
 *
 * @see cMapString
 * @see cMapWString
 * @param bOptimize should the mapping be optimized
 * @param mapMappings the mappings to check
 * @param inDefaultValue the to use default value
 * @param mapToCheckPairs the pairs which should be checked
 * 	key: the string to test
 * 	value: the mappings value wich should be return
 */
template<typename tMappingTarget>
	int testMapping( const bool bOptimize,
		const map< string, tMappingTarget > mapMappings,
		const tMappingTarget inDefaultValue,
		const map< string, tMappingTarget > mapToCheckPairs ) {
	
	
	int iReturn = 0;
	
	cMapString< tMappingTarget > mapString( inDefaultValue );
	
	if ( mapString.getName() != "cMapString" ) {
		cerr<<"Error: Class name is not \"cMapString\""<<endl;
		iReturn++;
	}
	if ( mapString.getDefaultValue() != inDefaultValue ) {
		cerr<<"Error: Wrong default value"<<endl;
		iReturn++;
	}
	//TODO getFlagsRegex()
	
	for ( typename map< string, tMappingTarget >::const_iterator
			itrMapping = mapMappings.begin();
			itrMapping != mapMappings.end(); ++itrMapping ) {
		
		mapString.setMapPair( itrMapping->first, itrMapping->second );
	}
	
	if ( bOptimize ) {
		//optimize mapper
		//TODO give uiMinNumberOfIndexChars
		mapString.optimize();
	}
	
	for ( typename map< string, tMappingTarget >::const_iterator
			itrMapping = mapToCheckPairs.begin();
			itrMapping != mapToCheckPairs.end(); ++itrMapping ) {
		
		const tMappingTarget mappingResult = mapString( itrMapping->first );
		
		if ( mappingResult != itrMapping->second ) {
			
			cerr<<"Error: The value \""<<itrMapping->first<<"\" is mapped to \""<<
				mappingResult<<"\" and not \""<<itrMapping->second<<"\""<<endl;
			iReturn++;
		}
	}
	
	
	return iReturn;
}


/**
 * This function tests the mapping strings into values of a type.
 *
 * @see cMapString
 * @see cMapWString
 * @param bOptimize should the mapping be optimized
 * @param mapMappings the mappings to check
 * @param inDefaultValue the to use default value
 * @param mapToCheckPairs the pairs which should be checked
 * 	key: the string to test
 * 	value: the mappings value wich should be return
 */
template<typename tMappingTarget>
	int testMappingWide( const bool bOptimize,
		const map< string, tMappingTarget > mapMappings,
		const tMappingTarget inDefaultValue,
		const map< string, tMappingTarget > mapToCheckPairs ) {
	
	int iReturn = 0;
	
	cMapWString< tMappingTarget > mapString( inDefaultValue );
	
	if ( mapString.getName() != "cMapWString" ) {
		cerr<<"Error: Class name is not \"cMapWString\""<<endl;
		iReturn++;
	}
	if ( mapString.getDefaultValue() != inDefaultValue ) {
		cerr<<"Error: Wrong default value"<<endl;
		iReturn++;
	}
	//TODO getFlagsRegex()
	
	for ( typename map< string, tMappingTarget >::const_iterator
			itrMapping = mapMappings.begin();
			itrMapping != mapMappings.end(); ++itrMapping ) {
		
		mapString.setMapPair( itrMapping->first, itrMapping->second );
	}
	
	if ( bOptimize ) {
		//optimize mapper
		//TODO give uiMinNumberOfIndexChars
		mapString.optimize();
	}
	
	for ( typename map< string, tMappingTarget >::const_iterator
			itrMapping = mapToCheckPairs.begin();
			itrMapping != mapToCheckPairs.end(); ++itrMapping ) {
		
		const tMappingTarget mappingResult = mapString(
			wstring( itrMapping->first.begin(), itrMapping->first.end() ) );
		
		if ( mappingResult != itrMapping->second ) {
			
			cerr<<"Error: The value \""<<itrMapping->first<<"\" is mapped to \""<<
				mappingResult<<"\" and not \""<<itrMapping->second<<"\""<<endl;
			iReturn++;
		}
	}
	
	
	return iReturn;
}




/**
 * This function tests the mapping strings into values of a type.
 *
 * @see cMapString
 * @see cMapWString
 * @param bWideString should wide strings be used
 * @param bOptimize should the mapping be optimized
 * @param mapMappings the mappings to check
 * @param inDefaultValue the to use default value
 * @param mapToCheckPairs the pairs which should be checked
 * 	key: the string to test
 * 	value: the mappings value wich should be return
 */
template<typename tMappingTarget>
	int testMapping( const bool bWideString, const bool bOptimize,
		const map< string, tMappingTarget > mapMappings,
		const tMappingTarget inDefaultValue,
		const map< string, tMappingTarget > mapToCheckPairs ) {
	
	if ( bWideString ) {
		return testMappingWide<int>( bOptimize,
			mapMappings, inDefaultValue, mapToCheckPairs );
	}
	return testMapping<int>( bOptimize,
		mapMappings, inDefaultValue, mapToCheckPairs );
}



/**
 * This function tests the mapping strings into values of a type.
 *
 * @see cMapString
 * @see cMapWString
 * @param bWideString should wide strings be used
 * @param bOptimize should the mapping be optimized
 */
int testMapping( const bool bWideString, const bool bOptimize ) {
	
	//number of errors in this test
	int iReturn = 0;
	
	cout<<endl<<"Running Testcase for "<<(bWideString ? "wstring" : "string")<<
		(bOptimize ? " optimized" : "")<<endl;
	cout<<      "============================"<<
		(bWideString ? " " : "")<<(bOptimize ? "==========" : "")<<endl;
	
	
	map< string, int > mapMappings;
	map< string, int > mapToCheckPairs;
	//create mapping
	mapMappings.insert( pair< string, int >( "1", 1 ) );
	//create pairs to check
	mapToCheckPairs.insert( pair< string, int >( "1", 1 ) );
	mapToCheckPairs.insert( pair< string, int >( "2", 0 ) );
	mapToCheckPairs.insert( pair< string, int >( "0", 0 ) );
	mapToCheckPairs.insert( pair< string, int >( "4", 0 ) );
	
	//TODO? regext flags
	
	iReturn += testMapping<int>( bWideString, bOptimize,
		mapMappings, 0, mapToCheckPairs );
	
	//create mapping
	mapMappings.clear();
	mapToCheckPairs.clear();
	//create mapping
	mapMappings.insert( pair< string, int >( "1", 1 ) );
	mapMappings.insert( pair< string, int >( "3", 3 ) );
	//create pairs to check
	mapToCheckPairs.insert( pair< string, int >( "0", 0 ) );
	mapToCheckPairs.insert( pair< string, int >( "1", 1 ) );
	mapToCheckPairs.insert( pair< string, int >( "2", 0 ) );
	mapToCheckPairs.insert( pair< string, int >( "3", 3 ) );
	mapToCheckPairs.insert( pair< string, int >( "4", 0 ) );
	
	iReturn += testMapping<int>( bWideString, bOptimize,
		mapMappings, 0, mapToCheckPairs );
	
	
	//create mapping
	mapMappings.clear();
	mapToCheckPairs.clear();
	//create mapping
	mapMappings.insert( pair< string, int >( "1", 1 ) );
	mapMappings.insert( pair< string, int >( "3", 3 ) );
	//create pairs to check
	mapToCheckPairs.insert( pair< string, int >( "0", 7 ) );
	mapToCheckPairs.insert( pair< string, int >( "1", 1 ) );
	mapToCheckPairs.insert( pair< string, int >( "2", 7 ) );
	mapToCheckPairs.insert( pair< string, int >( "3", 3 ) );
	mapToCheckPairs.insert( pair< string, int >( "4", 7 ) );
	
	iReturn += testMapping<int>( bWideString, bOptimize,
		mapMappings, 7, mapToCheckPairs );
	
	
	cout<<endl<<"Using more than 1 char"<<endl;
	cout<<      "----------------------"<<endl;
	
	//create mapping
	mapMappings.clear();
	mapToCheckPairs.clear();
	//create mapping
	mapMappings.insert( pair< string, int >( "1", 1 ) );
	mapMappings.insert( pair< string, int >( "3", 3 ) );
	mapMappings.insert( pair< string, int >( "33", 33 ) );
	//create pairs to check
	mapToCheckPairs.insert( pair< string, int >( "0", 7 ) );
	mapToCheckPairs.insert( pair< string, int >( "1", 1 ) );
	mapToCheckPairs.insert( pair< string, int >( "2", 7 ) );
	mapToCheckPairs.insert( pair< string, int >( "3", 3 ) );
	mapToCheckPairs.insert( pair< string, int >( "4", 7 ) );
	mapToCheckPairs.insert( pair< string, int >( "33", 33 ) );
	
	iReturn += testMapping<int>( bWideString, bOptimize,
		mapMappings, 7, mapToCheckPairs );
	
	
	//create mapping
	mapMappings.clear();
	mapToCheckPairs.clear();
	//create mapping
	mapMappings.insert( pair< string, int >( "1", 1 ) );
	mapMappings.insert( pair< string, int >( "3", 3 ) );
	mapMappings.insert( pair< string, int >( "33", 33 ) );
	mapMappings.insert( pair< string, int >( "Hello", 12 ) );
	//create pairs to check
	mapToCheckPairs.insert( pair< string, int >( "0", 0 ) );
	mapToCheckPairs.insert( pair< string, int >( "1", 1 ) );
	mapToCheckPairs.insert( pair< string, int >( "2", 0 ) );
	mapToCheckPairs.insert( pair< string, int >( "3", 3 ) );
	mapToCheckPairs.insert( pair< string, int >( "4", 0 ) );
	mapToCheckPairs.insert( pair< string, int >( "33", 33 ) );
	mapToCheckPairs.insert( pair< string, int >( "H", 0 ) );
	mapToCheckPairs.insert( pair< string, int >( "He", 0 ) );
	mapToCheckPairs.insert( pair< string, int >( "Hel", 0 ) );
	mapToCheckPairs.insert( pair< string, int >( "Hell", 0 ) );
	mapToCheckPairs.insert( pair< string, int >( "Hello", 12 ) );
	mapToCheckPairs.insert( pair< string, int >( "Hello ", 0 ) );
	
	iReturn += testMapping<int>( bWideString, bOptimize,
		mapMappings, 0, mapToCheckPairs );
	
	cout<<endl<<"Using wildchars"<<endl;
	cout<<      "---------------"<<endl;
	
	//create mapping
	mapMappings.clear();
	mapToCheckPairs.clear();
	//create mapping
	mapMappings.insert( pair< string, int >( "1", 1 ) );
	mapMappings.insert( pair< string, int >( "3", 3 ) );
	mapMappings.insert( pair< string, int >( "33", 33 ) );
	mapMappings.insert( pair< string, int >( "Hello", 12 ) );
	mapMappings.insert( pair< string, int >( "go .*", 13 ) );
	//create pairs to check
	mapToCheckPairs.insert( pair< string, int >( "0", 0 ) );
	mapToCheckPairs.insert( pair< string, int >( "1", 1 ) );
	mapToCheckPairs.insert( pair< string, int >( "2", 0 ) );
	mapToCheckPairs.insert( pair< string, int >( "3", 3 ) );
	mapToCheckPairs.insert( pair< string, int >( "4", 0 ) );
	mapToCheckPairs.insert( pair< string, int >( "33", 33 ) );
	mapToCheckPairs.insert( pair< string, int >( "H", 0 ) );
	mapToCheckPairs.insert( pair< string, int >( "He", 0 ) );
	mapToCheckPairs.insert( pair< string, int >( "Hel", 0 ) );
	mapToCheckPairs.insert( pair< string, int >( "Hell", 0 ) );
	mapToCheckPairs.insert( pair< string, int >( "Hello", 12 ) );
	mapToCheckPairs.insert( pair< string, int >( "Hello ", 0 ) );
	
	mapToCheckPairs.insert( pair< string, int >( "go .* ", 13 ) );
	mapToCheckPairs.insert( pair< string, int >( "go ", 13 ) );
	mapToCheckPairs.insert( pair< string, int >( "go", 0 ) );
	mapToCheckPairs.insert( pair< string, int >( "go now", 13 ) );
	mapToCheckPairs.insert( pair< string, int >( "go home", 13 ) );
	mapToCheckPairs.insert( pair< string, int >( "go.*", 0 ) );
	mapToCheckPairs.insert( pair< string, int >( "go by ", 13 ) );
	
	iReturn += testMapping<int>( bWideString, bOptimize,
		mapMappings, 0, mapToCheckPairs );
	
	
	cout<<endl<<"Using similar patterns"<<endl;
	cout<<      "----------------------"<<endl;
	
	//create mapping
	mapMappings.clear();
	mapToCheckPairs.clear();
	//create mapping
	mapMappings.insert( pair< string, int >( "KEY", 1 ) );
	mapMappings.insert( pair< string, int >( "KEY_2", 2 ) );
	mapMappings.insert( pair< string, int >( "KEY_SPACE", 3 ) );
	mapMappings.insert( pair< string, int >( "KEY_KEY", 4 ) );
	mapMappings.insert( pair< string, int >( "KE", 5 ) );
	//create pairs to check
	mapMappings.insert( pair< string, int >( "KEY", 1 ) );
	mapMappings.insert( pair< string, int >( "KEY_2", 2 ) );
	mapMappings.insert( pair< string, int >( "KEY_SPACE", 3 ) );
	mapMappings.insert( pair< string, int >( "KEY_KEY", 4 ) );
	mapMappings.insert( pair< string, int >( "KE", 5 ) );
	mapMappings.insert( pair< string, int >( "KEY ", 0 ) );
	mapMappings.insert( pair< string, int >( "K", 0 ) );
	
	
	iReturn += testMapping<int>( bWideString, bOptimize,
		mapMappings, 0, mapToCheckPairs );
	
	//create mapping
	mapMappings.clear();
	mapToCheckPairs.clear();
	//create mapping
	mapMappings.insert( pair< string, int >( "KEY", 1 ) );
	mapMappings.insert( pair< string, int >( "KEY_2", 2 ) );
	mapMappings.insert( pair< string, int >( "KEY_SPACE", 3 ) );
	mapMappings.insert( pair< string, int >( "KEY_KEY", 4 ) );
	mapMappings.insert( pair< string, int >( "KE", 5 ) );
	mapMappings.insert( pair< string, int >( "KEX_KEY", 6 ) );
	//create pairs to check
	mapMappings.insert( pair< string, int >( "KEY", 1 ) );
	mapMappings.insert( pair< string, int >( "KEY_2", 2 ) );
	mapMappings.insert( pair< string, int >( "KEY_SPACE", 3 ) );
	mapMappings.insert( pair< string, int >( "KEY_KEY", 4 ) );
	mapMappings.insert( pair< string, int >( "KE", 5 ) );
	mapMappings.insert( pair< string, int >( "KEY ", 0 ) );
	mapMappings.insert( pair< string, int >( "K", 0 ) );
	mapMappings.insert( pair< string, int >( "KEX_KEY", 6 ) );
	
	
	iReturn += testMapping<int>( bWideString, bOptimize,
		mapMappings, 0, mapToCheckPairs );
	
	
	return iReturn;
}





















