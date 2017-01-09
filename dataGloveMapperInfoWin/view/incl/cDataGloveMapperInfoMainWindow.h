
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
 */
/*
History:
28.03.2016  Oesterholz  created
*/


#ifndef ___N_DATA_GLOVE__N_INFO__C_DATA_GLOVE_MAPPER_INFO_MAIN_WINDOW_H__
#define ___N_DATA_GLOVE__N_INFO__C_DATA_GLOVE_MAPPER_INFO_MAIN_WINDOW_H__


#include "version.h"

#include <QString>
#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QCloseEvent>
#include <QPushButton>

#include "cWidgetDisplayMessageGloveStates.h"


namespace nDataGlove{

//forward declarations
class cDataGloveDGTechVHand;

namespace nInfo{

class cWidgetDisplaySamplingData;
class cThreadReaderMessageGloveStates;


class cDataGloveMapperInfoMainWindow: public QMainWindow {
		Q_OBJECT

friend class cMainWindowHandler;
public:
	
	/**
	 * parameter constructor for a Fib object node
	 */
	explicit cDataGloveMapperInfoMainWindow();
	
	/**
	 * destructor
	 */
	virtual ~cDataGloveMapperInfoMainWindow();
	
	/**
	 * @return the name of this class "cDataGloveMapperInfoMainWindow"
	 */
	virtual std::string getName() const;
	

//signals:
	
protected:
	
	/**
	 * This method creates the widgets used in this window.
	 */
	void createSubWidgets();
	
	/**
	 * This method creates the actions used in this window.
	 */
	virtual void createActions();
	
	/**
	 * This method creates the menu used in this window.
	 */
	virtual void createMenus();
	
	/**
	 * This method creates the tool bar used in this window.
	 */
	virtual void createToolBars();
	
	/**
	 * This method creates the status bar used in this window.
	 */
	virtual void createStatusBar();
	
	/**
	 * This method will read the settings for this window and restores the
	 * window to it.
	 * @see QSettings
	 */
	virtual void readSettings();
	
	/**
	 * This method will write the settings of this window.
	 * @see QSettings
	 */
	virtual void writeSettings();
	
	/**
	 * This method handels close events.
	 *
	 * @param pEventClose a pointer to the close event to handle
	 */
	virtual void closeEvent( QCloseEvent * pEventClose );
	

//members
	
	
	/**
	 * The thread the handel the messages send by the data glove.
	 */
	cThreadReaderMessageGloveStates * PThreadReaderMessageGloveStates;
	
	/**
	 * The name of the data glove communication device/file.
	 */
	QString strPathData;
	
	
	/**
	 * The central widget of this main window.
	 * It should contain the main graphical wiew (or scene) and the input
	 * variable widget of the main graphical scene.
	 * If NULL no input variable widget of the main graphical scene exists
	 * and the graphical view will be used directly as the central widget of
	 * this main window.
	 *
	 * @see pFibObjectGraphicsScene
	 * @see pFibObjectGraphicsView
	 */
	QWidget * pCentralWidget;
	
	/**
	 * The widget with the data glove state data.
	 */
	cWidgetDisplayMessageGloveStates * pWidgetDisplayMessageGloveStates;
	
	/**
	 * Button to choose a data glove device.
	 * @see strPathData
	 */
	QPushButton * pButtonDevice;
	
	
	///file menu
	QMenu * pMenuFile;
	///edit menu
	QMenu * pMenuMessages;
	///help menu
	QMenu * pMenuHelp;
	
	/**
	 * Actions
	 */
	///action for exit the application
	QAction * pActExit;
	
	
	///action for show about this application information
	QAction * pActAbout;
	
	///action for choose the file from where to read the data glove state data
	QAction * pActChooseFile;
	
	
private slots:
	
	/**
	 * This slot shows the about (this application) information.
	 */
	void about();
	
		
	/**
	 * This slot will open a dialog to choose the data glove device.
	 *
	 * @see strPathData
	 */
	void chooseDevice();
	
	
};//end class cDataGloveMapperInfoMainWindow

};//end namespace nInfo
};//end namespace nDataGlove


#endif //___N_DATA_GLOVE__N_INFO__C_DATA_GLOVE_MAPPER_INFO_MAIN_WINDOW_H__





