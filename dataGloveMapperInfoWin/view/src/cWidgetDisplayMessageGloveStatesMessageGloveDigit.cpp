
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



#include "cWidgetDisplayMessageGloveStatesMessageGloveDigit.h"




using namespace nDataGlove::nInfo;
using namespace nDataGlove::nMapper;


/**
 * The standard constructor for a widget to show data glove sampling data.
 *
 * @param pParent a pointer the parent of this widget
 */
cWidgetDisplayMessageGloveStatesMessageGloveDigit::
	cWidgetDisplayMessageGloveStatesMessageGloveDigit( QWidget * pParent,
			const Qt::Orientation inOrientation ) :
		QWidget( pParent ), FLayoutMain( nullptr ), FLayoutStates( nullptr ),
		FOrientation( inOrientation ), FLabelDigitName( nullptr ),
		FActualState( nullptr ), FLowerState( nullptr ), FHigherState( nullptr ) {
	
	createSubWidgets();
}


/**
 * destructor
 */
cWidgetDisplayMessageGloveStatesMessageGloveDigit::~cWidgetDisplayMessageGloveStatesMessageGloveDigit() {
	//nothing to do
}


/**
 * @return the name of this class "cWidgetDisplayMessageGloveStatesMessageGloveDigit"
 */
std::string cWidgetDisplayMessageGloveStatesMessageGloveDigit::getName() const {
	
	return std::string( "cWidgetDisplayMessageGloveStatesMessageGloveDigit" );
}


/**
 * @return the orientation of this widget
 * 	@see FOrientation
 */
Qt::Orientation cWidgetDisplayMessageGloveStatesMessageGloveDigit::orientation() const {
	
	return FOrientation;
}


/**
 * @return a string for the name of the digit
 */
QString cWidgetDisplayMessageGloveStatesMessageGloveDigit::getDigitName(
		const nDataGlove::nMapper::nMessageGloveStates::cMessageGloveDigit * inPNewDigitState ) {
	
	if ( ! inPNewDigitState ) {
		
		return tr("none");
	}
	
	return QString( inPNewDigitState->getDigitName().c_str() );
}


/**
 * This slot is called if a data glove digit state changed.
 *
 * @param inPNewDigitState the new data glove digit state
 */
void cWidgetDisplayMessageGloveStatesMessageGloveDigit::slotDigitStateChanged(
		const nDataGlove::nMapper::nMessageGloveStates::cMessageGloveDigit *
			inPNewDigitState ) {
	
	if ( inPNewDigitState ) {
		
		if ( FLabelDigitName ) {
			const long lCorrection = inPNewDigitState->getCorrection();
			if ( lCorrection == 0 ) {
				FLabelDigitName->setText( getDigitName( inPNewDigitState ) + " : " +
					QString::number( inPNewDigitState->getValue() ) );
			} else {
				FLabelDigitName->setText( QString( getDigitName( inPNewDigitState ) ) + " : " +
					QString::number( inPNewDigitState->getValue() + lCorrection ) + " = " +
					QString::number( inPNewDigitState->getValue() ) + " + " +
					QString::number( lCorrection ) +" Cor" );
			}
		}
		//set the values of the states
		const int iActiveNeighbour =
			inPNewDigitState->getIndexActiveNeighbour();
		
		if ( FActualState ) {
			if ( 0 <= iActiveNeighbour ) {
				FActualState->slotDigitStateChanged(
					inPNewDigitState->getNeighbourState( iActiveNeighbour ) );
			} else {  //no state is active
				FActualState->slotDigitStateChanged( nullptr );
			}
		}
	//TODO? if ( 0 <= iActiveNeighbour ) -> show states which are highter/lower than the value ?
		if ( FLowerState ) {
			if ( 0 <= iActiveNeighbour ) {
				FLowerState->slotDigitStateChanged(
					inPNewDigitState->getNeighbourState( iActiveNeighbour - 1 ) );
			} else {  //no state is active -> show first state
				FLowerState->slotDigitStateChanged(
					inPNewDigitState->getNeighbourState( 0 ) );
			}
			
		}
		if ( FHigherState ) {
			if ( 0 <= iActiveNeighbour ) {
				FHigherState->slotDigitStateChanged(
					inPNewDigitState->getNeighbourState( iActiveNeighbour + 1 ) );
			} else {  //no state is active
				if ( 1 < inPNewDigitState->getNeighbourStateCount() ) {
					//two states exists -> show last state
					FHigherState->slotDigitStateChanged(
						inPNewDigitState->getNeighbourState(
							inPNewDigitState->getNeighbourStateCount() - 1 ) );
				}
			}
		}
	} else {
		if ( FLabelDigitName ) {
			FLabelDigitName->setText( tr("none") );
		}
		if ( FActualState ) {
			FActualState->slotDigitStateChanged( nullptr );
		}
		if ( FLowerState ) {
			FLowerState->slotDigitStateChanged( nullptr );
		}
		if ( FHigherState ) {
			FHigherState->slotDigitStateChanged( nullptr );
		}
	}
}



/**
 * This method creates the subwidgets for this widget.
 */
bool cWidgetDisplayMessageGloveStatesMessageGloveDigit::createSubWidgets() {

	
	if ( FLayoutMain == nullptr ) {
		FLayoutMain = new QGridLayout( this );
	}
	if ( FLayoutStates == nullptr ) {
		FLayoutStates = new QGridLayout();
		FLayoutMain->addLayout( FLayoutStates, 1, 0, Qt::AlignCenter );
	}
	
//  +-(widget state)---------------------------------------------
//  | (state name (mapped from state type or given state name + parameter))
//  | (min state value) (target state value) (max state value)
//  +------------------------------------------------------------
	
	
	//create the new subwidgets
	FLabelDigitName = new QLabel( tr("name"), this );
	FLabelDigitName->setAlignment( Qt::AlignCenter );
	FLayoutMain->addWidget( FLabelDigitName, 0, 0 );
	
	
	FActualState = new cWidgetDisplayMessageGloveStatesDigitState( this );
	FLowerState = new cWidgetDisplayMessageGloveStatesDigitState( this );
	FHigherState = new cWidgetDisplayMessageGloveStatesDigitState( this );
	
	if ( FOrientation == Qt::Horizontal ) {
		FLayoutStates->addWidget( FLowerState, 0, 0 );
		FLayoutStates->addWidget( FActualState, 0, 1 );
		FLayoutStates->addWidget( FHigherState, 0, 2 );
	} else {  //Qt::Vertical
		FLayoutStates->addWidget( FLowerState, 0, 0 );
		FLayoutStates->addWidget( FActualState, 1, 0 );
		FLayoutStates->addWidget( FHigherState, 2, 0 );
	}
	
	
//TODO
//	FLayoutStates->setColumnStretch ( 0, 1 );
//	FLayoutMain->setSizeConstraint( QLayout::SetFixedSize );
	
	return true;
}






