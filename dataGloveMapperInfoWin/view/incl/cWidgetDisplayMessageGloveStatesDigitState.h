
//TODO check

/**
 * @file cWidgetDisplayMessageGloveStatesDigitState
 * file name: cWidgetDisplayMessageGloveStatesDigitState.h
 * @author Betti Oesterholz
 * @date 14.06.2016
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
 *  +-(widget state)---------------------------------------------
 *  | (state name (mapped from state type or given state name + parameter))
 *  | (min state value) (target state value) (max state value)
 *  +------------------------------------------------------------
 *
 * @see nMessageGloveStates_cDigitState
 * @see cWidgetDisplayMessageGloveStates
 * @see cDataGloveDGTechVHand
 * @see cDataGloveMapperInfoMainWindow
 */
/*
History:
14.06.2016  Oesterholz  created
*/


#ifndef ___N_DATA_GLOVE__N_INFO___C_WIDGET_DISPLAY_MESSAGE_GLOVE_STATES_DIGIT_STATE_H__
#define ___N_DATA_GLOVE__N_INFO___C_WIDGET_DISPLAY_MESSAGE_GLOVE_STATES_DIGIT_STATE_H__


#include "version.h"


#include <QWidget>
#include <QGridLayout>
#include <QLabel>

#include "nMessageGloveStates_cDigitState.h"



namespace nDataGlove{
namespace nInfo{

class iWidgetSamplingValue;

class cWidgetDisplayMessageGloveStatesDigitState: public QWidget{
		Q_OBJECT
public:
	
	
	/**
	 * The standard constructor for a widget to show data glove sampling data.
	 *
	 * @param pParent a pointer the parent of this widget
	 */
	cWidgetDisplayMessageGloveStatesDigitState( QWidget * pParent = NULL );
	
	/**
	 * destructor
	 */
	virtual ~cWidgetDisplayMessageGloveStatesDigitState();
	
	
	/**
	 * @return the name of this class "cWidgetDisplayMessageGloveStatesDigitState"
	 */
	virtual std::string getName() const;
	
	
	/**
	 * @return a string for the name of the digit
	 */
	static QString getDigitStateName( const nDataGlove::nMapper::nMessageGloveStates::cDigitState *
			inPNewDigitState );
	
public slots:
	
	/**
	 * This slot is called if a data glove digit state changed.
	 *
	 * @param inPNewDigitState the new data glove digit state
	 */
	void slotDigitStateChanged(
		const nDataGlove::nMapper::nMessageGloveStates::cDigitState *
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
	QGridLayout * FLayoutValues;
	
	
	/**
	 * The label for the digit state name.
	 */
	QLabel * FLabelStateName;
	
	/**
	 * The label for the digit state minimum value.
	 */
	QLabel * FLabelMinStateValue;
	
	/**
	 * The label for the digit state target value.
	 */
	QLabel * FLabelTargetStateValue;
	
	
	/**
	 * The label for the digit state maximum value.
	 */
	QLabel * FLabelMaxStateValue;
	
	
};//end class cWidgetDisplayMessageGloveStatesDigitState

};//end namespace nInfo
};//end namespace nDataGlove

#endif //___N_DATA_GLOVE__N_INFO___C_WIDGET_DISPLAY_MESSAGE_GLOVE_STATES_DIGIT_STATE_H__




