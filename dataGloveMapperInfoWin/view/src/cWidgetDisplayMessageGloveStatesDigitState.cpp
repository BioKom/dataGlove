
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



#include "cWidgetDisplayMessageGloveStatesDigitState.h"




using namespace nDataGlove::nInfo;
using namespace nDataGlove::nMapper::nMessageGloveStates;


/**
 * The standard constructor for a widget to show data glove sampling data.
 *
 * @param pParent a pointer the parent of this widget
 */
cWidgetDisplayMessageGloveStatesDigitState::
	cWidgetDisplayMessageGloveStatesDigitState( QWidget * pParent ) :
		QWidget( pParent ), FLayoutMain( nullptr ), FLayoutValues( nullptr ),
		FLabelStateName( nullptr ),
		FLabelMinStateValue( nullptr ), FLabelTargetStateValue( nullptr ),
		FLabelMaxStateValue( nullptr ) {
	
	createSubWidgets();
}


/**
 * destructor
 */
cWidgetDisplayMessageGloveStatesDigitState::~cWidgetDisplayMessageGloveStatesDigitState() {
	//nothing to do
}


/**
 * @return the name of this class "cWidgetDisplayMessageGloveStatesDigitState"
 */
std::string cWidgetDisplayMessageGloveStatesDigitState::getName() const {
	
	return std::string( "cWidgetDisplayMessageGloveStatesDigitState" );
}


/**
 * @return a string for the name of the digit
 */
QString cWidgetDisplayMessageGloveStatesDigitState::getDigitStateName(
		const nDataGlove::nMapper::nMessageGloveStates::cDigitState * inPNewDigitState ) {
	
	if ( ! inPNewDigitState ) {
		
		return tr("none");
	}
	const QString szParameter( inPNewDigitState->getParameter().c_str() );
	switch ( inPNewDigitState->getStateTyp() ) {
		case 1: { //modus
			return tr( "modus" ) + QString(" ") + szParameter;
		}; break;
		case 2: { //keyboard
			return tr( "key" ) + QString(" ") + szParameter;
		}; break;
		case 3: { //mouse
			return tr( "mouse" ) + QString(" ") + szParameter;
		}; break;
		case 101:{  //prepare modus
			return tr( "-> modus" ) + QString(" ") + szParameter;
		}; break;
		case 102:{  //prepare keyboard
			return tr( "-> key" ) + QString(" ") + szParameter;
		}; break;
		case 103:{  //prepare mouse
			return tr( "-> mouse" ) + QString(" ") + szParameter;
		}; break;
		default: ; //nothing
	};  //switch ( inPNewDigitState->getStateTyp() )
	
	if (  szParameter.isEmpty() ) {
		return QString( inPNewDigitState->getStateName().c_str() );
	}  //else
	return QString( inPNewDigitState->getStateName().c_str() ) +
		QString(": ") + szParameter;
}



/**
 * This slot is called if a data glove digit state changed.
 *
 * @param inPNewDigitState the new data glove digit state
 */
void cWidgetDisplayMessageGloveStatesDigitState::slotDigitStateChanged(
		const nDataGlove::nMapper::nMessageGloveStates::cDigitState *
			inPNewDigitState ) {
	
	if ( inPNewDigitState ) {
		
		if ( FLabelStateName ) {
			FLabelStateName->setText( getDigitStateName( inPNewDigitState ) );
		}
		if ( FLabelMinStateValue ) {
			FLabelMinStateValue->setText( tr("min ") + QString::number(
				inPNewDigitState->getMinValue() ) );
		}
		if ( FLabelTargetStateValue ) {
			FLabelTargetStateValue->setText( tr("target ") + QString::number(
				inPNewDigitState->getTargetValue() ) );
		}
		if ( FLabelMaxStateValue ) {
			FLabelMaxStateValue->setText( tr("max ") + QString::number(
				inPNewDigitState->getMaxValue() ) );
		}
	} else {
		if ( FLabelStateName ) {
			FLabelStateName->setText( tr("none") );
		}
		if ( FLabelMinStateValue ) {
			FLabelMinStateValue->setText( tr("min") );
		}
		if ( FLabelTargetStateValue ) {
			FLabelTargetStateValue->setText( tr("target") );
		}
		if ( FLabelMaxStateValue ) {
			FLabelMaxStateValue->setText( tr("max") );
		}
	}
}



/**
 * This method creates the subwidgets for this widget.
 */
bool cWidgetDisplayMessageGloveStatesDigitState::createSubWidgets() {

	
	if ( FLayoutMain == nullptr ) {
		FLayoutMain = new QGridLayout( this );
	}
	if ( FLayoutValues == nullptr ) {
		FLayoutValues = new QGridLayout();
		FLayoutMain->addLayout( FLayoutValues, 1, 0, Qt::AlignCenter );
	}
	
//  +-(widget state)---------------------------------------------
//  | (state name (mapped from state type or given state name + parameter))
//  | (min state value) (target state value) (max state value)
//  +------------------------------------------------------------
	
	
	//create the new subwidgets
	FLabelStateName = new QLabel( tr("name"), this );
	FLabelStateName->setAlignment( Qt::AlignCenter );
	FLayoutMain->addWidget( FLabelStateName, 0, 0 );
	
	
	FLabelMinStateValue = new QLabel( tr("min"), this );
	FLabelMinStateValue->setAlignment( Qt::AlignCenter );
	FLayoutValues->addWidget( FLabelMinStateValue, 0, 0 );
	
	FLabelTargetStateValue = new QLabel( tr("target"), this );
	FLabelTargetStateValue->setAlignment( Qt::AlignCenter );
	FLayoutValues->addWidget( FLabelTargetStateValue, 0, 1 );

	FLabelMaxStateValue = new QLabel( tr("max"), this );
	FLabelMaxStateValue->setAlignment( Qt::AlignCenter );
	FLayoutValues->addWidget( FLabelMaxStateValue, 0, 2 );
	
	
	FLayoutValues->setColumnStretch ( 1, 0 );
	FLayoutValues->setColumnStretch ( 1, 1 );
	FLayoutValues->setColumnStretch ( 1, 2 );
//TODO
//	FLayoutMain->setSizeConstraint( QLayout::SetFixedSize );
	
	return true;
}






