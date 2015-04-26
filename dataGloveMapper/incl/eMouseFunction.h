
//TODO check

/**
 * @file eMouseFunction
 * file name: eMouseFunction.h
 * @author Betti Oesterholz
 * @date 04.01.2015
 * @mail webmaster@BioKom.info
 *
 * System: C++
 *
 * This file specifies the type for mouse functions.
 *
 *
 * Copyright (C) @c GPL3 2015 Betti Oesterholz
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
 * This file specifies the type for mouse functions.
 * Mouse function can be klicks (e.g. LEFT_KLICK) or directions (e.g. HORIZONTAL).
 *
 * @pattern functor
 * @see cCallMouseFunction
 * @see cCallPrepareMouseFunction
 * @see iCallFunction
 * @see cDataGloveState
 * @see cEvaluateDataGloveState
 */
/*
History:
04.01.2015  Oesterholz  created
*/


#ifndef ___N_DATA_GLOVE__N_MAPPER__E_MOUSE_FUNCTION__
#define ___N_DATA_GLOVE__N_MAPPER__E_MOUSE_FUNCTION__


namespace nDataGlove{
namespace nMapper{


enum eMouseFunction {
	MOUSE_NON,
	MOUSE_HORIZONTAL, MOUSE_VERTICAL,
	MOUSE_MOVE_HORIZONTAL_VERTICAL,  //move horizontal and vertical
	MOUSE_WHEEL,
	MOUSE_BTN_LEFT_KLICK, MOUSE_BTN_LEFT_DOUBLE_KLICK,
	MOUSE_BTN_RIGHT_KLICK, MOUSE_BTN_RIGHT_DOUBLE_KLICK,
	MOUSE_BTN_MIDDLE_KLICK, MOUSE_BTN_MIDDLE_DOUBLE_KLICK,
	
	MOUSE_BTN_SIDE_KLICK, MOUSE_BTN_SIDE_DOUBLE_KLICK,
	MOUSE_BTN_EXTRA_KLICK, MOUSE_BTN_EXTRA_DOUBLE_KLICK,
	MOUSE_BTN_FORWARD_KLICK, MOUSE_BTN_FORWARD_DOUBLE_KLICK,
	MOUSE_BTN_BACK_KLICK, MOUSE_BTN_BACK_DOUBLE_KLICK,
	MOUSE_BTN_TASK_KLICK, MOUSE_BTN_TASK_DOUBLE_KLICK,
	
	
	MOUSE_BTN_LEFT_DOWN, MOUSE_BTN_LEFT_UP,
	MOUSE_BTN_RIGHT_DOWN, MOUSE_BTN_RIGHT_UP,
	MOUSE_BTN_MIDDLE_DOWN, MOUSE_BTN_MIDDLE_UP,

	MOUSE_BTN_SIDE_DOWN, MOUSE_BTN_SIDE_UP,
	MOUSE_BTN_EXTRA_DOWN, MOUSE_BTN_EXTRA_UP,
	MOUSE_BTN_FORWARD_DOWN, MOUSE_BTN_FORWARD_UP,
	MOUSE_BTN_BACK_DOWN, MOUSE_BTN_BACK_UP,
	MOUSE_BTN_TASK_DOWN, MOUSE_BTN_TASK_UP
	
};  //eMouseFunction

};//end namespace nMapper
};//end namespace nDataGlove

#endif  //___N_DATA_GLOVE__N_MAPPER__E_MOUSE_FUNCTION__












