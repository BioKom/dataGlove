
//TODO check

/**
 * @file cUInputDevice
 * file name: cUInputDevice.cpp
 * @author Betti Oesterholz
 * @date 12.09.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file implements the class handling a uinput device.
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
 * This file implements the class handling an uinput device.
 * An uinput device can be used to simulate inputs from input devices
 * (like a keyboard or a mous).
 *
 * @pattern singelton
 */
/*
History:
12.09.2014  Oesterholz  created
*/

/**
 * The time to wait after a button click.
 * Comment out to disable the wait delay.
 */
#define BUTTON_WAIT_DELAY 10000


#include "cUInputDevice.h"


#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <linux/uinput.h>

//#include <iostream>


using namespace std;


cUInputDevice * cUInputDevice::pUInputDeviceInstance = NULL;

/**
 * The name of the device file to open, as the uinput device.
 * @see fileDescrInputDev
 */
std::string cUInputDevice::szDeviceFile = "/dev/uinput";



namespace nUInputDevice{

	/**
	 * This class is for deleting the uinput device handler when the programm ends.
	 * @see cUInputDevice
	 */
	class cUInputDeviceDeleter {
	public:
		
		/**
		 * destructor which deletes the uinput device handler if not allready deleted
		 */
		~cUInputDeviceDeleter() {
			
			if ( cUInputDevice::pUInputDeviceInstance != NULL ) {
				//delete node handler
				delete cUInputDevice::pUInputDeviceInstance;
				cUInputDevice::pUInputDeviceInstance = NULL;
			}
		}
		
	};//end class cFibNodeHandlerDeleter
	
	/**
	 * Object to delete the Fib node handler when the programm ends.
	 */
	cUInputDeviceDeleter uInputDeviceDeleter;
	
};//end namespace nUInputDevice




/**
 * destructor
 */
cUInputDevice::~cUInputDevice() {
	
	if ( isDeviceOpen() ) {
		closeInputDevice();
	}
}


/**
 * This method returns a pointer to an instance of the uinput device
 * handler.
 * The device file will allready be opened, if possible.
 * (The uinput path should be correct and the application needs writing
 * access to the device file.)
 *
 * @see setUInputDeviceFile()
 * @see isDeviceOpen()
 * @see pUInputDeviceInstance
 * @return a pointer to an instance of the uinput device handler
 */
cUInputDevice * cUInputDevice::getInstance() {
	
	if ( pUInputDeviceInstance == NULL ) {
		//create new uinput device handler
		pUInputDeviceInstance = new cUInputDevice();
	}
	return pUInputDeviceInstance;
}


/**
 * @return the name of this class "cUInputDevice"
 */
std::string cUInputDevice::getName() const {
	
	return "cUInputDevice";
}


/**
 * This method sets the uinput device input file to the given file / path.
 * The standard device file / path is: "/dev/uinput"
 *
 * @param inSzDeviceFile the file path to set
 * @see szDeviceFile
 */
void cUInputDevice::setUInputDeviceFile(
		const std::string & inSzDeviceFile ) {
	
	if ( szDeviceFile == inSzDeviceFile ) {
		//uinput device input file path not changed -> done
		return;
	}
	if ( ( pUInputDeviceInstance != NULL ) &&
			( pUInputDeviceInstance->isDeviceOpen() ) ) {
		//close old device
		pUInputDeviceInstance->closeInputDevice();
	}
	szDeviceFile = inSzDeviceFile;
	//open new uinput device input file
	if ( pUInputDeviceInstance != NULL ) {
		pUInputDeviceInstance->openInputDevice();
	}
}


/**
 * @return the uinput device input file path
 */
std::string cUInputDevice::getUInputDeviceFile() const {
	
	return szDeviceFile;
}


///uinput device file methods

/**
 * @return the uinput device input file descriptor or 0,
 * 	if non could be opend
 * 	@see fileDescrInputDev
 */
int cUInputDevice::getUInputDeviceDescriptor() const {
	
	return fileDescrInputDev;
}

/**
 * @return true if the uinput device is open, else false
 */
bool cUInputDevice::isDeviceOpen() const {
	
	return ( 0 <= fileDescrInputDev );
}


/**
 * This function opens the uinput input device and returns the
 * descriptor for it.
 * It will also set the fileDescrInputDev.
 *
 * @see fileDescrInputDev
 * @returns the file descriptor for the input device or 0,
 * 	if non could be opend
 */
int cUInputDevice::openInputDevice() {
	
	struct uinput_user_dev uinputUserDev;

	//fileDescrInputDev = open("/dev/input/uinput", O_WRONLY | O_NONBLOCK);
	fileDescrInputDev = open( szDeviceFile.c_str(), O_WRONLY | O_NDELAY );
	if( fileDescrInputDev < 0 ) {
		return -1;
	}

	//init keys
	if( ioctl( fileDescrInputDev, UI_SET_EVBIT, EV_KEY ) < 0 ) {
		return -1;
	}
	//setup all keys
	//KEY 0 and 255 is reserved
	for ( unsigned int uiKey = 1; uiKey < 255; ++uiKey ) {
		
		if( ioctl( fileDescrInputDev, UI_SET_KEYBIT, uiKey ) < 0 ) {
			
			return -1;
		}
	}
	for ( unsigned int uiKey = 256; uiKey < 768; ++uiKey ) {
		
		if( ioctl( fileDescrInputDev, UI_SET_KEYBIT, uiKey ) < 0 ) {
			
			return -1;
		}
	}
	
	//init mouse
	if( ioctl( fileDescrInputDev, UI_SET_EVBIT, EV_REL) < 0 ) {
		return -1;
	}
	//init mous axis
	for ( unsigned int uiAxis = 0; uiAxis < 10; ++uiAxis ) {
		
		if( ioctl( fileDescrInputDev, UI_SET_RELBIT, uiAxis ) < 0 ) {
			
			return -1;
		}
	}
	
	//synchronizing feature
	if( ioctl( fileDescrInputDev, UI_SET_EVBIT, EV_SYN ) < 0 ) {
		return -1;
	}
	
	memset(&uinputUserDev, 0, sizeof(uinputUserDev));
	snprintf(uinputUserDev.name, UINPUT_MAX_NAME_SIZE, "uinputDataGlove");
	uinputUserDev.id.bustype = BUS_USB;
	uinputUserDev.id.vendor  = 0x4669;  //"Fi"(b)
	uinputUserDev.id.product = 0x4447;  //"DG"
	uinputUserDev.id.version = 1;

	if(write( fileDescrInputDev, &uinputUserDev, sizeof(uinputUserDev) ) < 0 ) {
		return -1;
	}
	
	if( ioctl( fileDescrInputDev, UI_DEV_CREATE ) < 0 ) {
		return -1;
	}
	
#ifdef BUTTON_WAIT_DELAY
	usleep( BUTTON_WAIT_DELAY );
#endif  //BUTTON_WAIT_DELAY
 //wait 10 ms
	
	return fileDescrInputDev;
}


/**
 * This function closes the uinput input device (if it was opened).
 *
 * @returns true if the uinput input device was closed, else false
 */
bool cUInputDevice::closeInputDevice() {
	
	if ( ! isDeviceOpen() ) {
		//input device open -> close it
		close( fileDescrInputDev );
		return true;
	}
	return false;
}



/**
 * This Function writes a message / event to synchronize to the input device.
 *
 * @returns synchronize message / event was writen, else false
 */
bool cUInputDevice::writeSynchronize() {
	
	if ( ! isDeviceOpen() ) {
		//no valid input device descriptor
		return false;
	}
	//synchronize
	struct input_event inputEvent;
	memset( &inputEvent, 0, sizeof(struct input_event) );
	inputEvent.type = EV_SYN;
	inputEvent.code = SYN_REPORT;
	inputEvent.value = 0;
	return ( write( fileDescrInputDev, &inputEvent, sizeof(inputEvent))
		>= 0 );
}


///functions for the keyboard

/**
 * This function writes the given key as pressed to the input device.
 *
 * @param inKey the key to write as pressed / set / down
 * @returns true if the key was writen, else false
 */
bool cUInputDevice::writeKeyDown( const unsigned int inKey ) {
	
	if ( ! isDeviceOpen() ) {
		//no valid input device descriptor
		return false;
	}
	struct input_event inputEvent;
	memset( &inputEvent, 0, sizeof(struct input_event) );
	inputEvent.type = EV_KEY;
	inputEvent.code = inKey;
	inputEvent.value = 1;  //set key
	return ( write( fileDescrInputDev, &inputEvent, sizeof(struct input_event))
		>= 0 );
}


/**
 * This function writes the given key as unpressed to the input device.
 *
 * @param inKey the key to write as unpressed / unset / up
 * @returns true if the key was writen, else false
 */
bool cUInputDevice::writeKeyUp( const unsigned int inKey ) {
	
	if ( ! isDeviceOpen() ) {
		//no valid input device descriptor
		return false;
	}
	struct input_event inputEvent;
	memset( &inputEvent, 0, sizeof(struct input_event) );
	inputEvent.type = EV_KEY;
	inputEvent.code = inKey;
	inputEvent.value = 0;  //unset key
	return ( write( fileDescrInputDev, &inputEvent, sizeof(struct input_event))
		>= 0 );
}


/**
 * This function sets the given key as pressed (und unpressed) on the
 * input device.
 *
 * @param inKey the key to set as pressed
 * @returns true if the key was writen, else false
 */
bool cUInputDevice::keyPressed( const unsigned int inKey ) {
	
	const bool bSuccessful = writeKeyDown( inKey ) and
		writeKeyUp( inKey ) and
		writeSynchronize();
#ifdef BUTTON_WAIT_DELAY
	usleep( BUTTON_WAIT_DELAY );
#endif  //BUTTON_WAIT_DELAY
	
	return bSuccessful;
}


/**
 * This function sets the two given key as pressed (und unpressed) on the
 * input device.
 * All keys will be pressedd down in the given order and than released
 * in the reverted order of the list.
 *
 * @param inKey1 the first key to set as pressed
 * @param inKey2 the second key to set as pressed
 * @returns true if the key was writen, else false
 */
bool cUInputDevice::key2Pressed( const unsigned int inKey1,
		const unsigned int inKey2 ) {
	
	const bool bSuccessful = writeKeyDown( inKey1 ) and
		writeKeyDown( inKey2 ) and
		writeKeyUp( inKey2 ) and
		writeKeyUp( inKey1 ) and
		writeSynchronize();
#ifdef BUTTON_WAIT_DELAY
	usleep( BUTTON_WAIT_DELAY );
#endif  //BUTTON_WAIT_DELAY
	
	return bSuccessful;
}


/**
 * This function sets the given key as pressed (und unpressed) on the
 * input device.
 * All keys will be pressedd down in the given order and than released
 * in the reverted order of the list.
 *
 * @param liKeys the keys to press
 * @returns true if the key was writen, else false
 */
bool cUInputDevice::keysPressed( const list< unsigned int > & liKeys ) {
	
	for ( list< unsigned int >::const_iterator
			itrKey = liKeys.begin(); itrKey != liKeys.end(); ++itrKey ) {
		
		if ( ! writeKeyDown( *itrKey ) ) {
			return false;
		}
	}
	for ( list< unsigned int >::const_reverse_iterator
			itrKey = liKeys.rbegin(); itrKey != liKeys.rend(); ++itrKey ) {
		
		if ( ! writeKeyUp( *itrKey ) ) {
			return false;
		}
	}
	if ( ! writeSynchronize() ) {
		return false;
	}
#ifdef BUTTON_WAIT_DELAY
	usleep( BUTTON_WAIT_DELAY );
#endif  //BUTTON_WAIT_DELAY
	
	return true;
}


/**
 * This function sets the given key as down / pressed on the input device.
 *
 * @param inKey the key to set as pressed
 * @returns true if the key was writen, else false
 */
bool cUInputDevice::keyDown( const unsigned int inKey ) {
	
	const bool bSuccessful = writeKeyDown( inKey ) and
		writeSynchronize();
#ifdef BUTTON_WAIT_DELAY
	usleep( BUTTON_WAIT_DELAY );
#endif  //BUTTON_WAIT_DELAY
	
	return bSuccessful;
}


/**
 * This function sets the two given keys as down / pressed on the input
 * device.
 * All keys will be pressed down in the given order of the list.
 *
 * @param inKey1 the first key to set as pressed
 * @param inKey2 the second key to set as pressed
 * @returns true if the key was writen, else false
 */
bool cUInputDevice::key2Down( const unsigned int inKey1,
		const unsigned int inKey2 ) {
	
	const bool bSuccessful = writeKeyDown( inKey1 ) and
		writeKeyDown( inKey2 ) and
		writeSynchronize();
#ifdef BUTTON_WAIT_DELAY
	usleep( BUTTON_WAIT_DELAY );
#endif  //BUTTON_WAIT_DELAY
	
	return bSuccessful;
}


/**
 * This function sets the given keys as down / pressed on the input
 * device.
 * All keys will be pressed down in the given order of the list.
 *
 * @param liKeys the keys to press
 * @returns true if the key was writen, else false
 */
bool cUInputDevice::keysDown( const std::list< unsigned int > & liKeys ) {
	
	for ( list< unsigned int >::const_iterator
			itrKey = liKeys.begin(); itrKey != liKeys.end(); ++itrKey ) {
		
		if ( ! writeKeyDown( *itrKey ) ) {
			return false;
		}
	}
	if ( ! writeSynchronize() ) {
		return false;
	}
#ifdef BUTTON_WAIT_DELAY
	usleep( BUTTON_WAIT_DELAY );
#endif  //BUTTON_WAIT_DELAY
	
	return true;
}



/**
 * This function sets the given key as up / unpressed on the input device.
 *
 * @param inKey the key to set as unpressed
 * @returns true if the key was writen, else false
 */
bool cUInputDevice::keyUp( const unsigned int inKey ) {
	
	const bool bSuccessful = writeKeyUp( inKey ) and
		writeSynchronize();
#ifdef BUTTON_WAIT_DELAY
	usleep( BUTTON_WAIT_DELAY );
#endif  //BUTTON_WAIT_DELAY
	
	return bSuccessful;
}


/**
 * This function sets the two given keys as up / unpressed on the input
 * device.
 * All keys will be unpressed up in the order of the given list.
 *
 * @param inKey1 the first key to set as unpressed
 * @param inKey2 the second key to set as unpressed
 * @returns true if the key was writen, else false
 */
bool cUInputDevice::key2Up( const unsigned int inKey1,
		const unsigned int inKey2 ) {
	
	const bool bSuccessful = writeKeyUp( inKey1 ) and
		writeKeyUp( inKey2 ) and
		writeSynchronize();
#ifdef BUTTON_WAIT_DELAY
	usleep( BUTTON_WAIT_DELAY );
#endif  //BUTTON_WAIT_DELAY
	
	return bSuccessful;
}


/**
 * This function sets the given keys as up / unpressed on the input
 * device.
 * All keys will be unpressed up in the given of the given list.
 *
 * @param liKeys the keys to unpress
 * @returns true if the key was writen, else false
 */
bool cUInputDevice::keysUp( const std::list< unsigned int > & liKeys ) {
	
	for ( list< unsigned int >::const_iterator
			itrKey = liKeys.begin(); itrKey != liKeys.end(); ++itrKey ) {
		
		if ( ! writeKeyUp( *itrKey ) ) {
			return false;
		}
	}
	if ( ! writeSynchronize() ) {
		return false;
	}
#ifdef BUTTON_WAIT_DELAY
	usleep( BUTTON_WAIT_DELAY );
#endif  //BUTTON_WAIT_DELAY
	
	return true;
}


/**
 * This function sets the two given keys as up / unpressed on the input
 * device.
 * All keys will be unpressed up in the reversed order of the given list.
 *
 * @param inKey1 the first key to set as unpressed
 * @param inKey2 the second key to set as unpressed
 * @returns true if the key was writen, else false
 */
bool cUInputDevice::key2UpReverse( const unsigned int inKey1,
		const unsigned int inKey2 ) {
	
	const bool bSuccessful = writeKeyUp( inKey2 ) and
		writeKeyUp( inKey1 ) and
		writeSynchronize();
#ifdef BUTTON_WAIT_DELAY
	usleep( BUTTON_WAIT_DELAY );
#endif  //BUTTON_WAIT_DELAY
	
	return bSuccessful;
}


/**
 * This function sets the given keys as up / unpressed on the input
 * device.
 * All keys will be unpressed up in the reversed order of the given list.
 *
 * @param liKeys the keys to unpress
 * @returns true if the key was writen, else false
 */
bool cUInputDevice::keysUpReverse( const std::list< unsigned int > & liKeys ) {
	
	for ( list< unsigned int >::const_reverse_iterator
			itrKey = liKeys.rbegin(); itrKey != liKeys.rend(); ++itrKey ) {
		
		if ( ! writeKeyUp( *itrKey ) ) {
			return false;
		}
	}
	if ( ! writeSynchronize() ) {
		return false;
	}
#ifdef BUTTON_WAIT_DELAY
	usleep( BUTTON_WAIT_DELAY );
#endif  //BUTTON_WAIT_DELAY
	
	return true;
}


///functions for the mouse


/**
 * This function sets that the mouse has moved.
 * The movement will be relativ.
 *
 * @param inHorizontal how far has the mouse moved horizontal
 * @param inVertical how far has the mouse moved vertical
 * @returns true if the move was writen, else false
 */
bool cUInputDevice::mouseMove( const int inHorizontal, const int inVertical ) {
	
	return writeRelativMouseMove( REL_X, inHorizontal ) and
		writeRelativMouseMove( REL_Y, inVertical ) and
		writeSynchronize();
}

/**
 * This function sets that the mouse has moved horizontal.
 * The movement will be relativ.
 *
 * @param inHowFar how far has the mouse moved
 * @returns true if the move was writen, else false
 */
bool cUInputDevice::mouseMoveHorizontal( const int inHowFar ) {
	
	return writeRelativMouseMove( REL_X, inHowFar ) and
		writeSynchronize();
}


/**
 * This function sets that the mouse has moved vertical.
 * The movement will be relativ.
 *
 * @param inHowFar how far has the mouse moved
 * @returns true if the move was writen, else false
 */
bool cUInputDevice::mouseMoveVertical( const int inHowFar ) {
	
	return writeRelativMouseMove( REL_Y, inHowFar ) and
		writeSynchronize();
}


/**
 * This function sets that the mouse scroll wheel has moved.
 * The movement will be relativ.
 *
 * @param inHowFar how far has the mouse scroll wheel moved
 * @returns true if the move was writen, else false
 */
bool cUInputDevice::mouseScroll( const int inHowFar ) {
	//TODO check REL_WHEEL
	return writeRelativMouseMove( REL_WHEEL, inHowFar ) and
		writeSynchronize();
}


/**
 * This function writes the given key as pressed to the input device.
 *
 * @param inAxis the axia for the move
 * 	possible values: REL_X; REL_Y; REL_Z; REL_RX; REL_RY; REL_RZ;
 * 		REL_HWHEEL; REL_DIAL; REL_WHEEL; REL_MISC; REL_MAX; REL_CNT

 * @param inHowFar how far was the move
 * @returns true if the key was writen, else false
 */
bool cUInputDevice::writeRelativMouseMove( const int inAxis,
		const int inHowFar ) {
	
	if ( ! isDeviceOpen() ) {
		//no valid input device descriptor
		return false;
	}
	struct input_event inputEvent;
	memset( &inputEvent, 0, sizeof(struct input_event) );
	inputEvent.type = EV_REL;
	inputEvent.code = inAxis;
	inputEvent.value = inHowFar;
	return ( write( fileDescrInputDev, &inputEvent, sizeof(struct input_event))
		>= 0 );
}


/**
 * This function sets the mouse left button was pressed (und unpressed).
 *
 * @returns true if the button was writen, else false
 */
bool cUInputDevice::mouseLeftClick() {
	
	const bool bSuccessful = writeKeyDown( BTN_LEFT ) and
		writeKeyUp( BTN_LEFT ) and
		writeSynchronize();
#ifdef BUTTON_WAIT_DELAY
	usleep( BUTTON_WAIT_DELAY );
#endif  //BUTTON_WAIT_DELAY
	
	return bSuccessful;
}



/**
 * This function sets the mouse right button was pressed (und unpressed).
 *
 * @returns true if the button was writen, else false
 */
bool cUInputDevice::mouseRightClick() {
	
	const bool bSuccessful = writeKeyDown( BTN_RIGHT ) and
		writeKeyUp( BTN_RIGHT ) and
		writeSynchronize();
#ifdef BUTTON_WAIT_DELAY
	usleep( BUTTON_WAIT_DELAY );
#endif  //BUTTON_WAIT_DELAY
	
	return bSuccessful;
}



/**
 * This function sets the mouse middle button was pressed (und unpressed).
 *
 * @returns true if the button was writen, else false
 */
bool cUInputDevice::mouseMiddleClick() {
	
	const bool bSuccessful = writeKeyDown( BTN_MIDDLE ) and
		writeKeyUp( BTN_MIDDLE ) and
		writeSynchronize();
#ifdef BUTTON_WAIT_DELAY
	usleep( BUTTON_WAIT_DELAY );
#endif  //BUTTON_WAIT_DELAY
	
	return bSuccessful;
}


/**
 * This function sets the mouse side button was pressed (und unpressed).
 *
 * @returns true if the button was writen, else false
 */
bool cUInputDevice::mouseSideClick() {
	
	const bool bSuccessful = writeKeyDown( BTN_SIDE ) and
		writeKeyUp( BTN_SIDE ) and
		writeSynchronize();
#ifdef BUTTON_WAIT_DELAY
	usleep( BUTTON_WAIT_DELAY );
#endif  //BUTTON_WAIT_DELAY
	
	return bSuccessful;
}


/**
 * This function sets the mouse extra button was pressed (und unpressed).
 *
 * @returns true if the button was writen, else false
 */
bool cUInputDevice::mouseExtraClick() {
	
	const bool bSuccessful = writeKeyDown( BTN_EXTRA ) and
		writeKeyUp( BTN_EXTRA ) and
		writeSynchronize();
#ifdef BUTTON_WAIT_DELAY
	usleep( BUTTON_WAIT_DELAY );
#endif  //BUTTON_WAIT_DELAY
	
	return bSuccessful;
}


/**
 * This function sets the mouse forward button was pressed (und unpressed).
 *
 * @returns true if the button was writen, else false
 */
bool cUInputDevice::mouseForwardClick() {
	
	const bool bSuccessful = writeKeyDown( BTN_FORWARD ) and
		writeKeyUp( BTN_FORWARD ) and
		writeSynchronize();
#ifdef BUTTON_WAIT_DELAY
	usleep( BUTTON_WAIT_DELAY );
#endif  //BUTTON_WAIT_DELAY
	
	return bSuccessful;
}


/**
 * This function sets the mouse backward button was pressed (und unpressed).
 *
 * @returns true if the button was writen, else false
 */
bool cUInputDevice::mouseBackwardClick() {
	
	const bool bSuccessful = writeKeyDown( BTN_BACK ) and
		writeKeyUp( BTN_BACK ) and
		writeSynchronize();
#ifdef BUTTON_WAIT_DELAY
	usleep( BUTTON_WAIT_DELAY );
#endif  //BUTTON_WAIT_DELAY
	
	return bSuccessful;
}

/**
 * This function sets the mouse task button was pressed (und unpressed).
 *
 * @returns true if the button was writen, else false
 */
bool cUInputDevice::mouseTaskClick() {
	
	const bool bSuccessful = writeKeyDown( BTN_TASK ) and
		writeKeyUp( BTN_TASK ) and
		writeSynchronize();
#ifdef BUTTON_WAIT_DELAY
	usleep( BUTTON_WAIT_DELAY );
#endif  //BUTTON_WAIT_DELAY
	
	return bSuccessful;
}



/**
 * constructor
 */
cUInputDevice::cUInputDevice() : fileDescrInputDev( -1 ) {
	
	if ( pUInputDeviceInstance != NULL ) {
		//delete old input device
		delete pUInputDeviceInstance;
	}
	pUInputDeviceInstance = this;
	fileDescrInputDev = openInputDevice();
}








