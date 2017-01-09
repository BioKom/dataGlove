
//TODO check

/**
 * @file cDataGloveMapperInfoMainWindow
 * file name: cDataGloveMapperInfoMainWindow.h
 * @author Betti Oesterholz
 * @date 28.03.2016
 * @mail webmaster@BioKom.info
 *
 * System: C++, Qt4
 *
 * This file specifies a class for the main window of a data glove mapper
 * info application.
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
 * This file specifies a class for the main window of a data glove mapper
 * info application.
 * The window will display the actual status of the data glove mapper
 * application. It will show the actual data glove sampling values, the
 * data of the actual and its neighbour states.
 *
 * It schould look like this:
 *
 *
 * +----------------------------------------------------------------------+
 * |  Menu                                   | Help                       |
 * +----------------------------------------------------------------------+
 * |                                                                      |
 * | (data of: lower neighbour state | actual state | higher state )      |
 * | (name lower state  | digit/sampling value name | name higher state ) |
 * | (lower state:  lower border - targed - higher border |  \
 *    actual state: lower border - targed - higher border |  \
 *    higher state: lower border - targed - higher border                 |
 * | (                 | actuale value |                         )        |
 * | (... data of the next digit/sampling value)                          |
 * |                                                                      |
 * | Clock: (clock value)   message counter: (message counter value)      |
 * +----------------------------------------------------------------------+
 *
 * Description:
 * 	* a click on [Menu]->[Data file] opens a file dialog to select the
 * 	  file where the data from the mapper is stored
 */
/*
History:
28.03.2016  Oesterholz  created
*/



#include "cDataGloveMapperInfoMainWindow.h"

#include <QMenuBar>
#include <QStatusBar>
#include <QSettings>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>
#include <QFileDialog>


#include "cWidgetDisplayMessageGloveStates.h"

#include "cThreadReaderMessageGloveStates.h"


using namespace std;
using namespace nDataGlove::nInfo;
using namespace nDataGlove;


/**
 * parameter constructor for a Fib object node
 */
cDataGloveMapperInfoMainWindow::cDataGloveMapperInfoMainWindow() :
		PThreadReaderMessageGloveStates( nullptr ),
		strPathData("dataGloveSamplinOutput.dat"),
		pCentralWidget( nullptr ), pWidgetDisplayMessageGloveStates( nullptr ),
		pButtonDevice( nullptr ),
		pMenuFile( nullptr ), pMenuMessages( nullptr ), pMenuHelp( nullptr ),
		pActExit( nullptr ), pActAbout( nullptr ), pActChooseFile( nullptr ) {
	
	DEBUG_OUT_L2(<<"cDataGloveMapperInfoMainWindow("<<this<<")::cDataGloveMapperInfoMainWindow() called"<<endl<<flush);
	
	setWindowTitle( QString( "Data glove view " ).append(
		QString( DATA_GLOVE_MANAGER_VERSION_NAME ) ) );
	
	setMinimumSize( 400, 256 );
	
	//restore the old session
	readSettings();
	
	//thread to read new data glove digit data messages and send them to this object
	PThreadReaderMessageGloveStates = new cThreadReaderMessageGloveStates();
	PThreadReaderMessageGloveStates->setFileMessageGloveStates( strPathData.toStdString() );
	
	createSubWidgets();
	createActions();
	createMenus();
	createToolBars();
	createStatusBar();
	
	PThreadReaderMessageGloveStates->start();
	
	DEBUG_OUT_L3(<<"cDataGloveMapperInfoMainWindow("<<this<<")::cDataGloveMapperInfoMainWindow() done"<<endl<<flush);
}


/**
 * destructor
 */
cDataGloveMapperInfoMainWindow::~cDataGloveMapperInfoMainWindow() {
	
	DEBUG_OUT_L2(<<"cDataGloveMapperInfoMainWindow("<<this<<")::~cDataGloveMapperInfoMainWindow() called"<<endl<<flush);
	
	//store the old session
	writeSettings();
	
	if ( PThreadReaderMessageGloveStates ) {
		
		PThreadReaderMessageGloveStates->quit();
		if ( pWidgetDisplayMessageGloveStates ) {
			PThreadReaderMessageGloveStates->unregisterSamplingMessageListener(
				pWidgetDisplayMessageGloveStates );
		}
		delete PThreadReaderMessageGloveStates;
	}
	
	DEBUG_OUT_L2(<<"cDataGloveMapperInfoMainWindow("<<this<<")::~cDataGloveMapperInfoMainWindow() done"<<endl<<flush);
}


/**
 * @return the name of this class "cDataGloveMapperInfoMainWindow"
 */
std::string cDataGloveMapperInfoMainWindow::getName() const {
	
	return std::string( "cDataGloveMapperInfoMainWindow" );
}


/**
 * This method creates the widgets used in this window.
 */
void cDataGloveMapperInfoMainWindow::createSubWidgets() {
	
	pCentralWidget = new QWidget( this );
	
	
	QVBoxLayout * pLayoutCentralWidget = new QVBoxLayout( pCentralWidget );
	pLayoutCentralWidget->setMargin( 0 );
	
	if ( pWidgetDisplayMessageGloveStates ) {
		PThreadReaderMessageGloveStates->unregisterSamplingMessageListener(
			pWidgetDisplayMessageGloveStates );
	}
	
	pWidgetDisplayMessageGloveStates = new cWidgetDisplayMessageGloveStates( this );
	pLayoutCentralWidget->addWidget( pWidgetDisplayMessageGloveStates );
	
	PThreadReaderMessageGloveStates->registerSamplingMessageListener(
		pWidgetDisplayMessageGloveStates );
	
	//pCentralWidget->setLayout( pLayoutCentralWidget );
	setCentralWidget( pCentralWidget );
	
	//create subwindows
	DEBUG_OUT_L3(<<"cDataGloveMapperInfoMainWindow("<<this<<")::cDataGloveMapperInfoMainWindow() create subwindows"<<endl<<flush);
}



/**
 * This method creates the actions used in this window.
 */
void cDataGloveMapperInfoMainWindow::createActions() {

	//action for exit the application
	pActExit = new QAction( tr("E&xit"), this );
	pActExit->setShortcuts( QKeySequence::Quit );
	pActExit->setStatusTip( tr("Exit the application") );
	connect ( pActExit, SIGNAL( triggered() ), this, SLOT( close() ) );
	
	//action for show about this application information
	pActAbout = new QAction( tr("&About"), this );
	pActAbout->setStatusTip( tr("Show the application's About box") );
	connect( pActAbout, SIGNAL( triggered() ), this, SLOT( about() ) );
	
	//action for choose the file from where to read the data glove state data
	pActChooseFile = new QAction( tr("&File"), this );
	pActChooseFile->setStatusTip( tr("Choose the file from where to read the data glove state data.") );
	connect( pActChooseFile, SIGNAL( triggered() ), this, SLOT( chooseDevice() ) );
}


/**
 * This method creates the menu used in this window.
 */
void cDataGloveMapperInfoMainWindow::createMenus() {
	//create file menu
	pMenuFile = menuBar()->addMenu( tr("&File") );
	pMenuFile->addAction( pActChooseFile );
	pMenuFile->addSeparator();
	pMenuFile->addAction( pActExit );
	
	//add seperator (for Motif-based styles, there help should be on right side)
	menuBar()->addSeparator();
	//create help menu
	pMenuHelp = menuBar()->addMenu( tr("&Help") );
	//TODO help page: pMenuHelp->addAction(aboutQtAct);
	pMenuHelp->addAction( pActAbout);
}

/**
 * This method creates the tool bar used in this window.
 */
void cDataGloveMapperInfoMainWindow::createToolBars() {
	/*TODO
	//tool bar for mode actions
	pToolBarFile = addToolBar( tr("Modus") );
	pToolBarFile->addAction( pActMouseModePointing );
	pToolBarFile->addAction( pActMouseModeDrawing );
	*/
}

/**
 * This method creates the status bar used in this window.
 */
void cDataGloveMapperInfoMainWindow::createStatusBar() {
	
	statusBar()->showMessage(tr("Ready"));
	/*TODO
	 * show state of data glove
	.. */
}


/**
 * This method will read the settings for this window and restores the
 * window to it.
 * @see QSettings
 */
void cDataGloveMapperInfoMainWindow::readSettings() {
	
	QSettings settings("Fib development", "Data glove digit info");
	QPoint pos = settings.value("mainWindow/pos", QPoint( 0, 0 ) ).toPoint();
	QSize size = settings.value("mainWindow/size", sizeHint() ).toSize();
	resize( size );
	move( pos);
	strPathData = settings.value("path/messageGloveStates",
		"dataGloveSamplinOutput.dat" ).toString();
}


/**
 * This method will write the settings of this window.
 * @see QSettings
 */
void cDataGloveMapperInfoMainWindow::writeSettings() {
	
	QSettings settings("Fib development", "Data glove digit info");
	settings.setValue("mainWindow/pos", pos() );
	settings.setValue("mainWindow/size", size() );
	settings.setValue("path/messageGloveStates", strPathData );
}


/**
 * This method handels close events.
 *
 * @param pEventClose a pointer to the close event to andle
 */
void cDataGloveMapperInfoMainWindow::closeEvent( QCloseEvent * pEventClose ) {
	
	DEBUG_OUT_L2(<<"cDataGloveMapperInfoMainWindow("<<this<<")::closeEvent( pEventClose="<<pEventClose<<") called"<<endl<<flush);
	//write the actual settings
	writeSettings();
	//check if the Fib object should be saved
	pEventClose->accept();
	//pEventClose->ignore();
}



/**
 * This slot shows the about (this application) information.
 */
void cDataGloveMapperInfoMainWindow::about() {
	
	QMessageBox::about( this, tr("About data glove info"),
		tr("<b>The data glove info window</b> is an application to show the actual data glove state."
			"The data glove info is free software (GNU GPL3 "
			"<a href='http://www.gnu.org/licenses/'>www.gnu.org/licenses/</a>) "
			"You can find information about the data glove info in the github repository "
			"<a href='http://www.github.com/BioKom/dataGlove/'>www.github.com/BioKom/Fib/</a> .") );
}



/**
 * This slot will open a dialog to choose the data glove device.
 *
 * @see strPathData
 * @see pDataGloveDGTechVHand
 */
void cDataGloveMapperInfoMainWindow::chooseDevice() {
	
	QSettings settings("Fib development", "Data glove digit info");
	//open file dialog
	QFileDialog fileDialog( nullptr, tr("Select communication file"),  settings.value(
			"mainWindow/chooseDevice/lastFile",
				"*" ).toString(),
		tr("dataGloveSamplinOutput.dat" ) );
	
	fileDialog.setFileMode( QFileDialog::ExistingFile );
	
	if ( fileDialog.exec() ){
		//a device was choosen
		QStringList liFileNames = fileDialog.selectedFiles();
		if ( liFileNames.size() != 1 ){
			//not one device choosen
			return;
		}
		//store the device path
		strPathData = //fileDialog.directory().absolutePath() +
			liFileNames.first();
		DEBUG_OUT_L2(<<"cDataGloveMapperInfoMainWindow("<<this<<")::chooseDevice() setting path to "<<strPathData.toStdString()<<endl<<flush);
		statusBar()->showMessage(tr("setting path to \"") + strPathData + "\"");
		
		//store actual device as last used device
		settings.setValue("mainWindow/chooseDevice/lastFile", liFileNames.front() );
		//inform the thread of the change
		PThreadReaderMessageGloveStates->setFileMessageGloveStates(
			strPathData.toStdString() );
	}//else
	//no device to open
}













