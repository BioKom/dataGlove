
//TODO check

/**
 * @file cCallKeybordFunction
 * file name: cCallKeybordFunction.h
 * @author Betti Oesterholz
 * @date 29.08.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file implements the class for calling a keyboard function.
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
 * This file implements the class for calling a keyboard function.
 * Keyboard functions can be pressed chars (e.g. 'a', 'B', '#') or other
 * keys (e.g. Strg, F1).
 *
 * @pattern functor
 * @see cCallPrepareKeybordFunction
 * @see iCallFunction
 * @see cDataGloveState
 * @see cEvaluateDataGloveState
 */
/*
History:
29.08.2014  Oesterholz  created
*/


#include "cCallKeybordFunction.h"

#ifdef DEBUG_CALL_FUNCTION
	#include <iostream>
#endif  //DEBUG_CALL_FUNCTION


#include "cEvaluateDataGloveState.h"
#include "cCallPrepareKeybordFunction.h"
#include "cUInputDevice.h"

#ifdef FEATURE_READ_DATA_TEXT_WITH_REGEX
	#include <regex>
#endif //FEATURE_READ_DATA_TEXT_WITH_REGEX

#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	#include "cMapWString.h"
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	#include "cMapString.h"
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR



using namespace nDataGlove::nMapper;
using namespace std;


namespace nDataGlove{
namespace nMapper{
namespace nCallKeybordFunction{

	class cMapperKeybordFunction : public
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
		cMapWString<eKeybordFunction>
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
		cMapString<eKeybordFunction>
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	{
	public:
		
		cMapperKeybordFunction() :
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
#ifdef FEATURE_READ_DATA_TEXT_WITH_REGEX
			cMapWString< eKeybordFunction >( KEYBOARD_NON,
				regex_constants::icase | regex_constants::ECMAScript )
#else  //FEATURE_READ_DATA_TEXT_WITH_REGEX
			cMapWString< eKeybordFunction >( KEYBOARD_NON )
#endif  //FEATURE_READ_DATA_TEXT_WITH_REGEX
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
#ifdef FEATURE_READ_DATA_TEXT_WITH_REGEX
			cMapString< eKeybordFunction >( KEYBOARD_NON,
				regex_constants::icase | regex_constants::ECMAScript )
#else  //FEATURE_READ_DATA_TEXT_WITH_REGEX
			cMapString< eKeybordFunction >( KEYBOARD_NON )
#endif  //FEATURE_READ_DATA_TEXT_WITH_REGEX
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
		{
			setMapPair( "F1", F1 );
			setMapPair( "F2", F2 );
			setMapPair( "F3", F3 );
			setMapPair( "F4", F4 );
			setMapPair( "F5", F5 );
			setMapPair( "F6", F6 );
			setMapPair( "F7", F7 );
			setMapPair( "F8", F8 );
			setMapPair( "F9", F9 );
			setMapPair( "F10", F10 );
			setMapPair( "F11", F11 );
			setMapPair( "F12", F12 );
			
			setMapPair( "SPACE", SPACE );
			setMapPair( "TAB", TAB );
			setMapPair( "TABULATOR", TAB );
			setMapPair( "BACK_TAB", BACK_TAB );
			setMapPair( "BACK_TABULATOR", BACK_TAB );
			setMapPair( "ESC", ESC );
			setMapPair( "ESCAPE", ESC );
			setMapPair( "ENTER", ENTER );
			setMapPair( "UPPER_CASE", UPPER_CASE );
			
			setMapPair( "UP", UP );
			setMapPair( "DOWN", DOWN );
			setMapPair( "LEFT", LEFT );
			setMapPair( "RIGHT", RIGHT );
			
			setMapPair( "PAGE_UP", PAGE_UP );
			setMapPair( "PAGE UP", PAGE_UP );
			setMapPair( "PAGE_DOWN", PAGE_DOWN );
			setMapPair( "PAGE DOWN", PAGE_DOWN );
			setMapPair( "POS1", POS1 );
			setMapPair( "POSITION1", POS1 );
			setMapPair( "POS 1", POS1 );
			setMapPair( "POSITION 1", POS1 );
			setMapPair( "POS_1", POS1 );
			setMapPair( "POSITION_1", POS1 );
			setMapPair( "HOME", POS1 );
			setMapPair( "END", END );
			setMapPair( "ENDE", END );
			
			setMapPair( "DELETE", DELETE );
			setMapPair( "DEL", DELETE );
			setMapPair( "REMOVE", REMOVE );
			setMapPair( "ENTF", REMOVE );
			setMapPair( "INSERT", INSERT );
			setMapPair( "INS", INSERT );
			
			setMapPair( "ALT", ALT );
			setMapPair( "CTRL", CTRL );
			setMapPair( "STRG", CTRL );
			
			setMapPair( "PRINT", PRINT );
			setMapPair( "MENUE", MENUE );
			setMapPair( "SCROLL", SCROLL );
			setMapPair( "ROLLEN", SCROLL );
			setMapPair( "BREAK", BREAK );
			setMapPair( "PAUSE", BREAK );
			
			setMapPair( "UNDO", UNDO );
			setMapPair( "UNUNDO", UNUNDO );
			setMapPair( "REDO", UNUNDO );
			
			setMapPair( "END_APPLICATION", END_APPLICATION );
			setMapPair( "COPY", COPY );
			setMapPair( "PAST", PAST );
			
			setMapPair( "NEXT_WORD", NEXT_WORD );
			setMapPair( "PREVIOS_WORD", PREVIOS_WORD );
			setMapPair( "MARK_NEXT_WORD", MARK_NEXT_WORD );
			
			setMapPair( "QUOTE", QUOTE );
			setMapPair( "'\"'", QUOTE );
			setMapPair( "SINGLE_QUOTE", SINGLE_QUOTE );
			setMapPair( "'''", SINGLE_QUOTE );
			setMapPair( "SEMICOLON", SEMICOLON );
			setMapPair( "';'", SEMICOLON );
			
			setMapPair( "COMMA", COMMA );
			setMapPair( "SLASH", SLASH );
			setMapPair( "BACKSLASH", BACKSLASH );
			
			setMapPair( "SPECIAL_CHAR_DEU_AE", SPECIAL_CHAR_DEU_AE );
			setMapPair( "SPECIAL_CHAR_DEU_UP_AE", SPECIAL_CHAR_DEU_UP_AE );
			setMapPair( "SPECIAL_CHAR_DEU_OE", SPECIAL_CHAR_DEU_OE );
			setMapPair( "SPECIAL_CHAR_DEU_UP_OE", SPECIAL_CHAR_DEU_UP_OE );
			setMapPair( "SPECIAL_CHAR_DEU_UE", SPECIAL_CHAR_DEU_UE );
			setMapPair( "SPECIAL_CHAR_DEU_UP_UE", SPECIAL_CHAR_DEU_UP_UE );
			setMapPair( "SPECIAL_CHAR_DEU_SZ", SPECIAL_CHAR_DEU_SZ );
			
			setMapPair( "DIACRITIC_GRAVE", DIACRITIC_GRAVE );
			setMapPair( "DIACRITIC_OVERING", DIACRITIC_OVERING );
			
			
			
//TODO create const object for it ( template<return_type>FindType ? )
//  search with tree (split search prases iterativ into groups)
			
			
		}
		
		
	};  //class cMapperKeybordFunction
	
	
	
	class cMapperKeyCode : public
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
		cMapWString< unsigned int >
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
		cMapString< unsigned int >
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	{
	public:
		
		cMapperKeyCode() :
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
#ifdef FEATURE_READ_DATA_TEXT_WITH_REGEX
			cMapWString< unsigned int >( KEYBOARD_NON,
				regex_constants::icase | regex_constants::ECMAScript )
#else  //FEATURE_READ_DATA_TEXT_WITH_REGEX
			cMapWString< unsigned int >( KEYBOARD_NON )
#endif  //FEATURE_READ_DATA_TEXT_WITH_REGEX
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
#ifdef FEATURE_READ_DATA_TEXT_WITH_REGEX
			cMapString< unsigned int >( KEYBOARD_NON,
				regex_constants::icase | regex_constants::ECMAScript )
#else  //FEATURE_READ_DATA_TEXT_WITH_REGEX
			cMapString< unsigned int >( KEYBOARD_NON )
#endif  //FEATURE_READ_DATA_TEXT_WITH_REGEX
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
		{
			setMapPair( "KEY_ESC", KEY_ESC );
			setMapPair( "KEY_1", KEY_1 );
			setMapPair( "KEY_2", KEY_2 );
			setMapPair( "KEY_3", KEY_3 );
			setMapPair( "KEY_4", KEY_4 );
			setMapPair( "KEY_5", KEY_5 );
			setMapPair( "KEY_6", KEY_6 );
			setMapPair( "KEY_7", KEY_7 );
			setMapPair( "KEY_8", KEY_8 );
			setMapPair( "KEY_9", KEY_9 );
			setMapPair( "KEY_0", KEY_0 );
			setMapPair( "KEY_MINUS", KEY_MINUS );
			setMapPair( "KEY_EQUAL", KEY_EQUAL );
			setMapPair( "KEY_BACKSPACE", KEY_BACKSPACE );
			setMapPair( "KEY_TAB", KEY_TAB );
			setMapPair( "KEY_Q", KEY_Q );
			setMapPair( "KEY_W", KEY_W );
			setMapPair( "KEY_E", KEY_E );
			setMapPair( "KEY_R", KEY_R );
			setMapPair( "KEY_T", KEY_T );
			setMapPair( "KEY_Y", KEY_Y );
			setMapPair( "KEY_U", KEY_U );
			setMapPair( "KEY_I", KEY_I );
			setMapPair( "KEY_O", KEY_O );
			setMapPair( "KEY_P", KEY_P );
			setMapPair( "KEY_LEFTBRACE", KEY_LEFTBRACE );
			setMapPair( "KEY_RIGHTBRACE", KEY_RIGHTBRACE );
			setMapPair( "KEY_ENTER", KEY_ENTER );
			setMapPair( "KEY_LEFTCTRL", KEY_LEFTCTRL );
			setMapPair( "KEY_A", KEY_A );
			setMapPair( "KEY_S", KEY_S );
			setMapPair( "KEY_D", KEY_D );
			setMapPair( "KEY_F", KEY_F );
			setMapPair( "KEY_G", KEY_G );
			setMapPair( "KEY_H", KEY_H );
			setMapPair( "KEY_J", KEY_J );
			setMapPair( "KEY_K", KEY_K );
			setMapPair( "KEY_L", KEY_L );
			setMapPair( "KEY_SEMICOLON", KEY_SEMICOLON );
			setMapPair( "KEY_APOSTROPHE", KEY_APOSTROPHE );
			setMapPair( "KEY_GRAVE", KEY_GRAVE );
			setMapPair( "KEY_LEFTSHIFT", KEY_LEFTSHIFT );
			setMapPair( "KEY_SHIFT", KEY_LEFTSHIFT );
			setMapPair( "KEY_BACKSLASH", KEY_BACKSLASH );
			setMapPair( "KEY_Z", KEY_Z );
			setMapPair( "KEY_X", KEY_X );
			setMapPair( "KEY_C", KEY_C );
			setMapPair( "KEY_V", KEY_V );
			setMapPair( "KEY_B", KEY_B );
			setMapPair( "KEY_N", KEY_N );
			setMapPair( "KEY_M", KEY_M );
			setMapPair( "KEY_COMMA", KEY_COMMA );
			setMapPair( "KEY_DOT", KEY_DOT );
			setMapPair( "KEY_SLASH", KEY_SLASH );
			setMapPair( "KEY_RIGHTSHIFT", KEY_RIGHTSHIFT );
			setMapPair( "KEY_KPASTERISK", KEY_KPASTERISK );
			setMapPair( "KEY_LEFTALT", KEY_LEFTALT );
			setMapPair( "KEY_SPACE", KEY_SPACE );
			setMapPair( "KEY_CAPSLOCK", KEY_CAPSLOCK );
			setMapPair( "KEY_F1", KEY_F1 );
			setMapPair( "KEY_F2", KEY_F2 );
			setMapPair( "KEY_F3", KEY_F3 );
			setMapPair( "KEY_F4", KEY_F4 );
			setMapPair( "KEY_F5", KEY_F5 );
			setMapPair( "KEY_F6", KEY_F6 );
			setMapPair( "KEY_F7", KEY_F7 );
			setMapPair( "KEY_F8", KEY_F8 );
			setMapPair( "KEY_F9", KEY_F9 );
			setMapPair( "KEY_F10", KEY_F10 );
			setMapPair( "KEY_NUMLOCK", KEY_NUMLOCK );
			setMapPair( "KEY_SCROLLLOCK", KEY_SCROLLLOCK );
			setMapPair( "KEY_KP7", KEY_KP7 );
			setMapPair( "KEY_KP8", KEY_KP8 );
			setMapPair( "KEY_KP9", KEY_KP9 );
			setMapPair( "KEY_KPMINUS", KEY_KPMINUS );
			setMapPair( "KEY_KP4", KEY_KP4 );
			setMapPair( "KEY_KP5", KEY_KP5 );
			setMapPair( "KEY_KP6", KEY_KP6 );
			setMapPair( "KEY_KPPLUS", KEY_KPPLUS );
			setMapPair( "KEY_KP1", KEY_KP1 );
			setMapPair( "KEY_KP2", KEY_KP2 );
			setMapPair( "KEY_KP3", KEY_KP3 );
			setMapPair( "KEY_KP0", KEY_KP0 );
			setMapPair( "KEY_KPDOT", KEY_KPDOT );
			setMapPair( "KEY_ZENKAKUHANKAKU", KEY_ZENKAKUHANKAKU );
			setMapPair( "KEY_102ND", KEY_102ND );
			setMapPair( "KEY_F11", KEY_F11 );
			setMapPair( "KEY_F12", KEY_F12 );
			setMapPair( "KEY_RO", KEY_RO );
			setMapPair( "KEY_KATAKANA", KEY_KATAKANA );
			setMapPair( "KEY_HIRAGANA", KEY_HIRAGANA );
			setMapPair( "KEY_HENKAN", KEY_HENKAN );
			setMapPair( "KEY_KATAKANAHIRAGANA", KEY_KATAKANAHIRAGANA );
			setMapPair( "KEY_MUHENKAN", KEY_MUHENKAN );
			setMapPair( "KEY_KPJPCOMMA", KEY_KPJPCOMMA );
			setMapPair( "KEY_KPENTER", KEY_KPENTER );
			setMapPair( "KEY_RIGHTCTRL", KEY_RIGHTCTRL );
			setMapPair( "KEY_KPSLASH", KEY_KPSLASH );
			setMapPair( "KEY_SYSRQ", KEY_SYSRQ );
			setMapPair( "KEY_RIGHTALT", KEY_RIGHTALT );
			setMapPair( "KEY_LINEFEED", KEY_LINEFEED );
			setMapPair( "KEY_HOME", KEY_HOME );
			setMapPair( "KEY_UP", KEY_UP );
			setMapPair( "KEY_PAGEUP", KEY_PAGEUP );
			setMapPair( "KEY_LEFT", KEY_LEFT );
			setMapPair( "KEY_RIGHT", KEY_RIGHT );
			setMapPair( "KEY_END", KEY_END );
			setMapPair( "KEY_DOWN", KEY_DOWN );
			setMapPair( "KEY_PAGEDOWN", KEY_PAGEDOWN );
			setMapPair( "KEY_INSERT", KEY_INSERT );
			setMapPair( "KEY_MACRO", KEY_MACRO );
			setMapPair( "KEY_MUTE", KEY_MUTE );
			setMapPair( "KEY_VOLUMEDOWN", KEY_VOLUMEDOWN );
			setMapPair( "KEY_VOLUMEUP", KEY_VOLUMEUP );
			setMapPair( "KEY_POWER", KEY_POWER );
			setMapPair( "KEY_KPEQUAL", KEY_KPEQUAL );
			setMapPair( "KEY_KPPLUSMINUS", KEY_KPPLUSMINUS );
			setMapPair( "KEY_PAUSE", KEY_PAUSE );
			setMapPair( "KEY_SCALE", KEY_SCALE );
			
			setMapPair( "KEY_KPCOMMA", KEY_KPCOMMA );
			setMapPair( "KEY_HANGEUL", KEY_HANGEUL );
			setMapPair( "KEY_HANGUEL", KEY_HANGUEL );
			setMapPair( "KEY_HANJA", KEY_HANJA );
			setMapPair( "KEY_YEN", KEY_YEN );
			setMapPair( "KEY_LEFTMETA", KEY_LEFTMETA );
			setMapPair( "KEY_RIGHTMETA", KEY_RIGHTMETA );
			setMapPair( "KEY_COMPOSE", KEY_COMPOSE );
			
			setMapPair( "KEY_STOP", KEY_STOP );
			setMapPair( "KEY_AGAIN", KEY_AGAIN );
			setMapPair( "KEY_PROPS", KEY_PROPS );
			setMapPair( "KEY_UNDO", KEY_UNDO );
			setMapPair( "KEY_FRONT", KEY_FRONT );
			setMapPair( "KEY_COPY", KEY_COPY );
			setMapPair( "KEY_OPEN", KEY_OPEN );
			setMapPair( "KEY_PASTE", KEY_PASTE );
			setMapPair( "KEY_FIND", KEY_FIND );
			setMapPair( "KEY_CUT", KEY_CUT );
			setMapPair( "KEY_HELP", KEY_HELP );
			setMapPair( "KEY_MENU", KEY_MENU );
			setMapPair( "KEY_CALC", KEY_CALC );
			setMapPair( "KEY_SETUP", KEY_SETUP );
			setMapPair( "KEY_SLEEP", KEY_SLEEP );
			setMapPair( "KEY_WAKEUP", KEY_WAKEUP );
			setMapPair( "KEY_FILE", KEY_FILE );
			setMapPair( "KEY_SENDFILE", KEY_SENDFILE );
			setMapPair( "KEY_DELETEFILE", KEY_DELETEFILE );
			setMapPair( "KEY_XFER", KEY_XFER );
			setMapPair( "KEY_PROG1", KEY_PROG1 );
			setMapPair( "KEY_PROG2", KEY_PROG2 );
			setMapPair( "KEY_WWW", KEY_WWW );
			setMapPair( "KEY_MSDOS", KEY_MSDOS );
			setMapPair( "KEY_COFFEE", KEY_COFFEE );
			setMapPair( "KEY_SCREENLOCK", KEY_SCREENLOCK );
			setMapPair( "KEY_DIRECTION", KEY_DIRECTION );
			setMapPair( "KEY_CYCLEWINDOWS", KEY_CYCLEWINDOWS );
			setMapPair( "KEY_MAIL", KEY_MAIL );
			setMapPair( "KEY_BOOKMARKS", KEY_BOOKMARKS );
			setMapPair( "KEY_COMPUTER", KEY_COMPUTER );
			setMapPair( "KEY_BACK", KEY_BACK );
			setMapPair( "KEY_FORWARD", KEY_FORWARD );
			setMapPair( "KEY_CLOSECD", KEY_CLOSECD );
			setMapPair( "KEY_EJECTCD", KEY_EJECTCD );
			setMapPair( "KEY_EJECTCLOSECD", KEY_EJECTCLOSECD );
			setMapPair( "KEY_NEXTSONG", KEY_NEXTSONG );
			setMapPair( "KEY_PLAYPAUSE", KEY_PLAYPAUSE );
			setMapPair( "KEY_PREVIOUSSONG", KEY_PREVIOUSSONG );
			setMapPair( "KEY_STOPCD", KEY_STOPCD );
			setMapPair( "KEY_RECORD", KEY_RECORD );
			setMapPair( "KEY_REWIND", KEY_REWIND );
			setMapPair( "KEY_PHONE", KEY_PHONE );
			setMapPair( "KEY_ISO", KEY_ISO );
			setMapPair( "KEY_CONFIG", KEY_CONFIG );
			setMapPair( "KEY_HOMEPAGE", KEY_HOMEPAGE );
			setMapPair( "KEY_REFRESH", KEY_REFRESH );
			setMapPair( "KEY_EXIT", KEY_EXIT );
			setMapPair( "KEY_MOVE", KEY_MOVE );
			setMapPair( "KEY_EDIT", KEY_EDIT );
			setMapPair( "KEY_SCROLLUP", KEY_SCROLLUP );
			setMapPair( "KEY_SCROLLDOWN", KEY_SCROLLDOWN );
			setMapPair( "KEY_KPLEFTPAREN", KEY_KPLEFTPAREN );
			setMapPair( "KEY_KPRIGHTPAREN", KEY_KPRIGHTPAREN );
			setMapPair( "KEY_NEW", KEY_NEW );
			setMapPair( "KEY_REDO", KEY_REDO );
			
			setMapPair( "KEY_F13", KEY_F13 );
			setMapPair( "KEY_F14", KEY_F14 );
			setMapPair( "KEY_F15", KEY_F15 );
			setMapPair( "KEY_F16", KEY_F16 );
			setMapPair( "KEY_F17", KEY_F17 );
			setMapPair( "KEY_F18", KEY_F18 );
			setMapPair( "KEY_F19", KEY_F19 );
			setMapPair( "KEY_F20", KEY_F20 );
			setMapPair( "KEY_F21", KEY_F21 );
			setMapPair( "KEY_F22", KEY_F22 );
			setMapPair( "KEY_F23", KEY_F23 );
			setMapPair( "KEY_F24", KEY_F24 );
			
			setMapPair( "KEY_PLAYCD", KEY_PLAYCD );
			setMapPair( "KEY_PAUSECD", KEY_PAUSECD );
			setMapPair( "KEY_PROG3", KEY_PROG3 );
			setMapPair( "KEY_PROG4", KEY_PROG4 );
			setMapPair( "KEY_DASHBOARD", KEY_DASHBOARD );
			setMapPair( "KEY_SUSPEND", KEY_SUSPEND );
			setMapPair( "KEY_CLOSE", KEY_CLOSE );
			setMapPair( "KEY_PLAY", KEY_PLAY );
			setMapPair( "KEY_FASTFORWARD", KEY_FASTFORWARD );
			setMapPair( "KEY_BASSBOOST", KEY_BASSBOOST );
			setMapPair( "KEY_PRINT", KEY_PRINT );
			setMapPair( "KEY_HP", KEY_HP );
			setMapPair( "KEY_CAMERA", KEY_CAMERA );
			setMapPair( "KEY_SOUND", KEY_SOUND );
			setMapPair( "KEY_QUESTION", KEY_QUESTION );
			setMapPair( "KEY_EMAIL", KEY_EMAIL );
			setMapPair( "KEY_CHAT", KEY_CHAT );
			setMapPair( "KEY_SEARCH", KEY_SEARCH );
			setMapPair( "KEY_CONNECT", KEY_CONNECT );
			setMapPair( "KEY_FINANCE", KEY_FINANCE );
			setMapPair( "KEY_SPORT", KEY_SPORT );
			setMapPair( "KEY_SHOP", KEY_SHOP );
			setMapPair( "KEY_ALTERASE", KEY_ALTERASE );
			setMapPair( "KEY_CANCEL", KEY_CANCEL );
			setMapPair( "KEY_BRIGHTNESSDOWN", KEY_BRIGHTNESSDOWN );
			setMapPair( "KEY_BRIGHTNESSUP", KEY_BRIGHTNESSUP );
			setMapPair( "KEY_MEDIA", KEY_MEDIA );
			
			setMapPair( "KEY_SWITCHVIDEOMODE", KEY_SWITCHVIDEOMODE );
			setMapPair( "KEY_KBDILLUMTOGGLE", KEY_KBDILLUMTOGGLE );
			setMapPair( "KEY_KBDILLUMDOWN", KEY_KBDILLUMDOWN );
			setMapPair( "KEY_KBDILLUMUP", KEY_KBDILLUMUP );

			setMapPair( "KEY_SEND", KEY_SEND );
			setMapPair( "KEY_REPLY", KEY_REPLY );
			setMapPair( "KEY_FORWARDMAIL", KEY_FORWARDMAIL );
			setMapPair( "KEY_SAVE", KEY_SAVE );
			setMapPair( "KEY_DOCUMENTS", KEY_DOCUMENTS );
			
			setMapPair( "KEY_BATTERY", KEY_BATTERY );
			
			setMapPair( "KEY_BLUETOOTH", KEY_BLUETOOTH );
			setMapPair( "KEY_WLAN", KEY_WLAN );
			setMapPair( "KEY_UWB", KEY_UWB );
			
			setMapPair( "KEY_UNKNOWN", KEY_UNKNOWN );
			
			setMapPair( "KEY_VIDEO_NEXT", KEY_VIDEO_NEXT );
			setMapPair( "KEY_VIDEO_PREV", KEY_VIDEO_PREV );
			setMapPair( "KEY_BRIGHTNESS_CYCLE", KEY_BRIGHTNESS_CYCLE );
			setMapPair( "KEY_BRIGHTNESS_ZERO", KEY_BRIGHTNESS_ZERO );
			setMapPair( "KEY_DISPLAY_OFF", KEY_DISPLAY_OFF );

			setMapPair( "KEY_WIMAX", KEY_WIMAX );
			setMapPair( "KEY_RFKILL", KEY_RFKILL );
			
			setMapPair( "KEY_MICMUTE", KEY_MICMUTE );
		
		}
		
		
	};  //class cMapperKeyCode
	
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

	
};//end namespace nCallKeybordFunction

using namespace nDataGlove::nMapper::nCallKeybordFunction;

	cMapperKeybordFunction cCallKeybordFunction::mapperKeybordFunction;
	cMapperKeyCode cCallKeybordFunction::mapperKeyCode;

};//end namespace nMapper
};//end namespace nDataGlove



#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR

/**
 * constructor
 *
 * @param inSzParameters A string with parameters for the call to a key code
 * @param bInPrepareNeeded if true preparing the keyboard function is
 * 	needed, else not
 * 	@see bPrepareNeeded
 * 	@see cCallPrepareKeybordFunction
 */
cCallKeybordFunction::cCallKeybordFunction( const std::wstring inSzParameters,
		const bool bInPrepareNeeded ) :
		keybordFunction( KEYBOARD_NON ),
		keybordFunctionForOperator( KEYBOARD_NON ),
		keyCode1( 0 ), keyCode2( 0 ), cChar( 0x0 ),
		bPrepareNeeded( bInPrepareNeeded ) {
	
	analyseAndSetParameters( inSzParameters );
}


/**
 * constructor
 *
 * @param cChar The char/key, which will be prepared with this operator.
 * 	@see cChar
 * 	@see keybordFunction
 * @param bInPrepareNeeded if true preparing the keyboard function is
 * 	needed, else not
 * 	@see bPrepareNeeded
 * 	@see cCallPrepareKeybordFunction
 */
cCallKeybordFunction::cCallKeybordFunction( const wchar_t cInChar,
	const bool bInPrepareNeeded ) :
		keybordFunction( CHAR ), keyCode1( 0 ), keyCode2( 0 ),
		cChar( cInChar ), bPrepareNeeded( bInPrepareNeeded ) {
	
	charToOperatorFunction( cInChar );
}

#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR


/**
 * constructor
 *
 * @param inSzParameters A string with parameters for the call to a key code
 * @param bInPrepareNeeded if true preparing the keyboard function is
 * 	needed, else not
 * 	@see bPrepareNeeded
 * 	@see cCallPrepareKeybordFunction
 */
cCallKeybordFunction::cCallKeybordFunction( const std::string inSzParameters,
		const bool bInPrepareNeeded ) :
		keybordFunction( KEYBOARD_NON ),
		keybordFunctionForOperator( KEYBOARD_NON ),
		keyCode1( 0 ), keyCode2( 0 ), cChar( 0x0 ),
		bPrepareNeeded( bInPrepareNeeded ) {
	
	analyseAndSetParameters( wstring(
		inSzParameters.begin(), inSzParameters.end() ) );
}



/**
 * constructor
 *
 * @param inKeyCode1 (First) Key code to call (like defined in <linux/input.h>)
 * 	@see keyCode1
 * @param bInPrepareNeeded if true preparing the keyboard function is
 * 	needed, else not
 * 	@see bPrepareNeeded
 * 	@see cCallPrepareKeybordFunction
 */
cCallKeybordFunction::cCallKeybordFunction( const int inKeyCode1,
		const bool bInPrepareNeeded ) :
		keybordFunction( INPUT_KEY_CODE_1 ),
		keybordFunctionForOperator( INPUT_KEY_CODE_1 ),
		keyCode1( inKeyCode1 ),
		keyCode2( 0 ), cChar( 0x0 ), bPrepareNeeded( bInPrepareNeeded ) {
	
	liKeys.push_back( keyCode1 );
}


/**
 * constructor
 *
 * @param inKeyCode1 First key code to call (like defined in <linux/input.h>)
 * 	@see keyCode1
 * @param inKeyCode1 second key code to call (like defined in <linux/input.h>)
 * 	@see keyCode2
 * @param bInPrepareNeeded if true preparing the keyboard function is
 * 	needed, else not
 * 	@see bPrepareNeeded
 * 	@see cCallPrepareKeybordFunction
 */
cCallKeybordFunction::cCallKeybordFunction( const int inKeyCode1,
		const int inKeyCode2,
		const bool bInPrepareNeeded ) :
		keybordFunction( INPUT_KEY_CODE_2 ),
		keybordFunctionForOperator( INPUT_KEY_CODE_2 ),
		keyCode1( inKeyCode1 ),
		keyCode2( inKeyCode2 ), cChar( 0x0 ), bPrepareNeeded( bInPrepareNeeded ) {
	
	liKeys.push_back( keyCode1 );
	liKeys.push_back( keyCode2 );
}


/**
 * constructor
 *
 * @param inLiKeys A list with key codes to call (like defined in <linux/input.h>)
 * 	@see keyCode1
 * @param bInPrepareNeeded if true preparing the keyboard function is
 * 	needed, else not
 * 	@see bPrepareNeeded
 * 	@see cCallPrepareKeybordFunction
 */
cCallKeybordFunction::cCallKeybordFunction( const std::list< unsigned int > inLiKeys,
	const bool bInPrepareNeeded ) :
		keybordFunction( INPUT_KEY_CODE ),
		keybordFunctionForOperator( INPUT_KEY_CODE ),
		keyCode1( 0 ), keyCode2( 0 ), liKeys( inLiKeys ),
		cChar( 0x0 ), bPrepareNeeded( bInPrepareNeeded ) {
	
	std::list< unsigned int >::const_iterator itrKey = liKeys.begin();
	if ( itrKey != liKeys.end() ) {
		//set first key code
		keyCode1 = (*itrKey);
		itrKey++;
		if ( itrKey != liKeys.end() ) {
			//set second key code
			keyCode2 = (*itrKey);
		}
	}
}


/**
 * constructor
 *
 * @param KeybordFunction The keyboard function, which will be prepared
 * 	with this operator.
 * 	@see keybordFunction
 * @param bInPrepareNeeded if true preparing the keyboard function is
 * 	needed, else not
 * 	@see bPrepareNeeded
 * 	@see cCallPrepareKeybordFunction
 */
cCallKeybordFunction::cCallKeybordFunction( const eKeybordFunction inKeybordFunction,
	const bool bInPrepareNeeded ) :
		keybordFunction( inKeybordFunction ), keyCode1( 0 ), keyCode2( 0 ),
		cChar( 0x0 ), bPrepareNeeded( bInPrepareNeeded ) {
	
	keyboardFunctionToOperatorFunction( inKeybordFunction );
}


/**
 * constructor
 *
 * @param cChar The char/key, which will be prepared with this operator.
 * 	@see cChar
 * 	@see keybordFunction
 * @param bInPrepareNeeded if true preparing the keyboard function is
 * 	needed, else not
 * 	@see bPrepareNeeded
 * 	@see cCallPrepareKeybordFunction
 */
cCallKeybordFunction::cCallKeybordFunction( const char cInChar,
	const bool bInPrepareNeeded ) :
		keybordFunction( CHAR ), keyCode1( 0 ), keyCode2( 0 ),
		cChar( cInChar ), bPrepareNeeded( bInPrepareNeeded ) {
	
	charToOperatorFunction( cInChar );
}



/**
 * destructor
 */
cCallKeybordFunction::~cCallKeybordFunction() {
	//nothing to do
}


/**
 * @return the name of this class "cCallKeybordFunction"
 */
std::string cCallKeybordFunction::getName() const {
	return std::string("cCallKeybordFunction");
}


/**
 * This functor operator is called the execute the function implemented
 * by this object.
 *
 * @return true if the call to the function was sucessfull, else false
 */
bool cCallKeybordFunction::operator()() {
	
	if ( bPrepareNeeded &&
			( ( cCallPrepareKeybordFunction::getPreparedFunction() == NULL ) ||
			( ! cCallPrepareKeybordFunction::getPreparedFunction()->
				IsPrepared( this ) ) ) ) {
		//keyboard function was not prepared, but should be
		//call parent operator (unprepare)
#ifdef DEBUG_CALL_FUNCTION
		cout<<"keyboard function was not prepared: "<<keybordFunction<<
			" ("<<keybordFunctionForOperator<<"; '"<<cChar<<"'; "<<
			keyCode1<<" "<<keyCode2<<")"<<endl;
#endif  //DEBUG_CALL_FUNCTION
		iCallFunction::operator()();
		return false;
	}
	//call keybord function
	
	//open / create uinput device
	cUInputDevice * pUInputDevice = cUInputDevice::getInstance();
	
	if ( ! pUInputDevice->isDeviceOpen() ) {
		return false;  //Error: Device could not be opened
	}
	
	//press keys
	switch ( keybordFunctionForOperator ) {
		case INPUT_KEY_CODE : {
			pUInputDevice->keysDown( liKeys );
		}; break;
		case INPUT_KEY_CODE_1 : {
			pUInputDevice->keyDown( keyCode1 );
		}; break;
		case INPUT_KEY_CODE_2 : {
			pUInputDevice->key2Down( keyCode1, keyCode2 );
		}; break;
		default: break;  //do nothing
	};  //end switch keybordFunctionForOperator
	
#ifdef DEBUG_CALL_FUNCTION
	cout<<"calling keyboard function: "<<keybordFunction<<
		" ("<<keybordFunctionForOperator<<"; '"<<cChar<<"'; "<<
		keyCode1<<" "<<keyCode2<<")"<<endl;
#endif  //DEBUG_CALL_FUNCTION
	return true;
}


/**
 * This method ends the operation.
 * (Call if the state changes.)
 */
void cCallKeybordFunction::end() {
	
	//open / create uinput device
	cUInputDevice * pUInputDevice = cUInputDevice::getInstance();
	
	if ( ! pUInputDevice->isDeviceOpen() ) {
		return;  //Error: Device could not be opened
	}
	//unpress keys
	switch ( keybordFunctionForOperator ) {
		case INPUT_KEY_CODE : {
			pUInputDevice->keysUpReverse( liKeys );
		}; break;
		case INPUT_KEY_CODE_1 : {
			pUInputDevice->keyUp( keyCode1 );
		}; break;
		case INPUT_KEY_CODE_2 : {
			pUInputDevice->key2UpReverse( keyCode1, keyCode2 );
		}; break;
		default: break;  //do nothing
	};  //end switch keybordFunctionForOperator
#ifdef DEBUG_CALL_FUNCTION
	cout<<"end calling keyboard function: "<<keybordFunction<<
		" ("<<keybordFunctionForOperator<<"; '"<<cChar<<"'; "<<
		keyCode1<<" "<<keyCode2<<")"<<endl;
#endif  //DEBUG_CALL_FUNCTION
}


/**
 * This method sets the parameter for the operation.
 * The parameter are readed from the given string.
 *
 * @param szFunctionParameter all parameters for this functor operation
 * 	as a string
 * @return true if the parameter could be set, else false
 */
bool cCallKeybordFunction::setParameter( const string & szFunctionParameter ) {
	
	return analyseAndSetParameters( wstring(
		szFunctionParameter.begin(), szFunctionParameter.end() ) );
}


#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR

/**
 * This method sets the parameter for the operation.
 * The parameter are readed from the given string.
 *
 * @param szFunctionParameter all parameters for this functor operation
 * 	as a string
 * @return true if the parameter could be set, else false
 */
bool cCallKeybordFunction::setParameter(
		const wstring & szFunctionParameter ) {
	
	return analyseAndSetParameters( szFunctionParameter );
}

#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR


/**
 * @return The keyboard function, which will be prepared
 * 	with this operator.
 * 	If it is "CHAR" cChar contains the to output char.
 * 	@see keybordFunction
 */
eKeybordFunction cCallKeybordFunction::getKeybordFunction() const {
	
	return keybordFunction;
}


/**
 * @return First key code like defined in <linux/input.h> or 0 if non is used.
 * 	Just valid if keybordFunction is "INPUT_KEY_CODE_1" or "INPUT_KEY_CODE_2".
 * 	@see keyCode1
 * @see keybordFunction
 * @see eKeybordFunction::INPUT_KEY_CODE_1
 * @see eKeybordFunction::INPUT_KEY_CODE_2
 */
int cCallKeybordFunction::getKeyCode1() const {
	
	return keyCode1;
}


/**
 * @return Second key code like defined in <linux/input.h> or 0 if non is used.
 * 	Just valid if keybordFunction is "INPUT_KEY_CODE_2".
 * 	@see keyCode2
 * @see keybordFunction
 * @see eKeybordFunction::INPUT_KEY_CODE_2
 */
int cCallKeybordFunction::getKeyCode2() const {
	
	return keyCode2;
}


/**
 * @return A list of key codes like defined in <linux/input.h> or empty if non is used.
 * 	Just valid if keybordFunction is "INPUT_KEY_CODE".
 * 	@see liKeys
 * @see keybordFunction
 * @see eKeybordFunction::INPUT_KEY_CODE
 */
std::list< unsigned int > cCallKeybordFunction::getKeyCodes() const {
	
	return liKeys;
}



/**
 * @return If true preparing the modus change is needed, else not.
 * 	@see bPrepareNeeded
 */
bool cCallKeybordFunction::getPrepareNeeded() const {
	
	return bPrepareNeeded;
}



/**
 * @return The char/key, which will be prepared with this operator.
 * 	Just valid if keybordFunction is "CHAR".
 * 	@see cChar
 * 	@see keybordFunction
 */
wchar_t cCallKeybordFunction::getChar() const {
	
	return cChar;
}


/**
 * @return  The keyboard function, which will be used with this operator.
 * 	If it is "CHAR" cChar contains the to output char.
 * 	This function will be used in the operator method.
 * 	@see keybordFunctionForOperator
 */
eKeybordFunction cCallKeybordFunction::getKeybordFunctionForOperator() const {
	
	return keybordFunctionForOperator;
}


/**
 * This function converts the given char to a key code to call with
 * the operator.
 *
 * @see keybordFunctionForOperator
 * @param cInChar the char to convert to a operator call
 */
void cCallKeybordFunction::charToOperatorFunction( const wchar_t cInChar ) {
	
	switch ( cInChar ) {
		case '1': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 2;
		}; break;
		case '2': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 3;
		}; break;
		case '3': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 4;
		}; break;
		case '4': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 5;
		}; break;
		case '5': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 6;
		}; break;
		case '6': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 7;
		}; break;
		case '7': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 8;
		}; break;
		case '8': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 9;
		}; break;
		case '9': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 10;
		}; break;
		case '0': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 11;
		}; break;
		case L'ß': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 12;
		}; break;
		case L'´': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 13;
		}; break;
		case '	': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 15;
		}; break;
		case 'q': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 16;
		}; break;
		case 'w': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 17;
		}; break;
		case 'e': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 18;
		}; break;
		case 'r': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 19;
		}; break;
		case 't': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 20;
		}; break;
		case 'z': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 21;
		}; break;
		case 'u': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 22;
		}; break;
		case 'i': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 23;
		}; break;
		case 'o': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 24;
		}; break;
		case 'p': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 25;
		}; break;
		case L'ü': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 26;
		}; break;
		case '+': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 27;
		}; break;
		case 'a': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 30;
		}; break;
		case 's': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 31;
		}; break;
		case 'd': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 32;
		}; break;
		case 'f': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 33;
		}; break;
		case 'g': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 34;
		}; break;
		case 'h': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 35;
		}; break;
		case 'j': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 36;
		}; break;
		case 'k': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 37;
		}; break;
		case 'l': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 38;
		}; break;
		case L'ö': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 39;
		}; break;
		case L'ä': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 40;
		}; break;
		case '^': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 41;
		}; break;
		case '#': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 43;
		}; break;
		case 'y': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 44;
		}; break;
		case 'x': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 45;
		}; break;
		case 'c': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 46;
		}; break;
		case 'v': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 47;
		}; break;
		case 'b': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 48;
		}; break;
		case 'n': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 49;
		}; break;
		case 'm': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 50;
		}; break;
		case ',': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 51;
		}; break;
		case '.': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 52;
		}; break;
		case '-': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 53;
		}; break;
		case '*': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 55;
		}; break;
		case ' ': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 57;
		}; break;
		case '<': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 86;
		}; break;
		case '/': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 98;
		}; break;
		case '=': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 117;
		}; break;
		case L'±': {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 118;
		}; break;
		case '!': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 2;
		}; break;
		case '"': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 3;
		}; break;
		case L'§': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 4;
		}; break;
		case '$': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 5;
		}; break;
		case '%': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 6;
		}; break;
		case '&': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 7;
		}; break;
		case '(': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 9;
		}; break;
		case ')': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 10;
		}; break;
		case '?': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 12;
		}; break;
		case '`': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 13;
		}; break;
		case 'Q': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 16;
		}; break;
		case 'W': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 17;
		}; break;
		case 'E': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 18;
		}; break;
		case 'R': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 19;
		}; break;
		case 'T': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 20;
		}; break;
		case 'Z': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 21;
		}; break;
		case 'U': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 22;
		}; break;
		case 'I': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 23;
		}; break;
		case 'O': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 24;
		}; break;
		case 'P': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 25;
		}; break;
		case L'Ü': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 26;
		}; break;
		case 'A': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 30;
		}; break;
		case 'S': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 31;
		}; break;
		case 'D': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 32;
		}; break;
		case 'F': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 33;
		}; break;
		case 'G': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 34;
		}; break;
		case 'H': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 35;
		}; break;
		case 'J': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 36;
		}; break;
		case 'K': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 37;
		}; break;
		case 'L': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 38;
		}; break;
		case L'Ö': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 39;
		}; break;
		case L'Ä': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 40;
		}; break;
		case L'°': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 41;
		}; break;
		case '\'': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 43;
		}; break;
		case 'Y': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 44;
		}; break;
		case 'X': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 45;
		}; break;
		case 'C': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 46;
		}; break;
		case 'V': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 47;
		}; break;
		case 'B': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 48;
		}; break;
		case 'N': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 49;
		}; break;
		case 'M': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 50;
		}; break;
		case ';': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 51;
		}; break;
		case ':': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 52;
		}; break;
		case '_': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 53;
		}; break;
		case '>': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 86;
		}; break;
		case '{': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_RIGHTALT;
			keyCode2 = 8;
		}; break;
		case '[': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_RIGHTALT;
			keyCode2 = 9;
		}; break;
		case ']': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_RIGHTALT;
			keyCode2 = 10;
		}; break;
		case '}': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_RIGHTALT;
			keyCode2 = 11;
		}; break;
		case '\\': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_RIGHTALT;
			keyCode2 = 12;
		}; break;
		case '|': {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_RIGHTALT;
			keyCode2 = 86;
		}; break;
	};  //end switch cInChar
	//fill liKeys
	if ( keybordFunctionForOperator == INPUT_KEY_CODE_1 ) {
		//just one key code exists
		liKeys.push_back( keyCode1 );
	} else if ( keybordFunctionForOperator == INPUT_KEY_CODE_2 ) {
		//two key codes exists
		liKeys.push_back( keyCode1 );
		liKeys.push_back( keyCode2 );
	}
}



/**
 * This function converts the given keyboard function to the members to
 * call with the operator.
 * Sets the value for:
 * 	@see keybordFunction
 * 	@see keybordFunctionForOperator
 *
 * Also sets if needed:
 * 	@see keyCode1
 * 	@see keyCode2
 * 	@see liKeys
 *
 * @param inKeybordFunction the keyboard function to set
 * 	@see keybordFunction
 */
void cCallKeybordFunction::keyboardFunctionToOperatorFunction(
		const eKeybordFunction inKeybordFunction ) {
	
	keybordFunction = inKeybordFunction;
	//empty old values
	keyCode1 = 0;
	keyCode2 = 0;
	liKeys.clear();
	
	switch ( inKeybordFunction ) {
		case F1: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_F1;
		}; break;
		case F2: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_F2;
		}; break;
		case F3: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_F3;
		}; break;
		case F4: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_F4;
		}; break;
		case F5: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_F5;
		}; break;
		case F6: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_F6;
		}; break;
		case F7: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_F7;
		}; break;
		case F8: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_F8;
		}; break;
		case F9: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_F9;
		}; break;
		case F10: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_F10;
		}; break;
		case F11: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_F11;
		}; break;
		case F12: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_F12;
		}; break;
		case TAB: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_TAB;
		}; break;
		case BACK_TAB: {
			//SHIFT + TAB
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = KEY_TAB;
		}; break;
		case ESC: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_ESC;
		}; break;
		case SPACE: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_SPACE;
		}; break;
		case ENTER: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_ENTER;
		}; break;
		case UPPER_CASE: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_CAPSLOCK;
		}; break;
		case CHAPS_LOCK: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_CAPSLOCK;
		}; break;
		case UP: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_UP;
		}; break;
		case DOWN: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_DOWN;
		}; break;
		case LEFT: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_LEFT;
		}; break;
		case RIGHT: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_RIGHT;
		}; break;
		case PAGE_UP: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_PAGEUP;
		}; break;
		case PAGE_DOWN: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_PAGEDOWN;
		}; break;
		case POS1: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_HOME;
		}; break;
		case END: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_END;
		}; break;
		case DELETE: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_BACKSPACE;
		}; break;
		case REMOVE: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_DELETE;
		}; break;
		case INSERT: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_INSERT;
		}; break;
		case ALT: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_RIGHTALT;
		}; break;
		case CTRL: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_LEFTCTRL;
		}; break;
		case PRINT: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_PRINT;
		}; break;
		case MENUE: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_MENU;
		}; break;
		case SCROLL: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_SCROLLLOCK;
		}; break;
		case BREAK: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_BREAK;
			break;
		};
		
		case UNDO: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = KEY_UNDO;
		}; break;
		case UNUNDO: {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = KEY_UNDO;
		}; break;
		
		case END_APPLICATION: {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTCTRL;
			keyCode2 = 16;
		}; break;
		
		case COPY: {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTCTRL;
			keyCode2 = 46;
		}; break;
		case PAST: {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTCTRL;
			keyCode2 = 47;
		}; break;
		
		case NEXT_WORD: {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTCTRL;
			keyCode2 = KEY_RIGHT;
		}; break;
		case PREVIOS_WORD: {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTCTRL;
			keyCode2 = KEY_LEFT;
		}; break;
		case MARK_NEXT_WORD: {
			keybordFunctionForOperator = INPUT_KEY_CODE;
			liKeys.push_back( KEY_LEFTCTRL );
			liKeys.push_back( KEY_LEFTSHIFT );
			liKeys.push_back( KEY_RIGHT );
		}; break;
		
		
		case QUOTE: {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 3;
		}; break;
		case SINGLE_QUOTE: {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 43;
		}; break;
		case SEMICOLON: {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 51;
		}; break;
		
		case SPECIAL_CHAR_DEU_AE: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 40;
		}; break;
		case SPECIAL_CHAR_DEU_UP_AE: {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 40;
		}; break;
		case SPECIAL_CHAR_DEU_OE: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 39;
		}; break;
		case SPECIAL_CHAR_DEU_UP_OE: {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 39;
		}; break;
		case SPECIAL_CHAR_DEU_UE: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 26;
		}; break;
		case SPECIAL_CHAR_DEU_UP_UE: {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 26;
		}; break;
		case SPECIAL_CHAR_DEU_SZ: {
			keybordFunctionForOperator = INPUT_KEY_CODE_1;
			keyCode1 = 12;
		}; break;
		
		case DIACRITIC_GRAVE: {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 13;
		}; break;
		case DIACRITIC_OVERING: {
			keybordFunctionForOperator = INPUT_KEY_CODE_2;
			keyCode1 = KEY_LEFTSHIFT;
			keyCode2 = 41;
		}; break;
		
		
		default: break;  //unknown keybord function
	};  //end switch inKeybordFunction
	//fill liKeys
	if ( keybordFunctionForOperator == INPUT_KEY_CODE_1 ) {
		//just one key code exists
		liKeys.push_back( keyCode1 );
	} else if ( keybordFunctionForOperator == INPUT_KEY_CODE_2 ) {
		//two key codes exists
		liKeys.push_back( keyCode1 );
		liKeys.push_back( keyCode2 );
	}
}


/**
 * This function returns the keyboard function for the given string.
 *
 * @param inSzParameters the string for which to return the keyboard function
 * @return the keyboard function for the given string, or KEYBOARD_NON if non could
 * 	be evaluated
 */
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
eKeybordFunction cCallKeybordFunction::getKeyboardFunction(
		const wstring & inSzParameters ) {
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
eKeybordFunction cCallKeybordFunction::getKeyboardFunction(
		const string & inSzParameters ) {
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	
	if ( inSzParameters.empty() ) {
		//no keyboard function to evaluate
		return KEYBOARD_NON;
	}
	
#ifdef FEATURE_READ_DATA_TEXT_WITH_REGEX
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	if ( regex_match( inSzParameters, wregex( L"KEY_*",
			regex_constants::icase | regex_constants::ECMAScript ) ) ) {
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	if ( regex_match( inSzParameters, regex( szKeyPrefix,
			regex_constants::icase | regex_constants::ECMAScript ) ) ) {
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
		//no keyboard function begins with "KEY_" -> done (quick exit)
		return KEYBOARD_NON;
	}
#endif  //FEATURE_READ_DATA_TEXT_WITH_REGEX
	
	//map from regular expressions (string) to keyboard function
	return mapperKeybordFunction( inSzParameters );
}


/**
 * This function returns the key code like defined in <linux/input.h> for
 * the given string.
 *
 * @param inSzKeyCode the string for which to return the key code
 * @return the key code for the given string, or 0 if non could be evaluated
 */
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
unsigned int cCallKeybordFunction::readKeyCode(
		const std::wstring & inSzKeyCode ) {
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
unsigned int cCallKeybordFunction::readKeyCode(
		const std::string & inSzKeyCode ) {
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	
	if ( inSzKeyCode.empty() ) {
		//no string to check
		return 0;
	}
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	if ( ! regex_match( inSzKeyCode, wregex( L"KEY_*",
			regex_constants::icase | regex_constants::ECMAScript ) ) ) {
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	if ( ! regex_match( inSzKeyCode, regex( "KEY_*",
			regex_constants::icase | regex_constants::ECMAScript ) ) ) {
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
		//all key codes begin with "KEY_" -> done (quick exit)
		return stringToLong( inSzKeyCode );
	}
	//map to key code
	return mapperKeyCode( inSzKeyCode );
}



/**
 * This method analyses and setst the given parameters.
 * It wil set:
 * 	@see keybordFunctionForOperator
 * 	@see keybordFunction
 * 	@see keyCode1
 * 	@see keyCode2
 * 	@see liKeys
 * 	@see cChar
 *
 * @param inSzParameters the string with the parameters for the keyboard function
 */
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
bool cCallKeybordFunction::analyseAndSetParameters( const wstring & inSzParameters ) {
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
bool cCallKeybordFunction::analyseAndSetParameters( const string & inSzParameters ) {
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	
	if ( inSzParameters.empty() ) {
		//no parameters -> no keyboard function
		return false;
	}
	if ( inSzParameters.size() == 1 ) {
		//char given
		cChar = inSzParameters[ 0 ];
		keybordFunction = CHAR;
		
		charToOperatorFunction( cChar );
		return true;
	}  //else more than one char in string
	
	//try to get a keyboard function
	const eKeybordFunction keybordFunctionTmp =
		getKeyboardFunction( inSzParameters );
	if ( keybordFunctionTmp != KEYBOARD_NON ) {
		//set read keyboard function
		keyboardFunctionToOperatorFunction( keybordFunctionTmp );
		return true;
	}// else read the keys into liKeys
	bool bAnalyseOk = true;
#ifdef FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	wstring szKeyCode;
#else  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	string szKeyCode;
#endif  //FEATURE_READ_DATA_GLOVE_STATES_WIDE_CHAR
	unsigned int iReadedKey = 0;
	unsigned int iIndexNextParameter = 0;
	unsigned int iIndexActualParameter = 0;
	while ( iIndexNextParameter < inSzParameters.size() ) {
		//while more parameter to read
		
		do {
			iIndexNextParameter = inSzParameters.find( ',', iIndexNextParameter );
			
			if ( ( iIndexNextParameter == string::npos ) ||
					( inSzParameters.size() <= iIndexNextParameter ) ) {
				//next comma ',' not found -> read till end
				iIndexNextParameter = inSzParameters.size();
				break;
			}
		} while ( ( iIndexNextParameter == 0 ) ||
				//or ',' is masked
				( inSzParameters.at( iIndexNextParameter - 1 ) == '\\' ) );
		//szKeyCode contains the part betwean two commas ','
		szKeyCode = inSzParameters.substr( iIndexActualParameter,
			iIndexNextParameter - iIndexActualParameter );
		
		iIndexActualParameter = iIndexNextParameter;
		//discard comma ','
		iIndexNextParameter++;
		if ( ! szKeyCode.empty() ) {
			//read the next key code
			iReadedKey = readKeyCode( szKeyCode );
			if ( iReadedKey != 0 ) {
				liKeys.push_back( iReadedKey );
			} else {  //unknown key
				bAnalyseOk = false;
			}
		}
	}  //end while more parameter to read
	
	//fill keyCode1 and keyCode2
	std::list< unsigned int >::const_iterator itrKey = liKeys.begin();
	if ( itrKey != liKeys.end() ) {
		//set first key code
		keyCode1 = (*itrKey);
		itrKey++;
		if ( itrKey != liKeys.end() ) {
			//set second key code
			keyCode2 = (*itrKey);
		}
	}
	
	if ( liKeys.size() == 1 ) {
		//exactly one key code
		keybordFunctionForOperator = INPUT_KEY_CODE_1;
	} else if ( liKeys.size() == 2 ) {
		//exactly two key codes
		keybordFunctionForOperator = INPUT_KEY_CODE_2;
	}
	
	return bAnalyseOk && ( ! liKeys.empty() );
}






