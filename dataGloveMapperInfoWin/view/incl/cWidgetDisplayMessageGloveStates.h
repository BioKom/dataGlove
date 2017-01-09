
//TODO check

/**
 * @file cWidgetDisplayMessageGloveStates
 * file name: cWidgetDisplayMessageGloveStates.h
 * @author Betti Oesterholz
 * @date 30.07.2016
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
 *
 * This file specifies a class for a widget, with the data glove digit
 * state data.
 *
 *
 * @see nMessageGloveStates_cMessageGloveDigit
 * @see cWidgetDisplayMessageGloveStatesDigitState
 * @see  cWidgetDisplayMessageGloveStatesMessageGloveDigit
 * @see cWidgetDisplayMessageGloveStates
 * @see cDataGloveDGTechVHand
 * @see cDataGloveMapperInfoMainWindow
 */
/*
History:
30.07.2016  Oesterholz  created
*/


#ifndef ___N_DATA_GLOVE__N_INFO___C_WIDGET_DISPLAY_MESSAGE_GLOVE_STATES_H__
#define ___N_DATA_GLOVE__N_INFO___C_WIDGET_DISPLAY_MESSAGE_GLOVE_STATES_H__


#include "version.h"

#include <vector>

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QMutex>

#include "cMessageGloveStates.h"
#include "iSetMessageGloveStates.h"
#include "cWidgetDisplayMessageGloveStatesMessageGloveDigit.h"



namespace nDataGlove{
namespace nInfo{


class cWidgetDisplayMessageGloveStates: public QWidget,
		public iSetMessageGloveStates{
		Q_OBJECT
public:
	
	
	/**
	 * The standard constructor for a widget to show data glove sampling data.
	 *
	 * @param pParent a pointer the parent of this widget
	 * @param inOrientation the orientation of this widget
	 * 	@see FOrientation
	 */
	cWidgetDisplayMessageGloveStates( QWidget * pParent = NULL,
		const Qt::Orientation inOrientation = Qt::Horizontal );
	
	/**
	 * destructor
	 */
	virtual ~cWidgetDisplayMessageGloveStates();
	
	
	/**
	 * @return the name of this class "cWidgetDisplayMessageGloveStates"
	 */
	virtual std::string getName() const;
	
	/**
	 * @return the orientation of this widget
	 * 	@see FOrientation
	 */
	Qt::Orientation orientation() const;
	
public slots:
	
	/**
	 * Sets the new data glove state message.
	 * Also cares that the given message is deleted.
	 *
	 * @see FLastMessageGloveStates
	 * @param pInMessageGloveStates a pointer the new data glove state message
	 */
	virtual void setMessageGloveStates(
		nDataGlove::nMapper::cMessageGloveStates *
		pInMessageGloveStates );
	
protected slots:
	
	/**
	 * Evaluates the last received message for the data glove state.
	 *
	 * @see FLastMessageGloveStates
	 * @see FMutexEvaluateMessageGloveStates
	 * @see setMessageGloveStates()
	 * @see FMutexLastMessageGloveStates
	 * @see signalEvaluateMessageGloveStates();
	 */
	virtual void slotEvaluateMessageGloveStates();
	
	
signals:
	
	/**
	 * Signals to evaluat the last received message for the data glove state.
	 * Should be connected to slotEvaluateMessageGloveStates().
	 * (This signal slot mechanism is implemented to get this class thread
	 * save for data glove state message evaluation.)
	 *
	 * @see slotEvaluateMessageGloveStates();
	 * @see FLastMessageGloveStates
	 * @see FMutexEvaluateMessageGloveStates
	 * @see setMessageGloveStates()
	 * @see FMutexLastMessageGloveStates
	 */
	void signalEvaluateMessageGloveStates();
	
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
	 * The orientation of this widget.
	 */
	Qt::Orientation FOrientation;
	
	/**
	 * The last received message for the data glove state.
	 * @see setMessageGloveStates()
	 * @see FMutexLastMessageGloveStates
	 */
	nDataGlove::nMapper::cMessageGloveStates * FLastMessageGloveStates;
	
	/**
	 * Mutex to protect the last received message for the data glove state.
	 * @see FLastMessageGloveStates
	 * @see setMessageGloveStates()
	 */
	QMutex FMutexLastMessageGloveStates;
	
	/**
	 * Mutex to protect the last received message for the data glove state.
	 * @see evaluateMessageGloveStates()
	 * @see FLastMessageGloveStates
	 */
	QMutex FMutexEvaluateMessageGloveStates;
	
	/**
	 * The widgets for the digits of the data glove
	 */
	std::vector< cWidgetDisplayMessageGloveStatesMessageGloveDigit * >
		FVecWidgetsGloveDigits;
	
	
};//end class cWidgetDisplayMessageGloveStates

};//end namespace nInfo
};//end namespace nDataGlove

#endif //___N_DATA_GLOVE__N_INFO___C_WIDGET_DISPLAY_MESSAGE_GLOVE_STATES_H__




