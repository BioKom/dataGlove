
//TODO check

/**
 * @file tUInputDevice
 * file name: tUInputDevice.cpp
 * @author Betti Oesterholz
 * @date 14.09.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This test is for the class cUInputDevice.
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
 * This file contains the test for the class cUInputDevice, which
 * is a class for information about a Fib object.
 *
 * You should open an empty text editor, maximize it and select it after
 * this test starts. Else the input will be not controlebel.
 *
 * TODO test more methods
 */
/*
History:
14.09.2014  Oesterholz  created
*/

#include "version.h"

#include "cUInputDevice.h"

#include <iostream>
#include <fstream>
#include <ostream>
#include <istream>
#include <ctime>
#include <stack>

#include <unistd.h>


using namespace std;


/**
 * This function tests the key pressed function keyPressed() for the given
 * key and prints the information for the key.
 *
 * @see cUInputDevice::keyPressed()
 * @param uiKey the key to test
 * @return the number of errors occured in the test
 */
int testPressed( const unsigned int uiKey );

/**
 * This function tests the two key pressed function key2Pressed() for the
 * given two keys and prints the information for the key.
 *
 * @see cUInputDevice::key2Pressed()
 * @param uiKey1 the first key to test
 * @param uiKey2 the second key to test
 * @return the number of errors occured in the test
 */
int testPressed2( const unsigned int uiKey1,
		const unsigned int uiKey2 );


int main(int argc, char* argv[]){

//	unsigned long ulTestphase = 0;//actual phase of the test 
	int iReturn = 0;//returnvalue of the test; the number of occured Errors
	
	cout<<endl<<"Running Test for cUInputDevice"<<endl;
	cout<<      "=============================="<<endl;
	cout<<"(root access needed)"<<endl<<endl;
	
	//open / create uinput device
	cUInputDevice * pUInputDevice = cUInputDevice::getInstance();
	
	if ( ! pUInputDevice->isDeviceOpen() ) {
		
		cerr<<"Error: Device could not be opened"<<endl;
		iReturn++;
		
	} else {
		
		//test keyboard
	//TODO back to 10
		const unsigned int uiDelaySeconds = 5;
		cout<<"start test keyboard in "<<uiDelaySeconds<<
			" seconds (please open an empty editor)"<<endl;
		for ( unsigned int iSeconds = uiDelaySeconds; 0 < iSeconds; --iSeconds ) {
			cout<<iSeconds<<";"<<flush;
			sleep( 1 );
		}
		cout<<endl;

		srand(time(NULL));
		cout<<"keyboard write some keys:"<<endl;
		
		if ( ! pUInputDevice->keyPressed( KEY_B ) ) {
			cerr<<"Error: Could not write key: KEY_B"<<endl;
			iReturn++;
		}
		if ( ! pUInputDevice->keyPressed( KEY_E ) ) {
			cerr<<"Error: Could not write key: KEY_E"<<endl;
			iReturn++;
		}
		if ( ! pUInputDevice->keyPressed( KEY_T ) ) {
			cerr<<"Error: Could not write key: KEY_T"<<endl;
			iReturn++;
		}
		if ( ! pUInputDevice->keyPressed( KEY_T ) ) {
			cerr<<"Error: Could not write key: KEY_T"<<endl;
			iReturn++;
		}
		if ( ! pUInputDevice->keyPressed( KEY_I ) ) {
			cerr<<"Error: Could not write key: KEY_I"<<endl;
			iReturn++;
		}
		
		list< unsigned int > liKeys;
		
		//liKeys.push_back( KEY_ENTER );
		liKeys.push_back( KEY_SPACE );
		if ( ! pUInputDevice->keysPressed( liKeys ) ) {
			cerr<<"Error: Could not write key list: KEY_SPACE"<<endl;
			iReturn++;
		}
		
		liKeys.clear();
		liKeys.push_back( KEY_RIGHTSHIFT );
		liKeys.push_back( KEY_B );
		if ( ! pUInputDevice->keysPressed( liKeys ) ) {
			cerr<<"Error: Could not write key list: KEY_RIGHTSHIFT, KEY_B"<<endl;
			iReturn++;
		}
		
		if ( ! pUInputDevice->keyPressed( KEY_X ) ) {
			cerr<<"Error: Could not write key: KEY_X"<<endl;
			iReturn++;
		}
		if ( ! pUInputDevice->keyPressed( KEY_BACKSPACE ) ) {
			cerr<<"Error: Could not write key: KEY_BACKSPACE"<<endl;
			iReturn++;
		}
		
		if ( ! pUInputDevice->keyPressed( KEY_E ) ) {
			cerr<<"Error: Could not write key: KEY_E"<<endl;
			iReturn++;
		}
		if ( ! pUInputDevice->keyPressed( KEY_T ) ) {
			cerr<<"Error: Could not write key: KEY_T"<<endl;
			iReturn++;
		}
		if ( ! pUInputDevice->keyPressed( KEY_T ) ) {
			cerr<<"Error: Could not write key: KEY_T"<<endl;
			iReturn++;
		}
		if ( ! pUInputDevice->keyPressed( KEY_I ) ) {
			cerr<<"Error: Could not write key: KEY_I"<<endl;
			iReturn++;
		}
		
		if ( ! pUInputDevice->keyPressed( KEY_SPACE ) ) {
			cerr<<"Error: Could not write key: KEY_SPACE"<<endl;
			iReturn++;
		}
		
		if ( ! pUInputDevice->key2Pressed( KEY_LEFTSHIFT, KEY_T ) ) {
			cerr<<"Error: Could not write 2 key: KEY_LEFTSHIFT and KEY_T"<<endl;
			iReturn++;
		}
		
		
		cout<<"keyboard write single key:"<<endl;
		pUInputDevice->key2Pressed( KEY_LEFTSHIFT, KEY_ENTER );
		pUInputDevice->key2Pressed( KEY_LEFTSHIFT, KEY_ENTER );
		
		//the list with the keys to test
		list< unsigned int > liCharsToTest;
		
		for ( unsigned int uiKey = 2; uiKey <= 57; ++uiKey ) {
			
			liCharsToTest.push_back( uiKey );
		}
		liCharsToTest.push_back( KEY_102ND );  //86
		liCharsToTest.push_back( KEY_KPSLASH );  //98
		liCharsToTest.push_back( KEY_KPEQUAL );  //117
		liCharsToTest.push_back( KEY_KPPLUSMINUS );  //118
		
		for ( list< unsigned int >::const_iterator
				itrKey = liCharsToTest.begin(); itrKey != liCharsToTest.end();
				++itrKey ) {
			
			if( testPressed( *itrKey ) != 0 ) {
				
				cerr<<"Error: Could not write key ("<<(*itrKey)<<")"<<endl;
				iReturn++;
			}
		}
		
		cout<<"keyboard write with shift keys:"<<endl;
		for ( list< unsigned int >::const_iterator
				itrKey = liCharsToTest.begin(); itrKey != liCharsToTest.end();
				++itrKey ) {
			
			if( testPressed2( KEY_LEFTSHIFT, *itrKey ) != 0 ) {
				
				cerr<<"Error: Could not write key with shift ("<<(*itrKey)<<")"<<endl;
				iReturn++;
			}
		}
		
	 /*
		cout<<"keyboard write with left alt keys:"<<endl;
		for ( list< unsigned int >::const_iterator
				itrKey = liCharsToTest.begin(); itrKey != liCharsToTest.end();
				++itrKey ) {
			
			if( testPressed2( KEY_LEFTALT, *itrKey ) != 0 ) {
				
				cerr<<"Error: Could not write key with left alt set ("<<(*itrKey)<<")"<<endl;
				iReturn++;
			}
		}
		cout<<"keyboard write with right alt keys:"<<endl;
		for ( list< unsigned int >::const_iterator
				itrKey = liCharsToTest.begin(); itrKey != liCharsToTest.end();
				++itrKey ) {
			
			if( testPressed2( KEY_RIGHTALT, *itrKey ) != 0 ) {
				
				cerr<<"Error: Could not write key with right alt set ("<<(*itrKey)<<")"<<endl;
				iReturn++;
			}
		}
		*/
		
		
		//TODO don't press KEY_POWER (116)
		
		/*
		for ( unsigned int uiKey = 128; uiKey < 255; ++uiKey ) {
			
			sleep( 1 );
			
			if( testPressed( uiKey ) != 0 ) {
				
				cerr<<"Error: Could not write key ("<<uiKey<<")"<<endl;
				iReturn++;
			}
		}
		for ( unsigned int uiKey = 256; uiKey < 768; ++uiKey ) {
			
			sleep( 1 );
			
			if( testPressed( uiKey ) != 0 ) {
				
				cerr<<"Error: Could not write key ("<<uiKey<<")"<<endl;
				iReturn++;
			}
		}
		*/
		//TODO write move keys
		
	
//TODO implement
	
		
		
		//test mouse
		pUInputDevice->mouseLeftClick();
		pUInputDevice->mouseRightClick();
		
		pUInputDevice->mouseMove( 20, 20 );
		pUInputDevice->mouseMoveHorizontal( 40 );
		pUInputDevice->mouseMoveV( -10 );
		pUInputDevice->mouseScroll( -7 );
		
		
	}
	
	
	if ( iReturn == 0 ){
	
		cout<<endl<<endl<<"Test successfull: no errors occoured"<<endl;
	}else{
		cerr<<endl<<endl<<"Test failed: "<<iReturn<<" errors occoured"<<endl;
	}

	return iReturn;
}


/**
 * This function types the given number.
 *
 * @param uiNumber the number to type
 * @return true if typing was sucessfull, else false
 */
bool typeNumber( unsigned int uiNumber ) {
	
	stack< unsigned int > stackLettersToWrite;
	
	unsigned int uiLetter = 0;
	
	while ( 0 < uiNumber ) {
		uiLetter = uiNumber - uiNumber / 10 * 10;
		uiNumber /= 10;
		/*convert letters:
		#define KEY_1   2
		#define KEY_2   3
		#define KEY_3   4
		#define KEY_4   5
		#define KEY_5   6
		#define KEY_6   7
		#define KEY_7   8
		#define KEY_8   9
		#define KEY_9   10
		#define KEY_0   11
		*/
		if ( uiLetter == 0 ) {
			stackLettersToWrite.push( KEY_0 );
		} else {
			stackLettersToWrite.push( uiLetter + 1 );
		}
	}
	
	cUInputDevice * pUInputDevice = cUInputDevice::getInstance();
	if ( ! pUInputDevice->isDeviceOpen() ) {
		
		cerr<<"Error: Device could not be opened"<<endl;
		return false;
	}  //else device open
	
	if ( stackLettersToWrite.empty() ) {
		//the number is 0 -> type 0
		if ( ! pUInputDevice->keyPressed( KEY_0 ) ) {
			cerr<<"Error: Could not write number key '0'"<<endl;
			return false;
		}
	} else {  //write number letters
		while ( ! stackLettersToWrite.empty() ) {
			if ( ! pUInputDevice->keyPressed( stackLettersToWrite.top() ) ) {
				cerr<<"Error: Could not write number key "<<
					stackLettersToWrite.top()<<endl;
				return false;
			}
			stackLettersToWrite.pop();
		}
	}

	return true;
}


/**
 * This function tests the key pressed function keyPressed() for the given
 * key and prints the information for the key.
 *
 * @see cUInputDevice::keyPressed()
 * @param uiKey the key to test
 * @return the number of errors occured in the test
 */
int testPressed( const unsigned int uiKey ) {
	
	int iReturn = 0;
	
	cUInputDevice * pUInputDevice = cUInputDevice::getInstance();
	
	if ( ! pUInputDevice->isDeviceOpen() ) {
		
		cerr<<"Error: Device could not be opened"<<endl;
		iReturn++;
		return iReturn;
	}  //else device open
	
	//type key number
	if ( ! typeNumber( uiKey ) ) {
		cerr<<"Error: Could not write key number"<<endl;
		iReturn++;
	}
	//print ": "
	if ( ! pUInputDevice->key2Pressed( KEY_LEFTSHIFT, KEY_DOT ) ) {
		cerr<<"Error: Could not write key ':'"<<endl;
		return false;
	}
	if ( ! pUInputDevice->keyPressed( KEY_SPACE ) ) {
		cerr<<"Error: Could not write key SPACE"<<endl;
		return false;
	}
	
	//print '"'
	if ( ! pUInputDevice->key2Pressed( KEY_LEFTSHIFT, KEY_2 ) ) {
		cerr<<"Error: Could not write key '\"'"<<endl;
		iReturn++;
	}
	
	//print the key to test
	if ( ! pUInputDevice->keyPressed( uiKey ) ) {
		cerr<<"Error: Could not write key "<<uiKey<<endl;
		iReturn++;
	}
	
	//print '"'
	if ( ! pUInputDevice->key2Pressed( KEY_LEFTSHIFT, KEY_2 ) ) {
		cerr<<"Error: Could not write key '\"'"<<endl;
		iReturn++;
	}
	
	if ( ! pUInputDevice->keyPressed( KEY_ENTER ) ) {
		cerr<<"Error: Could not write key ENTER"<<endl;
		iReturn++;
	}
	
	return iReturn;
}



/**
 * This function tests the two key pressed function key2Pressed() for the
 * given two keys and prints the information for the key.
 *
 * @see cUInputDevice::key2Pressed()
 * @param uiKey1 the first key to test
 * @param uiKey2 the second key to test
 * @return the number of errors occured in the test
 */
int testPressed2( const unsigned int uiKey1,
		const unsigned int uiKey2 ) {
	
	int iReturn = 0;
	
	cUInputDevice * pUInputDevice = cUInputDevice::getInstance();
	
	if ( ! pUInputDevice->isDeviceOpen() ) {
		
		cerr<<"Error: Device could not be opened"<<endl;
		iReturn++;
		return iReturn;
	}  //else device open
	
	//type key number
	if ( ! typeNumber( uiKey1 ) ) {
		cerr<<"Error: Could not write key number"<<endl;
		iReturn++;
	}
	//print " "
	if ( ! pUInputDevice->keyPressed( KEY_SPACE ) ) {
		cerr<<"Error: Could not write key ';'"<<endl;
		return false;
	}
	if ( ! typeNumber( uiKey2 ) ) {
		cerr<<"Error: Could not write key number"<<endl;
		iReturn++;
	}
	//print ": "
	if ( ! pUInputDevice->key2Pressed( KEY_LEFTSHIFT, KEY_DOT ) ) {
		cerr<<"Error: Could not write key ':'"<<endl;
		return false;
	}
	if ( ! pUInputDevice->keyPressed( KEY_SPACE ) ) {
		cerr<<"Error: Could not write key SPACE"<<endl;
		return false;
	}
	
	//print '"'
	if ( ! pUInputDevice->key2Pressed( KEY_LEFTSHIFT, KEY_2 ) ) {
		cerr<<"Error: Could not write key '\"'"<<endl;
		iReturn++;
	}
	
	//print the key to test
	if ( ! pUInputDevice->key2Pressed( uiKey1, uiKey2 ) ) {
		cerr<<"Error: Could not write keys "<<uiKey1<<" and "<<uiKey2<<endl;
		iReturn++;
	}
	
	//print '"'
	if ( ! pUInputDevice->key2Pressed( KEY_LEFTSHIFT, KEY_2 ) ) {
		cerr<<"Error: Could not write key '\"'"<<endl;
		iReturn++;
	}
	
	if ( ! pUInputDevice->keyPressed( KEY_ENTER ) ) {
		cerr<<"Error: Could not write key ENTER"<<endl;
		iReturn++;
	}
	
	return iReturn;
}




