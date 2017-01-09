
//TODO check

/**
 * @file dataGloveMapperInfo
 * file name: dataGloveMapperInfo.cpp
 * @author Betti Oesterholz
 * @date 26.07.2016
 * @mail webmaster@BioKom.info
 *
 * System: C++, Qt4
 *
 * This file contains the main function for the data glove manager application.
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
 * This file contains the main function for the data glove manager application.
 * It implements an application, with a grapical user interface, for
 * managing a data glove and displayes the readed values.
 */
/*
History:
26.07.2016  Oesterholz  created
*/


//TODO rework


#include <QtGui>
#include <QApplication>


#include "cDataGloveMapperInfoMainWindow.h"


int main(int argc, char **argv){
	//the QApplication is the basis of the Qt application
	QApplication app(argc, argv);
	app.setOrganizationName("Fib-development");
	app.setApplicationName("Data glove view");
	app.setOrganizationDomain("www.Fib-development.org");
	app.setApplicationVersion("V0.0.0");
	//init the translation/localization system
	QString locale = QLocale::system().name();

	QTranslator translator;
	const QString strTranslationFile = QString("dataGloveMapperInfo_") + locale;
	translator.load( strTranslationFile );
	app.installTranslator(&translator);
	
	//no window created -> create empty main window
	nDataGlove::nInfo::cDataGloveMapperInfoMainWindow * pNewMainWindow =
		new nDataGlove::nInfo::cDataGloveMapperInfoMainWindow();
	//show the new window
	pNewMainWindow->show();
	
	
	return app.exec();
}












