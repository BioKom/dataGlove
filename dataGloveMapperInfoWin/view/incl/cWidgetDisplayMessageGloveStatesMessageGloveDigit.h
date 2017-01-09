
//TODO check

/**
 * @file cWidgetDisplayMessageGloveStatesMessageGloveDigit
 * file name: cWidgetDisplayMessageGloveStatesMessageGloveDigit.h
 * @author Betti Oesterholz
 * @date 26.06.2016
 * @mail webmaster@BioKom.info
 *
 * System: C++, Qt4
 *
 * This file specifies a class for a widget, with the data glove digit
 * state data.
 *
 *
 * Copyright (C) @c GPL3 2016 Betti Oesterholz
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
 * This file specifies a class for a widget, with the data glove digit
 * state data.
 *
 * view:
  +---------------------------------------------------------------------------------------------
  |       (Digit Name  Value: (value for the digit) )
  |+-(widget lower state)--------++-(widget actual state)-------++-(widget higher state )------+
  || (lower state name)          || (actual state name)         || (higher state name)         |
  || (min target max state value)|| (min target max state value)|| (min target max state value)|
  ++-----------------------------++-----------------------------++-----------------------------+
  +---------------------------------------------------------------------------------------------
 *
 * @see nMessageGloveStates_cMessageGloveDigit
 * @see cWidgetDisplayMessageGloveStatesDigitState
 * @see cWidgetDisplayMessageGloveStates
 * @see cDataGloveDGTechVHand
 * @see cDataGloveMapperInfoMainWindow
 */
/*
History:
26.06.2016  Oesterholz  created
*/


#ifndef ___N_DATA_GLOVE__N_INFO___C_WIDGET_DISPLAY_MESSAGE_GLOVE_STATES_MESSAGE_GLOVE_DIGIT_H__
#define ___N_DATA_GLOVE__N_INFO___C_WIDGET_DISPLAY_MESSAGE_GLOVE_STATES_MESSAGE_GLOVE_DIGIT_H__


#include "version.h"


#include <QWidget>
#include <QGridLayout>
#include <QLabel>

#include "nMessageGloveStates_cMessageGloveDigit.h"
#include "cWidgetDisplayMessageGloveStatesDigitState.h"



namespace nDataGlove{
namespace nInfo{

class iWidgetSamplingValue;

class cWidgetDisplayMessageGloveStatesMessageGloveDigit: public QWidget{
		Q_OBJECT
public:
	
	
	/**
	 * The standard constructor for a widget to show data glove sampling data.
	 *
	 * @param pParent a pointer the parent of this widget
	 * @param inOrientation the orientation of this widget
	 * 	@see FOrientation
	 */
	cWidgetDisplayMessageGloveStatesMessageGloveDigit( QWidget * pParent = NULL,
		const Qt::Orientation inOrientation = Qt::Horizontal );
	
	/**
	 * destructor
	 */
	virtual ~cWidgetDisplayMessageGloveStatesMessageGloveDigit();
	
	
	/**
	 * @return the name of this class "cWidgetDisplayMessageGloveStatesMessageGloveDigit"
	 */
	virtual std::string getName() const;
	
	/**
	 * @return the orientation of this widget
	 * 	@see FOrientation
	 */
	Qt::Orientation orientation() const;
	
	/**
	 * @return a string for the name of the digit
	 */
	static QString getDigitName(
		const nDataGlove::nMapper::nMessageGloveStates::cMessageGloveDigit *
			inPNewDigitState );
	
public slots:
	
	/**
	 * This slot is called if a data glove digit state changed.
	 *
	 * @param inPNewDigitState the new data glove digit state
	 */
	void slotDigitStateChanged(
		const nDataGlove::nMapper::nMessageGloveStates::cMessageGloveDigit *
			inPNewDigitState );
	
protected:
	
	/**
	 * This method creates the subwidgets for this widget.
	 */
	bool createSubWidgets();
	
	/**
	 * The layout to organize the layout of this widget.
	 */
	QGridLayout * FLayoutMain;
	
	/**
	 * The layout to organize the layout of the values in the widget.
	 */
	QGridLayout * FLayoutStates;
	
	/**
	 * The orientation of this widget.
	 */
	Qt::Orientation FOrientation;
	
	/**
	 * The label for the digit name.
	 */
	QLabel * FLabelDigitName;
	
	/**
	 * The widget for the actual state.
	 */
	cWidgetDisplayMessageGloveStatesDigitState * FActualState;
	
	/**
	 * The widget for the lower state.
	 */
	cWidgetDisplayMessageGloveStatesDigitState * FLowerState;
	
	/**
	 * The widget for the higher state.
	 */
	cWidgetDisplayMessageGloveStatesDigitState * FHigherState;
	
	
};//end class cWidgetDisplayMessageGloveStatesMessageGloveDigit

};//end namespace nInfo
};//end namespace nDataGlove

#endif //___N_DATA_GLOVE__N_INFO___C_WIDGET_DISPLAY_MESSAGE_GLOVE_STATES_MESSAGE_GLOVE_DIGIT_H__




