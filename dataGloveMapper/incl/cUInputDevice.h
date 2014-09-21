
//TODO check

/**
 * @file cUInputDevice
 * file name: cUInputDevice.h
 * @author Betti Oesterholz
 * @date 12.09.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies the class handling a uinput device.
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
 * This file specifies the class handling an uinput device.
 * An uinput device can be used to simulate inputs from input devices
 * (like a keyboard or a mous).
 *
 * @pattern singelton
 */
/*
History:
12.09.2014  Oesterholz  created
*/


#ifndef ___C_U_INPUT_DEVICE__
#define ___C_U_INPUT_DEVICE__


#include "version.h"

#include <string>
#include <list>
#include <linux/input.h>


//forward declarations
namespace nUInputDevice{
	class cUInputDeviceDeleter;
};//end namespace nUInputDevice


class cUInputDevice {
friend class nUInputDevice::cUInputDeviceDeleter;
public:

	/**
	 * destructor
	 */
	~cUInputDevice();
	
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
	static cUInputDevice * getInstance();
	
	/**
	 * @return the name of this class "cUInputDevice"
	 */
	std::string getName() const;
	
	
	/**
	 * This method sets the uinput device input file to the given file / path.
	 * The standard device file / path is: "/dev/uinput"
	 *
	 * @param inSzDeviceFile the file path to set
	 * @see szDeviceFile
	 */
	static void setUInputDeviceFile( const std::string & inSzDeviceFile );
	
	/**
	 * @return the uinput device input file path
	 */
	std::string getUInputDeviceFile() const;
	
///uinput device file methods
	
	/**
	 * @return the uinput device input file descriptor or 0,
	 * 	if non could be opend
	 * 	@see fileDescrInputDev
	 */
	int getUInputDeviceDescriptor() const;
	
	/**
	 * @return true if the uinput device is open, else false
	 */
	bool isDeviceOpen() const;
	
	/**
	 * This function opens the uinput input device and returns the
	 * descriptor for it.
	 * It will also set the fileDescrInputDev.
	 *
	 * @see fileDescrInputDev
	 * @returns the file descriptor for the input device or 0,
	 * 	if non could be opend
	 */
	int openInputDevice();

	/**
	 * This function closes the uinput input device (if it was opened).
	 *
	 * @returns true if the uinput input device was closed, else false
	 */
	bool closeInputDevice();
	


	/**
	 * This Function writes a message / event to synchronize to the input device.
	 *
	 * @returns synchronize message / event was writen, else false
	 */
	bool writeSynchronize();
	
///functions for the keyboard
	
	/**
	 * This function writes the given key as pressed to the input device.
	 *
	 * @param inKey the key to write as pressed / set / down
	 * @returns true if the key was writen, else false
	 */
	bool writeKeyDown( const unsigned int inKey );


	/**
	 * This function writes the given key as unpressed to the input device.
	 *
	 * @param inKey the key to write as unpressed / unset / up
	 * @returns true if the key was writen, else false
	 */
	bool writeKeyUp( const unsigned int inKey );

	/**
	 * This function sets the given key as pressed (und unpressed) on the
	 * input device.
	 *
	 * @param inKey the key to set as pressed
	 * @returns true if the key was writen, else false
	 */
	bool keyPressed( const unsigned int inKey );

	/**
	 * This function sets the two given keys as pressed (und unpressed) on the
	 * input device.
	 * All keys will be pressedd down in the given order and than released
	 * in the reverted order of the list.
	 *
	 * @param inKey1 the first key to set as pressed
	 * @param inKey2 the second key to set as pressed
	 * @returns true if the key was writen, else false
	 */
	bool key2Pressed( const unsigned int inKey1,
			const unsigned int inKey2 );

	/**
	 * This function sets the given keys as pressed (und unpressed) on the
	 * input device.
	 * All keys will be pressed down in the given order and than released
	 * in the reverted order of the list.
	 *
	 * @param liKeys the keys to press
	 * @returns true if the key was writen, else false
	 */
	bool keysPressed( const std::list< unsigned int > & liKeys );
	
	
	/**
	 * This function sets the given key as down / pressed on the input device.
	 *
	 * @param inKey the key to set as pressed
	 * @returns true if the key was writen, else false
	 */
	bool keyDown( const unsigned int inKey );

	/**
	 * This function sets the two given keys as down / pressed on the input
	 * device.
	 * All keys will be pressed down in the given order of the list.
	 *
	 * @param inKey1 the first key to set as pressed
	 * @param inKey2 the second key to set as pressed
	 * @returns true if the key was writen, else false
	 */
	bool key2Down( const unsigned int inKey1,
			const unsigned int inKey2 );

	/**
	 * This function sets the given keys as down / pressed on the input
	 * device.
	 * All keys will be pressed down in the given order of the list.
	 *
	 * @param liKeys the keys to press
	 * @returns true if the key was writen, else false
	 */
	bool keysDown( const std::list< unsigned int > & liKeys );
	
	
	/**
	 * This function sets the given key as up / unpressed on the input device.
	 *
	 * @param inKey the key to set as unpressed
	 * @returns true if the key was writen, else false
	 */
	bool keyUp( const unsigned int inKey );

	/**
	 * This function sets the two given keys as up / unpressed on the input
	 * device.
	 * All keys will be unpressed up in the order of the given list.
	 *
	 * @param inKey1 the first key to set as unpressed
	 * @param inKey2 the second key to set as unpressed
	 * @returns true if the key was writen, else false
	 */
	bool key2Up( const unsigned int inKey1,
			const unsigned int inKey2 );

	/**
	 * This function sets the given keys as up / unpressed on the input
	 * device.
	 * All keys will be unpressed up in the given of the given list.
	 *
	 * @param liKeys the keys to unpress
	 * @returns true if the key was writen, else false
	 */
	bool keysUp( const std::list< unsigned int > & liKeys );
	
	
	/**
	 * This function sets the two given keys as up / unpressed on the input
	 * device.
	 * All keys will be unpressed up in the reversed order of the given list.
	 *
	 * @param inKey1 the first key to set as unpressed
	 * @param inKey2 the second key to set as unpressed
	 * @returns true if the key was writen, else false
	 */
	bool key2UpReverse( const unsigned int inKey1,
			const unsigned int inKey2 );

	/**
	 * This function sets the given keys as up / unpressed on the input
	 * device.
	 * All keys will be unpressed up in the reversed order of the given list.
	 *
	 * @param liKeys the keys to unpress
	 * @returns true if the key was writen, else false
	 */
	bool keysUpReverse( const std::list< unsigned int > & liKeys );
	
	
///functions for the mouse
	
	/**
	 * This function sets that the mouse has moved.
	 * The movement will be relativ.
	 *
	 * @param inHorizontal how far has the mouse moved horizontal
	 * @param inVertical how far has the mouse moved vertical
	 * @returns true if the move was writen, else false
	 */
	bool mouseMove( const int inHorizontal, const int inVertical );
	
	/**
	 * This function sets that the mouse has moved horizontal.
	 * The movement will be relativ.
	 *
	 * @param inHowFar how far has the mouse moved
	 * @returns true if the move was writen, else false
	 */
	bool mouseMoveHorizontal( const int inHowFar );
	
	/**
	 * This function sets that the mouse has moved horizontal.
	 * The movement will be relativ.
	 *
	 * @param inHowFar how far has the mouse moved
	 * @returns true if the move was writen, else false
	 */
	inline bool mouseMoveH( const int inHowFar ) {
		
		return mouseMoveHorizontal( inHowFar );
	}
	
	/**
	 * This function sets that the mouse has moved vertical.
	 * The movement will be relativ.
	 *
	 * @param inHowFar how far has the mouse moved
	 * @returns true if the move was writen, else false
	 */
	bool mouseMoveVertical( const int inHowFar );
	

	/**
	 * This function sets that the mouse has moved vertical.
	 * The movement will be relativ.
	 *
	 * @param inHowFar how far has the mouse moved
	 * @returns true if the move was writen, else false
	 */
	inline bool mouseMoveV( const int inHowFar ) {
		
		return mouseMoveVertical( inHowFar );
	}
	
	/**
	 * This function sets that the mouse scroll wheel has moved.
	 * The movement will be relativ.
	 *
	 * @param inHowFar how far has the mouse scroll wheel moved
	 * @returns true if the move was writen, else false
	 */
	bool mouseScroll( const int inHowFar );
	
	/**
	 * This function writes the given key as pressed to the input device.
	 *
	 * @param inAxis the axia for the move
	 * 	possible values: REL_X; REL_Y; REL_Z; REL_RX; REL_RY; REL_RZ;
	 * 		REL_HWHEEL; REL_DIAL; REL_WHEEL; REL_MISC; REL_MAX; REL_CNT

	 * @param inHowFar how far was the move
	 * @returns true if the key was writen, else false
	 */
	bool writeRelativMouseMove( const int inAxis,
			const int inHowFar );

	/**
	 * This function sets the mouse left button was pressed (und unpressed).
	 *
	 * @returns true if the button was writen, else false
	 */
	bool mouseLeftClick();

	/**
	 * This function sets the mouse right button was pressed (und unpressed).
	 *
	 * @returns true if the button was writen, else false
	 */
	bool mouseRightClick();

	/**
	 * This function sets the mouse middle button was pressed (und unpressed).
	 *
	 * @returns true if the button was writen, else false
	 */
	bool mouseMiddleClick();

	/**
	 * This function sets the mouse side button was pressed (und unpressed).
	 *
	 * @returns true if the button was writen, else false
	 */
	bool mouseSideClick();

	/**
	 * This function sets the mouse extra button was pressed (und unpressed).
	 *
	 * @returns true if the button was writen, else false
	 */
	bool mouseExtraClick();

	/**
	 * This function sets the mouse forward button was pressed (und unpressed).
	 *
	 * @returns true if the button was writen, else false
	 */
	bool mouseForwardClick();

	/**
	 * This function sets the mouse backward button was pressed (und unpressed).
	 *
	 * @returns true if the button was writen, else false
	 */
	bool mouseBackwardClick();

	/**
	 * This function sets the mouse task button was pressed (und unpressed).
	 *
	 * @returns true if the button was writen, else false
	 */
	bool mouseTaskClick();
	
	
protected:
	
	/**
	 * constructor
	 */
	cUInputDevice();
	
	
	
///members
	
	static cUInputDevice * pUInputDeviceInstance;
	
	/**
	 * The name of the device file to open, as the uinput device.
	 * @see fileDescrInputDev
	 */
	static std::string szDeviceFile;
	
	/**
	 * The file descriptor for the input device or 0, if non is or could be
	 * opend.
	 * @see szDeviceFile
	 */
	int fileDescrInputDev;
	
	
};//end class cUInputDevice



#endif //___C_U_INPUT_DEVICE__





