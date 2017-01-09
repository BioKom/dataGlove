
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
 * This file implements a class for a widget, with the data glove digit
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
 * This file implements a class for a widget, with the data glove digit
 * state data.
 *
 *
 * @see nMessageGloveStates_cMessageGloveDigit
 * @see cWidgetDisplayMessageGloveStatesDigitState
 * @see cWidgetDisplayMessageGloveStatesMessageGloveDigit
 * @see cDataGloveDGTechVHand
 * @see cDataGloveMapperInfoMainWindow
 */
/*
History:
30.07.2016  Oesterholz  created
*/



#include "cWidgetDisplayMessageGloveStates.h"




using namespace nDataGlove::nInfo;
using namespace nDataGlove::nMapper;


/**
 * The standard constructor for a widget to show data glove sampling data.
 *
 * @param pParent a pointer the parent of this widget
 */
cWidgetDisplayMessageGloveStates::
	cWidgetDisplayMessageGloveStates( QWidget * pParent,
			const Qt::Orientation inOrientation ) :
		QWidget( pParent ), FLayoutMain( nullptr ),
		FOrientation( inOrientation ), FLastMessageGloveStates( nullptr ) {
	
	createSubWidgets();
	
	QObject::connect( this , SIGNAL(signalEvaluateMessageGloveStates()),
		this, SLOT(slotEvaluateMessageGloveStates()));
}


/**
 * destructor
 */
cWidgetDisplayMessageGloveStates::~cWidgetDisplayMessageGloveStates() {
	
	FMutexLastMessageGloveStates.lock();
	
	if ( FLastMessageGloveStates ) {
		delete FLastMessageGloveStates;
	}
	FMutexLastMessageGloveStates.unlock();

}


/**
 * @return the name of this class "cWidgetDisplayMessageGloveStates"
 */
std::string cWidgetDisplayMessageGloveStates::getName() const {
	
	return std::string( "cWidgetDisplayMessageGloveStates" );
}

/**
 * @return the orientation of this widget
 * 	@see FOrientation
 */
Qt::Orientation cWidgetDisplayMessageGloveStates::orientation() const {
	
	return FOrientation;
}


/**
 * Sets the new data glove state message.
 * Also cares that the given message is deleted.
 *
 * @param pInMessageGloveStates a pointer the new data glove state message
 */
void cWidgetDisplayMessageGloveStates::setMessageGloveStates(
		nDataGlove::nMapper::cMessageGloveStates * pInMessageGloveStates ) {
	
	nDataGlove::nMapper::cMessageGloveStates * pExistingLastMessageGloveState =
		nullptr;
	FMutexLastMessageGloveStates.lock();
	
	pExistingLastMessageGloveState = FLastMessageGloveStates;
	FLastMessageGloveStates = pInMessageGloveStates;
	
	FMutexLastMessageGloveStates.unlock();
	
	if ( pExistingLastMessageGloveState ) {
		//free memory from bevore last existing message, it isn't needed anymore
		delete pExistingLastMessageGloveState;
	}
	//signal to evaluate the message
	emit signalEvaluateMessageGloveStates();
}


/**
 * Evaluates the last received message for the data glove state.
 *
 * @see FLastMessageGloveStates
 * @see FMutexEvaluateMessageGloveStates
 * @see setMessageGloveStates()
 * @see FMutexLastMessageGloveStates
 */
void cWidgetDisplayMessageGloveStates::slotEvaluateMessageGloveStates() {
	
	nDataGlove::nMapper::cMessageGloveStates * pMessageGloveState = nullptr;
	//evaluate just one message at a time
	FMutexEvaluateMessageGloveStates.lock();
	FMutexLastMessageGloveStates.lock();
	
	pMessageGloveState = FLastMessageGloveStates;
	FLastMessageGloveStates = nullptr;
	
	FMutexLastMessageGloveStates.unlock();
	
	//evaluate the message
	if ( pMessageGloveState ) {
		
		//create the missing digit widgets
		unsigned int iActualDigit = FVecWidgetsGloveDigits.size();
		const unsigned int uiNumberOfDigits = pMessageGloveState->getDigitCount();
		while ( iActualDigit < uiNumberOfDigits ) {
			
			cWidgetDisplayMessageGloveStatesMessageGloveDigit * pActualState =
				new cWidgetDisplayMessageGloveStatesMessageGloveDigit(
					this, FOrientation );
			
			FVecWidgetsGloveDigits.push_back( pActualState );
			
			if ( FOrientation == Qt::Horizontal ) {
				FLayoutMain->addWidget( pActualState, iActualDigit, 0 );
			} else {  //Qt::Vertical
				FLayoutMain->addWidget( pActualState, 0, iActualDigit );
			}
			++iActualDigit;
		}
		//set the values of the digit widgets
		for ( iActualDigit = 0; iActualDigit < uiNumberOfDigits; ++iActualDigit ) {
			
			FVecWidgetsGloveDigits[ iActualDigit ]->slotDigitStateChanged(
				pMessageGloveState->getDigit( iActualDigit ) );
		}
		//clear the remaining digit widgets
		for ( ;iActualDigit < FVecWidgetsGloveDigits.size(); ++iActualDigit ) {
			
			FVecWidgetsGloveDigits[ iActualDigit ]->slotDigitStateChanged(
				nullptr );
		}
		delete pMessageGloveState;
	} else {  //clear data from all sub widgets
		for ( auto digitGlovewidget : FVecWidgetsGloveDigits ) {
			digitGlovewidget->slotDigitStateChanged( nullptr );
		}
	}
	FMutexEvaluateMessageGloveStates.unlock();
}



/**
 * This method creates the subwidgets for this widget.
 */
bool cWidgetDisplayMessageGloveStates::createSubWidgets() {

	
	if ( FLayoutMain == nullptr ) {
		FLayoutMain = new QGridLayout( this );
	}
	//init the digit widgets in setMessageGloveStates()
	
//TODO
//	FLayoutStates->setColumnStretch ( 0, 1 );
//	FLayoutMain->setSizeConstraint( QLayout::SetFixedSize );
	
	return true;
}






