#
# @author Betti Oesterholz
# @date 30.07.2016
# @mail webmaster@BioKom.info
#
# System: qmake
#
# This is the makefile for the data glove mapper info C++ system.
#
#
# Copyright (C) @c GPL3 2016 Betti Oesterholz
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#
#
# This is the makefile for the data glove Mapper Info C++ system.
# It implements an application, with a grapical user interface, for
# showing the data glove mapper state.
#
#
# call:
# 	qmake dataGloveMapperInfo.pro
# 		to create the data glove mapper info make file
# TODO
# 	qmake dataGloveMapperInfo.pro -project "CONFIG += debug"
# 		to create the data glove mapper info make file for the debugging version
#
# 	add "-d" for debugging
#
# Then call:
# 	make
#
#
#
##########################################################################
#
# History:
# 30.07.2016   Oesterholz   created


##########################################################################
#
# config variable
#
##########################################################################

# comment in for normal mode
# CONFIG += qt warn_on

# comment in for debug mode
# CONFIG += qt warn_on test_debug

# comment in for test mode
CONFIG += qt warn_on debug test_debug qtestlib

##########################################################################
#
# folder variables
#
##########################################################################


# folders of the data glove manager code
DIR_SRC  =src/
DIR_INCL =incl/

# Folder of the testcases
DIR_TEST =test/

# Folder of the translation/localization files
DIR_TRANSLATIONS=translations/


# Folder of the objects
DIR_OBJ =obj/

# Folder of the test objects
DIR_OBJ_TEST =obj_test/

# Folder of the executebels
DIR_BIN =bin/

# Folder of the test executebels
DIR_TESTCASE = testcase/


# Path to the project base directory
DIR_PROJECT_BASE =../

# TODO weg
# Path to the data glove communication module
# DIR_DATA_GLOVE_COM =$${DIR_PROJECT_BASE}dataGloveDGTechVHandCommunication/
# DIR_DATA_GLOVE_COM_INCL =$${DIR_DATA_GLOVE_COM}incl/ \
#	$${DIR_DATA_GLOVE_COM}nModelDataGloveDGTechVHand/incl/ \
#	$${DIR_PROJECT_BASE}dataGloveMapper/incl/
# DIR_DATA_GLOVE_COM_LIB  =$${DIR_DATA_GLOVE_COM}lib/

# Path to the data mapper module
DIR_DATA_GLOVE_MAPPER =$${DIR_PROJECT_BASE}dataGloveMapper/
DIR_DATA_GLOVE_MAPPER_INCL =$${DIR_DATA_GLOVE_MAPPER}incl/
DIR_DATA_GLOVE_MAPPER_LIB  =$${DIR_DATA_GLOVE_MAPPER}lib/


# Path to the tinyxml module
# DIR_TINY_XML =$${DIR_PROJECT_BASE}tinyxml/


##########################################################################
#
# qmake variables
#
##########################################################################

# the executebel to create
TARGET = $${DIR_BIN}dataGloveMapperInfo
# the source file with the main() function for the executebel to create
TARGET_SOURCE = $${DIR_SRC}*.cpp


TEMPLATE = app
DEPENDPATH  += .
INCLUDEPATH += . $${DIR_INCL} model/$${DIR_INCL} view/$${DIR_INCL} control/$${DIR_INCL}\
	$${DIR_PROJECT_BASE} $${DIR_DATA_GLOVE_MAPPER_INCL}
#  $${DIR_DATA_GLOVE_COM_INCL} $${DIR_TINY_XML}

OBJECTS_DIR = $${DIR_OBJ}obj
MOC_DIR = $${DIR_OBJ}moc
RCC_DIR = $${DIR_OBJ}rcc
UI_DIR  = $${DIR_OBJ}ui
# RESOURCES = resources.qrc
LIBS_ADDITIONAL = -L$${DIR_DATA_GLOVE_MAPPER_LIB} -lDataGloveMapper -lboost_serialization
# TODO weg
# -L$${DIR_DATA_GLOVE_COM_LIB} -lDataGloveCom
QMAKE_CXXFLAGS += -std=c++11

DEFINES += TIXML_USE_STL



test_debug {
	TARGET = $${DIR_TESTCASE}test_dataGloveMapperInfo
	CONFIG += debug
	OBJECTS_DIR = $${DIR_OBJ_TEST}obj
	MOC_DIR = $${DIR_OBJ_TEST}moc
	RCC_DIR = $${DIR_OBJ_TEST}rcc
	UI_DIR  = $${DIR_OBJ_TEST}ui
	LIBS_ADDITIONAL = -L$${DIR_DATA_GLOVE_MAPPER_LIB} -lDataGloveMapper_test -lboost_serialization
}




LIBS += $${LIBS_ADDITIONAL}

# Input
SOURCES += $${TARGET_SOURCE} \
#	model/$${DIR_SRC}*.cpp \
	view/$${DIR_SRC}*.cpp \
	control/$${DIR_SRC}*.cpp
	
HEADERS += version.h\
#	model/$${DIR_INCL}*.h \
	view/$${DIR_INCL}*.h \
	control/$${DIR_INCL}*.h



##########################################################################
#
# qmake QT Linguist variables
#
##########################################################################

# TODO
# TRANSLATIONS = $${DIR_TRANSLATIONS}dataGloveMapperInfo_de.ts \
#	$${DIR_TRANSLATIONS}dataGloveMapperInfo_fr.ts








