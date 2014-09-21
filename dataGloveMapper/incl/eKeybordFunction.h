
//TODO check

/**
 * @file eKeybordFunction
 * file name: eKeybordFunction.h
 * @author Betti Oesterholz
 * @date 28.08.2014
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies the type for keyboard functions.
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
 * This file specifies the type for keyboard functions.
 * Keyboard function can be keys (e.g. Strg, F1), but not chars.
 *
 * @pattern functor
 * @see cCallKeybordFunction
 * @see cCallPrepareKeybordFunction
 * @see iCallFunction
 * @see cDataGloveState
 * @see cEvaluateDataGloveState
 */
/*
History:
28.08.2014  Oesterholz  created
*/


#ifndef ___N_DATA_GLOVE__N_MAPPER__E_KEYBOARD_FUNCTION__
#define ___N_DATA_GLOVE__N_MAPPER__E_KEYBOARD_FUNCTION__


namespace nDataGlove{
namespace nMapper{


enum eKeybordFunction {
	NON,
	CHAR,  ///a char should be given as the keybord fuction
	INPUT_KEY_CODE,  ///a key codes like defined in <linux/input.h>
	INPUT_KEY_CODE_1,  ///one key code like defined in <linux/input.h>
	INPUT_KEY_CODE_2,  ///two key codes like defined in <linux/input.h>
	F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	TAB, ESC, ENTER,
	UPPER_CASE,
	UP, DOWN, LEFT, RIGHT,
	PAGE_UP, PAGE_DOWN, POS1, END,
	DELETE, REMOVE /*ENTF*/,INSERT,
	ALT, CTRL /*STRG*/,
	PRINT, MENUE, SCROLL /*Rollen*/, BREAK /*Pause*/
	
};  //eKeybordFunction

};//end namespace nMapper
};//end namespace nDataGlove

#endif  //___N_DATA_GLOVE__N_MAPPER__E_KEYBOARD_FUNCTION__












